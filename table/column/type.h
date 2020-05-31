#pragma once
#include <iostream>
#include <vector>
#include "../../misc/misc.h"

namespace db_column
{
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

struct flag_type
{
		flag_type()=default;
		flag_type(std::initializer_list<bool> list);
		bool& operator[](const TYPE type);
		const bool& operator[](const TYPE type) const;
	private:
		bool _data[16];
};
        
inline std::string type_to_string(const std::pair<TYPE, uint64_t> data)
{
#define type  first/* macro for pair*/
#define value second
	switch(data.type)
	{
		case TYPE::BOOL:
			return ((static_cast<bool>(data.value)) ? std::string("true") : std::string("false"));
		case TYPE::UINT8_T:
			return std::to_string(static_cast<uint8_t>(data.value));
		case TYPE::INT8_T:
			return std::to_string(static_cast<int8_t>(data.value));
		case TYPE::UINT16_T:
			return std::to_string(static_cast<uint16_t>(data.value));
		case TYPE::INT16_T:
			return std::to_string(static_cast<int16_t>(data.value));
		case TYPE::UINT32_T:
			return std::to_string(static_cast<uint32_t>(data.value));
		case TYPE::INT32_T:
			return std::to_string(static_cast<int32_t>(data.value));
		case TYPE::UINT64_T:
			return std::to_string(static_cast<uint64_t>(data.value));
		case TYPE::INT64_T:
			return std::to_string(static_cast<int64_t>(data.value));
		case TYPE::FLOAT:
			return std::to_string(reinterpret_cast<const float&>(data.value));
		case TYPE::DOUBLE:
			return std::to_string(reinterpret_cast<const double&>(data.value));
		default:
			return std::string("");
	}
#undef type
#undef value
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
uint64_t set_type_data(std::pair<TYPE, uint64_t> data, const TYPE type_will);
}; /*end of namespace db_column*/

namespace std
{
	template <>
	struct pair <db_column::TYPE, uint64_t&>
	{
		db_column::TYPE first;
		uint64_t& second;

		pair<db_column::TYPE, uint64_t&>& operator=(pair<db_column::TYPE, uint64_t>& rhs);
		pair<db_column::TYPE, uint64_t&>& operator+=(pair<db_column::TYPE, uint64_t>& rhs);
		pair<db_column::TYPE, uint64_t&>& operator+=(pair<db_column::TYPE, uint64_t>&& rhs);

		pair<db_column::TYPE, uint64_t&>& operator-=(pair<db_column::TYPE, uint64_t>& rhs);
		pair<db_column::TYPE, uint64_t&>& operator-=(pair<db_column::TYPE, uint64_t>&& rhs);

		pair<db_column::TYPE, uint64_t&>& operator*=(pair<db_column::TYPE, uint64_t>& rhs);
		pair<db_column::TYPE, uint64_t&>& operator*=(pair<db_column::TYPE, uint64_t>&& rhs);

		pair<db_column::TYPE, uint64_t&>& operator/=(pair<db_column::TYPE, uint64_t>& rhs);
		pair<db_column::TYPE, uint64_t&>& operator/=(pair<db_column::TYPE, uint64_t>&& rhs);
	};
bool operator==(pair<db_column::TYPE, uint64_t> lhs,
				pair<db_column::TYPE, uint64_t> rhs);

bool operator!=(pair<db_column::TYPE, uint64_t> lhs,
				pair<db_column::TYPE, uint64_t> rhs);

bool operator< (pair<db_column::TYPE, uint64_t> lhs,
				pair<db_column::TYPE, uint64_t> rhs);

bool operator> (pair<db_column::TYPE, uint64_t> lhs,
				pair<db_column::TYPE, uint64_t> rhs);

bool operator<=(pair<db_column::TYPE, uint64_t> lhs,
				pair<db_column::TYPE, uint64_t> rhs);

bool operator>=(pair<db_column::TYPE, uint64_t> lhs,
				pair<db_column::TYPE, uint64_t> rhs);
};

std::ostream& operator<< (std::ostream& os, const std::pair<db_column::TYPE, uint64_t>& data);
