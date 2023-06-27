#pragma once

#include "common.h"

class Message;
typedef std::shared_ptr<Message> Message_p;

class CommandLineMessage;
class CommandAddMessage;
class CommandRemMessage;

class MessageBuilder
{
public:
	inline Message_p make_CommandLineMessage(
		common::ModuleID _sid,
		common::ModuleID _rid,
		const std::string &_line
		)
	{
		return std::make_shared<CommandLineMessage>(_sid, _rid, _line);
	}

	inline Message_p make_CommandAddMessage(
		common::ModuleID _sid,
		common::ModuleID _rid,
		const std::string &_w,
		const std::string &_t
		)
	{
		return std::make_shared<CommandAddMessage>(_sid, _rid, _w, _t);
	};

	inline Message_p make_CommandRemMessage(
		common::ModuleID _sid,
		common::ModuleID _rid,
		const std::string &_w,
		)
	{
		return std::make_shared<CommandRemMessage>(_sid, _rid, _w);
	};
};

class Message
{
public:
	common::MessageID id;
	common::CommandStatus status;
	common::ModuleID sid, rid;

	Message() = delete;
	Message(common::ModuleID _sid,
	        common::ModuleID _rid,
	        common::MessageID _id
	        ):
		sid(_sid), rid(_rid), id(_id), status(common::CommandStatus::IN_PROGRESS)
			{};
};

class CommandLineMessage: public Message
{
public:
	std::string line;

	CommandLine() = delete;
	CommandLine(common::ModuleID _sid,
	            common::ModuleID _rid,
	            const std::string &_line
	            ):
		Message(_sid, _rid,
		        common::MessageID::CommandLineRequest
		        ),
		line(_line)
			{};
};

class CommandMessage: public Message
{
public:
	common::CommandID cmd;

	CommandMessage() = delete;
	CommandMessage(common::ModuleID _sid,
	               common::ModuleID _rid,
	               common::CommandID _cmd
	               ):
		Message(_sid, _rid,
		        common::MessageID::CommandRequest
		        ),
		cmd(_cmd)
			{};
};

class CommandAddMessage: public CommandMessage
{
public:
	std::string w, t;

	CommandAddMessage() = delete;
	CommandAddMessage(common::ModuleID _sid,
	                  common::ModuleID _rid,
	                  const std::string &_w,
	                  const std::string &_t
	                  ):
		CommandMessage(common::ModuleID _sid,
		               common::ModuleID _rid,
		               common::CommandID::ADD
		               ),
		w(_w), t(_t)
			{};
};

class CommandRemMessage: public CommandMessage
{
public:
	std::string w;

	CommandRemMessage() = delete;
	CommandRemMessage(common::ModuleID _sid,
	                  common::ModuleID _rid,
	                  const std::string &_w
	                  ):
		CommandMessage(common::ModuleID _sid,
		               common::ModuleID _rid,
		               common::CommandID::REM
		               ),
		w(_w)
			{};
};
