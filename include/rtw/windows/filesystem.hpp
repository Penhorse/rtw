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
	const auto attribs = GetFileAttributesA(path.c_str());
	
	return
		attribs != INVALID_FILE_ATTRIBUTES &&
		!(attribs & FILE_ATTRIBUTE_DIRECTORY);
}

inline bool is_directory(const std::string & path)
{
	const auto attribs = GetFileAttributesA(path.c_str());
	
	return (attribs & FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY;
}

inline bool make_directory(const std::string & path)
{
	return CreateDirectory(path.c_str(), nullptr) != 0;
}

inline std::deque<std::string> directory_contents(const std::string & path)
{
	WIN32_FIND_DATA find_data;
	
	const auto f = FindFirstFileA((path + "\\*").c_str(), &find_data);
	
	if(f == INVALID_HANDLE_VALUE) return std::deque<std::string>();
	
	std::deque<std::string> result;
	
	do
	{
		const auto file_name = find_data.cFileName;
		const auto full_path = path + '\\' + file_name;
		
		if(file_name[0] != '.')
		{
			result.push_back(full_path);
		}
	}
	while(FindNextFileA(f, &find_data));
	
	FindClose(f);
	
	return result;
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
