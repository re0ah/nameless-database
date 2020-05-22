#include "dyn_type.h"
#include <algorithm>

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
	const union Data tmp{._uint64_t{data}};
	_data.push_back(tmp);
}

const std::vector<Data>& Column::all_data() const
{
	return this->_data;
}

Column::~Column()
{
	switch(this->type())
	{
		case TYPE::STRING:
			size_t size = _data.size();
			for (int i = 0; i < size; i++)
			{
				delete (std::string*)_data[i]._struct;
			}
			break;
	}
}

TYPE Column::type() const
{
	return _type;
}

uint64_t Column::data(const size_t pos) const
{
	return _data[pos]._uint64_t;
}

/*
   inline template function for reduction same code. Also, allows sum int + string
   supported types:
   					1.  uint8_t
					2.  int8_t
					3.  uint16_t
					4.  int16_t
					5.  uint32_t
					6.  int32_t
					7.  uint64_t
					8.  int64_t
					9.  float
					10. double
 */
template <typename T>
inline void add_base_type(std::vector<Data>& _data,
						  const size_t pos,
						  const uint64_t val,
						  const TYPE type)
{
	T* ptr;
	/*type check and set pointer*/
	static_assert((std::is_same<T, bool>::value)         ||
                  (std::is_same<T, uint8_t>::value)      ||
                  (std::is_same<T, uint16_t>::value)     ||
                  (std::is_same<T, uint32_t>::value)     ||
                  (std::is_same<T, uint64_t>::value)     ||
                  (std::is_same<T, int8_t>::value)       ||
                  (std::is_same<T, int16_t>::value)      ||
                  (std::is_same<T, int32_t>::value)      ||
                  (std::is_same<T, int64_t>::value)      ||
                  (std::is_same<T, float>::value)        ||
                  (std::is_same<T, double>::value)       ||
                  (std::is_same<T, std::string*>::value),
                  "pass not supported type in function \"add_base_type\"");

	if constexpr (std::is_same<T, uint8_t>::value)
		ptr = &_data[pos]._uint8_t;
	else if constexpr (std::is_same<T, int8_t>::value)
		ptr = &_data[pos]._int8_t;
	else if constexpr (std::is_same<T, int16_t>::value)
		ptr = &_data[pos]._int16_t;
	else if constexpr (std::is_same<T, uint16_t>::value)
		ptr = &_data[pos]._uint16_t;
	else if constexpr (std::is_same<T, int32_t>::value)
		ptr = &_data[pos]._int32_t;
	else if constexpr (std::is_same<T, uint32_t>::value)
		ptr = &_data[pos]._uint32_t;
	else if constexpr (std::is_same<T, int64_t>::value)
		ptr = &_data[pos]._int64_t;
	else if constexpr (std::is_same<T, uint64_t>::value)
		ptr = &_data[pos]._uint64_t;
	else if constexpr (std::is_same<T, float>::value)
		ptr = &_data[pos]._float;
	else if constexpr (std::is_same<T, double>::value)
		ptr = &_data[pos]._double;
	switch(type)
	{
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
				_data[pos]._bool += static_cast<bool>(val);
				break;
			}
		case TYPE::FLOAT:
		case TYPE::DOUBLE:
			if constexpr (std::is_same<T, float>::value)
				*ptr += data_cast<float>(val);
			else if (std::is_same<T, double>::value)
				*ptr += data_cast<double>(val);
			else
				*ptr += static_cast<T>(val);
			break;
		case TYPE::STRING:
			{
				std::string* tmp = (std::string*)val;
				/*for another types need another function for transfer of string to T.
				  maybe create template function which on type solve for what type need call?*/
				if constexpr ((std::is_same<T, int8_t>::value)	||
							  (std::is_same<T, int16_t>::value) ||
							  (std::is_same<T, int32_t>::value) ||
							  (std::is_same<T, int64_t>::value))
				{
					*ptr += static_cast<T>(std::stoll(*tmp));
				}
				else if ((std::is_same<T, uint8_t>::value)  ||
				  		 (std::is_same<T, uint16_t>::value) ||
						 (std::is_same<T, uint32_t>::value) ||
						 (std::is_same<T, uint64_t>::value))
				{
					*ptr += static_cast<T>(std::stoull(*tmp));
				}
				else if (std::is_same<T, float>::value)
				{
					*ptr += static_cast<T>(std::stof(*tmp));
				}
				else if (std::is_same<T, double>::value)
				{
					*ptr += static_cast<T>(std::stod(*tmp));
				}
			}
			break;
	}
}

/*СЕМЬ ОЗОРНЫХ ШАГОВ ЗА ГОРИЗОНТ, СЕМЬ ЛЕДЯНЫХ МОСТОВ ЗА ГОРИЗОНТ, СЕМЬ ПРОЛИВНЫХ ДОЖДЕЙ ЗА ГОРИЗОНТ*/

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

/*
   inline template function for reduction same code. Used in method "add" in case "STRING"
   supported types:
   					1.  bool
   					2.  uint8_t
					3.  int8_t
					4.  uint16_t
					5.  int16_t
					6.  uint32_t
					7.  int32_t
					8.  uint64_t
					9.  int64_t
					10. float
					11. double
*/
template <typename T>
inline void add_to_string(std::string* str, const uint64_t val)
{
	/*type check*/
	static_assert((std::is_same<T, bool>::value)	 ||
				  (std::is_same<T, uint8_t>::value)  ||
				  (std::is_same<T, int8_t>::value)   ||
				  (std::is_same<T, uint16_t>::value) ||
				  (std::is_same<T, int16_t>::value)  ||
				  (std::is_same<T, uint32_t>::value) ||
				  (std::is_same<T, int32_t>::value)  ||
				  (std::is_same<T, uint64_t>::value) ||
				  (std::is_same<T, int64_t>::value)  ||
				  (std::is_same<T, float>::value)    ||
				  (std::is_same<T, double>::value),
				  "pass not supported type for function \"add_to_string\"");
	if (check_num_in_str(str)) /*if number*/
	{
		/*need do:
		 	  1. transform string to number
			  2. sum this number with argument "val"
			  3. transform result to string and store
		*/
		T tmp;
		if constexpr (std::is_same<T, float>::value)
			tmp = std::stof(*str) + data_cast<float>(val);
		else if (std::is_same<T, double>::value)
			tmp = std::stod(*str) + data_cast<double>(val);
		else if ((std::is_same<T, uint8_t>::value)  ||
				 (std::is_same<T, uint16_t>::value) ||
				 (std::is_same<T, uint32_t>::value) ||
				 (std::is_same<T, uint64_t>::value))
			tmp = std::stoull(*str) + static_cast<T>(val);
		else
			tmp = std::stoll(*str) + static_cast<T>(val);
		*str = std::to_string(tmp);
	}
	else /*if not number*/
	{
		/*need do:
		 	  1. transform number to string
			  2. concate this number with string
		*/
		if constexpr(std::is_same<T, float>::value)
			*str += std::to_string(data_cast<float>(val));
		else if (std::is_same<T, double>::value)
			*str += std::to_string(data_cast<double>(val));
		else
			*str += std::to_string(static_cast<T>(val));
	}
}

void Column::add(const size_t pos, const uint64_t val, const TYPE type)
{
	switch(this->_type)
	{
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
				std::string* t1 = (std::string*)_data[pos]._struct;
				std::string* t2;
				switch(type)
				{
					case TYPE::BOOL:
					case TYPE::INT8_T:
					case TYPE::INT16_T:
					case TYPE::INT32_T:
					case TYPE::INT64_T:
						add_to_string<int64_t>(t1, val);
						break;
					case TYPE::UINT8_T:
					case TYPE::UINT16_T:
					case TYPE::UINT32_T:
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

/*
   inline template function for reduction same code. Used in method "set"
   supported types:
   					1.  bool
   					2.  uint8_t
					3.  int8_t
					4.  uint16_t
					5.  int16_t
					6.  uint32_t
					7.  int32_t
					8.  uint64_t
					9.  int64_t
					10. float
					11. double
 */
template <typename T>
inline void set_base_type(std::vector<Data>& _data,
						  const size_t pos,
						  const uint64_t val,
						  const TYPE type)
{
	/*type check*/
	static_assert((std::is_same<T, bool>::value)	 ||
				  (std::is_same<T, uint8_t>::value)  ||
				  (std::is_same<T, int8_t>::value)   ||
				  (std::is_same<T, uint16_t>::value) ||
				  (std::is_same<T, int16_t>::value)  ||
				  (std::is_same<T, uint32_t>::value) ||
				  (std::is_same<T, int32_t>::value)  ||
				  (std::is_same<T, uint64_t>::value) ||
				  (std::is_same<T, int64_t>::value)  ||
				  (std::is_same<T, float>::value)    ||
				  (std::is_same<T, double>::value),
				  "pass not supported type for function \"set_base_type\"");
	switch(type)
	{
		case TYPE::BOOL:
		case TYPE::UINT8_T:
		case TYPE::INT8_T:
		case TYPE::UINT16_T:
		case TYPE::INT16_T:
		case TYPE::UINT32_T:
		case TYPE::INT32_T:
		case TYPE::UINT64_T:
		case TYPE::INT64_T:
			_data[pos]._uint64_t = val;
			break;
		case TYPE::FLOAT:
			if constexpr ((std::is_same<T, float>::value) ||
						  (std::is_same<T, double>::value))
				_data[pos]._uint64_t = val;
			else
				_data[pos]._uint64_t = data_cast<float>(val);
			break;
		case TYPE::DOUBLE:
			if constexpr ((std::is_same<T, float>::value) ||
						  (std::is_same<T, double>::value))
				_data[pos]._uint64_t = val;
			else
				_data[pos]._uint64_t = data_cast<double>(val);
			break;
		case TYPE::STRING:
			{
				std::string* tmp = (std::string*)val;
				if constexpr (std::is_same<T, bool>::value)
				{
					if(*tmp == "true")
						_data[pos]._bool = 1;
					else if(*tmp == "false")
						_data[pos]._bool = 0;
				}
				else if ((std::is_same<T, int8_t>::value)  ||
						 (std::is_same<T, int16_t>::value) ||
						 (std::is_same<T, int32_t>::value) ||
						 (std::is_same<T, int64_t>::value))
				{
					_data[pos]._int64_t = static_cast<T>(std::stoll(*tmp));
				}
				else if ((std::is_same<T, uint8_t>::value)  ||
				  		 (std::is_same<T, uint16_t>::value) ||
						 (std::is_same<T, uint32_t>::value) ||
						 (std::is_same<T, uint64_t>::value))
				{
					_data[pos]._uint64_t = static_cast<T>(std::stoull(*tmp));
				}
				else if (std::is_same<T, float>::value)
				{
					_data[pos]._float = static_cast<T>(std::stof(*tmp));
				}
				else if (std::is_same<T, double>::value)
				{
					_data[pos]._double = static_cast<T>(std::stod(*tmp));
				}
			}
			break;
	}
}

void Column::set(const size_t pos, const uint64_t val, const TYPE type)
{
	switch(this->_type)
	{
		case TYPE::BOOL:
			set_base_type<bool>(_data, pos, val, type);
			break;
		case TYPE::INT8_T:
		case TYPE::INT16_T:
		case TYPE::INT32_T:
		case TYPE::INT64_T:
			set_base_type<int64_t>(_data, pos, val, type);
		case TYPE::UINT8_T:
		case TYPE::UINT16_T:
		case TYPE::UINT32_T:
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
				std::string* t1 = (std::string*)_data[pos]._struct;
				std::string* t2;
				switch(type)
				{
					case TYPE::BOOL:
						if (val != 0)
							*t1 = "true";
						else
							*t1 = "false";
						break;
					case TYPE::UINT8_T:
					case TYPE::UINT16_T:
					case TYPE::UINT32_T:
					case TYPE::UINT64_T:
						*t1 = std::to_string(val);
						break;
					case TYPE::INT8_T:
					case TYPE::INT16_T:
					case TYPE::INT32_T:
					case TYPE::INT64_T:
						*t1 = std::to_string(data_cast<int64_t>(val));
						break;
					case TYPE::FLOAT:
						*t1 = std::to_string(data_cast<float>(val));
						break;
					case TYPE::DOUBLE:
						*t1 = std::to_string(data_cast<double>(val));
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
