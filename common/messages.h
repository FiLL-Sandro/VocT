#pragma once

#include "common.h"

class Message;
typedef std::shared_ptr<Message> Message_p;

class CommandLineMessage;
typedef std::shared_ptr<CommandLineMessage> CommandLineMessage_p;

class CommandAddMessage;
typedef std::shared_ptr<CommandAddMessage> CommandAddMessage_p;

class CommandRemMessage;
typedef std::shared_ptr<CommandRemMessage> CommandRemMessage_p;

static inline Message_p make_CommandLineMessage(
	common::ModuleID _sid,
	common::ModuleID _rid,
	const std::string &_line
	)
{
	return std::dynamic_pointer_cast<Message>(
	       	std::make_shared<CommandLineMessage>(_sid, _rid, _line)
	       );
}

static inline Message_p make_CommandAddMessage(
	common::ModuleID _sid,
	common::ModuleID _rid,
	const std::string &_w,
	const std::string &_t
	)
{
	return std::dynamic_pointer_cast<Message>(
	       	std::make_shared<CommandAddMessage>(_sid, _rid, _w, _t)
	       );
}

static inline Message_p make_CommandRemMessage(
	common::ModuleID _sid,
	common::ModuleID _rid,
	const std::string &_w
	)
{
	return std::dynamic_pointer_cast<Message>(
	       	std::make_shared<CommandRemMessage>(_sid, _rid, _w)
	       );
}

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
		id(_id),
		status(common::CommandStatus::IN_PROGRESS),
		sid(_sid), rid(_rid)
			{};
	virtual ~Message() = default;
};

class CommandLineMessage: public Message
{
public:
	std::string line;

	CommandLineMessage() = delete;
	CommandLineMessage(common::ModuleID _sid,
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
		Message(_sid, _rid, common::MessageID::CommandRequest),
		cmd(_cmd)
			{};
	virtual ~CommandMessage() = default;
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
		CommandMessage(_sid, _rid, common::CommandID::ADD),
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
		CommandMessage(_sid, _rid, common::CommandID::REM),
		w(_w)
			{};
};
