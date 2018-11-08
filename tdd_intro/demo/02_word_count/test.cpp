/*
Given a phrase, count the occurrences of each word in that phrase. Ignore whitespaces and punctual symbols
For example for the input "olly olly in come free please please let it be in such manner olly"
olly: 3
in: 2
come: 1
free: 1
please: 2
let: 1
it: 1
be: 1
manner: 1
such: 1
*/

#include <gtest/gtest.h>
#include <string>
#include <map>
#include <locale>

using Words = std::vector<std::string>;
using StringPos = size_t;



StringPos skip_whitespace(const std::string& phrase, StringPos start_pos)
{
    auto default_locale = std::locale();

    while (std::isspace(phrase[start_pos], default_locale))
    {
        ++start_pos;
    }

    return start_pos;
}

Words split_words(const std::string& phrase)
{
    return { phrase };
}


TEST(WordCount, skip_whitespace_skips_single_whitespace)
{
    EXPECT_EQ(1, skip_whitespace(" test", 0));
    EXPECT_EQ(6, skip_whitespace("test  none", 4));
}

TEST(WordCount, split_of_single_word_returns_self_as_single_element)
{
    Words expected = { "hello" };
    EXPECT_EQ(expected, split_words("hello"));
}
