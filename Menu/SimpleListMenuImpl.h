#pragma once

#include "Menu.h"

typedef std::pair< std::string, std::function<int(MenuOpt&)> > Opt;
typedef std::map< std::string, std::function<int(MenuOpt&)> > Opts;

class SimpleListMenuImpl: public MenuImpl
{
private:
/*
TODO:
* константная ссылка
*/
	Opts opts;

	std::optional<MenuOpt> parse_cmd(const std::string &line);
	void show_opts(void);
public:
	SimpleListMenuImpl() = delete;
	SimpleListMenuImpl(const Opts& _opts):
		opts(_opts)
			{}

	int _run(MenuOpt &opt);
	std::optional<MenuOpt> _get_cmd();
};
