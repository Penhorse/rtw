#pragma once

#include <future>

namespace rtw
{

template <class Function, class... Args>
using future_of = std::future<typename std::result_of<Function(Args...)>::type>;

template <class T>
bool is_ready(const std::future<T> & f)
{
	return f.wait_for(std::chrono::seconds(0)) == std::future_status::ready;
}
	
} // namespace rtw
