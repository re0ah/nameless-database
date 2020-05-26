#include "dyn_type.h"

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
			*this_str = *(std::string*)val;
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
			switch(type)
			{
				case TYPE::FLOAT:
					{const float& this_float = reinterpret_cast<const float&>(_data[pos]);
					const float& arg_float  = reinterpret_cast<const float&>(val);
					return this_float == arg_float;}
				case TYPE::DOUBLE:
					{const float& this_float = reinterpret_cast<const float&>(_data[pos]);
					const double& arg_float = reinterpret_cast<const double&>(val);
					return this_float == arg_float;}
				default:
					return false;
			}
		case TYPE::DOUBLE:
			switch(type)
			{
				case TYPE::FLOAT:
					{const double& this_float = reinterpret_cast<const double&>(_data[pos]);
					const float& arg_float  = reinterpret_cast<const float&>(val);
					return this_float == arg_float;}
				case TYPE::DOUBLE:
					{const double& this_float = reinterpret_cast<const double&>(_data[pos]);
					const double& arg_float = reinterpret_cast<const double&>(val);
					return this_float == arg_float;}
				default:
					return false;
			}
		case TYPE::STRING:
			{
			const std::string* const this_string = (std::string*)_data[pos];
			const std::string* const arg_string  = (std::string*)val;
			return ((type == TYPE::STRING) ? (*this_string == *arg_string) : false);
			}
	}
	return false;
}
