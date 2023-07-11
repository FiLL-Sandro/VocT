#include "Menu.h"

static Log log("Menu", Log::LogLevel::LL_DEBUG);

static Message_p add_parser(std::vector<std::string> &args);
static Message_p rem_parser(std::vector<std::string> &args);
static Message_p open_parser(std::vector<std::string> &args);
static Message_p close_parser(std::vector<std::string> &args);

Opts Menu::opts
{
	{"add", add_parser}
	, {"rem", rem_parser}
	, {"open", open_parser}
	, {"close", close_parser}
};

static Message_p add_parser(std::vector<std::string> &args)
{
	Message_p p;

	if (args.size() != 3)
	{
		LOG_ERROR("wrong format of command: add <word> <translate>\n");
	}
	else
	{
		p = make_CommandAddMessage(common::ModuleID::MENU,
		                           common::ModuleID::DICTIONARY,
		                           args[1],
		                           args[2]);
	}
	return p;
}

static Message_p rem_parser(std::vector<std::string> &args)
{
	Message_p p;

	if (args.size() != 2)
	{
		LOG_ERROR("wrong format of command: rem <word>\n");
	}
	else
	{
		p = make_CommandRemMessage(common::ModuleID::MENU,
		                           common::ModuleID::DICTIONARY,
		                           args[1]);
	}
	return p;
}

static Message_p open_parser(std::vector<std::string> &args)
{
	Message_p p;

	if (args.size() != 2)
	{
		LOG_ERROR("wrong format of command: open <filepath>\n");
	}
	else
	{
		p = make_CommonCommandMessage(common::ModuleID::MENU,
		                              common::ModuleID::DICTIONARY,
		                              common::CommandID::OPEN,
		                              args[1]);
	}
	return p;
}

static Message_p close_parser(std::vector<std::string> &args)
{
	Message_p p;

	if (args.size() != 2)
	{
		LOG_ERROR("wrong format of command: close <filepath>\n");
	}
	else
	{
		p = make_CommonCommandMessage(common::ModuleID::MENU,
		                              common::ModuleID::DICTIONARY,
		                              common::CommandID::CLOSE,
		                              args[1]);
	}
	return p;
}

void Menu::show_opts(void)
{
	for (auto i : opts)
	{
		printf("* %s\n", i.first.c_str());
	}
}

Message_p Menu::parse_cmd(const std::string &line)
{
	if (!line.empty())
	{
		Message_p p;
		std::vector<std::string> args = common::split_string(line, " ");

		if (!args.empty())
		{
			auto opt = Menu::opts.find(args.front());
			if (opt != Menu::opts.end())
			{
				return opt->second(args);
			}
			else
			{
				LOG_ERROR("unknown command: %s\n", args.front().c_str());
			}
		}
		else
		{
			LOG_ERROR("cannot parse line: %s\n", line.c_str());
		}
	}
	else
	{
		LOG_ERROR("empty line?! WTF?!\n");
	}
	return Message_p{};
}

int Menu::check_message()
{
	Message_p request = recv_req();

	if (!request)
		return 0;

	switch (request->id)
	{
		case common::MessageID::CommandLineRequest:
			{
				Message_p _request;
				CommandLineMessage_p _p = std::dynamic_pointer_cast<CommandLineMessage>(request);
				if (!_p)
				{
					LOG_ERROR("cannot convert to CommandLineMessage\n");
					goto CommandLineRequest_bad;
				}

				_request = parse_cmd(_p->line);
				if (!_request)
					goto CommandLineRequest_bad;

				_request = send_req_sync(_request, common::MessageID::CommandReply);
				if (!_request)
					goto CommandLineRequest_bad;

				request->id = common::MessageID::CommandLineReply;
				request->status = common::CommandStatus::SUCCESS;
				send_rep(request);
				break;
CommandLineRequest_bad:
				send_rep_error(request, common::MessageID::CommandLineReply);
				break;
			}
			break;
		default:
			LOG_ERROR("unknown message: id == %d\n", static_cast<int>(request->id));
			break;
	}
	are_showed = false;
	return 0;
}

int Menu::_run(void)
{
	if (!are_showed)
	{
		show_opts();
		are_showed = true;
	}
	std::this_thread::sleep_for(std::chrono::seconds(1));
	return 0;
}
