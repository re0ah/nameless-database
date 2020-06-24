#ifndef DATE_H_
#define DATE_H_
#include <stdlib.h>
#include <stdint.h>
#include "string.h"

/*not RAII object*/
/*thanks Hocok_B_KapMaHe https://qna.habr.com/q/95177 for alghoritms*/
struct date
{
    time_t data;

    date()=delete;
    ~date()=delete;

    static const size_t SECONDS_IN_MINUTE = 60;
    static const size_t SECONDS_IN_HOUR   = SECONDS_IN_MINUTE * 60;
    static const size_t SECONDS_IN_DAY    = SECONDS_IN_HOUR * 24;
    static const size_t SECONDS_IN_YEAR   = 31622400;
    static const size_t SECONDS_IN_LEAP_YEAR = 31536000;

    static constexpr const uint8_t DAYS_IN_MONTH[2][12] = {
                                            {
                                                31, /*1,  january*/
                                                28, /*2,  february*/
                                                31, /*3,  march*/
                                                30, /*4,  april*/
                                                31, /*5,  may*/
                                                30, /*6,  june*/
                                                31, /*7,  july*/
                                                31, /*8,  august*/
                                                30, /*9,  september*/
                                                31, /*10, october*/
                                                30, /*11, november*/
                                                31, /*12, december*/
                                            },
                                            {/*leap year*/
                                                31, /*1,  january*/
                                                29, /*2,  february*/
                                                31, /*3,  march*/
                                                30, /*4,  april*/
                                                31, /*5,  may*/
                                                30, /*6,  june*/
                                                31, /*7,  july*/
                                                31, /*8,  august*/
                                                30, /*9,  september*/
                                                31, /*10, october*/
                                                30, /*11, november*/
                                                31, /*12, december*/
                                              }
    };
    size_t years_from_1970() const;
    size_t days_from_1970() const;

    size_t leap_years_from_1970() const;
    size_t days_from_1970_without_leap_years() const;

    bool   if_leap_year() const;

    size_t day_passed_year() const;

    size_t seconds_from_day() const;

    size_t year() const;
    size_t month() const;
    size_t day() const;
    size_t hour() const;
    size_t minute() const;
    size_t second() const;


    static size_t years_from_1970(const time_t seconds);
    static size_t days_from_1970(const time_t seconds);

    static size_t leap_years_from_1970(const time_t seconds);
    static size_t days_from_1970_without_leap_years(const time_t seconds);

    static bool   if_leap_year(const time_t seconds);

    static size_t day_passed_year(const time_t seconds);

    static size_t seconds_from_day(const time_t seconds);

    static size_t year(const time_t seconds);
    static size_t month(const time_t seconds);
    static size_t day(const time_t seconds);
    static size_t hour(const time_t seconds);
    static size_t minute(const time_t seconds);
    static size_t second(const time_t seconds);

    template<typename T>
    struct date& operator=(const T& val);
    struct date& operator=(const struct string& val);
    struct date& operator=(const struct date& val);

    template <typename T>
    struct date& operator+=(const T& val);
    struct date& operator+=(const struct string& val);
    struct date& operator+=(const struct date& val);

    template <typename T>
    struct date& operator-=(const T& val);
    struct date& operator-=(const struct string& val);
    struct date& operator-=(const struct date& val);

    template <typename T>
    struct date& operator*=(const T& val);
    struct date& operator*=(const struct string& val);
    struct date& operator*=(const struct date& val);

    template <typename T>
    struct date& operator/=(const T& val);
    struct date& operator/=(const struct string& val);
    struct date& operator/=(const struct date& val);

    template <typename T>
    bool operator==(const T& val);
    bool operator==(const struct string& val);
    bool operator==(const struct date& val);

    template <typename T>
    bool operator!=(const T& val);
    bool operator!=(const struct string& val);
    bool operator!=(const struct date& val);

    template <typename T>
    bool operator<(const T& val);
    bool operator<(const struct string& val);
    bool operator<(const struct date& val);

    template <typename T>
    bool operator>(const T& val);
    bool operator>(const struct string& val);
    bool operator>(const struct date& val);

    template <typename T>
    bool operator<=(const T& val);
    bool operator<=(const struct string& val);
    bool operator<=(const struct date& val);

    template <typename T>
    bool operator>=(const T& val);
    bool operator>=(const struct string& val);
    bool operator>=(const struct date& val);
};

template<typename T>
struct date& date::operator=(const T& val)
{
    this->data = val.data;
    return *this;
}

template <typename T>
struct date& date::operator+=(const T& val)
{
    return *this;
}

template <typename T>
struct date& date::operator-=(const T& val)
{
    return *this;
}

template <typename T>
struct date& date::operator*=(const T& val)
{
    return *this;
}

template <typename T>
struct date& date::operator/=(const T& val)
{
    return *this;
}

template <typename T>
bool date::operator==(const T& val)
{

    return false;
}

template <typename T>
bool date::operator!=(const T& val)
{

    return false;
}

template <typename T>
bool date::operator<(const T& val)
{

    return false;
}

template <typename T>
bool date::operator>(const T& val)
{

    return false;
}

template <typename T>
bool date::operator<=(const T& val)
{

    return false;
}

template <typename T>
bool date::operator>=(const T& val)
{

    return false;
}

#endif
