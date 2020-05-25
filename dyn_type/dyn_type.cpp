#include "dyn_type.h"
#include <algorithm>
#include <string>
#include <iostream>

using namespace db_col;

std::ostream& db_col::operator<< (std::ostream& os, const TYPE& type)
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

Column::Column(const Column&& col) :
	_type{col.type()},
	_data{col._data}
{}

Column::~Column()
{
	if (_type == TYPE::STRING)
	{
		for (size_t i = 0, size = _data.size(); i < size; i++)
		{
			delete (std::string*)_data[i];
		}
	}
}

Column& Column::operator=(const Column& col)
{
	this->_type = col.type();
	this->_data = col.all_data();
	return *this;
}

Column& Column::operator=(const Column&& col)
{
	this->_type = col.type();
	this->_data = col.all_data();
	return *this;
}

TYPE Column::type() const
{
	return this->_type;
}

size_t Column::size() const
{
	return this->_data.size();
}

void Column::reserve(const size_t size)
{
	_data.reserve(size);
}

void Column::push_back(const uint64_t data)
{
	_data.push_back(data);
}

void Column::resize(const size_t size)
{
	_data.resize(size, 0);
}

void Column::set_data(const size_t pos, const uint64_t data)
{
	if (_data.size() > pos)
	{
		_data[pos] = data;
	}
	else
	{
		resize(pos + 1);
		_data[pos] = data;
	}
}

bool check_num_in_str(const std::string* const str)
{
	/*come down*/
	char* ptr;
	strtod(str->c_str(), &ptr);
	return *ptr == 0;
}

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

uint64_t Column::set_type_data(uint64_t data,
							   const TYPE type_now,
							   const TYPE type_will) const
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

void Column::set_type_column(const TYPE type)
{
	for (size_t i = 0, size = _data.size(); i < size; i++)
	{
		_data[i] = set_type_data(_data[i], this->_type, type);
	}
	this->_type = type;
}

const std::vector<uint64_t>& Column::all_data() const
{
	return this->_data;
}

void Column::print_stdout(const size_t pos) const
{
	switch(this->_type)
	{
		case TYPE::NONE:
			break;
		case TYPE::BOOL:
			std::cout << static_cast<bool>(_data[pos]);
			break;
		case TYPE::UINT8_T:
			std::cout << static_cast<uint8_t>(_data[pos]);
			break;
		case TYPE::INT8_T:
			std::cout << static_cast<int8_t>(_data[pos]);
			break;
		case TYPE::UINT16_T:
			std::cout << static_cast<uint16_t>(_data[pos]);
			break;
		case TYPE::INT16_T:
			std::cout << static_cast<int16_t>(_data[pos]);
			break;
		case TYPE::UINT32_T:
			std::cout << static_cast<uint32_t>(_data[pos]);
			break;
		case TYPE::INT32_T:
			std::cout << static_cast<int32_t>(_data[pos]);
			break;
		case TYPE::UINT64_T:
			std::cout << static_cast<uint64_t>(_data[pos]);
			break;
		case TYPE::INT64_T:
			std::cout << static_cast<int64_t>(_data[pos]);
			break;
		case TYPE::FLOAT:
			std::cout << reinterpret_cast<const float&>(_data[pos]);
			break;
		case TYPE::DOUBLE:
			std::cout << reinterpret_cast<const double&>(_data[pos]);
			break;
		case TYPE::STRING:
			std::string* this_str = (std::string*)_data[pos];
			if (this_str == nullptr)
			{
				std::cout << "";
			}
			else
			{
				std::cout << *((std::string*)_data[pos]);
			}
			break;
	}
}

void Column::print_type_stdout() const
{
	std::cout << "type = " << this->_type;
}

void Column::print_all_stdout() const
{
	for (size_t i = 0, size = _data.size(); i < size; i++)
	{
		std::cout << i << ": ";
		print_stdout(i);
		std::cout << '\n';
	}
}

void Column::print_info_stdout() const
{
	print_type_stdout();
	std::cout << '\n';
	print_all_stdout();
	std::cout << std::endl;
}

uint64_t Column::data(const size_t pos) const
{
	return _data[pos];
}

void Column::set(const size_t pos, uint64_t val, const TYPE type)
{
	if(this->_type == TYPE::NONE)
		return;

	if (this->_type == type)
	{
		if (this->_type == TYPE::BOOL)
		{
			_data[pos] = val;
		}
		else if (this->_type == TYPE::STRING)
		{
			std::string* this_str = (std::string*)this->_data[pos];
			*this_str += *(std::string*)val;
		}
	}
	else
	{
		val = set_type_data(val, type, this->_type);
	}

	_data[pos] = val;
}

void Column::add(const size_t pos, uint64_t val, const TYPE type)
{
	if ((this->_type == TYPE::NONE) ||
	    (this->_type == TYPE::BOOL))
		return;
	
	if (this->_type == type)
	{
		if (this->_type == TYPE::STRING)
		{
			std::string* this_str = (std::string*)this->_data[pos];
			*this_str += *(std::string*)val;
		}
	}
	else
	{
		val = set_type_data(val, type, this->_type);
	}

	switch(this->_type)
	{
		case TYPE::UINT8_T:
			reinterpret_cast<uint8_t&>(_data[pos]) += static_cast<uint8_t>(val);
			break;
		case TYPE::INT8_T:
			reinterpret_cast<int8_t&>(_data[pos]) += static_cast<int8_t>(val);
			break;
		case TYPE::UINT16_T:
			reinterpret_cast<uint16_t&>(_data[pos]) += static_cast<uint16_t>(val);
			break;
		case TYPE::INT16_T:
			reinterpret_cast<int16_t&>(_data[pos]) += static_cast<int16_t>(val);
			break;
		case TYPE::UINT32_T:
			reinterpret_cast<uint32_t&>(_data[pos]) += static_cast<uint32_t>(val);
			break;
		case TYPE::INT32_T:
			reinterpret_cast<int32_t&>(_data[pos]) += static_cast<int32_t>(val);
			break;
		case TYPE::UINT64_T:
			reinterpret_cast<uint64_t&>(_data[pos]) += static_cast<uint64_t>(val);
			break;
		case TYPE::INT64_T:
			reinterpret_cast<int64_t&>(_data[pos]) += static_cast<int64_t>(val);
			break;
		case TYPE::FLOAT:
			reinterpret_cast<float&>(_data[pos]) += reinterpret_cast<float&>(val);
			break;
		case TYPE::DOUBLE:
			reinterpret_cast<double&>(_data[pos]) += reinterpret_cast<double&>(val);
			break;
		default:
			break;
	}
}

void Column::sub(const size_t pos, uint64_t val, const TYPE type)
{
	if((this->_type == TYPE::NONE) ||
	   (this->_type == TYPE::BOOL) ||
	   (this->_type == TYPE::STRING))
		return;

	if(this->_type != type)
	{
		val = set_type_data(val, type, this->_type);
	}
	switch(this->_type)
	{
		case TYPE::UINT8_T:
			reinterpret_cast<uint8_t&>(_data[pos]) -= static_cast<uint8_t>(val);
			break;
		case TYPE::INT8_T:
			reinterpret_cast<int8_t&>(_data[pos]) -= static_cast<int8_t>(val);
			break;
		case TYPE::UINT16_T:
			reinterpret_cast<uint16_t&>(_data[pos]) -= static_cast<uint16_t>(val);
			break;
		case TYPE::INT16_T:
			reinterpret_cast<int16_t&>(_data[pos]) -= static_cast<int16_t>(val);
			break;
		case TYPE::UINT32_T:
			reinterpret_cast<uint32_t&>(_data[pos]) -= static_cast<uint32_t>(val);
			break;
		case TYPE::INT32_T:
			reinterpret_cast<int32_t&>(_data[pos]) -= static_cast<int32_t>(val);
			break;
		case TYPE::UINT64_T:
			reinterpret_cast<uint64_t&>(_data[pos]) -= static_cast<uint64_t>(val);
			break;
		case TYPE::INT64_T:
			reinterpret_cast<int64_t&>(_data[pos]) -= static_cast<int64_t>(val);
			break;
		case TYPE::FLOAT:
			reinterpret_cast<float&>(_data[pos]) -= reinterpret_cast<float&>(val);
			break;
		case TYPE::DOUBLE:
			reinterpret_cast<double&>(_data[pos]) -= reinterpret_cast<double&>(val);
			break;
		default:
			break;
	}
}

void Column::mul(const size_t pos, uint64_t val, const TYPE type)
{
	if((this->_type == TYPE::NONE) ||
	   (this->_type == TYPE::BOOL) ||
	   (this->_type == TYPE::STRING))
		return;

	if(this->_type != type)
	{
		val = set_type_data(val, type, this->_type);
	}
	switch(this->_type)
	{
		case TYPE::UINT8_T:
			reinterpret_cast<uint8_t&>(_data[pos]) *= static_cast<uint8_t>(val);
			break;
		case TYPE::INT8_T:
			reinterpret_cast<int8_t&>(_data[pos]) *= static_cast<int8_t>(val);
			break;
		case TYPE::UINT16_T:
			reinterpret_cast<uint16_t&>(_data[pos]) *= static_cast<uint16_t>(val);
			break;
		case TYPE::INT16_T:
			reinterpret_cast<int16_t&>(_data[pos]) *= static_cast<int16_t>(val);
			break;
		case TYPE::UINT32_T:
			reinterpret_cast<uint32_t&>(_data[pos]) *= static_cast<uint32_t>(val);
			break;
		case TYPE::INT32_T:
			reinterpret_cast<int32_t&>(_data[pos]) *= static_cast<int32_t>(val);
			break;
		case TYPE::UINT64_T:
			reinterpret_cast<uint64_t&>(_data[pos]) *= static_cast<uint64_t>(val);
			break;
		case TYPE::INT64_T:
			reinterpret_cast<int64_t&>(_data[pos]) *= static_cast<int64_t>(val);
			break;
		case TYPE::FLOAT:
			reinterpret_cast<float&>(_data[pos]) *= reinterpret_cast<float&>(val);
			break;
		case TYPE::DOUBLE:
			reinterpret_cast<double&>(_data[pos]) *= reinterpret_cast<double&>(val);
			break;
		default:
			break;
	}
}

void Column::div(const size_t pos, uint64_t val, const TYPE type)
{
	if((this->_type == TYPE::NONE) ||
	   (this->_type == TYPE::BOOL) ||
	   (this->_type == TYPE::STRING))
		return;

	if(this->_type != type)
	{
		val = set_type_data(val, type, this->_type);
	}
	switch(this->_type)
	{
		case TYPE::UINT8_T:
			reinterpret_cast<uint8_t&>(_data[pos]) /= static_cast<uint8_t>(val);
			break;
		case TYPE::INT8_T:
			reinterpret_cast<int8_t&>(_data[pos]) /= static_cast<int8_t>(val);
			break;
		case TYPE::UINT16_T:
			reinterpret_cast<uint16_t&>(_data[pos]) /= static_cast<uint16_t>(val);
			break;
		case TYPE::INT16_T:
			reinterpret_cast<int16_t&>(_data[pos]) /= static_cast<int16_t>(val);
			break;
		case TYPE::UINT32_T:
			reinterpret_cast<uint32_t&>(_data[pos]) /= static_cast<uint32_t>(val);
			break;
		case TYPE::INT32_T:
			reinterpret_cast<int32_t&>(_data[pos]) /= static_cast<int32_t>(val);
			break;
		case TYPE::UINT64_T:
			reinterpret_cast<uint64_t&>(_data[pos]) /= static_cast<uint64_t>(val);
			break;
		case TYPE::INT64_T:
			reinterpret_cast<int64_t&>(_data[pos]) /= static_cast<int64_t>(val);
			break;
		case TYPE::FLOAT:
			reinterpret_cast<float&>(_data[pos]) /= reinterpret_cast<float&>(val);
			break;
		case TYPE::DOUBLE:
			reinterpret_cast<double&>(_data[pos]) /= reinterpret_cast<double&>(val);
			break;
		default:
			break;
	}
}

bool Column::equal(const size_t pos, const uint64_t val, const TYPE type) const
{
	switch(this->_type)
	{
		case TYPE::NONE:
			return type == TYPE::NONE;
		case TYPE::BOOL:
			return ((type == TYPE::BOOL) ? (_data[pos] == val) : false);
		case TYPE::UINT8_T:
		case TYPE::INT8_T:
		case TYPE::UINT16_T:
		case TYPE::INT16_T:
		case TYPE::UINT32_T:
		case TYPE::INT32_T:
		case TYPE::UINT64_T:
		case TYPE::INT64_T:
			{
			bool check_type = ((type == TYPE::NONE)  || (type == TYPE::STRING) ||
							   (type == TYPE::FLOAT) || (type == TYPE::DOUBLE));
			return ((check_type) ? (_data[pos] == val) : false);
			}
		case TYPE::FLOAT:
			if (type == TYPE::FLOAT)
			{
				const float& this_float = reinterpret_cast<const float&>(_data[pos]);
				const float& arg_float  = reinterpret_cast<const float&>(val);
				return this_float == arg_float;
			}
			else if (type == TYPE::DOUBLE)
			{
				const float& this_float = reinterpret_cast<const float&>(_data[pos]);
				const double& arg_float = reinterpret_cast<const double&>(val);
				return this_float == arg_float;
			}
			return false;
		case TYPE::DOUBLE:
			if (type == TYPE::FLOAT)
			{
				const double& this_float = reinterpret_cast<const double&>(_data[pos]);
				const float& arg_float = reinterpret_cast<const float&>(val);
				return this_float == arg_float;
			}
			else if (type == TYPE::DOUBLE)
			{
				const double& this_float = reinterpret_cast<const double&>(_data[pos]);
				const double& arg_float = reinterpret_cast<const double&>(val);
				return this_float == arg_float;
			}
			return false;
		case TYPE::STRING:
			{
			const std::string* const this_string = (std::string*)_data[pos];
			const std::string* const arg_string  = (std::string*)val;
			return ((type == TYPE::STRING) ? (*this_string == *arg_string) : false);
			}
	}
	return false;
}
