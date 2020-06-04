#pragma once
#include <QString>
#include <stdint.h>

/*snatch from https://stackoverflow.com/questions/17032310/how-to-make-a-variadic-is-same
  compare all variadic types with T - if no match then return false*/
template <class T, class... Ts>
struct type_check : std::disjunction<std::is_same<T, Ts>...> {};

bool check_num_in_str(const QString* const str);

/*for another types need another function for transfer of string to T.*/
template <typename T>
constexpr inline T str_to_num(const QString* const str)
{
	static_assert(type_check<T, uint8_t, uint16_t,
							 uint32_t, uint64_t, int8_t,
							 int16_t, int32_t, int64_t,
							 float, double>::value,
                  "pass not supported type in function \"str_to_num\"");
    if (type_check<T, uint8_t, uint16_t>::value)
    {
        return str->toUShort();
    }
    else if (std::is_same<T, uint32_t>::value)
    {
        return str->toUInt();
    }
    else if (std::is_same<T, uint64_t>::value)
	{
        return str->toULongLong();
	}
    else if (type_check<T, int8_t, int16_t>::value)
    {
        return str->toShort();
    }
    else if (std::is_same<T, int32_t>::value)
    {
        return str->toInt();
    }
    else if (std::is_same<T, int64_t>::value)
    {
        return str->toLongLong();
    }
    else if (std::is_same<T, float>::value)
    {
        return str->toFloat();
    }
    else if (std::is_same<T, double>::value)
    {
        return str->toDouble();
    }
    return 0;
}
