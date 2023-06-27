#pragma once

#include <cstdio>
#include <cstring>

#include <memory>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <optional>
#include <exception>

#include "Log.h"
#include "messages.h"

namespace common
{
	enum class ModuleID
	{
		MID_MENU = 0,
		MID_DICTIONARY,
		MID_MAX,
		MID_UNKNOWN = MID_MAX
	};

	enum class MessageID
	{
		CommandLineRequest = 0,
		CommandLineReply,
		CommandRequest,
		CommandReply,
		MAX
	};

	enum class CommandID
	{
		ADD = 0,
		REM,
		MAX
	};

	enum class CommandStatus
	{
		SUCCESS = 0,
		IN_PROGRESS,
		FAILED,
		MAX
	};

	std::string ModuleID2str(ModuleID id);
	std::vector<std::string> split_string(const std::string &line, const std::string &delim);
}
