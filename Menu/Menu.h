#pragma once

#include <utility>
#include <functional>
#include <memory>

#include <common.h>

class MenuOpt
{
public:
	std::string name;
	std::vector<std::string> args;

	MenuOpt(const std::vector<std::string> &_args)
	{
		if (!_args.empty())
		{
			name = _args.front();
			args.insert(args.begin(), _args.begin() + 1, _args.end());
		}
	}

	void dump(void)
	{
		printf("command %s:\n", name.c_str());
		printf("args: ");
		for (auto i : args)
		{
			printf("%s ", i.c_str());
		}
		printf("\n");
	}
};

class MenuImpl
{
public:
	virtual int _run(MenuOpt &opt) = 0;
	virtual std::optional<MenuOpt> _get_cmd() { return std::optional<MenuOpt>(); };
};

class Menu
{
protected:
	std::shared_ptr<MenuImpl> bridge;

	inline std::optional<MenuOpt> get_cmd(void)
		{ return bridge->_get_cmd(); }
public:
	Menu() = delete;
	Menu(std::shared_ptr<MenuImpl> br):
		bridge(br)
			{}

	int run(void);
};
