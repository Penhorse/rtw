#pragma once

#include <ctime>
#include <algorithm>
#include <iterator>
#include <set>
#include <string>
#include <utility>

namespace rtw
{
	
/*

usage:
--------------------------------------------------------------------------------

	SpellCorrect::Dictionary dictionary { "gone", "one", "two", "three" };

	SpellCorrect corrector(dictionary);

	const auto one_correction = corrector.get_one_correction("there");

	std::cout << one_correction << std::endl;

	std::cout << "-----" << std::endl;

	const auto multiple = corrector.get_corrections("onf");

	for(const auto & x : multiple)
	{
		std::cout << x << std::endl;
	}

output: ------------------------------------------------------------------------

	three
	-----
	gone
	one

````````````````````````````````````````````````````````````````````````````````
 
*/
class SpellCorrect
{

public:

	static const int DEFAULT_SEARCH_TIMEOUT = 2;

	using Words = std::set<std::string>;
	using Dictionary = Words;
	using Corrections = Words;

	SpellCorrect(const Dictionary & dictionary, int search_timeout = DEFAULT_SEARCH_TIMEOUT);

	Corrections get_corrections(const std::string & word) const;
	std::string get_one_correction(const std::string & word) const;

private:

	using Split = std::pair<std::string, std::string>;

	static std::set<Split> get_splits(const std::string & word);
	static Words get_deletes(const std::set<Split> & splits);
	static Words get_transposes(const std::set<Split> & splits);
	static Words get_replaces(const std::set<Split> & splits);
	static Words get_inserts(const std::set<Split> & splits);
	static Words get_word_edits_of_d1(const std::string & word);
	
	Words get_known_edits_of_d2(const std::string & word) const;
	std::string get_one_known_edit_of_d2(const std::string & word) const;
	Words known_words(const Words & words) const;

	bool timeout_elapsed() const;

	Dictionary           dictionary_;
	mutable std::clock_t search_start_;
	int                  search_timeout_;

};

inline SpellCorrect::SpellCorrect(const Dictionary & dictionary, int search_timeout) :
	dictionary_(dictionary),
	search_timeout_(search_timeout)
{
	// nothing
}

inline auto SpellCorrect::get_splits(const std::string & word) -> std::set<Split>
{
	std::set<Split> result;

	for(std::string::size_type i = 0; i < word.size(); i++)
	{
		result.insert({ word.substr(0, i), word.substr(i) });
	}

	return result;
}

inline auto SpellCorrect::get_deletes(const std::set<Split> & splits) -> Words
{
	Words result;

	for(const auto & split : splits)
	{
		result.insert(split.first + split.second.substr(1));
	}

	return result;
}

inline auto SpellCorrect::get_transposes(const std::set<Split> & splits) -> Words
{
	Words result;

	for(const auto & split : splits)
	{
		if(split.second.size() > 1)
		{
			result.insert(split.first + split.second[1] + split.second[0] + split.second.substr(2));
		}
	}

	return result;
}

inline auto SpellCorrect::get_replaces(const std::set<Split> & splits) -> Words
{
	Words result;

	for(char c = 33; c < 127; c++)
	{
		for(const auto & split : splits)
		{
			result.insert(split.first + c + split.second.substr(1));
		}
	}

	return result;
}

inline auto SpellCorrect::get_inserts(const std::set<Split> & splits) -> Words
{
	Words result;

	for(char c = 33; c < 127; c++)
	{
		for(const auto & split : splits)
		{
			result.insert(split.first + c + split.second);
		}
	}

	return result;
}

inline auto SpellCorrect::get_word_edits_of_d1(const std::string & word) -> Words
{
	Words result;

	const auto splits     = get_splits(word);
	const auto deletes    = get_deletes(splits);
	const auto transposes = get_transposes(splits);
	const auto replaces   = get_replaces(splits);
	const auto inserts    = get_inserts(splits);

	std::copy(deletes.begin(), deletes.end(), std::inserter(result, result.end()));
	std::copy(transposes.begin(), transposes.end(), std::inserter(result, result.end()));
	std::copy(replaces.begin(), replaces.end(), std::inserter(result, result.end()));
	std::copy(inserts.begin(), inserts.end(), std::inserter(result, result.end()));
	
	return result;
}

inline bool SpellCorrect::timeout_elapsed() const
{
	return ((std::clock() - search_start_) / CLOCKS_PER_SEC) >= search_timeout_;
}

inline auto SpellCorrect::get_known_edits_of_d2(const std::string & word) const -> Words
{
	search_start_ = std::clock();

	Words result;

	const auto word_edits = get_word_edits_of_d1(word);

	for(const auto & edit : word_edits)
	{
		const auto known_edits = known_words(get_word_edits_of_d1(edit));

		std::copy(known_edits.begin(), known_edits.end(), std::inserter(result, result.end()));

		if(timeout_elapsed())
		{
			return result;
		}
	}

	return result;
}

inline std::string SpellCorrect::get_one_known_edit_of_d2(const std::string & word) const
{
	search_start_ = std::clock();

	const auto word_edits = get_word_edits_of_d1(word);

	for(const auto & edit : word_edits)
	{
		const auto known_edits = known_words(get_word_edits_of_d1(edit));

		if(!known_edits.empty()) return *known_edits.begin();

		if(timeout_elapsed()) return std::string();
	}

	return std::string();
}

inline auto SpellCorrect::known_words(const Words & words) const -> Words
{
	Words result;

	std::copy_if(
			words.begin(),
			words.end(),
			std::inserter(result, result.end()),
			[this](const std::string & word)
			{
				return dictionary_.find(word) != dictionary_.end();
			});

	return result;
}

inline auto SpellCorrect::get_corrections(const std::string & word) const -> Corrections
{
	Corrections result;

	const auto known_edits_of_d1 = known_words(get_word_edits_of_d1(word));
	const auto known_edits_of_d2 = get_known_edits_of_d2(word);

	std::copy(known_edits_of_d1.begin(), known_edits_of_d1.end(), std::inserter(result, result.end()));
	std::copy(known_edits_of_d2.begin(), known_edits_of_d2.end(), std::inserter(result, result.end()));

	return result;
}

inline std::string SpellCorrect::get_one_correction(const std::string & word) const
{
	const auto known_edits_of_d1 = known_words(get_word_edits_of_d1(word));

	if(!known_edits_of_d1.empty())
	{
		return *known_edits_of_d1.begin();
	}

	return get_one_known_edit_of_d2(word);
}

} // namespace rtw
