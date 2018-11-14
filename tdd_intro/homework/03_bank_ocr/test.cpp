/*### Bank OCR

Your manager has recently purchased a machine that assists in reading letters and faxes sent in by branch offices. The machine scans the paper documents, and produces a file with a number of entries. You will write a program to parse this file.

#### Specification
#### User Story 1

The following format is created by the machine:
```
    _  _     _  _  _  _  _
  | _| _||_||_ |_   ||_||_|
  ||_  _|  | _||_|  ||_| _|
```
Each entry is 3 lines long, and each line has 27 characters. 3 lines of each entry contain an account number written using pipes and underscores.

Each account number should have 9 digits, all of which should be in the range 0-9. A normal file contains around 500 entries.

Write a program that can take this file and parse it into actual account numbers.

Example input and output
```
 _  _  _  _  _  _  _  _  _
| || || || || || || || || |
|_||_||_||_||_||_||_||_||_|

=> 000000000

  |  |  |  |  |  |  |  |  |
  |  |  |  |  |  |  |  |  |

=> 111111111

 _  _  _  _  _  _  _  _  _
 _| _| _| _| _| _| _| _| _|
|_ |_ |_ |_ |_ |_ |_ |_ |_

=> 222222222

 _  _  _  _  _  _  _  _  _
 _| _| _| _| _| _| _| _| _|
 _| _| _| _| _| _| _| _| _|

=> 333333333

|_||_||_||_||_||_||_||_||_|
  |  |  |  |  |  |  |  |  |

=> 444444444

 _  _  _  _  _  _  _  _  _
|_ |_ |_ |_ |_ |_ |_ |_ |_
 _| _| _| _| _| _| _| _| _|

=> 555555555

 _  _  _  _  _  _  _  _  _
|_ |_ |_ |_ |_ |_ |_ |_ |_
|_||_||_||_||_||_||_||_||_|

=> 666666666

 _  _  _  _  _  _  _  _  _
  |  |  |  |  |  |  |  |  |
  |  |  |  |  |  |  |  |  |

=> 777777777

 _  _  _  _  _  _  _  _  _
|_||_||_||_||_||_||_||_||_|
|_||_||_||_||_||_||_||_||_|

=> 888888888

 _  _  _  _  _  _  _  _  _
|_||_||_||_||_||_||_||_||_|
 _| _| _| _| _| _| _| _| _|

=> 999999999

    _  _     _  _  _  _  _
  | _| _||_||_ |_   ||_||_|
  ||_  _|  | _||_|  ||_| _|

=> 123456789
```
*/
#include <gtest/gtest.h>
#include <string>
#include <array>
#include <exception>

const unsigned short DIGIT_LENGTH = 3;
const unsigned short LINES_IN_DIGIT = 3;

class ParsingException : public std::exception
{
public:
    ParsingException() :
        std::exception("Failed to parse digit") {}
};

class Digit
{
public:
    Digit(std::initializer_list<std::string> lines);

    unsigned char parse() const;

public:
    friend bool operator==(const Digit& lhs, const Digit& rhs);
    friend bool operator!=(const Digit& lhs, const Digit& rhs);

private:
    std::array<std::string, LINES_IN_DIGIT> lines_;
};

const unsigned short DIGITS_ON_DISPLAY = 9;
const unsigned short DIGITS_COUNT = 10;

struct Display
{
    std::string lines[LINES_IN_DIGIT];
};

std::array<Digit, DIGITS_COUNT> DIGITS =
{{
     { " _ ",
       "| |",
       "|_|"
     },
     { "   ",
       "  |",
       "  |"
     },
     { " _ ",
       " _|",
       "|_ "
     },
     { " _ ",
       " _|",
       " _|"
     },
     { "   ",
       "|_|",
       "  |"
     },
     { " _ ",
       "|_ ",
       " _|"
     },
     { " _ ",
       "|_ ",
       "|_|"
     },
     { " _ ",
       "  |",
       "  |"
     },
     { " _ ",
       "|_|",
       "|_|"
     },
     { " _ ",
       "|_|",
       " _|"
     }
}};

const Display s_displayAll0 = { " _  _  _  _  _  _  _  _  _ ",
                                "| || || || || || || || || |",
                                "|_||_||_||_||_||_||_||_||_|"
};

const Display s_displayAll1 = { "                           ",
                                "  |  |  |  |  |  |  |  |  |",
                                "  |  |  |  |  |  |  |  |  |"
};

const Display s_displayAll2 = {  " _  _  _  _  _  _  _  _  _ ",
                                 " _| _| _| _| _| _| _| _| _|",
                                 "|_ |_ |_ |_ |_ |_ |_ |_ |_ "
};

const Display s_displayAll3 = { " _  _  _  _  _  _  _  _  _ ",
                                " _| _| _| _| _| _| _| _| _|",
                                " _| _| _| _| _| _| _| _| _|"
};

const Display s_displayAll4 = { "                           ",
                                "|_||_||_||_||_||_||_||_||_|",
                                "  |  |  |  |  |  |  |  |  |"
};

const Display s_displayAll5 = { " _  _  _  _  _  _  _  _  _ ",
                                "|_ |_ |_ |_ |_ |_ |_ |_ |_ ",
                                " _| _| _| _| _| _| _| _| _|"
};

const Display s_displayAll6 = { " _  _  _  _  _  _  _  _  _ ",
                                "|_ |_ |_ |_ |_ |_ |_ |_ |_ ",
                                "|_||_||_||_||_||_||_||_||_|"
};

const Display s_displayAll7 = { " _  _  _  _  _  _  _  _  _ ",
                                "  |  |  |  |  |  |  |  |  |",
                                "  |  |  |  |  |  |  |  |  |"
};

const Display s_displayAll8 = { " _  _  _  _  _  _  _  _  _ ",
                                "|_||_||_||_||_||_||_||_||_|",
                                "|_||_||_||_||_||_||_||_||_|"
};

const Display s_displayAll9 = { " _  _  _  _  _  _  _  _  _ ",
                                "|_||_||_||_||_||_||_||_||_|",
                                " _| _| _| _| _| _| _| _| _|"
};

const Display s_display123456789 = { "    _  _     _  _  _  _  _ ",
                                     "  | _| _||_||_ |_   ||_||_|",
                                     "  ||_  _|  | _||_|  ||_| _|"
};


Digit::Digit(std::initializer_list<std::string> lines) :
    lines_()
{
    size_t i = 0;
    for (const auto& line : lines)
    {
        lines_[i++] = line;
    }
}

unsigned char Digit::parse() const
{
    for (size_t i = 0; i < DIGITS.size(); ++i)
    {
        if (DIGITS[i] == *this)
        {
            return static_cast<unsigned char>(i);
        }
    }

    return 255;
}

bool operator==(const Digit& lhs, const Digit& rhs)
{
    return lhs.lines_[0] == rhs.lines_[0]
            && lhs.lines_[1] == rhs.lines_[1]
            && lhs.lines_[2] == rhs.lines_[2];
}


bool operator!=(const Digit& lhs, const Digit& rhs)
{
    return !(lhs == rhs);
}

TEST(BankOCR, TwoDigitsAreEqual)
{
    Digit lhs({"|-|", "-|-", "   "});
    Digit rhs({"|-|", "-|-", "   "});

    EXPECT_EQ(lhs, rhs);
}

TEST(BankOCR, TwoDigitsAreNotEqual)
{
    Digit lhs({"|--", "-|-", "   "});
    Digit rhs({"|-|", "-|-", " | "});

    EXPECT_NE(lhs, rhs);
}


TEST(BankOCR, SingleDigitParsedCorrectly)
{
    EXPECT_EQ(0, DIGITS[0].parse());
    EXPECT_EQ(1, DIGITS[1].parse());
    EXPECT_EQ(2, DIGITS[2].parse());
    EXPECT_EQ(3, DIGITS[3].parse());
    EXPECT_EQ(4, DIGITS[4].parse());
    EXPECT_EQ(5, DIGITS[5].parse());
    EXPECT_EQ(6, DIGITS[6].parse());
    EXPECT_EQ(7, DIGITS[7].parse());
    EXPECT_EQ(8, DIGITS[8].parse());
    EXPECT_EQ(9, DIGITS[9].parse());
}

TEST(BankOCR, SingleDigitIsNotParsed)
{
    EXPECT_THROW(Digit({"123", "456", "789"}), ParsingException);
}
