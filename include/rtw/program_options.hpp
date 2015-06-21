#pragma once

#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#include <rtw/spell_correct.hpp>

namespace rtw
{
	
/*

runtime usage example:
--------------------------------------------------------------------------------
./path/to/program --path test.txt --something arg1 arg2 arg3 --foo -b

````````````````````````````````````````````````````````````````````````````````

setup:
--------------------------------------------------------------------------------
	// argv[0] is required for print_usage() to work properly
	ProgramOptions::Desc desc(argv[0]);
 
	// the first two parameters specify the minimum and maximum number of
	// allowed values for the option (0 for no limit)
	//
	// the last parameter specifies whether or not the option is optional
	// (true) or required (false). default is false
	desc.add_value(1, 1, "path", 'p', "file path", false);
	desc.add_value(1, 0, "something", "something else", false);

	// only the given values are allowed. if the user supplies anything else an
	// error is generated
	desc.add_switch(1, 1, "version", 'v', { "1", "2", "3" }, "file version");

	// flags are options which don't expect any values
	desc.add_flag("foo", "foo description");

	// you can also specify options like this:
	const auto option_bar =
		ProgramOptions::Desc::Option::make_flag("bar", "bar description");

	// and supply the short-form character here:
	desc.add_option(option_bar, 'b');
 
	ProgramOptions options(desc);
	 
	ProgramOptions::Result result;
 
	// parse the command line
	auto error = options.parse(argc, argv, &result);
 
	if(error)
	{
		// handle error
	}

	// check that all the non-optional options were supplied
	error = desc.check_required_options(result);

	if(error)
	{
		// handle error
	}
 
````````````````````````````````````````````````````````````````````````````````
 
printing the program usage:
--------------------------------------------------------------------------------
	desc.print_usage();
 
output: ------------------------------------------------------------------------
	./path/to/program --path <file path> --something <something else> --version
	<1|2|3> --foo <foo description> --bar <bar description>
 
````````````````````````````````````````````````````````````````````````````````
 
printing program help:
--------------------------------------------------------------------------------
	desc.print_help();

output: ------------------------------------------------------------------------
	./path/to/program --path <file path> --something <something else> --version
	<1|2|3> --foo <foo description> --bar <bar description>

	   --path, -p        file path
	  --something   something else
	--version, -v     file version
	        --foo  foo description
	    --bar, -b  bar description

````````````````````````````````````````````````````````````````````````````````

using results:
--------------------------------------------------------------------------------
	const auto file_path = result.options["path"][0];
	const auto foo       = result.has_flag("foo");
	
````````````````````````````````````````````````````````````````````````````````

*/
class ProgramOptions
{

public:

	struct Error
	{
		std::string desc;

		operator bool() const { return !desc.empty(); }

		static Error unexpected_x_y(const std::string & x, const std::string & y);
		static Error unexpected_param(const std::string & param);
		static Error unknown_option(const std::string & option, const std::string & did_you_mean = std::string());
		static Error invalid_switch_value(
				const std::string & desc,
				const std::string & value,
				const std::set<std::string> & allowed_values);
		static Error missing_param(const std::string & option);
		static Error not_enough_values(const std::string & option, int min_values, int values_given);
		static Error option_is_required(const std::string & option);
		static Error unexpected_option(const std::string & option);
	};

	enum class OptionType
	{
		Flag,
		Value,
		Switch,
	};
	
	struct Result
	{
		std::set<std::string> flags;
		std::map<std::string, std::vector<std::string>> options;

		bool has_flag(const std::string & key) const;
		bool has_option(const std::string & key) const;
		bool exists(OptionType type, const std::string & key) const;
	};

	class Desc
	{

	public:

		struct Option
		{
			OptionType type;
			bool optional;
			int min_values;
			int max_values;
			std::string key;
			std::set<std::string> allowed_values;
			std::string desc;

			bool operator<(const Option & rhs) const;
			bool value_is_allowed(const std::string & value) const;

			static Option make_flag(const std::string & key, const std::string & desc);
			static Option make_value(int min_values, int max_values, const std::string & key, const std::string & desc, bool optional = false);
			static Option make_switch(int min_values, int max_values, const std::string & key, const std::set<std::string> & allowed_values, const std::string & desc, bool optional = false);

			Error invalid_switch_value(const std::string & value) const;
		};

		Desc(const char * const argv0);

		void add_flag(const std::string & long_key, const std::string & desc);
		void add_flag(const std::string & long_key, char short_key, const std::string & desc);
		void add_value(int min_values, int max_values, const std::string & long_key, const std::string & desc, bool optional = false);
		void add_value(int min_values, int max_values, const std::string & long_key, char short_key, const std::string & desc, bool optional = false);
		void add_switch(int min_values, int max_values, const std::string & long_key, const std::set<std::string> & allowed_values, const std::string & desc, bool optional = false);
		void add_switch(int min_values, int max_values, const std::string & long_key, char short_key, const std::set<std::string> & allowed_values, const std::string & desc, bool optional = false);
		void add_option(const Option & option);
		void add_option(const Option & option, char short_key);

		bool find(std::string key, Option * option) const;
		bool find_suggestion(const std::string & key, Option * option, int timeout) const;
		Error check_required_options(const Result & result) const;

		void print_usage(std::ostream & os = std::cout) const;
		void print_help(std::ostream & os = std::cout) const;

	private:

		using OptionSet   = std::set<Option>;
		using ShortKeyMap = std::map<char, std::string>;

		std::string argv0_;
		OptionSet   options_;
		ShortKeyMap short_key_map_;

	};

	ProgramOptions(const Desc & desc, int did_you_mean_timeout = SpellCorrect::DEFAULT_SEARCH_TIMEOUT);

	Error parse(int argc, const char * argv[], Result * result) const;

private:

	Error parse_key(const char * const argv, std::string * key) const;

	enum class ParserState
	{
		ParseKey,
		ParseValue,
	};

	Desc desc_;
	int  did_you_mean_timeout_;

};

inline auto ProgramOptions::Error::unexpected_x_y(const std::string & x, const std::string & y) -> Error
{
	return { std::string("unexpected ") + x + ": '" + y + "'" };
}

inline auto ProgramOptions::Error::unexpected_param(const std::string & param) -> Error
{
	return unexpected_x_y("parameter", param);
}

inline auto ProgramOptions::Error::unknown_option(const std::string & option, const std::string & did_you_mean) -> Error
{
	std::stringstream ss;

	ss << "unknown option specified: '" << option << "'";

	if(!did_you_mean.empty())
	{
		ss << " (did you mean '--" << did_you_mean << "'?)";
	}

	return { ss.str() };
}

inline auto ProgramOptions::Error::invalid_switch_value(
		const std::string & desc,
		const std::string & value,
		const std::set<std::string> & allowed_values) -> Error
{
	auto result = std::string("invalid ") + desc + ": " + value + " (should be ";

	size_t counter = 0;

	for(const auto v : allowed_values)
	{
		result += v;

		if(++counter < allowed_values.size())
		{
			result += counter < allowed_values.size() - 1 ? ", " : " or ";
		}
	}

	result += ")";

	return { result };
}

inline auto ProgramOptions::Error::missing_param(const std::string & option) -> Error
{
	return { std::string("parameter not specified for option '") + option + "'" };
}

inline auto ProgramOptions::Error::not_enough_values(const std::string & option, int min_values, int values_given) -> Error
{
	std::stringstream ss;

	ss << "not enough values given for option '" << option << "' (requires " << min_values << ", given " << values_given << ")";

	return { ss.str() };
}

inline auto ProgramOptions::Error::option_is_required(const std::string & option) -> Error
{
	return { std::string("--") + option + " is required" };
}

inline auto ProgramOptions::Error::unexpected_option(const std::string & option) -> Error
{
	return unexpected_x_y("option", option);
}

inline bool ProgramOptions::Result::has_flag(const std::string & key) const
{
	return flags.find(key) != flags.end();
}

inline bool ProgramOptions::Result::has_option(const std::string & key) const
{
	return options.find(key) != options.end();
}

inline bool ProgramOptions::Result::exists(OptionType type, const std::string & key) const
{
	switch(type)
	{
		case OptionType::Flag:
		{
			return has_flag(key);
		}
		case OptionType::Value:
		case OptionType::Switch:
		{
			return has_option(key);
		}
		default:
		{
			return false;
		}
	}
}

inline bool ProgramOptions::Desc::Option::operator<(const Option & rhs) const
{
	if (type < rhs.type) return true;
	if (type > rhs.type) return false;

	if (optional < rhs.optional) return true;
	if (optional > rhs.optional) return false;

	if (min_values < rhs.min_values) return true;
	if (min_values > rhs.min_values) return false;

	if (max_values < rhs.max_values) return true;
	if (max_values > rhs.max_values) return false;

	if (key < rhs.key) return true;
	if (key > rhs.key) return false;

	if (allowed_values < rhs.allowed_values) return true;
	if (allowed_values > rhs.allowed_values) return false;

	if (desc < rhs.desc) return true;
	if (desc > rhs.desc) return false;

	return false;
}

inline bool ProgramOptions::Desc::Option::value_is_allowed(const std::string & value) const
{
	for(const auto & allowed_value : allowed_values)
	{
		if(value.compare(0, allowed_value.size(), value) == 0) return true;
	}

	return false;
}

inline auto ProgramOptions::Desc::Option::make_flag(const std::string & key, const std::string & desc) -> Option
{
	return { OptionType::Flag, true, 0, 0, key, {}, desc };
}

inline auto ProgramOptions::Desc::Option::make_value(int min_values, int max_values, const std::string & key, const std::string & desc, bool optional) -> Option
{
	return { OptionType::Value, optional, min_values, max_values, key, {}, desc };
}

inline auto ProgramOptions::Desc::Option::make_switch(int min_values, int max_values, const std::string & key, const std::set<std::string> & allowed_values, const std::string & desc, bool optional) -> Option
{
	return { OptionType::Switch, optional, min_values, max_values, key, allowed_values, desc };
}

inline auto ProgramOptions::Desc::Option::invalid_switch_value(const std::string & value) const -> Error
{
	return Error::invalid_switch_value(desc, value, allowed_values);
}

inline ProgramOptions::Desc::Desc(const char * const argv0) :
	argv0_(argv0)
{
	// nothing
}

inline void ProgramOptions::Desc::add_flag(const std::string & long_key, const std::string & desc)
{
	options_.insert({ OptionType::Flag, true, 0, 0, long_key, {}, desc });
}

inline void ProgramOptions::Desc::add_flag(const std::string & long_key, char short_key, const std::string & desc)
{
	add_flag(long_key, desc);

	short_key_map_[short_key] = long_key;
}

inline void ProgramOptions::Desc::add_value(int min_values, int max_values, const std::string & long_key, const std::string & desc, bool optional)
{
	options_.insert({ OptionType::Value, optional, min_values, max_values, long_key, {}, desc });
}

inline void ProgramOptions::Desc::add_value(int min_values, int max_values, const std::string & long_key, char short_key, const std::string & desc, bool optional)
{
	add_value(min_values, max_values, long_key, desc, optional);

	short_key_map_[short_key] = long_key;
}

inline void ProgramOptions::Desc::add_switch(int min_values, int max_values, const std::string & long_key, const std::set<std::string> & allowed_values, const std::string & desc, bool optional)
{
	options_.insert({ OptionType::Switch, optional, min_values, max_values, long_key, allowed_values, desc });
}

inline void ProgramOptions::Desc::add_switch(int min_values, int max_values, const std::string & long_key, char short_key, const std::set<std::string> & allowed_values, const std::string & desc, bool optional)
{
	add_switch(min_values, max_values, long_key, allowed_values, desc, optional);

	short_key_map_[short_key] = long_key;
}

inline void ProgramOptions::Desc::add_option(const Option & option)
{
	options_.insert(option);
}

inline void ProgramOptions::Desc::add_option(const Option & option, char short_key)
{
	add_option(option);

	short_key_map_[short_key] = option.key;
}

inline bool ProgramOptions::Desc::find(std::string key, Option * option) const
{
	if(key.size() == 1)
	{
		const auto short_key = short_key_map_.find(key[0]);

		if(short_key != short_key_map_.end())
		{
			key = short_key->second;
		}
	}

	for(const auto & opt : options_)
	{
		if(opt.key == key)
		{
			*option = opt;

			return true;
		}
	}

	return false;
}

inline bool ProgramOptions::Desc::find_suggestion(
		const std::string & key,
		Option * option,
		int timeout) const
{
	SpellCorrect::Dictionary dictionary;

	for(const auto & opt : options_)
	{
		dictionary.insert(opt.key);
	}

	const auto suggestion = SpellCorrect(dictionary, timeout).get_one_correction(key);

	if(suggestion.empty()) return false;

	for(const auto & opt : options_)
	{
		if(opt.key == suggestion)
		{
			*option = opt;

			return true;
		}
	}

	return false;
}

inline auto ProgramOptions::Desc::check_required_options(const Result & result) const -> Error
{
	for(const auto opt : options_)
	{
		if(!opt.optional)
		{
			if(!result.exists(opt.type, opt.key))
			{
				return Error::option_is_required(opt.key);
			}
		}
	}

	return Error();
}

inline ProgramOptions::ProgramOptions(const Desc & desc, int did_you_mean_timeout) :
	desc_(desc),
	did_you_mean_timeout_(did_you_mean_timeout)
{
	// nothing
}

inline void ProgramOptions::Desc::print_usage(std::ostream & os) const
{
	os << "usage: " << argv0_;

	for(const auto & opt : options_)
	{
		os << " ";

		if(opt.optional) os << "[";
	
		os << "--" << opt.key;

		switch(opt.type)
		{
			case OptionType::Switch:
			{
				os << " <";

				size_t counter = 0;

				for(auto allowed_value : opt.allowed_values)
				{
					os << allowed_value;

					if(++counter < opt.allowed_values.size()) os << "|";
				}

				os << ">";

				break;
			}
			case OptionType::Value:
			{
				os << " <" << opt.desc << ">";
				break;
			}
			default:
			{
				break;
			}
		}

		if(opt.optional) os << "]";
	}

	os << std::endl;
}

inline void ProgramOptions::Desc::print_help(std::ostream & os) const
{
	print_usage(os);

	os << std::endl;

	std::string::size_type longest_key = 0;
	std::string::size_type longest_desc = 0;

	for(const auto & opt : options_)
	{
		if(opt.key.size() > longest_key) longest_key = opt.key.size();
		if(opt.desc.size() > longest_desc) longest_desc = opt.desc.size();
	}

	for(const auto & opt : options_)
	{
		auto key_string = std::string("--") + opt.key;

		for(const auto & short_key : short_key_map_)
		{
			if(short_key.second == opt.key)
			{
				key_string += std::string(", -") + short_key.first;

				break;
			}
		}

		os << std::setw(longest_key + 6) << key_string;
	   	os << std::setw(longest_desc + 2) << opt.desc << std::endl;
	}
}

inline auto ProgramOptions::parse_key(const char * const argv, std::string * key) const -> Error
{
	if(argv[0] != '-') return Error::unexpected_param(argv);

	*key = argv[1] != '-' ? &(argv[1]) : &(argv[2]);

	return Error();
}

inline auto ProgramOptions::parse(int argc, const char * argv[], Result * result) const -> Error
{
	Desc::Option option;
	ParserState state = ParserState::ParseKey;
	int values_read = 0;

	for(int i = 1; i < argc; i++)
	{
		switch(state)
		{
			case ParserState::ParseKey:
			{
				std::string key;

				const auto error = parse_key(argv[i], &key);

				if(error) return error;

				if(!desc_.find(key, &option))
				{
					if(desc_.find_suggestion(key, &option, did_you_mean_timeout_))
					{
						return Error::unknown_option(argv[i], option.key);
					}
					else
					{
						return Error::unknown_option(argv[i]);
					}
				}

				if(option.type == OptionType::Flag)
				{
					result->flags.insert(option.key);
				}
				else
				{
					state = ParserState::ParseValue;
				}

				break;
			}
			case ParserState::ParseValue:
			{
				std::string value = argv[i];

				if(value[0] == '-') return Error::unexpected_option(value);

				values_read++;

				const auto next_arg_is_option = i < argc - 1 && argv[i + 1][0] == '-';
				const auto no_more_args = i >= argc -1;

				if(option.min_values > values_read)
				{
					if(no_more_args || next_arg_is_option)
					{
						return Error::not_enough_values(option.key, option.min_values, values_read);
					}
				}

				if(option.type == OptionType::Switch)
				{
					if(!option.value_is_allowed(value))
					{
						return Error::invalid_switch_value(option.desc, value, option.allowed_values);
					}
				}

				result->options[option.key].push_back(value);

				if(values_read == option.max_values || (option.max_values == 0 && next_arg_is_option) || no_more_args)
				{
					values_read = 0;
					state       = ParserState::ParseKey;
				}

				break;
			}
		}
	}

	if(state == ParserState::ParseValue)
	{
		return Error::missing_param(argv[argc-1]);
	}

	return Error();
}

} // namespace rtw
