#pragma once

#include <list>
#include <mutex>
#include <array>
#include <thread>

#include "common.h"

typedef std::list<Message_p> queue;

class ipc
{
protected:
	static std::array<
		std::mutex,
		static_cast<int>(common::ModuleID::MAX)
	> lockers;
	static std::array<
		queue,
		static_cast<int>(common::ModuleID::MAX)
	> queues_req, queues_rep;
	common::ModuleID id;

	int insert_msg(int _id, queue &q, Message_p msg);
public:
	ipc() = delete;
	ipc(common::ModuleID _id);
	~ipc();

	Message_p recv_rep(common::MessageID mid);
	Message_p recv_req(void);
	int send_rep(Message_p msg);
	int send_req(Message_p msg);
};

class _thread: protected ipc
{
public:
	_thread() = delete;
	_thread(common::ModuleID _id):
		ipc(_id)
			{}

	void operator()();
	virtual int _run() = 0;
	virtual int check_message() { return 0; }
	virtual bool maybe_exit() { return false; }
};
