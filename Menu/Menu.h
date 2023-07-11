#pragma once

#include <utility>
#include <functional>
#include <memory>

#include <common.h>
#include <_thread.h>

typedef std::pair< std::string, std::function<Message_p(std::vector<std::string>&)> > Opt;
typedef std::map< std::string, std::function<Message_p(std::vector<std::string>&)> > Opts;

/**
 * Данный класс реализует парсинг и валидацию команд.
 * Отсюда будут отправляться команды в Dictionary
 */
class Menu: public _thread
{
private:
	static Opts opts;
	bool are_showed;

	void show_opts(void);
	Message_p parse_cmd(const std::string &line);
public:
	Menu():
		_thread(common::ModuleID::MENU),
		are_showed(false)
			{}

	int _run();
	int check_message();
};
