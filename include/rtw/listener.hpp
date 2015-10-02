#pragma once

namespace rtw
{

template <class T> class Listenable;

template <class T>
class Listener
{

private:

	virtual void notify(const T & msg) = 0;

friend class Listenable<T>;
	
};
	
} // namespace rtw
