#pragma once

#include <algorithm>
#include <cctype>

namespace rtw
{
namespace strings
{

template <class Predicate>
std::vector<std::string> split(const std::string & s, Predicate p)
{
	std::vector<std::string> result;

	std::string token;

	for(const auto c : s)
	{
		if(p(c) && !token.empty())
		{
			result.push_back(token);
			token.clear();
		}
		else
		{
			token.push_back(c);
		}
	}

	if(!token.empty())
	{
		result.push_back(token);
	}

	return result;
}

void toupper(std::string * s)
{
	std::transform(s->begin(), s->end(), s->begin(), ::toupper);
}
	
std::string toupper(const std::string & s)
{
	std::string result(s);

	std::transform(s.begin(), s.end(), result.begin(), ::toupper);

	return result;
}
} // namespace strings
} // namespace rtw
