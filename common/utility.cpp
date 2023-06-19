// #include <regex>

#include "common.h"

static Log log("utility", Log::LogLevel::LL_DEBUG);

std::vector<std::string> common::split_string(const std::string &line, const std::string &delim)
{
	std::vector<std::string> result;
	char *tok = NULL, *save_ptr = NULL;
	char *line2 = strdup(line.c_str());

	if (line2)
	{
		for (tok = strtok_r(line2, delim.c_str(), &save_ptr);
		     tok;
		     tok = strtok_r(NULL, delim.c_str(), &save_ptr))
		{
			std::string string(tok);

			LOG_DEBUG("string == %s\n", string.c_str());
			result.push_back(string);
		}
		free(line2);
	}

	return result;
}

std::string common::ModuleID2str(ModuleID id)
{
	switch(id)
	{
	case ModuleID::MID_MENU:
		return "MENU";
	case ModuleID::MID_DICTIONARY:
		return "DICTIONARY";
	default:
		return "unknown";
	}
	return "unknown";
}
