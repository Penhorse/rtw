#pragma once

#include "../unix/dynamic_library.hpp"

namespace rtw
{
namespace dylib
{

inline std::string get_filename(const std::string & name)
{
	return std::string("lib") + name + ".dylib";
}

} // namespace dylib
} // namespace rtw
