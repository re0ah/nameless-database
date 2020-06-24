#include "date.h"
#include "string.h"
#include "type.h"

size_t date::years_from_1970() const
{
    return date::years_from_1970(this->data);
}

size_t date::days_from_1970() const
{
    return date::days_from_1970(this->data);
}

size_t date::leap_years_from_1970() const
{
    return date::leap_years_from_1970(this->data);
}

size_t date::days_from_1970_without_leap_years() const
{
    return date::days_from_1970_without_leap_years(this->data);
}

bool date::if_leap_year() const
{
    return date::if_leap_year(this->data);
}

size_t date::day_passed_year() const
{
    return date::day_passed_year(this->data);
}

size_t date::seconds_from_day() const
{
    return date::seconds_from_day(this->data);
}

size_t date::year() const
{
    return date::year(this->data);
}

size_t date::month() const
{
    return date::month(this->data);
}

size_t date::day() const
{
    return date::day(this->data);
}

size_t date::hour() const
{
    return date::hour(this->data);
}

size_t date::minute() const
{
    return date::minute(this->data);
}

size_t date::second() const
{
    return date::second(this->data);
}

size_t date::years_from_1970(const time_t seconds)
{
    return seconds / 31436000;
}

size_t date::days_from_1970(const time_t seconds)
{
    return seconds / 86400;
}

size_t date::leap_years_from_1970(const time_t seconds)
{
    return (years_from_1970(seconds) + 1) / 4;
}

size_t date::days_from_1970_without_leap_years(const time_t seconds)
{
    return days_from_1970(seconds) - leap_years_from_1970(seconds);
}

bool date::if_leap_year(const time_t seconds)
{
    return (year(seconds) % 4) == 0;
}

size_t date::day_passed_year(const time_t seconds)
{
    return days_from_1970_without_leap_years(seconds) % 365;
}

size_t date::seconds_from_day(const time_t seconds)
{
    return seconds - (days_from_1970(seconds) * 86400);
}

size_t date::year(const time_t seconds)
{
    return years_from_1970(seconds) + 1970;
}

size_t date::month(const time_t seconds)
{
    int day_passed = day_passed_year(seconds);
    for(int i = 0; i < 12; i++)
    {
        day_passed -= DAYS_IN_MONTH[if_leap_year(seconds)][i];
        if(day_passed <= 0)
        {
            return i + 1;
        }
    }
    return -1;
}

size_t date::day(const time_t seconds)
{
    int day_passed = day_passed_year(seconds) + 1;
    for(int i = 0; i < 12; i++)
    {
        if(day_passed <= 31)
        {
            return day_passed;
        }
        day_passed -= DAYS_IN_MONTH[if_leap_year(seconds)][i];
    }
    return -1;
}

size_t date::hour(const time_t seconds)
{
    return seconds_from_day(seconds) / 3600;
}

size_t date::minute(const time_t seconds)
{
    return (seconds_from_day(seconds) - (hour(seconds) * 3600)) / 60;
}

size_t date::second(const time_t seconds)
{
    return (seconds_from_day(seconds) - (hour(seconds) * 3600)) -
           (minute(seconds) * 60);
}

struct date& date::operator=(const struct string& val)
{
	
    return *this;
}

struct date& date::operator=(const struct date& val)
{
    this->data = val.data;
    return *this;
}

struct date& date::operator+=(const struct string& val)
{
    return *this;
}

struct date& date::operator+=(const struct date& val)
{
    return *this;
}

struct date& date::operator-=(const struct string& val)
{
    return *this;
}

struct date& date::operator-=(const struct date& val)
{
    return *this;
}

struct date& date::operator*=(const struct string& val)
{
    return *this;
}

struct date& date::operator*=(const struct date& val)
{
    return *this;
}

struct date& date::operator/=(const struct string& val)
{
    return *this;
}

struct date& date::operator/=(const struct date& val)
{
    return *this;
}

bool date::operator==(const struct string& val)
{

    return false;
}

bool date::operator==(const struct date& val)
{

    return false;
}

bool date::operator!=(const struct string& val)
{

    return false;
}

bool date::operator!=(const struct date& val)
{

    return false;
}

bool date::operator<(const struct string& val)
{

    return false;
}

bool date::operator<(const struct date& val)
{

    return false;
}

bool date::operator>(const struct string& val)
{

    return false;
}

bool date::operator>(const struct date& val)
{

    return false;
}

bool date::operator<=(const struct string& val)
{

    return false;
}

bool date::operator<=(const struct date& val)
{

    return false;
}

bool date::operator>=(const struct string& val)
{

    return false;
}

bool date::operator>=(const struct date& val)
{

    return false;
}
