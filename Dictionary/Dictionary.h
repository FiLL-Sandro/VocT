#pragma once

#include <common.h>
#include <_thread.h>

#include "cJSON.h"
#include "Db.h"

class Dictionary: public _thread
{
private:
	std::string filepath;
	Db db;

	int parse_dictionary(std::ifstream &dictionary);
	void save_dictionary();
	static char* read_file(const std::string &filepath, size_t &size);

	void open_handler(CommonCommandMessage_p p, Message_p reply);
	void close_handler(Message_p reply);
	void add_handler(CommandAddMessage_p p, Message_p reply);
	void rem_handler(CommandRemMessage_p p, Message_p reply);
	void dump_handler(Message_p reply);

public:
	Dictionary():
		_thread(common::ModuleID::DICTIONARY)
			{}

	int _run();
	int check_message();
	// virtual bool maybe_exit() { return false; }
};
