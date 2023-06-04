#include "Menu.h"

static Log log("Menu", Log::LogLevel::LL_DEBUG);

int Menu::run(void)
{
	std::optional<MenuOpt> o = get_cmd();
	int rc = 0;

	if (o)
	{
		log.debug("command dump\n");
		o.value().dump();

		rc = bridge->_run(o.value());
	}
	else
	{
		log.error("unknown command\n");
	}

	return rc;
}
#if 0
void Menu::print()
{
	int i = 0;

	for (auto it = leafs.begin(); it != leafs.end(); ++it)
	{
		wprintf(L"%d. %s\n", i++, it->first.c_str());
	}
}

void Menu::handle(std::string cmd)
{
	auto it = leafs.find(cmd);
	if (it != leafs.end())
	{
		wprintf(L"cmd == %s\n", it->first.c_str());
		it->second(NULL);
	}
}
#endif
