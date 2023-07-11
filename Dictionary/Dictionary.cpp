#include "Dictionary.h"

static Log log("Dictionary", Log::LogLevel::LL_DEBUG);

int Dictionary::parse_dictionary(std::ifstream &dictionary)
{
	std::string line;

	while (std::getline(dictionary, line).good())
	{
		std::vector<std::string> args{ common::split_string(line, ":") };

		if (args.size() != 2)
		{
			LOG_ERROR("wrong format of entry(%s) in the Dictionary: <word>:<translate>\n", line.c_str());
			return -1;
		}

		db.emplace(args[0], args[1]);
	}
	return 0;
}

void Dictionary::save_dictionary()
{
	if (filepath.empty())
	{
		LOG_ERROR("cannot save temporary dictionary\n");
		return;
	}

	std::ofstream ofs{ filepath };
	if (!ofs.is_open())
	{
		LOG_ERROR("cannot open file %s\n", filepath.c_str());
		return;
	}

	for (auto i : db)
	{
		ofs << i.first << ":" << i.second << std::endl;
	}
}

void Dictionary::open_handler(CommonCommandMessage_p p, Message_p reply)
{
	std::ifstream ifs;

	filepath = p->line;

	if (!common::file_exist(filepath))
	{
		LOG_ERROR("%s aren't exist\n", filepath.c_str());
		goto error;
	}

	ifs.open(filepath);
	if (!ifs.is_open())
	{
		LOG_ERROR("cannot open %s\n", filepath.c_str());
		goto error;
	}

	if (parse_dictionary(ifs))
	{
		LOG_ERROR("cannot parse the Dictionary\n");
		goto error;
	}

	reply->id = common::MessageID::CommandReply;
	reply->status = common::CommandStatus::SUCCESS;
	return;
error:
	filepath.clear();
	reply->id = common::MessageID::CommandReply;
	reply->status = common::CommandStatus::FAILED;
	return;
}

void Dictionary::close_handler(Message_p reply)
{
	if (!filepath.empty())
	{
		save_dictionary();
		filepath.clear();
	}
	reply->id = common::MessageID::CommandReply;
	reply->status = common::CommandStatus::SUCCESS;
	return;
}

void Dictionary::add_handler(CommandAddMessage_p p, Message_p reply)
{
	if (!p)
	{
		LOG_ERROR("cannot convert to CommandAddMessage\n");
		reply->id = common::MessageID::CommandReply;
		reply->status = common::CommandStatus::FAILED;
		return;
	}

	LOG_DEBUG("try to add %s:%s\n", p->w.c_str(), p->t.c_str());

	db.emplace(p->w, p->t);
	reply->id = common::MessageID::CommandReply;
	reply->status = common::CommandStatus::SUCCESS;
	return;
}

void Dictionary::dump_handler(Message_p reply)
{
	printf("Dictionary content:\n");
	for (auto i : db)
	{
		printf("%s:%s\n", i.first.c_str(), i.second.c_str());
	}

	reply->id = common::MessageID::CommandReply;
	reply->status = common::CommandStatus::SUCCESS;
	return;
}

void Dictionary::rem_handler(CommandRemMessage_p p, Message_p reply)
{
	if (!p)
	{
		LOG_ERROR("cannot convert to CommandRemMessage\n");
		reply->id = common::MessageID::CommandReply;
		reply->status = common::CommandStatus::FAILED;
		return;
	}

	LOG_DEBUG("try to remove %s\n", p->w.c_str());
	auto it = db.find(p->w);
	if (it != db.end())
	{
		db.erase(it);
	}

	reply->id = common::MessageID::CommandReply;
	reply->status = common::CommandStatus::SUCCESS;
	return;
}

int Dictionary::check_message()
{
	Message_p request = recv_req();

	if (!request)
		return 0;

	switch (request->id)
	{
		case common::MessageID::CommandRequest:
			{
				CommandMessage_p _p = std::dynamic_pointer_cast<CommandMessage>(request);
				if (!_p)
				{
					LOG_ERROR("cannot convert to CommandMessage\n");
					goto CommandRequest_bad;
				}

				switch(_p->cmd)
				{
					case common::CommandID::OPEN:
						open_handler( std::dynamic_pointer_cast<CommonCommandMessage>(_p), request );
						break;
					case common::CommandID::CLOSE:
						close_handler( request );
						break;
					case common::CommandID::ADD:
						add_handler( std::dynamic_pointer_cast<CommandAddMessage>(_p), request );
						break;
					case common::CommandID::REM:
						rem_handler( std::dynamic_pointer_cast<CommandRemMessage>(_p), request );
						break;
					case common::CommandID::DUMP:
						dump_handler( request );
						break;
					default:
						LOG_ERROR("unknown command: id == %d\n", static_cast<int>(_p->cmd));
						break;
				}

				send_rep(request);
				break;
CommandRequest_bad:
				send_rep_error(request, common::MessageID::CommandReply);
				break;
			}
			break;
		default:
			LOG_ERROR("unknown message: id == %d\n", static_cast<int>(request->id));
			break;
	}
	return 0;
}

int Dictionary::_run()
{
	std::this_thread::sleep_for(std::chrono::seconds(1));
	return 0;
}
