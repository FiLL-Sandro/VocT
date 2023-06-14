#pragma once

#include <list>
#include <mutex>
#include <array>

#include "common.h"

typedef struct message_s
{
	message_s() = delete;
	message_s(common::ModuleID _sid, common::ModuleID _rid, size_t _size);
	~message_s();

	common::ModuleID sid, rid;
	size_t size;
	void *data;
} message_t;
typedef std::shared_ptr<message_s> message_ptr;
typedef std::list<message_ptr> queue;

class ipc
{
private:
	static std::array<
		std::mutex,
		static_cast<int>(common::ModuleID::MID_MAX)
	> lockers;
	static std::array<
		queue,
		static_cast<int>(common::ModuleID::MID_MAX)
	> queues;
	common::ModuleID id;

public:
	ipc() = delete;
	ipc(common::ModuleID _id);
	~ipc();

	message_ptr create_msg(common::ModuleID rid, size_t size);
	int send_msg(message_ptr msg);
	message_ptr recv_msg(void);
};

class _thread
{
public:
	virtual void operator()();
	virtual int _run() = 0;
	virtual bool maybe_exit() { return false; }
};

class _thread_with_ipc: public _thread, public ipc
{
public:
	_thread_with_ipc() = delete;
	_thread_with_ipc(common::ModuleID _id):
		ipc(_id)
			{}

	void operator()();
	int check_message();
	virtual int _check_message(message_ptr msg) = 0;
};
