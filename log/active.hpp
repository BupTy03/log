#ifndef ACTIVE_HPP
#define ACTIVE_HPP

#include <queue>
#include <functional>
#include <thread>
#include <mutex>

/*
	active class represents a worker that takes the messages(functors), 
	puts them in a queue and processes in FIFO order in separate thread
*/
class active
{
public:
	// std::queue will be replaced by any lockfree queue in the future
	// and we don't have to use std::mutex to syncronise access to it
	template<class F>
	using message_queue = std::queue<F>;
	using message = std::function<void()>;

	explicit active()
		: separateThread_{ 
		[this] {
			while (!done_) 
			{
				if (!messageQueue_.empty())
				{
					message msg;
					{
						std::lock_guard<std::mutex> lock(mtx_);
						msg = messageQueue_.front();
						messageQueue_.pop();
					}
					if (msg) msg();
				}
			}
		}
	}{}
	~active()
	{
		send([this] { done_ = true; });
		separateThread_.join();
	}

	void send(message m) 
	{ 
		std::lock_guard<std::mutex> lock(mtx_);
		messageQueue_.push(std::move(m)); 
	}

private:
	bool done_{ false };
	message_queue<message> messageQueue_;
	std::thread separateThread_;
	std::mutex mtx_;
};

#endif // !ACTIVE_HPP