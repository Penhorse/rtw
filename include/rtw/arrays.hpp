#pragma once

namespace rtw
{

namespace arrays
{

template <class T, int Size> int length(T(&)[Size])
{
	return Size;
}
	
} // namespace arrays

} // namespace rtw
