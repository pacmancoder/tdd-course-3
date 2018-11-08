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

namespace Internal
{
    using StringPos = size_t;

    void skip_whitespace(const std::string& phrase, StringPos& start_pos)
    {
        auto default_locale = std::locale();

        while (start_pos < phrase.size() && std::isspace(phrase[start_pos], default_locale))
        {
            ++start_pos;
        }
    }
}

Words split_words(const std::string& phrase)
{
    return { phrase };
}


TEST(WordCount, skip_whitespace_skips_single_whitespace)
{
    using namespace Internal;

    StringPos position = 0;
    skip_whitespace(" test", position);
    EXPECT_EQ(1, position);
    position = 4;
    skip_whitespace("test  none", position);
    EXPECT_EQ(6, position);
}

TEST(WordCount, skip_whitespace_no_whitespace_returns_start_pos)
{
    using namespace Internal;

    StringPos position = 1;
    skip_whitespace("test", position);
    EXPECT_EQ(1, position);
    position = 5;
    skip_whitespace("test none", position);
    EXPECT_EQ(5, position);
}

TEST(WordCount, skip_whitespace_whitespace_on_end_returns_phrase_size)
{
    using namespace Internal;

    StringPos position = 2;
    skip_whitespace("    ", position);
    EXPECT_EQ(4, position);
}

TEST(WordCount, skip_whitespace_out_of_bounds_pos_does_not_changes)
{
    using namespace Internal;

    StringPos position = 4;
    skip_whitespace("hi", position);
    EXPECT_EQ(4, position);
}

TEST(WordCount, split_of_single_word_returns_self_as_single_element)
{
    Words expected = { "hello" };
    EXPECT_EQ(expected, split_words("hello"));
}
