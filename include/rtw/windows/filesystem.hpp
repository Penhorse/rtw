#pragma once

#include <string>

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

} // namespace fs
	
} // namespace rtw
