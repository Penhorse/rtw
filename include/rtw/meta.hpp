#pragma once

namespace rtw
{

namespace meta
{

//
// FIELD
//
// you can use this macro like this if you want:
//``````````````````````````````````````````````````````````````````````````````
//	class Fart
//	{
//		FIELD(int, butt);
//	};
//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
//
// it's equivalent to this:
//``````````````````````````````````````````````````````````````````````````````
//	class Fart
//	{
//	
//	public:
//
//		int butt() const { return butt_; }
//
//	private:
//
//		int butt_;
//	
//	};
//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
//
#define FIELD(Type, Name)\
	public:\
		Type Name() const { return Name##_; }\
	private:\
		Type Name##_

//
// returns true if [range] contains [value], otherwise false
// i dunno why this is in meta.hpp
// it shouldn't be
//
template <class T, class Range> bool is_one_of(const T value, const Range & range)
{
	for(const auto item : list)
	{
		if(value == item) return true;
	}

	return false;
}
	
} // namespace meta
	
} // namespace rtw
