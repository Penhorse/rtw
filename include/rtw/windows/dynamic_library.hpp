#pragma once

#include <string>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

namespace rtw
{
namespace dylib
{
	
inline std::string get_filename(const std::string & name)
{
	return name + ".dll";
}

inline void close(void * library)
{
	FreeLibrary((HMODULE)library);
}

inline void * load(const std::string & path)
{
	return LoadLibrary(path.c_str());
}

inline void * get_function(void * library, const std::string & name)
{
	return GetProcAddress((HMODULE)library, name.c_str());
}

} // namespace dylib
} // namespace rtw