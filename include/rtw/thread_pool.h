#pragma once

#include <rtw/sync_queue.h>

#include "future_util.h"

namespace rtw
{

//
// this is based on tyler-hardin/thread_pool except i rewrote it to be more
// terrible
//
class ThreadPool
{

public:

	ThreadPool(int num_threads);
	~ThreadPool();

	void join();

	template <class Function, class... Args>
	future_of<Function, Args...>
	async(Function f, Args ... args)
	{
		using Result = typename std::result_of<Function(Args...)>::type;

		const auto ready = new std::atomic<bool>(false);
		const auto p     = new std::promise<Result>();

		//
		// this is the function we actually push onto the queue
		//
		// it calls the client's function, stores the result in the promise
		// and notifies the result wrapper that the result is ready
		//
		auto task_wrapper =
			[p, ready](Function f, Args ... args)
			{
				p->set_value(f(args...));

				ready->store(true);
			};

		//
		// this guy waits for a notification that the client's function has been
		// run and the result is ready, then returns the result
		//
		auto result_wrapper =
			[p, ready]() -> Result
			{
				while(!ready->load()) std::this_thread::yield();

				auto result = p->get_future().get();

				delete p;
				delete ready;

				return result;
			};

		tasks_->push(
				std::async(
					std::launch::deferred,
					task_wrapper,
					f,
					args...));


		return std::async(std::launch::deferred, result_wrapper);
	}

private:

	void thread_func();
	static void s_thread_func(ThreadPool * pool);

	using TaskQueue    = SyncQueue<std::future<void>>;
	using TaskQueuePtr = std::unique_ptr<TaskQueue>;

	TaskQueuePtr            tasks_;
	std::deque<std::thread> threads_;

};

inline ThreadPool::ThreadPool(int num_threads) :
	tasks_(TaskQueuePtr(new TaskQueue()))
{
	for(int i = 0; i < num_threads; i++)
	{
		threads_.emplace_back(&s_thread_func, this);
	}
}

inline ThreadPool::~ThreadPool()
{
	tasks_->kill();
	
	join();
}

//
// wait for all tasks to finish
//
inline void ThreadPool::join()
{
	for(auto & t : threads_)
	{
		t.join();
	}
}

inline void ThreadPool::thread_func()
{
	for(;;)
	{
		auto task = tasks_->pop();

		if(!task) return;

		task.get().get();
	}
}

inline void ThreadPool::s_thread_func(ThreadPool * const pool)
{
	pool->thread_func();
}

} // namespace rtw
