#pragma once

#include <future>

namespace rtw
{

template <class Function, class... Args>
using future_of = std::future<typename std::result_of<Function(Args...)>::type>;
	
} // namespace rtw
