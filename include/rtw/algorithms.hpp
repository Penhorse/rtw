#pragma once

#include <algorithm>
#include <map>
#include <vector>

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
		const typename Container::value_type & item)
{
	return std::find(c.begin(), c.end(), item) != c.end();
}

//
// returns true if map [c] contains [key]
//
template <class Key, class Value>
bool contains(
		const std::map<Key, Value> & c,
		const Key & key)
{
	return c.find(key) != c.end();
}

//
// it works on arrays too #wow #omg
//
template <class T, int Size>
bool contains(const T(&c)[Size], T item)
{
	return std::find(std::begin(c), std::end(c), item) != std::end(c);
}

template <class T>
std::vector<T> get_last_n_elements(const std::vector<T> & c, size_t n)
{
	return std::vector<T>(c.end() - std::min(c.size(), n), c.end());
}

} // namespace algo
	
} // namespace rtw
