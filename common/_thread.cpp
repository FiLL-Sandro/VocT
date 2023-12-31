#include "_thread.h"

#define ModuleID2int(_mid) \
	static_cast<int>(_mid)

static Log log("_thread", Log::LogLevel::LL_DEBUG);

std::array<
	queue,
	static_cast<int>(common::ModuleID::MAX)
> ipc::queues_req, ipc::queues_rep;

std::array<
	std::mutex,
	static_cast<int>(common::ModuleID::MAX)
> ipc::lockers;

bool _thread::fatal = false;

ipc::ipc(common::ModuleID _id):
	id(_id)
		{}

ipc::~ipc()
{
	int _id = ModuleID2int(id);
	std::lock_guard<std::mutex> l(ipc::lockers[_id]);
	ipc::queues_rep[_id].clear();
	ipc::queues_req[_id].clear();
}

int ipc::insert_msg(int _id, queue &q, Message_p msg)
{
	std::lock_guard<std::mutex> l(ipc::lockers[_id]);
	q.push_back(msg);
	return 0;
}

int ipc::send_req(Message_p msg)
{
	if (!msg)
	{
		LOG_ERROR("attempt to send empty message\n");
		return -1;
	}

	int _id = ModuleID2int(msg->rid);
	if (!insert_msg(_id, ipc::queues_req[_id], msg))
	{
		log.info("send request to %s\n", common::ModuleID2str(msg->rid));
	}
	return 0;
}

Message_p ipc::send_req_sync(Message_p msg, common::MessageID reply_id)
{
	Message_p reply;

	if (send_req(msg))
	{
		LOG_ERROR("cannot send %s\n", common::MessageID2str(msg->id));
		goto fail;
	}

	reply = recv_rep(reply_id);
	if (!reply)
	{
		LOG_ERROR("%s aren't received\n", common::MessageID2str(reply_id));
		goto fail;
	}

	if (reply->status != common::CommandStatus::SUCCESS)
	{
		LOG_ERROR("%s processing are failed\n", common::MessageID2str(msg->id));
		reply.reset();
	}
fail:
	return reply;
}

int ipc::send_rep(Message_p msg)
{
	if (!msg)
	{
		LOG_ERROR("attempt to send empty message\n");
		return -1;
	}

	int _id = ModuleID2int(msg->sid);
	if (!insert_msg(_id, ipc::queues_rep[_id], msg))
	{
		log.info("send reply to %s\n", common::ModuleID2str(msg->sid));
	}
	return 0;
}

int ipc::send_rep_error(Message_p msg, common::MessageID reply_id)
{
	msg->id = reply_id;
	msg->status = common::CommandStatus::FAILED;
	return send_rep(msg);
}

Message_p ipc::recv_req(void)
{
	int _id = ModuleID2int(id);
	queue &q = ipc::queues_req[_id];
	std::lock_guard<std::mutex> l(ipc::lockers[_id]);
	Message_p p;

	if (!q.empty())
	{
		p = q.front();
		q.pop_front();

		log.info("receive request from %s\n", common::ModuleID2str(p->sid));
		LOG_DEBUG("%s queue size == %lu\n",
		          common::ModuleID2str(id), q.size());
	}
	return p;
}

Message_p ipc::recv_rep(common::MessageID mid)
{
	int _id = ModuleID2int(id);
	std::mutex &m = ipc::lockers[_id];
	Message_p p;

	for (int i = 0; i < 5; ++i)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
		if (m.try_lock())
		{
			queue &q = ipc::queues_rep[_id];
			auto it = std::find_if(q.begin(), q.end(),
			                       [mid](Message_p p) { return p->id == mid; }
			                       );

			if (*it)
			{
				p = *it;
				q.erase(it);

				log.info("receive reply from %s\n", common::ModuleID2str(p->rid));
				LOG_DEBUG("%s queue size == %lu\n",
				          common::ModuleID2str(id), q.size());
				m.unlock();
				break;
			}
			m.unlock();
		}
	}
	return p;
}

void _thread::operator()()
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
