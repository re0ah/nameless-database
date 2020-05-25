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
constexpr inline void set_type_base(std::vector<uint64_t>& this_vector,
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
			for (size_t i = 0, size = this_vector.size(); i < size; i++)
			{
				if constexpr (type_check<T, uint8_t, uint16_t,
										 uint32_t, uint64_t>::value)
				{ /*unsigned -> unsigned*/
					this_vector[i] = static_cast<T>(this_vector[i]);
				}
				else if(type_check<T, int8_t, int16_t,
								   int32_t, int64_t>::value)
				{ /*signed -> unsigned*/
					this_vector[i] = abs(reinterpret_cast<T&>(this_vector[i]));
				}
				else
				{ /*float | double -> unsigned*/
					this_vector[i] = reinterpret_cast<T&>(this_vector[i]);
				}
			}
			break;
		case TYPE::INT8_T:
		case TYPE::INT16_T:
		case TYPE::INT32_T:
		case TYPE::INT64_T:
			for (size_t i = 0, size = this_vector.size(); i < size; i++)
			{
				if constexpr (type_check<T, uint8_t, uint16_t,
										 uint32_t, uint64_t>::value)
				{ /*unsigned -> signed*/
					this_vector[i] = abs(static_cast<T>(this_vector[i]));
				}
				else if(type_check<T, int8_t, int16_t,
								   int32_t, int64_t>::value)
				{ /*signed -> signed*/
					this_vector[i] = static_cast<T>(this_vector[i]);
				}
				else
				{ /*float | double -> signed*/
					this_vector[i] = reinterpret_cast<T&>(this_vector[i]);
				}
			}
			break;
		case TYPE::FLOAT:
			for (size_t i = 0, size = this_vector.size(); i < size; i++)
			{
				if constexpr (type_check<T, uint8_t, uint16_t, int8_t,
										 int16_t, int32_t, int64_t,
										 uint32_t, uint64_t>::value)
				{ /*unsigned -> float*/
					float& f = reinterpret_cast<float&>(this_vector[i]);
					f = static_cast<T>(this_vector[i]);
				}
				else if(type_check<T, double>::value)
				{ /*double -> float*/
					double& d = reinterpret_cast<double&>(this_vector[i]);
					float&  f = reinterpret_cast<float&>(this_vector[i]);
					f = static_cast<float>(d);
				}
			}
			break;
		case TYPE::DOUBLE:
			for (size_t i = 0, size = this_vector.size(); i < size; i++)
			{
				if constexpr (type_check<T, uint8_t, uint16_t, int8_t,
										 int16_t, int32_t, int64_t,
										 uint32_t, uint64_t>::value)
				{ /*unsigned -> double*/
					double& d = reinterpret_cast<double&>(this_vector[i]);
					d = static_cast<T>(this_vector[i]);
				}
				else if(type_check<T, float>::value)
				{ /*float -> double*/
					double& d = reinterpret_cast<double&>(this_vector[i]);
					float&  f = reinterpret_cast<float&>(this_vector[i]);
					d = static_cast<double>(f);
				}
			}
			break;
		case TYPE::STRING:
			for (size_t i = 0, size = this_vector.size(); i < size; i++)
			{
				const T data_value = reinterpret_cast<T&>(this_vector[i]);
				if constexpr(std::is_same<T, bool>::value)
				{
					std::string* str = new std::string(((data_value) ? "true" : "false"));
					this_vector[i] = (uint64_t)str;
				}
				else
				{
					std::string* str = new std::string(std::to_string(data_value));
					this_vector[i] = (uint64_t)str;
				}
			}
			break;
	}
}

template <typename T>
constexpr inline void set_type_string(std::vector<uint64_t>& this_vector,
						 		 	  const TYPE type)
{
	static_assert(type_check<T, bool, uint8_t, uint16_t,
							 uint32_t, uint64_t, int8_t,
							 int16_t, int32_t, int64_t,
							 float, double>::value,
    			             "pass not supported type in function \"set_type_string\"");
	for (size_t i = 0, size = this_vector.size(); i < size; i++)
	{
		std::string* this_string = (std::string*)this_vector[i];
		if constexpr(type_check<T, bool>::value)
		{
			const bool new_value = (*this_string == "true");
			this_vector[i] = new_value;
		}
		else
		{
			if (check_num_in_str(this_string))
			{
				const T new_value = str_to_num<T>(this_string);
				this_vector[i] = new_value;
			}
			else
			{
				this_vector[i] = 0;
			}
		}
		delete this_string;
	}
}

void Column::set_type(const TYPE type)
{
	if (this->_type == type)
		return;
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
	switch(this->_type)
	{
		case TYPE::NONE:
			break;
		case TYPE::BOOL:
			set_type_base<bool>(this->_data, type);
			break;
		case TYPE::UINT8_T:
			set_type_base<uint8_t>(this->_data, type);
			break;
		case TYPE::INT8_T:
			set_type_base<int8_t>(this->_data, type);
			break;
		case TYPE::UINT16_T:
			set_type_base<uint16_t>(this->_data, type);
			break;
		case TYPE::INT16_T:
			set_type_base<int16_t>(this->_data, type);
			break;
		case TYPE::UINT32_T:
			set_type_base<uint32_t>(this->_data, type);
			break;
		case TYPE::INT32_T:
			set_type_base<int32_t>(this->_data, type);
			break;
		case TYPE::UINT64_T:
			set_type_base<uint64_t>(this->_data, type);
			break;
		case TYPE::INT64_T:
			set_type_base<int64_t>(this->_data, type);
			break;
		case TYPE::FLOAT:
			set_type_base<float>(this->_data, type);
			break;
		case TYPE::DOUBLE:
			set_type_base<double>(this->_data, type);
			break;
		case TYPE::STRING:
			switch(type)
			{
				case TYPE::NONE:
				case TYPE::STRING:
					break;
				case TYPE::BOOL:
					set_type_string<bool>(this->_data, type);
					break;
				case TYPE::UINT8_T:
					set_type_string<uint8_t>(this->_data, type);
					break;
				case TYPE::INT8_T:
					set_type_string<int8_t>(this->_data, type);
					break;
				case TYPE::UINT16_T:
					set_type_string<uint16_t>(this->_data, type);
					break;
				case TYPE::INT16_T:
					set_type_string<int16_t>(this->_data, type);
					break;
				case TYPE::UINT32_T:
					set_type_string<uint32_t>(this->_data, type);
					break;
				case TYPE::INT32_T:
					set_type_string<int32_t>(this->_data, type);
					break;
				case TYPE::UINT64_T:
					set_type_string<uint64_t>(this->_data, type);
					break;
				case TYPE::INT64_T:
					set_type_string<int64_t>(this->_data, type);
					break;
				case TYPE::FLOAT:
					set_type_string<float>(this->_data, type);
					break;
				case TYPE::DOUBLE:
					set_type_string<double>(this->_data, type);
					break;
			}
			break;
	}
	_type = type;
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
			std::cout << *((double*)_data[pos]);
			break;
		case TYPE::DOUBLE:
			std::cout << *((double*)_data[pos]);
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
		case TYPE::STRING:
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
			{
				_data[pos] = val;
			}
			else
			{
				_data[pos] = reinterpret_cast<const float&>(val);
			}
			break;
		case TYPE::DOUBLE:
			/*for float & double not need cast data, but since they
			  stores as uint64_t, for other types need cast*/
			if constexpr (type_check<T, float, double>::value)
			{
				_data[pos] = val;
			}
			else
			{
				_data[pos] = reinterpret_cast<const double&>(val);
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
			set_base_type<bool>(this->_data, pos, val, type);
			break;
		case TYPE::INT8_T:
			set_base_type<int8_t>(this->_data, pos, val, type);
			break;
		case TYPE::INT16_T:
			set_base_type<int16_t>(this->_data, pos, val, type);
			break;
		case TYPE::INT32_T:
			set_base_type<int32_t>(this->_data, pos, val, type);
			break;
		case TYPE::INT64_T:
			set_base_type<int64_t>(this->_data, pos, val, type);
			break;
		case TYPE::UINT8_T:
			set_base_type<uint8_t>(this->_data, pos, val, type);
			break;
		case TYPE::UINT16_T:
			set_base_type<uint16_t>(this->_data, pos, val, type);
			break;
		case TYPE::UINT32_T:
			set_base_type<uint32_t>(this->_data, pos, val, type);
			break;
		case TYPE::UINT64_T:
			set_base_type<uint64_t>(this->_data, pos, val, type);
			break;
		case TYPE::FLOAT:
			set_base_type<float>(this->_data, pos, val, type);
			break;
		case TYPE::DOUBLE:
			set_base_type<double>(this->_data, pos, val, type);
			break;
		case TYPE::STRING:
			if (type == TYPE::STRING)
			{
				*((std::string*)this->_data[pos]) = *((std::string*)val);
			}
			break;
	}
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
			{
				reinterpret_cast<T&>(_data[pos]) += reinterpret_cast<const T&>(val);
			}
			else
			{
				reinterpret_cast<T&>(_data[pos]) += static_cast<T>(val);
			}
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

void Column::add(const size_t pos, const uint64_t val, const TYPE type)
{
	switch(this->_type)
	{
		case TYPE::NONE:
			break;
		case TYPE::BOOL:
			add_base_type<bool>(this->_data, pos, val, type);
			break;
		case TYPE::UINT8_T:
			add_base_type<uint8_t>(this->_data, pos, val, type);
			break;
		case TYPE::INT8_T:
			add_base_type<int8_t>(this->_data, pos, val, type);
			break;
		case TYPE::UINT16_T:
			add_base_type<uint16_t>(this->_data, pos, val, type);
			break;
		case TYPE::INT16_T:
			add_base_type<int16_t>(this->_data, pos, val, type);
			break;
		case TYPE::UINT32_T:
			add_base_type<uint32_t>(this->_data, pos, val, type);
			break;
		case TYPE::INT32_T:
			add_base_type<int32_t>(this->_data, pos, val, type);
			break;
		case TYPE::UINT64_T:
			add_base_type<uint64_t>(this->_data, pos, val, type);
			break;
		case TYPE::INT64_T:
			add_base_type<int64_t>(this->_data, pos, val, type);
			break;
		case TYPE::FLOAT:
			add_base_type<float>(this->_data, pos, val, type);
			break;
		case TYPE::DOUBLE:
			add_base_type<double>(this->_data, pos, val, type);
			break;
		case TYPE::STRING:
			if (type == TYPE::STRING)
			{
				std::string* this_str = (std::string*)this->_data[pos];
				*this_str += *(std::string*)val;
			}
			break;
	}
}

template <typename T>
inline void sub_base_type(std::vector<uint64_t>& _data,
						  const size_t pos,
						  const uint64_t val,
						  const TYPE type)
{
	static_assert(type_check<T, bool, uint8_t, uint16_t,
							 uint32_t, uint64_t, int8_t,
							 int16_t, int32_t, int64_t,
							 float, double, std::string*>::value,
                  "pass not supported type in function \"sub_base_type\"");
	switch(type)
	{
		case TYPE::NONE:
		case TYPE::STRING:
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
				reinterpret_cast<bool&>(_data[pos]) -= static_cast<bool>(val);
				break;
			}
		case TYPE::FLOAT:
		case TYPE::DOUBLE:
			if constexpr (type_check<T, float, double>::value)
			{
				reinterpret_cast<T&>(_data[pos]) -= reinterpret_cast<const T&>(val);
			}
			else
			{
				reinterpret_cast<T&>(_data[pos]) -= static_cast<T>(val);
			}
			break;
	}
}

void Column::sub(const size_t pos, const uint64_t val, const TYPE type)
{
	switch(this->_type)
	{
		case TYPE::NONE:
		case TYPE::STRING:
			break;
		case TYPE::BOOL:
			sub_base_type<bool>(this->_data, pos, val, type);
			break;
		case TYPE::UINT8_T:
			sub_base_type<uint8_t>(this->_data, pos, val, type);
			break;
		case TYPE::INT8_T:
			sub_base_type<int8_t>(this->_data, pos, val, type);
			break;
		case TYPE::UINT16_T:
			sub_base_type<uint16_t>(this->_data, pos, val, type);
			break;
		case TYPE::INT16_T:
			sub_base_type<int16_t>(this->_data, pos, val, type);
			break;
		case TYPE::UINT32_T:
			sub_base_type<uint32_t>(this->_data, pos, val, type);
			break;
		case TYPE::INT32_T:
			sub_base_type<int32_t>(this->_data, pos, val, type);
			break;
		case TYPE::UINT64_T:
			sub_base_type<uint64_t>(this->_data, pos, val, type);
			break;
		case TYPE::INT64_T:
			sub_base_type<int64_t>(this->_data, pos, val, type);
			break;
		case TYPE::FLOAT:
			sub_base_type<float>(this->_data, pos, val, type);
			break;
		case TYPE::DOUBLE:
			sub_base_type<double>(this->_data, pos, val, type);
			break;
	}
}

template <typename T>
inline void mul_base_type(std::vector<uint64_t>& _data,
						  const size_t pos,
						  const uint64_t val,
						  const TYPE type)
{
	static_assert(type_check<T, uint8_t, uint16_t,
							 uint32_t, uint64_t, int8_t,
							 int16_t, int32_t, int64_t,
							 float, double, std::string*>::value,
                  "pass not supported type in function \"sub_base_type\"");
	switch(type)
	{
		case TYPE::NONE:
		case TYPE::STRING:
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
		case TYPE::FLOAT:
		case TYPE::DOUBLE:
			if constexpr (type_check<T, float, double>::value)
			{
				reinterpret_cast<T&>(_data[pos]) *= reinterpret_cast<const T&>(val);
			}
			else
			{
				reinterpret_cast<T&>(_data[pos]) *= static_cast<T>(val);
			}
			break;
	}
}

void Column::mul(const size_t pos, const uint64_t val, const TYPE type)
{
	switch(this->_type)
	{
		case TYPE::NONE:
		case TYPE::BOOL:
		case TYPE::STRING:
			break;
		case TYPE::UINT8_T:
			mul_base_type<uint8_t>(this->_data, pos, val, type);
			break;
		case TYPE::INT8_T:
			mul_base_type<int8_t>(this->_data, pos, val, type);
			break;
		case TYPE::UINT16_T:
			mul_base_type<uint16_t>(this->_data, pos, val, type);
			break;
		case TYPE::INT16_T:
			mul_base_type<int16_t>(this->_data, pos, val, type);
			break;
		case TYPE::UINT32_T:
			mul_base_type<uint32_t>(this->_data, pos, val, type);
			break;
		case TYPE::INT32_T:
			mul_base_type<int32_t>(this->_data, pos, val, type);
			break;
		case TYPE::UINT64_T:
			mul_base_type<uint64_t>(this->_data, pos, val, type);
			break;
		case TYPE::INT64_T:
			mul_base_type<int64_t>(this->_data, pos, val, type);
			break;
		case TYPE::FLOAT:
			mul_base_type<float>(this->_data, pos, val, type);
			break;
		case TYPE::DOUBLE:
			mul_base_type<double>(this->_data, pos, val, type);
			break;
	}
}

template <typename T>
inline void div_base_type(std::vector<uint64_t>& _data,
						  const size_t pos,
						  const uint64_t val,
						  const TYPE type)
{
	static_assert(type_check<T, uint8_t, uint16_t,
							 uint32_t, uint64_t, int8_t,
							 int16_t, int32_t, int64_t,
							 float, double, std::string*>::value,
                  "pass not supported type in function \"sub_base_type\"");
	switch(type)
	{
		case TYPE::NONE:
		case TYPE::STRING:
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
		case TYPE::FLOAT:
		case TYPE::DOUBLE:
			if constexpr (type_check<T, float, double>::value)
			{
				reinterpret_cast<T&>(_data[pos]) /= reinterpret_cast<const T&>(val);
			}
			else
			{
				reinterpret_cast<T&>(_data[pos]) /= static_cast<T>(val);
			}
			break;
	}
}

void Column::div(const size_t pos, const uint64_t val, const TYPE type)
{
	switch(this->_type)
	{
		case TYPE::NONE:
		case TYPE::BOOL:
		case TYPE::STRING:
			break;
		case TYPE::UINT8_T:
			div_base_type<uint8_t>(this->_data, pos, val, type);
			break;
		case TYPE::INT8_T:
			div_base_type<int8_t>(this->_data, pos, val, type);
			break;
		case TYPE::UINT16_T:
			div_base_type<uint16_t>(this->_data, pos, val, type);
			break;
		case TYPE::INT16_T:
			div_base_type<int16_t>(this->_data, pos, val, type);
			break;
		case TYPE::UINT32_T:
			div_base_type<uint32_t>(this->_data, pos, val, type);
			break;
		case TYPE::INT32_T:
			div_base_type<int32_t>(this->_data, pos, val, type);
			break;
		case TYPE::UINT64_T:
			div_base_type<uint64_t>(this->_data, pos, val, type);
			break;
		case TYPE::INT64_T:
			div_base_type<int64_t>(this->_data, pos, val, type);
			break;
		case TYPE::FLOAT:
			div_base_type<float>(this->_data, pos, val, type);
			break;
		case TYPE::DOUBLE:
			div_base_type<double>(this->_data, pos, val, type);
			break;
	}
}
