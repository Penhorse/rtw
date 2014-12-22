#pragma once

namespace rtw
{

namespace meta
{

#define FIELD(Type, Name)\
	public:\
		Type Name() const { return Name##_; }\
	private:\
		Type Name##_

template <class T, class U> bool is_one_of(const T value, const U & list)
{
	for(const auto item : list)
	{
		if(value == item) return true;
	}

	return false;
}
	
} // namespace meta
	
} // namespace rtw
