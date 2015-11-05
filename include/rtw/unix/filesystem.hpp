#pragma once

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

#include <deque>
#include <string>

namespace rtw
{
namespace fs
{

static const char PATH_SEPARATORS[] = { '/' };

inline bool is_file(const std::string & path)
{
	struct stat st;

	return stat(path.c_str(), &st) == 0 && S_ISREG(st.st_mode);
}

inline bool is_directory(const std::string & path)
{
	struct stat st;

	return stat(path.c_str(), &st) == 0 && S_ISDIR(st.st_mode);
}

inline bool make_directory(const std::string & path)
{
	return mkdir(path.c_str(), S_IRWXU) == 0;
}

inline std::deque<std::string> directory_contents(const std::string & path)
{
	const auto dir = opendir(path.c_str());

	if(!dir) return std::deque<std::string>();

	struct dirent * ent;

	std::deque<std::string> result;

	while((ent = readdir(dir)) != nullptr)
	{
		const auto file_name = ent->d_name;
		const auto full_path = path + '/' + file_name;

		if(file_name[0] != '.')
		{
			result.push_back(full_path);
		}
	}

	closedir(dir);

	return result;
}

inline std::string absolute(const std::string & path)
{
	const auto real_path = realpath(path.c_str(), nullptr);

	if(!real_path) return "";

	const std::string result(real_path);

	free(real_path);

	return result;
}

} // namespace fs
} // namespace rtw
