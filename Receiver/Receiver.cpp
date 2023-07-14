#include "Receiver.h"

static Log log("Receiver", Log::LogLevel::LL_DEBUG);

int Receiver::_run()
{
	std::string line;
	Message_p message;
	CommandLineMessage_p reply;

	std::getline(std::cin, line);
	LOG_DEBUG("line: %s\n", line.c_str());

	message = make_CommandLineMessage(id, common::ModuleID::MENU, line);
	if (!message)
	{
		LOG_ERROR("cannot allocate memory for CommandLineMessage\n");
		goto fail;
	}

	if (send_req(message))
	{
		LOG_ERROR("cannot send CommandLineMessage\n");
		goto fail;
	}

	message = recv_rep(common::MessageID::CommandLineReply);
	if (!message)
	{
		LOG_ERROR("CommandLineReply aren't received\n");
		goto fail;
	}

	reply = std::dynamic_pointer_cast<CommandLineMessage>(message);
	if (reply)
	{
		if (reply->status != common::CommandStatus::SUCCESS)
		{
			LOG_ERROR("cannot handle command: %s\n", line.c_str());
			goto fail;
		}
	}
	return 0;
fail:
	set_fatal_status(true);
	return -1;
}
