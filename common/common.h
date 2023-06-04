#pragma once

#include <cstdio>
#include <cstring>

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <optional>
#include <exception>

#include "Log.h"

namespace common
{
	std::vector<std::string> split_string(const std::string &line, const std::string &delim);
}
