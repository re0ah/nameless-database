#pragma once
#include <iostream>
#include <string>
#include <algorithm>
#include "../misc/misc.h"

enum class TYPE
{
    NONE     = 0,
    BOOL     = 1,
    UINT8_T  = 2,
    INT8_T   = 3,
    UINT16_T = 4,
    INT16_T  = 5,
    UINT32_T = 6,
    INT32_T  = 7,
    UINT64_T = 8,
    INT64_T  = 9,
    FLOAT    = 10,
    DOUBLE   = 11,
    STRING   = 12
};      
const size_t ENUM_TYPE_SIZE = static_cast<size_t>(TYPE::STRING);
        
inline std::string type_to_string(const TYPE type, const uint64_t val)
{
	switch(type)
	{
		case TYPE::BOOL:
			return ((static_cast<bool>(val)) ? std::string("true") : std::string("false"));
		case TYPE::UINT8_T:
			return std::to_string(static_cast<uint8_t>(val));
		case TYPE::INT8_T:
			return std::to_string(static_cast<int8_t>(val));
		case TYPE::UINT16_T:
			return std::to_string(static_cast<uint16_t>(val));
		case TYPE::INT16_T:
			return std::to_string(static_cast<int16_t>(val));
		case TYPE::UINT32_T:
			return std::to_string(static_cast<uint32_t>(val));
		case TYPE::INT32_T:
			return std::to_string(static_cast<int32_t>(val));
		case TYPE::UINT64_T:
			return std::to_string(static_cast<uint64_t>(val));
		case TYPE::INT64_T:
			return std::to_string(static_cast<int64_t>(val));
		case TYPE::FLOAT:
			return std::to_string(reinterpret_cast<const float&>(val));
		case TYPE::DOUBLE:
			return std::to_string(reinterpret_cast<const double&>(val));
		default:
			return std::string("");
	}
}

std::ostream& operator<< (std::ostream& os, const TYPE& type);

/*set type
	  Interaction with types:
	  	None:
			all types -> nothing do
---------------------------------------------------------------
		Bool:	
			Uint8_t, Int8_t, Uint16_t, Int16_t, Uint32_t,
			Int32_t, Uint64_t, Int64_t, Float, Double -> nothing do

			String -> if true then "true", same for false
---------------------------------------------------------------
		Uint8_t, Int8_t, Uint16_t, Int16_t, Uint32_t,
		Int32_t, Uint64_t, Int64_t, Float, Double:
			None, Bool -> nothing do

			Uint8_t, int8_t, uint16_t, int16_t,
			uint32_t, int32_t, uint64_t, int64_t,
			float, double -> cast, for (signed -> unsigned) lost sign
			
			String -> transfer, allocate str and save
---------------------------------------------------------------
		String:
			None -> nothing do
			
			Bool -> if "true" then true, same for "false"
			
			Uint8_t, int8_t, uint16_t, int16_t,
			uint32_t, int32_t, uint64_t, int64_t,
			float, double -> transfer
*/
uint64_t set_type_data(uint64_t data, const TYPE type_now, const TYPE type_will);
