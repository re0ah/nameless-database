#include "dyn_type.h"
#include <algorithm>
#include <string>
#include <cstring>

using namespace db_col;

Column::Column() :
	_type{TYPE::NONE}
{}

Column::Column(const TYPE type) :
	_type{type}
{}

Column::Column(const Column& col) :
	_type{col.type()},
	_data{col._data}
{}

Column& Column::operator=(const Column& col)
{
	this->_type = col.type();
	this->_data = col.all_data();
	return *this;
}

void Column::reserve(const size_t size)
{
	_data.reserve(size);
}

void Column::push_back(const uint64_t data)
{
	_data.push_back(data);
}

const std::vector<uint64_t>& Column::all_data() const
{
	return this->_data;
}

Column::~Column()
{
	if (_type == TYPE::STRING)
	{
		size_t size = _data.size();
		for (size_t i = 0; i < size; i++)
		{
			delete (std::string*)_data[i];
		}
	}
}

TYPE Column::type() const
{
	return _type;
}

uint64_t Column::data(const size_t pos) const
{
	return _data[pos];
}

/*for another types need another function for transfer of string to T.*/
template <typename T>
constexpr inline T str_to_num(const std::string* str)
{
	static_assert(type_check<T, uint8_t, uint16_t,
							 uint32_t, uint64_t, int8_t,
							 int16_t, int32_t, int64_t,
							 float, double>::value,
                  "pass not supported type in function \"str_to_num\"");
	if constexpr (type_check<T, uint8_t, uint16_t, uint32_t>::value)
		return static_cast<T>(std::stoul(*str));
	else if (std::is_same<T, uint64_t>::value)
		return std::stoull(*str);
	else if (type_check<T, int8_t, int16_t, int32_t>::value)
		return static_cast<T>(std::stoi(*str));
	else if (std::is_same<T, int64_t>::value)
		return std::stoll(*str);
	else if (std::is_same<T, float>::value)
		return std::stof(*str);
	else if (std::is_same<T, double>::value)
		return std::stod(*str);
	return 0;
}

template <typename T>
inline void add_base_type(std::vector<uint64_t>& _data,
						  const size_t pos,
						  const uint64_t val,
						  const TYPE type)
{
	static_assert(type_check<T, bool, uint8_t, uint16_t,
							 uint32_t, uint64_t, int8_t,
							 int16_t, int32_t, int64_t,
							 float, double, std::string*>::value,
                  "pass not supported type in function \"add_base_type\"");
	switch(type)
	{
		case TYPE::NONE:
			break;
		case TYPE::BOOL:
		case TYPE::UINT8_T:
		case TYPE::INT8_T:
		case TYPE::UINT16_T:
		case TYPE::INT16_T:
		case TYPE::UINT32_T:
		case TYPE::INT32_T:
		case TYPE::UINT64_T:
		case TYPE::INT64_T:
			if constexpr (std::is_same<T, bool>::value)
			{
				reinterpret_cast<bool&>(_data[pos]) += static_cast<bool>(val);
				break;
			}
		case TYPE::FLOAT:
		case TYPE::DOUBLE:
			if constexpr (type_check<T, float, double>::value)
				reinterpret_cast<T&>(_data[pos]) += reinterpret_cast<const T&>(val);
			else
				reinterpret_cast<T&>(_data[pos]) += static_cast<T>(val);
			break;
		case TYPE::STRING:
			if constexpr (!std::is_same<T, bool>::value)
				reinterpret_cast<T&>(_data[pos]) += str_to_num<T>((std::string*)val);
			break;
	}
}

/*СЕМЬ ОЗОРНЫХ ШАГОВ ЗА ГОРИЗОНТ,
  СЕМЬ ЛЕДЯНЫХ МОСТОВ ЗА ГОРИЗОНТ,
  СЕМЬ ПРОЛИВНЫХ ДОЖДЕЙ ЗА ГОРИЗОНТ
p.s. пусть будет легко*/

/*
	return: true, if str can interprite as number
 			false, if str don't can interprite as number
 */
bool check_num_in_str(const std::string* const str)
{
	/*come down*/
	char* ptr;
	strtod(str->c_str(), &ptr);
	return *ptr == 0;
}

template <typename T>
inline void add_to_string(std::string* str, const uint64_t val)
{
	static_assert(type_check<T, bool, uint8_t, uint16_t,
							 uint32_t, uint64_t, int8_t,
							 int16_t, int32_t, int64_t,
							 float, double>::value,
                  "pass not supported type in function \"add_to_string\"");
	if (check_num_in_str(str)) /*if number*/
	{
		/*need do:
		 	  1. transform string to number
			  2. sum this number with argument "val"
			  3. transform result to string and store
		*/
		*str = std::to_string(str_to_num<T>(str) + reinterpret_cast<const T&>(val));
	}
	else /*if not number*/
	{
		/*need do:
		 	  1. transform number to string
			  2. concate this number with string
		*/
		*str += std::to_string(reinterpret_cast<const T&>(val));
	}
}

void Column::add(const size_t pos, const uint64_t val, const TYPE type)
{
	switch(this->_type)
	{
		case TYPE::NONE:
			break;
		case TYPE::BOOL:
			add_base_type<bool>(_data, pos, val, type);
			break;
		case TYPE::UINT8_T:
			add_base_type<uint8_t>(_data, pos, val, type);
			break;
		case TYPE::INT8_T:
			add_base_type<int8_t>(_data, pos, val, type);
			break;
		case TYPE::UINT16_T:
			add_base_type<uint16_t>(_data, pos, val, type);
			break;
		case TYPE::INT16_T:
			add_base_type<int16_t>(_data, pos, val, type);
			break;
		case TYPE::UINT32_T:
			add_base_type<uint32_t>(_data, pos, val, type);
			break;
		case TYPE::INT32_T:
			add_base_type<int32_t>(_data, pos, val, type);
			break;
		case TYPE::UINT64_T:
			add_base_type<uint64_t>(_data, pos, val, type);
			break;
		case TYPE::INT64_T:
			add_base_type<int64_t>(_data, pos, val, type);
			break;
		case TYPE::FLOAT:
			add_base_type<float>(_data, pos, val, type);
			break;
		case TYPE::DOUBLE:
			add_base_type<double>(_data, pos, val, type);
			break;
		case TYPE::STRING:
			{
				std::string* t1 = (std::string*)_data[pos];
				std::string* t2;
				switch(type)
				{
					case TYPE::NONE:
						break;
					case TYPE::BOOL:
					case TYPE::INT8_T:
						add_to_string<int8_t>(t1, val);
						break;
					case TYPE::INT16_T:
						add_to_string<int16_t>(t1, val);
						break;
					case TYPE::INT32_T:
						add_to_string<int32_t>(t1, val);
						break;
					case TYPE::INT64_T:
						add_to_string<int64_t>(t1, val);
						break;
					case TYPE::UINT8_T:
						add_to_string<uint8_t>(t1, val);
						break;
					case TYPE::UINT16_T:
						add_to_string<uint16_t>(t1, val);
						break;
					case TYPE::UINT32_T:
						add_to_string<uint32_t>(t1, val);
						break;
					case TYPE::UINT64_T:
						add_to_string<uint64_t>(t1, val);
						break;
					case TYPE::FLOAT:
						add_to_string<float>(t1, val);
						break;
					case TYPE::DOUBLE:
						add_to_string<double>(t1, val);
						break;
					case TYPE::STRING:
						t2 = (std::string*)val;
						*t1 += *t2;
						break;
				}
			}
			break;
	}
}

template <typename T>
inline void set_base_type(std::vector<uint64_t>& _data,
						  const size_t pos,
						  const uint64_t val,
						  const TYPE type)
{
	static_assert(type_check<T, bool, uint8_t, uint16_t,
							 uint32_t, uint64_t, int8_t,
							 int16_t, int32_t, int64_t,
							 float, double>::value,
                  "pass not supported type in function \"set_base_type\"");
	switch(type)
	{
		case TYPE::NONE:
			break;
		case TYPE::BOOL:
		case TYPE::UINT8_T:
		case TYPE::INT8_T:
		case TYPE::UINT16_T:
		case TYPE::INT16_T:
		case TYPE::UINT32_T:
		case TYPE::INT32_T:
		case TYPE::UINT64_T:
		case TYPE::INT64_T:
			_data[pos] = val;
			break;
		case TYPE::FLOAT:
			/*for float & double not need cast data, but since they
			  stores as uint64_t, for other types need cast*/
			if constexpr (type_check<T, float, double>::value)
				_data[pos] = val;
			else
				_data[pos] = reinterpret_cast<const float&>(val);
			break;
		case TYPE::DOUBLE:
			/*for float & double not need cast data, but since they
			  stores as uint64_t, for other types need cast*/
			if constexpr (type_check<T, float, double>::value)
				_data[pos] = val;
			else
				_data[pos] = reinterpret_cast<const double&>(val);
			break;
		case TYPE::STRING:
			{
				std::string* tmp = (std::string*)val;
				if constexpr (std::is_same<T, bool>::value)
				{
					if(*tmp == "true")
						reinterpret_cast<bool&>(_data[pos]) = 1;
					else if(*tmp == "false")
						reinterpret_cast<bool&>(_data[pos]) = 0;
				}
				else
					reinterpret_cast<T&>(_data[pos]) = str_to_num<T>(tmp);
			}
			break;
	}
}

void Column::set(const size_t pos, const uint64_t val, const TYPE type)
{
	switch(this->_type)
	{
		case TYPE::NONE:
			break;
		case TYPE::BOOL:
			set_base_type<bool>(_data, pos, val, type);
			break;
		case TYPE::INT8_T:
			set_base_type<int8_t>(_data, pos, val, type);
			break;
		case TYPE::INT16_T:
			set_base_type<int16_t>(_data, pos, val, type);
			break;
		case TYPE::INT32_T:
			set_base_type<int32_t>(_data, pos, val, type);
			break;
		case TYPE::INT64_T:
			set_base_type<int64_t>(_data, pos, val, type);
			break;
		case TYPE::UINT8_T:
			set_base_type<uint8_t>(_data, pos, val, type);
			break;
		case TYPE::UINT16_T:
			set_base_type<uint16_t>(_data, pos, val, type);
			break;
		case TYPE::UINT32_T:
			set_base_type<uint32_t>(_data, pos, val, type);
			break;
		case TYPE::UINT64_T:
			set_base_type<uint64_t>(_data, pos, val, type);
			break;
		case TYPE::FLOAT:
			set_base_type<float>(_data, pos, val, type);
			break;
		case TYPE::DOUBLE:
			set_base_type<double>(_data, pos, val, type);
			break;
		case TYPE::STRING:
			{
				std::string* t1 = (std::string*)_data[pos];
				std::string* t2;
				switch(type)
				{
					case TYPE::NONE:
						break;
					case TYPE::BOOL:
						if (val != 0)
							*t1 = "true";
						else
							*t1 = "false";
						break;
					case TYPE::UINT8_T:
						*t1 = std::to_string(static_cast<uint8_t>(val));
						break;
					case TYPE::UINT16_T:
						*t1 = std::to_string(static_cast<uint16_t>(val));
						break;
					case TYPE::UINT32_T:
						*t1 = std::to_string(static_cast<uint32_t>(val));
						break;
					case TYPE::UINT64_T:
						*t1 = std::to_string(static_cast<uint64_t>(val));
						break;
					case TYPE::INT8_T:
						*t1 = std::to_string(static_cast<int8_t>(val));
						break;
					case TYPE::INT16_T:
						*t1 = std::to_string(static_cast<int16_t>(val));
						break;
					case TYPE::INT32_T:
						*t1 = std::to_string(static_cast<int32_t>(val));
						break;
					case TYPE::INT64_T:
						*t1 = std::to_string(static_cast<int64_t>(val));
						break;
					case TYPE::FLOAT:
//						*t1 = std::to_string(reinterpret_cast<float>(val));
						break;
					case TYPE::DOUBLE:
//						*t1 = std::to_string(reinterpret_cast<double>(val));
						break;
					case TYPE::STRING:
						t2 = (std::string*)val;
						*t1 = *t2;
						break;
				}
			}
		break;
	}
}
