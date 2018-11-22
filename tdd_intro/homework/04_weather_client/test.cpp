/*
Weather Client

You are going to develop a program that gets the statistics about weather in the current city
using information from a certain server. The goal is to calculate statistics using the data from weather server.

To communicate with the weather server you have to implement interface IWeatherServer,
which provides the raw string from the real server for the requested day and time.

The real server (i.e. "weather.com") gets the requests in this format:
"<date>;<time>", for example:
"31.08.2018;03:00"

The server answers on requests with string like this:
"20;181;5.1"
This string contains the weather for the requested time and means next:
"<air_temperature_in_celsius>;<wind_direction_in_degrees>:<wind_speed>".
Wind direction value may be in range from 0 to 359 inclusively, temperature may be negative.

The task:
1. Implement fake server, because interacting with real network is inacceptable within the unit tests.
To do this, you need to use real server responses. Fortunately, you've collected some results for the several dates from the weather server.
Each line means "<request>" : "<response>":

"31.08.2018;03:00" : "20;181;5.1"
"31.08.2018;09:00" : "23;204;4.9"
"31.08.2018;15:00" : "33;193;4.3"
"31.08.2018;21:00" : "26;179;4.5"

"01.09.2018;03:00" : "19;176;4.2"
"01.09.2018;09:00" : "22;131;4.1"
"01.09.2018;15:00" : "31;109;4.0"
"01.09.2018;21:00" : "24;127;4.1"

"02.09.2018;03:00" : "21;158;3.8"
"02.09.2018;09:00" : "25;201;3.5"
"02.09.2018;15:00" : "34;258;3.7"
"02.09.2018;21:00" : "27;299;4.0"

IMPORTANT:
* Server returns empty string if request is invalid.
* Real server stores weather only for times 03:00, 09:00, 15:00 and 21:00 for every date. Do not use other hours in a day.

2. Implement IWeatherClient using fake server.
*/

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <utility>
#include <cstdint>
#include <string>
#include <array>
#include <stdexcept>

struct Weather
{
    short temperature = 0;
    unsigned short windDirection = 0;
    double windSpeed = 0;
    bool operator==(const Weather& right)
    {
        return temperature == right.temperature &&
               windDirection == right.windDirection &&
               std::abs(windSpeed - right.windSpeed) < 0.01;
    }
};

std::tuple<uint8_t, uint8_t, uint16_t> SplitDate(const std::string& str)
{
    const size_t DAY_TOKEN_SIZE = 2;
    const size_t MONTH_TOKEN_SIZE = 2;
    const size_t YEAR_TOKEN_SIZE = 4;

    const size_t SEPARATOR_SIZE = 1;

    const size_t DAY_TOKEN_POS = 0;
    const size_t MONTH_TOKEN_POS = DAY_TOKEN_POS + DAY_TOKEN_SIZE + SEPARATOR_SIZE;
    const size_t YEAR_TOKEN_POS = MONTH_TOKEN_POS + MONTH_TOKEN_SIZE + SEPARATOR_SIZE;

    const size_t DATE_TOKEN_SIZE = DAY_TOKEN_SIZE + MONTH_TOKEN_SIZE + YEAR_TOKEN_SIZE + SEPARATOR_SIZE * 2;

    const char SEPARATOR_CHAR = '.';
    const std::array<size_t, 2> SEPARATOR_POSITIONS = {{
            DAY_TOKEN_POS + DAY_TOKEN_SIZE,
            MONTH_TOKEN_POS + MONTH_TOKEN_SIZE
    }};

    if (str.size() != DATE_TOKEN_SIZE)
    {
        throw std::invalid_argument("Invalid date string size");
    }

    for (auto pos : SEPARATOR_POSITIONS)
    {
        if (str[pos] != SEPARATOR_CHAR)
        {
            throw std::invalid_argument("Invalid separator");
        }
    }

    auto day   =  std::stoul(str.substr(DAY_TOKEN_POS, DAY_TOKEN_SIZE));
    auto month =  std::stoul(str.substr(MONTH_TOKEN_POS, MONTH_TOKEN_SIZE));
    auto year  =  std::stoul(str.substr(YEAR_TOKEN_POS, YEAR_TOKEN_SIZE));

    return std::make_tuple(day, month, year);
}

void ValidateTime(const std::string& str)
{
    if (str == "03:00" || str == "09:00" || str == "15:00" || str == "21:00")
    {
        return;
    }

    throw std::invalid_argument("Time validation failed");
}

std::tuple<std::string, std::string> SplitRequest(const std::string&)
{
    return std::make_tuple("", "");
}

class IWeatherServer
{
public:
    virtual ~IWeatherServer() { }
    // Returns raw response with weather for the given day and time in request
    virtual std::string GetWeather(const std::string& request) = 0;
};

// Implement this interface
class IWeatherClient
{
public:
    virtual ~IWeatherClient() { }
    virtual double GetAverageTemperature(IWeatherServer& server, const std::string& date) = 0;
    virtual double GetMinimumTemperature(IWeatherServer& server, const std::string& date) = 0;
    virtual double GetMaximumTemperature(IWeatherServer& server, const std::string& date) = 0;
    virtual double GetAverageWindDirection(IWeatherServer& server, const std::string& date) = 0;
    virtual double GetMaximumWindSpeed(IWeatherServer& server, const std::string& date) = 0;
};


TEST(WeatherServerTest, SplitDateReturnsThreeCorespondingParts)
{
    ASSERT_EQ(std::make_tuple(31, 12, 2012), SplitDate("31.12.2012"));
}

TEST(WeatherServerTest, SplitDateThrowsOnInvalidSize)
{
    ASSERT_THROW(SplitDate("1"), std::invalid_argument);
}

TEST(WeatherServerTest, SplitDateThrowsOnInvalidSeparator)
{
    ASSERT_THROW(SplitDate("31x12.2012"), std::invalid_argument);
    ASSERT_THROW(SplitDate("31.12x2012"), std::invalid_argument);
    ASSERT_THROW(SplitDate("31x12x2012"), std::invalid_argument);
}

TEST(WeatherServerTest, SplitDateThrowsOnInvalidNumbers)
{
    ASSERT_THROW(SplitDate("xx.12.2012"), std::invalid_argument);
    ASSERT_THROW(SplitDate("31.yy.2012"), std::invalid_argument);
    ASSERT_THROW(SplitDate("31.12.zzzz"), std::invalid_argument);
    ASSERT_THROW(SplitDate("xx.yy.2012"), std::invalid_argument);
    ASSERT_THROW(SplitDate("xx.12.zzzz"), std::invalid_argument);
    ASSERT_THROW(SplitDate("31.yy.zzzz"), std::invalid_argument);
    ASSERT_THROW(SplitDate("xx.yy.zzzz"), std::invalid_argument);
}

TEST(WeatherServerTest, TimeValidationPassesOnCorrectStrings)
{
    ASSERT_NO_THROW(ValidateTime("03:00"));
    ASSERT_NO_THROW(ValidateTime("09:00"));
    ASSERT_NO_THROW(ValidateTime("15:00"));
    ASSERT_NO_THROW(ValidateTime("21:00"));
}

TEST(WeatherServerTest, TimeValidationThrowsOnInvalidString)
{
    ASSERT_THROW(ValidateTime("21:32"), std::invalid_argument);
}

TEST(WeatherServerTest, SplitRequestReturnsTwoCorespondingParts)
{
    ASSERT_EQ(std::make_tuple("31.08.2018", "03:00"), SplitRequest("31.08.2018;03:00"));
}
