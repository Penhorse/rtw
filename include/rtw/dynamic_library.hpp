#pragma once

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

DynamicLibrary::DynamicLibrary(const std::string & name) :
	name_(name),
	library_(nullptr)
{
	// nothing
}

bool DynamicLibrary::load(const std::deque<std::string> & search_paths)
{
	if(library_)
	{
		close(library_);
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

	return (library_ = load(name_)) != nullptr;
}

bool DynamicLibrary::loaded() const
{
	return library_;
}

DynamicLibrary::~DynamicLibrary()
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