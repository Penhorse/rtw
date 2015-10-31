#pragma once

#include <dlfcn.h>
#include <string>

namespace rtw
{
namespace dylib
{

std::string get_filename(const std::string & name)
{
	return std::string("lib") + name + ".so";
}

void close(void * library)
{
	dlclose(library);
}

void * DynamicLibrary::load(const std::string & path)
{
	return dlopen(path.c_str(), RTLD_LAZY);
}

void * DynamicLibrary::get_function(void * library, const std::string & name)
{
	return dlsym(library, name.c_str());
}

}