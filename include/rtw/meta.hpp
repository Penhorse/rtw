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
// this one's the same except the accessor returns a (const Type &)
//
#define FIELD_REF(Type, Name)\
	public:\
		const Type & Name() const { return Name##_; }\
	private:\
		Type Name##_

//
// NoCopy
//
// you know what this is
//``````````````````````````````````````````````````````````````````````````````
//	class Fart : private NoCopy
//	{
//		...
//	};
//,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
//
class NoCopy
{

protected:

	NoCopy() = default;
	NoCopy(const NoCopy &) = delete;
	NoCopy & operator=(const NoCopy &) = delete;
};

} // namespace meta
	
} // namespace rtw
