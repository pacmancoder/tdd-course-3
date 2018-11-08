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
    using CharKindPredicate = bool (*)(char, const std::locale&);

    void skip_while(const std::string& phrase, StringPos& start_pos, CharKindPredicate predicate)
    {
        auto default_locale = std::locale();

        while (start_pos < phrase.size() && predicate(phrase[start_pos], default_locale))
        {
            ++start_pos;
        }
    }

    void skip_whitespace(const std::string& phrase, StringPos& start_pos)
    {
        skip_while(phrase, start_pos, std::isspace);
    }

    void skip_word(const std::string& phrase, StringPos& start_pos)
    {
        skip_while(phrase, start_pos, std::isalnum);
    }

    std::string next_word(const std::string& phrase, StringPos& current_pos)
    {
        skip_whitespace(phrase, current_pos);
        StringPos word_begin = current_pos;
        skip_word(phrase, current_pos);
        return phrase.substr(word_begin, current_pos - word_begin);
    }
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

TEST(WordCount, skip_word_for_single_word_returns_word_size)
{
    using namespace Internal;

    StringPos position = 0;
    skip_word("hello", position);
    EXPECT_EQ(5, position);
}

TEST(WordCount, next_word_returns_same_word_and_set_pos_to_end)
{
    using namespace Internal;

    StringPos position = 0;
    EXPECT_EQ("hello", next_word("hello", position));
    EXPECT_EQ(5, position);
}

TEST(WordCount, next_word_called_twice_returns_2_words_and_empty_string)
{
    using namespace Internal;

    const std::string phrase = "hello world";

    StringPos position = 0;
    EXPECT_EQ("hello", next_word(phrase, position));
    EXPECT_EQ(5, position);
    EXPECT_EQ("world", next_word(phrase, position));
    EXPECT_EQ(11, position);
    EXPECT_EQ("", next_word(phrase, position));
    EXPECT_EQ(11, position);
}


TEST(WordCount, next_word_ignores_trailing_whitespaces)
{
    using namespace Internal;

    const std::string phrase = "   hello world   ";

    StringPos position = 0;
    EXPECT_EQ("hello", next_word(phrase, position));
    EXPECT_EQ(8, position);
    EXPECT_EQ("world", next_word(phrase, position));
    EXPECT_EQ(14, position);
    EXPECT_EQ("", next_word(phrase, position));
    EXPECT_EQ(17, position);
}
