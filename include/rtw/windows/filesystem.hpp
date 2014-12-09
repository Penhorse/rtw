#pragma once

#include <deque>
#include <string>

#include <windows.h>

#include "rtw/error.hpp"

namespace rtw
{

namespace fs
{

static const char PATH_SEPARATORS[] = { '/', '\\' };

inline bool is_file(const std::string & path)
{
	RTW_NOT_IMPLEMENTED;
}

inline bool is_directory(const std::string & path)
{
	RTW_NOT_IMPLEMENTED;
}

inline bool make_directory(const std::string & path)
{
	return CreateDirectory(path.c_str(), nullptr) != 0;
}

inline std::deque<std::string> directory_contents(const std::string & path)
{
	RTW_NOT_IMPLEMENTED;
}

inline std::string absolute(const std::string & path)
{
	const auto length = GetFullPathNameA(path.c_str(), 0, nullptr, nullptr);
	
	if(length < 1) return "";
	
	char * const buffer = new char[length + 1];
	
	GetFullPathNameA(path.c_str(), length + 1, buffer, nullptr);

	const std::string result(buffer);

	delete[] buffer;

	return result;
}

} // namespace fs
	
} // namespace rtw
