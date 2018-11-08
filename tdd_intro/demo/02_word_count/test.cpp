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

using Words = std::vector<std::string>;

Words split_words(const std::string& phrase)
{
    return { phrase };
}

TEST(WordCount, split_of_single_word_returns_self_as_single_element) {
    Words expected = { "hello" };
    EXPECT_EQ(expected, split_words("hello"));
}
