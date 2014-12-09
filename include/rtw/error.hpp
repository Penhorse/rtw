#pragma once

#include <functional>
#include <iostream>
#include <string>
#include <stdexcept>

#if defined(_WIN32)
#define __func__ __FUNCTION__
#endif

#define RTW_NOT_IMPLEMENTED\
	throw std::runtime_error(std::string("not implemented: ") + __func__);

namespace rtw
{

namespace error
{

template <class T, T success, T failure>
inline T basic_pokemon(std::function<void()> func)
{
	try
	{
		func();

		return success;
	}
	catch(const std::runtime_error & e)
	{
		std::cerr << "fatal error: " << e.what() << std::endl;

		return failure;
	}
	catch(...)
	{
		std::cerr << "fatal error: unknown exception" << std::endl;

		return failure;
	}
}

inline int pokemon(std::function<void()> func)
{
	return basic_pokemon<int, EXIT_SUCCESS, EXIT_FAILURE>(func);
}

inline std::string failed_to_do_x(const std::string x)
{
	return std::string("failed to ") + x;
}

inline std::string failed_to_do_x_with_y(const std::string x, const std::string y)
{
	return failed_to_do_x(x) + " '" + y + "'";
}
	
} // namespace error

} // namespace rtw
