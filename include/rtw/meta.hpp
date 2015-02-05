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

} // namespace meta
	
} // namespace rtw
