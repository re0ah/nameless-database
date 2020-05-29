#include "type.h"

std::ostream& operator<< (std::ostream& os, const TYPE& type)
{
    static const std::string type_str[] = { "none",     /*0*/
                                            "bool",     /*1*/
                                            "uint8_t",  /*2*/
                                            "int8_t",   /*3*/
                                            "uint16_t", /*4*/
                                            "int16_t",  /*5*/
                                            "uint32_t", /*6*/
                                            "int32_t",  /*7*/
                                            "uint64_t", /*8*/
                                            "int64_t",  /*9*/
                                            "float",    /*10*/
                                            "double",   /*11*/
                                            "string"};  /*12*/
    os << type_str[static_cast<size_t>(type)];
    return os;
}

template <typename T>
constexpr inline uint64_t set_type_base(const uint64_t value,
						 		    	const TYPE type_will)
{
	static_assert(type_check<T, bool, uint8_t, uint16_t,
							 uint32_t, uint64_t, int8_t,
							 int16_t, int32_t, int64_t,
							 float, double>::value,
    			             "pass not supported type in function \"set_type_base\"");
	switch(type_will)
	{
		case TYPE::UINT8_T:
		case TYPE::UINT16_T:
		case TYPE::UINT32_T:
		case TYPE::UINT64_T:
			if constexpr (type_check<T, uint8_t, uint16_t,
									 uint32_t, uint64_t>::value)
			{ /*unsigned -> unsigned*/
				return static_cast<T>(value);
			}
			else if(type_check<T, int8_t, int16_t,
							   int32_t, int64_t>::value)
			{ /*signed -> unsigned*/
				return abs(reinterpret_cast<const T&>(value));
			}
			else
			{ /*float | double -> unsigned*/
				return reinterpret_cast<const T&>(value);
			}
			return 0;
		case TYPE::INT8_T:
		case TYPE::INT16_T:
		case TYPE::INT32_T:
		case TYPE::INT64_T:
			if constexpr (type_check<T, uint8_t, uint16_t,
									 uint32_t, uint64_t>::value)
			{ /*unsigned -> signed*/
				return abs(static_cast<T>(value));
			}
			else if(type_check<T, int8_t, int16_t,
							   int32_t, int64_t>::value)
			{ /*signed -> signed*/
				return static_cast<T>(value);
			}
			else
			{ /*float | double -> signed*/
				return reinterpret_cast<const T&>(value);
			}
			return 0;
		case TYPE::FLOAT:
			if constexpr (type_check<T, uint8_t, uint16_t, int8_t,
									 int16_t, int32_t, int64_t,
									 uint32_t, uint64_t>::value)
			{ /*unsigned -> float*/
				float f = reinterpret_cast<const float&>(value);
				f = static_cast<T>(value);
				return reinterpret_cast<uint64_t&>(f);
			}
			else if(type_check<T, double>::value)
			{ /*double -> float*/
				double d = reinterpret_cast<const double&>(value);
				float  f = reinterpret_cast<const float&>(value);
				f = static_cast<float>(d);
				return reinterpret_cast<uint64_t&>(f);
			}
			return 0;
		case TYPE::DOUBLE:
			if constexpr (type_check<T, uint8_t, uint16_t, int8_t,
									 int16_t, int32_t, int64_t,
									 uint32_t, uint64_t>::value)
			{ /*unsigned -> double*/
				double d = reinterpret_cast<const double&>(value);
				d = static_cast<T>(value);
				return reinterpret_cast<uint64_t&>(d);
			}
			else if(type_check<T, float>::value)
			{ /*float -> double*/
				double d = reinterpret_cast<const double&>(value);
				float  f = reinterpret_cast<const float&>(value);
				d = static_cast<double>(f);
				return reinterpret_cast<uint64_t&>(d);
			}
			return 0;
		case TYPE::STRING:
			{const T data_value = reinterpret_cast<const T&>(value);
			if constexpr(std::is_same<T, bool>::value)
			{
				std::string* str = new std::string(((data_value) ? "true" : "false"));
				return (uint64_t)str;
			}
			else
			{
				std::string* str = new std::string(std::to_string(data_value));
				return (uint64_t)str;
			}
			return 0;}
		default:
			return 0;
	}
}

template <typename T>
constexpr inline uint64_t set_type_string(const uint64_t value)
{
	static_assert(type_check<T, bool, uint8_t, uint16_t,
							 uint32_t, uint64_t, int8_t,
							 int16_t, int32_t, int64_t,
							 float, double>::value,
    			             "pass not supported type in function \"set_type_string\"");
	std::string* this_string = (std::string*)value;
	if constexpr(type_check<T, bool>::value)
	{
		return static_cast<bool>(*this_string == "true");
	}
	else
	{
		if (check_num_in_str(this_string))
		{
			const T new_value = str_to_num<T>(this_string);
			return new_value;
		}
		else
		{
			return 0;
		}
	}
	delete this_string;
}

uint64_t set_type_data(const std::pair<TYPE, uint64_t> data,
					   const TYPE type_will)
{
#define type  first
#define value second
	if (data.type == type_will)
	{
		return data.value;
	}

	switch(data.type)
	{
		case TYPE::BOOL:
			return set_type_base<bool>(data.value, type_will);
		case TYPE::UINT8_T:
			return set_type_base<uint8_t>(data.value, type_will);
		case TYPE::INT8_T:
			return set_type_base<int8_t>(data.value, type_will);
		case TYPE::UINT16_T:
			return set_type_base<uint16_t>(data.value, type_will);
		case TYPE::INT16_T:
			return set_type_base<int16_t>(data.value, type_will);
		case TYPE::UINT32_T:
			return set_type_base<uint32_t>(data.value, type_will);
		case TYPE::INT32_T:
			return set_type_base<int32_t>(data.value, type_will);
		case TYPE::UINT64_T:
			return set_type_base<uint64_t>(data.value, type_will);
		case TYPE::INT64_T:
			return set_type_base<int64_t>(data.value, type_will);
		case TYPE::FLOAT:
			return set_type_base<float>(data.value, type_will);
		case TYPE::DOUBLE:
			return set_type_base<double>(data.value, type_will);
		case TYPE::STRING:
			switch(type_will)
			{
				case TYPE::BOOL:
					return set_type_string<bool>(data.value);
				case TYPE::UINT8_T:
					return set_type_string<uint8_t>(data.value);
				case TYPE::INT8_T:
					return set_type_string<int8_t>(data.value);
				case TYPE::UINT16_T:
					return set_type_string<uint16_t>(data.value);
				case TYPE::INT16_T:
					return set_type_string<int16_t>(data.value);
				case TYPE::UINT32_T:
					return set_type_string<uint32_t>(data.value);
				case TYPE::INT32_T:
					return set_type_string<int32_t>(data.value);
				case TYPE::UINT64_T:
					return set_type_string<uint64_t>(data.value);
				case TYPE::INT64_T:
					return set_type_string<int64_t>(data.value);
				case TYPE::FLOAT:
					return set_type_string<float>(data.value);
				case TYPE::DOUBLE:
					return set_type_string<double>(data.value);
				default:
					return data.value;
			}
		default:
			return data.value;
	}
#undef type
#undef value
}