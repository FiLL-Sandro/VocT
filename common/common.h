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
#include <algorithm>

#include "Log.h"

namespace common
{
	enum class ModuleID
	{
		MENU = 0,
		DICTIONARY,
		RECEIVER,
		TEST1,
		TEST2,
		MAX,
		UNKNOWN = MAX
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

#include "messages.h"
