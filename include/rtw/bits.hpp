#pragma once

namespace rtw
{

namespace bits
{

inline bool flags_set(long mask, long flags)
{
	return ((mask & flags) == flags);
}

template <class T> long make_mask(T v0)
{
	return long(v0);
}

template <class T, class ...Args> long make_mask(T v0, Args... v)
{
	return make_bitmask(v0) | make_bitmask(v...);
}
	
} // namespace bits

} // namespace rtw
