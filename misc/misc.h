#pragma once
#include <string>
#include <stdint.h>

/*snatch from https://stackoverflow.com/questions/17032310/how-to-make-a-variadic-is-same
  compare all variadic types with T - if no match then return false*/
template <class T, class... Ts>
struct type_check : std::disjunction<std::is_same<T, Ts>...> {};

bool check_num_in_str(const std::string* const str);

/*for another types need another function for transfer of string to T.*/
template <typename T>
constexpr inline T str_to_num(const std::string* const str)
{
	static_assert(type_check<T, uint8_t, uint16_t,
							 uint32_t, uint64_t, int8_t,
							 int16_t, int32_t, int64_t,
							 float, double>::value,
                  "pass not supported type in function \"str_to_num\"");
	if constexpr (type_check<T, uint8_t, uint16_t, uint32_t>::value)
	{
		return static_cast<T>(std::stoul(*str));
	}
	else if (std::is_same<T, uint64_t>::value)
	{
		return std::stoull(*str);
	}
	else if (type_check<T, int8_t, int16_t, int32_t>::value)
	{
		return static_cast<T>(std::stoi(*str));
	}
	else if (std::is_same<T, int64_t>::value)
	{
		return std::stoll(*str);
	}
	else if (std::is_same<T, float>::value)
	{
		return std::stof(*str);
	}
	else if (std::is_same<T, double>::value)
	{
		return std::stod(*str);
	}
	return 0;
}
