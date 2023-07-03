#pragma once

#include <common.h>
#include <_thread.h>

/**
 * Класс Receiver необходим для обеспечения забора данных от пользователя
 */
class Receiver: public _thread
{
public:
	Receiver():
		_thread(common::ModuleID::RECEIVER)
			{}
	// ~Receiver();

	int _run();
	// int check_message();
	// virtual bool maybe_exit() { return false; }
};
