#pragma once

#if defined(_WIN32)

#include "windows/dynamic_library.hpp"

#elif defined(__linux__) || defined(__APPLE__)

#include "linux/dynamic_library.hpp"

#endif

#include <deque>
#include <functional>
#include <string>

namespace rtw
{
	
class DynamicLibrary
{

public:

	DynamicLibrary(const std::string & name);
	~DynamicLibrary();

	bool load(const std::deque<std::string> & search_paths = {});

	template <class Signature>
	std::function<Signature> get_function(const std::string & name) const;

	bool loaded() const;

private:

	std::string name_;
	void *      library_;

};

inline DynamicLibrary::DynamicLibrary(const std::string & name) :
	name_(name),
	library_(nullptr)
{
	// nothing
}

inline bool DynamicLibrary::load(const std::deque<std::string> & search_paths)
{
	if(library_)
	{
		dylib::close(library_);
	}

	if(!search_paths.empty())
	{
		for(const auto & search_path : search_paths)
		{
			const auto full_path = search_path + "/" + name_;

			library_ = dylib::load(full_path);

			if(library_) return true;
		}
	}

	return (library_ = dylib::load(name_)) != nullptr;
}

inline bool DynamicLibrary::loaded() const
{
	return library_;
}

inline DynamicLibrary::~DynamicLibrary()
{
	if(library_)
	{
		dylib::close(library_);
	}
}

template <class Signature>
std::function<Signature> DynamicLibrary::get_function(const std::string & name) const
{
	return library_ ? reinterpret_cast<Signature*>(dylib::get_function(library_, name)) : nullptr;
}

} // namespace rtw