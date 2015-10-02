#pragma once

#include <deque>

#include <rtw/listener.hpp>

namespace rtw
{

template <class T>
class Listenable
{

public:

	void register_listener(std::weak_ptr<Listener<T>> listener)
	{
		listeners_.insert(listeners_.end(), listener);
	}

protected:

	void notify(const T & msg) const
	{
		for(const auto & listener : listeners_)
		{
			if(!listener.expired())
			{
				listener.lock()->notify(msg);
			}
		}
	}

private:

	std::deque<std::weak_ptr<Listener<T>>> listeners_;
};
	
} // namespace rtw
