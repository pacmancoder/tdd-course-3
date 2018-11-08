#include <gtest/gtest.h>

/*
Convert a ternary number, represented as a string (e.g. '102012'), to its decimal equivalent using first principles.

The program should consider strings specifying an invalid ternary as the value 0.

Ternary numbers contain three symbols: 0, 1, and 2.

The last place in a ternary number is the 1's place. The second to last is the 3's place, the third to last is the 9's place, etc.

# "102012"
    1       0       2       0       1       2    # the number
1*3^5   + 0*3^4 + 2*3^3 + 0*3^2 + 1*3^1 + 2*3^0    # the value
  243   +   0 +    54 +     0 +     3 +     2 =  302

If your language provides a method in the standard library to perform the conversion, pretend it doesn't exist and implement it yourself.
*/

const char TERNARY_BASE_CHAR = '0';
const int TERNARY_MAX_EXPONENT = 2;
const int TERNARY_DIGITS_COUNT = 3;

int powi(int value, unsigned int exp)
{
    return exp != 0 ? value * powi(value, exp - 1) : 1;
}

int ternary_string_to_int_recursive(const std::string& value)
{
    unsigned char digit_exponent = value.front() - TERNARY_BASE_CHAR;

    if (digit_exponent > TERNARY_MAX_EXPONENT)
    {
        return -1;
    }

    if (value.size() > 1)
    {
        auto previous_digits_weight = ternary_string_to_int_recursive(value.substr(1));

        if (previous_digits_weight < 0)
        {
            return previous_digits_weight;
        }

        auto current_digits_weight = digit_exponent * powi(TERNARY_DIGITS_COUNT, value.size() - 1);
        return current_digits_weight + previous_digits_weight;
    }

    return digit_exponent;
}

int ternary_string_to_int(const std::string& value)
{
    int num = ternary_string_to_int_recursive(value);
    return num >= 0 ? num : 0;
}

TEST(TernaryNumbers, powi_for_0_returns_1)
{
    ASSERT_EQ(1, powi(42, 0));
}

TEST(TernaryNumbers, powi_for_2_in_exp_3_returns_8)
{
    ASSERT_EQ(8, powi(2, 3));
}

TEST(TernaryNumbers, single_char_equals_correct_integer)
{
    ASSERT_EQ(0, ternary_string_to_int("0"));
    ASSERT_EQ(1, ternary_string_to_int("1"));
    ASSERT_EQ(2, ternary_string_to_int("2"));
}

TEST(TernaryNumbers, invalid_single_char_returns_0)
{
    ASSERT_EQ(0, ternary_string_to_int("q"));
}

TEST(TernaryNumbers, invalid_special_char_lower_than_0_returns_0)
{
    ASSERT_EQ(0, ternary_string_to_int("/"));
}

TEST(TernaryNumbers, multi_digit_number_returns_integer)
{
    ASSERT_EQ(12321, ternary_string_to_int("121220100"));
}

TEST(TernaryNumbers, multi_digit_invalid_number_returns_0)
{
    ASSERT_EQ(0, ternary_string_to_int("121x20100"));
}
