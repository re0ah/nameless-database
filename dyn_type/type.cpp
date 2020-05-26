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
constexpr inline void set_type_base(uint64_t& data,
						 		    const TYPE type)
{
	static_assert(type_check<T, bool, uint8_t, uint16_t,
							 uint32_t, uint64_t, int8_t,
							 int16_t, int32_t, int64_t,
							 float, double>::value,
    			             "pass not supported type in function \"set_type_base\"");
	switch(type)
	{
		case TYPE::NONE:
		case TYPE::BOOL:
			break;
		case TYPE::UINT8_T:
		case TYPE::UINT16_T:
		case TYPE::UINT32_T:
		case TYPE::UINT64_T:
			if constexpr (type_check<T, uint8_t, uint16_t,
									 uint32_t, uint64_t>::value)
			{ /*unsigned -> unsigned*/
				data = static_cast<T>(data);
			}
			else if(type_check<T, int8_t, int16_t,
							   int32_t, int64_t>::value)
			{ /*signed -> unsigned*/
				data = abs(reinterpret_cast<T&>(data));
			}
			else
			{ /*float | double -> unsigned*/
				data = reinterpret_cast<T&>(data);
			}
			break;
		case TYPE::INT8_T:
		case TYPE::INT16_T:
		case TYPE::INT32_T:
		case TYPE::INT64_T:
			if constexpr (type_check<T, uint8_t, uint16_t,
									 uint32_t, uint64_t>::value)
			{ /*unsigned -> signed*/
				data = abs(static_cast<T>(data));
			}
			else if(type_check<T, int8_t, int16_t,
							   int32_t, int64_t>::value)
			{ /*signed -> signed*/
				data = static_cast<T>(data);
			}
			else
			{ /*float | double -> signed*/
				data = reinterpret_cast<T&>(data);
			}
			break;
		case TYPE::FLOAT:
			if constexpr (type_check<T, uint8_t, uint16_t, int8_t,
									 int16_t, int32_t, int64_t,
									 uint32_t, uint64_t>::value)
			{ /*unsigned -> float*/
				float& f = reinterpret_cast<float&>(data);
				f = static_cast<T>(data);
			}
			else if(type_check<T, double>::value)
			{ /*double -> float*/
				double& d = reinterpret_cast<double&>(data);
				float&  f = reinterpret_cast<float&>(data);
				f = static_cast<float>(d);
			}
			break;
		case TYPE::DOUBLE:
			if constexpr (type_check<T, uint8_t, uint16_t, int8_t,
									 int16_t, int32_t, int64_t,
									 uint32_t, uint64_t>::value)
			{ /*unsigned -> double*/
				double& d = reinterpret_cast<double&>(data);
				d = static_cast<T>(data);
			}
			else if(type_check<T, float>::value)
			{ /*float -> double*/
				double& d = reinterpret_cast<double&>(data);
				float&  f = reinterpret_cast<float&>(data);
				d = static_cast<double>(f);
			}
			break;
		case TYPE::STRING:
			const T data_value = reinterpret_cast<T&>(data);
			if constexpr(std::is_same<T, bool>::value)
			{
				std::string* str = new std::string(((data_value) ? "true" : "false"));
				data = (uint64_t)str;
			}
			else
			{
				std::string* str = new std::string(std::to_string(data_value));
				data = (uint64_t)str;
			}
			break;
	}
}

template <typename T>
constexpr inline void set_type_string(uint64_t& element,
						 		 	  const TYPE type)
{
	static_assert(type_check<T, bool, uint8_t, uint16_t,
							 uint32_t, uint64_t, int8_t,
							 int16_t, int32_t, int64_t,
							 float, double>::value,
    			             "pass not supported type in function \"set_type_string\"");
	std::string* this_string = (std::string*)element;
	if constexpr(type_check<T, bool>::value)
	{
		const bool new_value = (*this_string == "true");
		element = new_value;
	}
	else
	{
		if (check_num_in_str(this_string))
		{
			const T new_value = str_to_num<T>(this_string);
			element = new_value;
		}
		else
		{
			element = 0;
		}
	}
	delete this_string;
}

uint64_t set_type_data(uint64_t data,
					   const TYPE type_now,
					   const TYPE type_will)
{
	if (type_now == type_will)
	{
		return data;
	}
#if 0
	/*yes, it's very short but... jump table better?
	  switch more easier for reading, but takes a lot of space.
	  I could cut code a couple of times...

	  The temptation is great
	 */
	static void (*fptr[])(std::vector<uint64_t>& this_vector, const TYPE type) =
			{
				set_type_base<bool>,	   /*0  | T -> bool     |*/
				set_type_base<bool>,	   /*1  | T -> bool     |*/
				set_type_base<uint8_t>,	   /*2  | T -> uint8_t  |*/
				set_type_base<int8_t>,	   /*3  | T -> int8_t   |*/
				set_type_base<uint16_t>,   /*4  | T -> uint16_t |*/
				set_type_base<int16_t>,	   /*5  | T -> int16_t  |*/
				set_type_base<uint32_t>,   /*6  | T -> uint32_t |*/
				set_type_base<int32_t>,	   /*7  | T -> int32_t  |*/
				set_type_base<uint64_t>,   /*8  | T -> uint64_t |*/
				set_type_base<int64_t>,	   /*9  | T -> int64_t  |*/
				set_type_base<float>,	   /*10 | T -> float    |*/
				set_type_base<double>,	   /*11 | T -> double   |*/
				set_type_string<bool>,	   /*12 | string -> bool     |*/
				set_type_string<bool>,	   /*13 | string -> bool     |*/
				set_type_string<uint8_t>,  /*14 | string -> uint8_t  |*/
				set_type_string<int8_t>,   /*15 | string -> int8_t   |*/
				set_type_string<uint16_t>, /*16 | string -> uint16_t |*/
				set_type_string<int16_t>,  /*17 | string -> int16_t  |*/
				set_type_string<uint32_t>, /*18 | string -> uint32_t |*/
				set_type_string<int32_t>,  /*19 | string -> int32_t  |*/
				set_type_string<uint64_t>, /*20 | string -> uint64_t |*/
				set_type_string<int64_t>,  /*21 | string -> int64_t  |*/
				set_type_string<float>,	   /*22 | string -> float    |*/
				set_type_string<double>,   /*23 | string -> double   |*/
			};
	fptr[(int)this->_type + (int)type](this->_data, type);
#endif
	switch(type_now)
	{
		case TYPE::NONE:
			break;
		case TYPE::BOOL:
			set_type_base<bool>(data, type_will);
			break;
		case TYPE::UINT8_T:
			set_type_base<uint8_t>(data, type_will);
			break;
		case TYPE::INT8_T:
			set_type_base<int8_t>(data, type_will);
			break;
		case TYPE::UINT16_T:
			set_type_base<uint16_t>(data, type_will);
			break;
		case TYPE::INT16_T:
			set_type_base<int16_t>(data, type_will);
			break;
		case TYPE::UINT32_T:
			set_type_base<uint32_t>(data, type_will);
			break;
		case TYPE::INT32_T:
			set_type_base<int32_t>(data, type_will);
			break;
		case TYPE::UINT64_T:
			set_type_base<uint64_t>(data, type_will);
			break;
		case TYPE::INT64_T:
			set_type_base<int64_t>(data, type_will);
			break;
		case TYPE::FLOAT:
			set_type_base<float>(data, type_will);
			break;
		case TYPE::DOUBLE:
			set_type_base<double>(data, type_will);
			break;
		case TYPE::STRING:
			switch(type_will)
			{
				case TYPE::NONE:
				case TYPE::STRING:
					break;
				case TYPE::BOOL:
					set_type_string<bool>(data, type_will);
					break;
				case TYPE::UINT8_T:
					set_type_string<uint8_t>(data, type_will);
					break;
				case TYPE::INT8_T:
					set_type_string<int8_t>(data, type_will);
					break;
				case TYPE::UINT16_T:
					set_type_string<uint16_t>(data, type_will);
					break;
				case TYPE::INT16_T:
					set_type_string<int16_t>(data, type_will);
					break;
				case TYPE::UINT32_T:
					set_type_string<uint32_t>(data, type_will);
					break;
				case TYPE::INT32_T:
					set_type_string<int32_t>(data, type_will);
					break;
				case TYPE::UINT64_T:
					set_type_string<uint64_t>(data, type_will);
					break;
				case TYPE::INT64_T:
					set_type_string<int64_t>(data, type_will);
					break;
				case TYPE::FLOAT:
					set_type_string<float>(data, type_will);
					break;
				case TYPE::DOUBLE:
					set_type_string<double>(data, type_will);
					break;
			}
			break;
	}
	return data;
}
