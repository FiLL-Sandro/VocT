#include "_thread.h"

static Log log("_thread", Log::LogLevel::LL_DEBUG);

std::array<
	queue,
	static_cast<int>(common::ModuleID::MID_MAX)
> ipc::queues;

std::array<
	std::mutex,
	static_cast<int>(common::ModuleID::MID_MAX)
> ipc::lockers;

message_s::message_s(common::ModuleID _sid, common::ModuleID _rid, size_t _size):
	sid(_sid), rid(_rid), size(_size)
{
	data = new char[size];
	if (!data)
	{
		LOG_ERROR("cannot allocate memory for data\n");
		return;
	}
}

message_s::~message_s()
{
	if (data)
	{
		LOG_DEBUG("clean data of message\n");
		delete data;
	}
}

ipc::ipc(common::ModuleID _id):
	id(_id)
		{}

ipc::~ipc()
{
	int _id = static_cast<int>(id);
	std::lock_guard<std::mutex> l(ipc::lockers[_id]);
	queue &q = ipc::queues[_id];

	q.clear();
}

message_ptr ipc::create_msg(common::ModuleID rid, size_t size)
{
	message_ptr p;

	try
	{
		p = std::make_shared<message_s>(id, rid, size);
	}
	catch (...)
	{
		LOG_ERROR("cannot allocate memory for message_s\n");
	}
	return p;
}

int ipc::send_msg(message_ptr msg)
{
	if (!msg)
	{
		LOG_ERROR("attmpt to send empty message\n");
		return -1;
	}

	int _id = static_cast<int>(msg->rid);
	std::lock_guard<std::mutex> l(ipc::lockers[_id]);
	queue &q = ipc::queues[_id];

	log.info("send message to %s\n", common::ModuleID2str(msg->rid).c_str());

	q.push_back(msg);
	return 0;
}

message_ptr ipc::recv_msg(void)
{
	int _id = static_cast<int>(id);
	std::lock_guard<std::mutex> l(ipc::lockers[_id]);
	queue &q = ipc::queues[_id];
	message_ptr p;

	if (!q.empty())
	{
		log.info("receive message from %s\n", common::ModuleID2str(q.front()->sid).c_str());
		LOG_DEBUG("%s queue size == %lu\n",
		          common::ModuleID2str(id).c_str(), q.size());

		p = q.front();
		q.pop_front();
	}
	return p;
}

void _thread::operator()()
{
	int rc = 0;

	while (!rc && !maybe_exit())
	{
		if (!rc)
		{
			rc = _run();
		}
	}
}

int _thread_with_ipc::check_message()
{
	message_ptr msg{recv_msg()};

	if (msg)
		return _check_message(msg);
	return 0;
}

void _thread_with_ipc::operator()()
{
	int rc = 0;

	while (!rc && !maybe_exit())
	{
		rc = check_message();
		if (!rc)
		{
			rc = _run();
		}
	}
}
