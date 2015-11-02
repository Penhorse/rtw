#pragma once

#include <dlfcn.h>
#include <string>

namespace rtw
{
namespace dylib
{

inline std::string get_filename(const std::string & name)
{
	return std::string("lib") + name + ".so";
}

inline void close(void * library)
{
	dlclose(library);
}

inline void * load(const std::string & path)
{
	return dlopen(path.c_str(), RTLD_LAZY);
}

inline void * get_function(void * library, const std::string & name)
{
	return dlsym(library, name.c_str());
}

} // namespace dylib
} // namespace rtw
