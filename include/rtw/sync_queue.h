#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

#include <rtw/meta.hpp>

namespace rtw
{

//
// the result of a SyncQueue::pop operation
//
// if a consumer is waiting on pop and SyncQueue is killed, a 'dead'
// SyncQueueResult is returned
//
// clients should always check valid() or operator bool() before calling get()
//
template <class T>
class SyncQueueResult
{

public:

	SyncQueueResult() : valid_(false) {}
	SyncQueueResult(T && value) : valid_(true), value_(std::forward<T>(value)) {}
	SyncQueueResult(SyncQueueResult<T> && rhs) : valid_(rhs.valid_), value_(std::forward<T>(rhs.value_)) {}

	T & get()
	{
		if(!valid_)
		{
			throw std::runtime_error(ERR_SYNC_QUEUE_RESULT_INVALID_READ);
		}

		return value_;
	}

	operator bool() const { return valid_; }

	FIELD(bool, valid);

private:

	SyncQueueResult(const SyncQueueResult<T> &) = delete;

	T value_;

	static constexpr auto ERR_SYNC_QUEUE_RESULT_INVALID_READ =
		"a bad programmer tried to read a value popped from a synchronized "
		"queue without checking that it's valid"
		;

};

//
// clients should always call kill() before the SyncQueue object is destroyed
//
template <class T>
class SyncQueue
{

public:

	using Result = SyncQueueResult<T>;

	SyncQueue();
	
	void kill();
	void push(T && value);
	Result pop();

private:

	SyncQueue(const SyncQueue &);
	SyncQueue & operator=(const SyncQueue &);

	Result pop_successful_result();
	Result make_dead_result();

	std::condition_variable cond_;
	std::mutex              push_pop_mutex_;
	std::queue<T>           queue_;
	bool                    dying_;
	
};

template <class T> SyncQueue<T>::SyncQueue() :
	dying_(false)
{
	// nothing
}

//
// notifies all consumers waiting in pop() that the SyncQueue is
// dying. pop() will return a 'dead' result
//
template <class T> void SyncQueue<T>::kill()
{
	std::lock_guard<std::mutex> lock(push_pop_mutex_);

	dying_ = true;

	cond_.notify_all();
}

//
// push a value onto the queue and notify one consumer
//
template <class T> void SyncQueue<T>::push(T && value)
{
	std::lock_guard<std::mutex> push_lock(push_pop_mutex_);

	queue_.push(std::forward<T>(value));

	cond_.notify_one();
}

template <class T> auto SyncQueue<T>::pop() -> Result
{
	std::unique_lock<std::mutex> pop_lock(push_pop_mutex_);

	//
	// if the SyncQueue was already killed then just return a 'dead' result
	//
	if(dying_) return make_dead_result();

	while(queue_.empty())
	{
		//
		// wait for an item
		//
		cond_.wait(pop_lock);

		//
		// the SyncQueue started dying when we were waiting for an item. return
		// a 'dead' result
		//
		if(dying_) return make_dead_result();
	}

	return pop_successful_result();
}

template <class T> auto SyncQueue<T>::pop_successful_result() -> Result
{
	auto result = Result(std::move(queue_.front()));

	queue_.pop();
	
	return result;
}

template <class T> auto SyncQueue<T>::make_dead_result() -> Result
{
	return Result();
}
	
} // namespace rtw
