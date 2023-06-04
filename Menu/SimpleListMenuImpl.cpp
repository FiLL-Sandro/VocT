#include <iostream>

#include "SimpleListMenuImpl.h"

static Log log("SimpleListMenuImpl", Log::LogLevel::LL_DEBUG);

int SimpleListMenuImpl::_run(MenuOpt &opt)
{
	int rc = 0;
	auto o = opts.find(opt.name);

	if (o != opts.end())
	{
		rc = o->second(opt);
	}

	return rc;
}

std::optional<MenuOpt> SimpleListMenuImpl::parse_cmd(const std::string &line)
{
	if (line.empty())
		return std::optional<MenuOpt>();
	return std::optional<MenuOpt>(common::split_string(line, " "));
}

void SimpleListMenuImpl::show_opts(void)
{
	for (auto i : opts)
	{
		printf("* %s\n", i.first.c_str());
	}
}

std::optional<MenuOpt> SimpleListMenuImpl::_get_cmd()
{
	std::string line;

	show_opts();

	std::getline(std::cin, line);
	log.debug("line: %s\n", line.c_str());
	return parse_cmd(line);
}
