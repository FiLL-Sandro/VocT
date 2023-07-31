#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#include "Dictionary.h"

static Log log("Dictionary", Log::LogLevel::LL_DEBUG);

void Dictionary::save_dictionary()
{
	std::string _filepath{"/tmp/tmp.json"};

	if (filepath.empty())
	{
		LOG_DEBUG("filepath aren't set -- dictionary will be saved to \"%s\"\n",
		          _filepath.c_str());
	}
	else
		_filepath = filepath;

	db.dump2file(_filepath);
}

char* Dictionary::read_file(const std::string &filepath, size_t &size)
{
	struct stat st;

	if (!stat(filepath.c_str(), &st))
	{
		int fd = open(filepath.c_str(), O_RDONLY);
		if (fd >= 0)
		{
			LOG_DEBUG("size of file: %ld\n", st.st_size);
			void *content = mmap(NULL, st.st_size, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
			close(fd);
			if (content)
			{
				size = st.st_size;
				return static_cast<char*>(content);
			}
		}
		else
		{
			LOG_ERROR("open: %s\n", strerror(errno));
		}
	}
	else
	{
		LOG_ERROR("stat: %s\n", strerror(errno));
	}
	return NULL;
}

void Dictionary::open_handler(CommonCommandMessage_p p, Message_p reply)
{
	if (filepath.empty())
	{
		filepath = p->line;

		if (common::create_file(filepath) < 0)
		{
			LOG_ERROR("cannot create file: \"%s\"\n", filepath.c_str());
			goto error;
		}

		size_t size;
		char *content = read_file(filepath, size);
		if (content)
		{
			const char *_content = !size ? "{}" : content;

			if (db.from_json(cJSON_Parse(_content)))
			{
				munmap(content, size);
				LOG_ERROR("cannot parse the Dictionary\n");
				goto error;
			}
			munmap(content, size);
		}
	}
	else
	{
		LOG_ERROR("dictionary already openned: \"%s\"\n", filepath.c_str());
	}
	reply->id = common::MessageID::CommandReply;
	reply->status = common::CommandStatus::SUCCESS;
	return;
error:
	filepath.clear();
	db.clear();
	reply->id = common::MessageID::CommandReply;
	reply->status = common::CommandStatus::FAILED;
	return;
}

void Dictionary::close_handler(Message_p reply)
{
	save_dictionary();
	filepath.clear();
	db.clear();
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

	db.add(p->w, p->t);
	reply->id = common::MessageID::CommandReply;
	reply->status = common::CommandStatus::SUCCESS;
	return;
}

void Dictionary::dump_handler(Message_p reply)
{
	printf("Dictionary content:\n");
	db.dump2screen();

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
	db.rem(p->w);

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
