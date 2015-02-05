#pragma once

namespace rtw
{

namespace algo
{

//
// returns true if container [c] contains [item]
//
template <class Container>
bool contains(
		const Container & c,
		const typename decltype(c.begin())::value_type & item)
{
	return std::find(c.begin(), c.end(), item) != c.end();
}

//
// it works on arrays too #wow #omg
//
template <class T, int Size>
bool contains(const T(&c)[Size], T item)
{
	return std::find(std::begin(c), std::end(c), item) != std::end(c);
}
	
} // namespace algo
	
} // namespace rtw
