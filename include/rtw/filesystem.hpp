#pragma once

#if defined(_WIN32)

#include "windows/filesystem.hpp"

#elif defined(__linux__)

#include "linux/filesystem.hpp"

#else

#error(platform not supported yet)

#endif

#include <algorithm>
#include <string>

#include "meta.hpp"

namespace rtw
{

namespace fs
{

inline std::string extension_of(const std::string & path)
{
	const auto last_dot = path.rfind('.');

	return
		last_dot != std::string::npos
			? path.substr(last_dot + 1)
			: std::string();
}

inline bool is_path_separator(const char c)
{
	return meta::is_one_of(c, PATH_SEPARATORS);
}
	
inline std::string file_name(const std::string & path)
{
	const auto last_delim =
		std::find_if(path.rbegin(), path.rend(), is_path_separator);

	return std::string(last_delim.base(), path.end());
}

inline char path_separator()
{
	return PATH_SEPARATORS[0];
}

inline std::string join(const std::string & path)
{
	return path;
}

template <class ...Args> std::string join(const std::string & path, Args... tail)
{
	return
		path.empty()
			? join(tail...)
			: path + path_separator() + join(tail...);
}

} // namespace fs
	
} // namespace rtw
