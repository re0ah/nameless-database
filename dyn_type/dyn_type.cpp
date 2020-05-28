#include "dyn_type.h"

using namespace db_col;

Column::Column() :
	_type{TYPE::NONE},
	_flags{COLUMN_FLAGS_DEFAULT_NONE}
{}

Column::Column(const TYPE type) :
	_type{type},
	_flags{COLUMN_FLAGS_DEFAULT[static_cast<size_t>(type)]}
{}

Column::Column(const Column& col) :
	_type{col.type()},
	_data{col._data},
	_flags{col._flags}
{}

Column::Column(const Column&& col) :
	_type{col.type()},
	_data{std::move(col._data)},
	_flags{std::move(col._flags)}
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
	this->_flags = col.flags();
	return *this;
}

Column& Column::operator=(const Column&& col)
{
	this->_type = col.type();
	this->_data = std::move(col.all_data());
	this->_flags = std::move(col.flags());
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
	set_flags_default(type);
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

void Column::print_flags_set_stdout() const
{
	std::cout << "flags_set:\n";
	for(size_t i = 0; i <= ENUM_TYPE_SIZE; i++)
	{
		std::cout << "\t" << TYPE(i) << " = " << flag_set(TYPE(i)) << '\n';
	}
	std::cout << std::endl;
}

void Column::print_flags_add_stdout() const
{
	std::cout << "flags_add:\n";
	for(size_t i = 0; i <= ENUM_TYPE_SIZE; i++)
	{
		std::cout << "\t" << TYPE(i) << " = " << flag_add(TYPE(i)) << '\n';
	}
	std::cout << std::endl;
}

void Column::print_flags_sub_stdout() const
{
	std::cout << "flags_sub:\n";
	for(size_t i = 0; i <= ENUM_TYPE_SIZE; i++)
	{
		std::cout << "\t" << TYPE(i) << " = " << flag_sub(TYPE(i)) << '\n';
	}
	std::cout << std::endl;
}

void Column::print_flags_mul_stdout() const
{
	std::cout << "flags_mul:\n";
	for(size_t i = 0; i <= ENUM_TYPE_SIZE; i++)
	{
		std::cout << "\t" << TYPE(i) << " = " << flag_mul(TYPE(i)) << '\n';
	}
	std::cout << std::endl;
}

void Column::print_flags_div_stdout() const
{
	std::cout << "flags_div:\n";
	for(size_t i = 0; i <= ENUM_TYPE_SIZE; i++)
	{
		std::cout << "\t" << TYPE(i) << " = " << flag_div(TYPE(i)) << '\n';
	}
	std::cout << std::endl;
}

void Column::print_flags_equal_stdout() const
{
	std::cout << "flags_equal:\n";
	for(size_t i = 0; i <= ENUM_TYPE_SIZE; i++)
	{
		std::cout << "\t" << TYPE(i) << " = " << flag_equal(TYPE(i)) << '\n';
	}
	std::cout << std::endl;
}

void Column::print_flags_less_stdout() const
{
	std::cout << "flags_less:\n";
	for(size_t i = 0; i <= ENUM_TYPE_SIZE; i++)
	{
		std::cout << "\t" << TYPE(i) << " = " << flag_less(TYPE(i)) << '\n';
	}
	std::cout << std::endl;
}

void Column::print_flags_stdout() const
{
	print_flags_set_stdout();
	print_flags_add_stdout();
	print_flags_sub_stdout();
	print_flags_mul_stdout();
	print_flags_div_stdout();
	print_flags_equal_stdout();
	print_flags_less_stdout();
}

void Column::print_info_stdout() const
{
	print_type_stdout();
	std::cout << '\n';
	print_all_stdout();
	print_flags_stdout();
	std::cout << std::endl;
}

uint64_t Column::data(const size_t pos) const
{
	return _data[pos];
}

void Column::set(const size_t pos, uint64_t val, const TYPE type)
{
	if(flag_set(type) == false)
	{
		return;
	}

	if (this->_type == type)
	{
		if(this->_type == TYPE::STRING)
		{
			std::string* this_str = (std::string*)this->_data[pos];
			*this_str = *((std::string*)val);
			return;
		}
	}
	else
	{
		if(this->_type == TYPE::STRING)
		{
			std::string* this_str = (std::string*)this->_data[pos];
			*this_str = std::move(type_to_string(type, val));
			return;
		}
		val = set_type_data(val, type, this->_type);
	}

	_data[pos] = val;
}

void Column::add(const size_t pos, uint64_t val, const TYPE type)
{
	if(flag_add(type) == false)
	{
		return;
	}

	if (this->_type == type)
	{
		if(this->_type == TYPE::STRING)
		{
			std::string* this_str = (std::string*)this->_data[pos];
			*this_str += *((std::string*)val);
			return;
		}
	}
	else
	{
		if(this->_type == TYPE::STRING)
		{
			std::string* this_str = (std::string*)this->_data[pos];
			*this_str += std::move(type_to_string(type, val));
			return;
		}
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
	if((flag_sub(type) == false) ||
	   (this->_type == TYPE::STRING))
	{
		return;
	}

	if (this->_type != type)
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
	if((flag_mul(type) == false) ||
	   (this->_type == TYPE::STRING))
	{
		return;
	}

	if (this->_type != type)
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
	if((flag_div(type) == false) ||
	   (this->_type == TYPE::STRING))
	{
		return;
	}

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

bool Column::equal(const size_t pos, uint64_t val, const TYPE type) const
{
	if(flag_equal(type) == false)
	{
		return false;
	}

	if (this->_type == type)
	{
		if(this->_type == TYPE::STRING)
		{
			std::string* this_str = (std::string*)this->_data[pos];
			return (*this_str == *((std::string*)val));
		}
	}
	else
	{
		if(this->_type == TYPE::STRING)
		{
			return false;
		}
		val = set_type_data(val, type, this->_type);
	}

	switch(this->_type)
	{
		case TYPE::NONE:
			return type == TYPE::NONE;
		case TYPE::UINT8_T:
			return reinterpret_cast<const uint8_t&>(_data[pos]) == static_cast<uint8_t>(val);
		case TYPE::INT8_T:
			return reinterpret_cast<const int8_t&>(_data[pos]) == static_cast<int8_t>(val);
		case TYPE::UINT16_T:
			return reinterpret_cast<const uint16_t&>(_data[pos]) == static_cast<uint16_t>(val);
		case TYPE::INT16_T:
			return reinterpret_cast<const int16_t&>(_data[pos]) == static_cast<int16_t>(val);
		case TYPE::UINT32_T:
			return reinterpret_cast<const uint32_t&>(_data[pos]) == static_cast<uint32_t>(val);
		case TYPE::INT32_T:
			return reinterpret_cast<const int32_t&>(_data[pos]) == static_cast<int32_t>(val);
		case TYPE::UINT64_T:
			return reinterpret_cast<const uint64_t&>(_data[pos]) == static_cast<uint64_t>(val);
		case TYPE::INT64_T:
			return reinterpret_cast<const int64_t&>(_data[pos]) == static_cast<int64_t>(val);
		case TYPE::FLOAT:
			return reinterpret_cast<const float&>(_data[pos]) == reinterpret_cast<float&>(val);
		case TYPE::DOUBLE:
			return reinterpret_cast<const double&>(_data[pos]) == reinterpret_cast<double&>(val);
		default:
			return false;
	}
}

bool Column::not_equal(const size_t pos, uint64_t val, const TYPE type) const
{
	return !equal(pos, val, type);
}

bool Column::less(const size_t pos, uint64_t val, const TYPE type) const
{
	if(flag_less(type) == false)
	{
		return false;
	}

	if (this->_type == type)
	{
		if(this->_type == TYPE::STRING)
		{
			std::string* this_str = (std::string*)this->_data[pos];
			return ((*this_str.size()) < (*((std::string*)val).size()));
		}
	}
	else
	{
		if(this->_type == TYPE::STRING)
		{
			return false;
		}
		val = set_type_data(val, type, this->_type);
	}

	switch(this->_type)
	{
		case TYPE::UINT8_T:
			return reinterpret_cast<const uint8_t&>(_data[pos]) < static_cast<uint8_t>(val);
		case TYPE::INT8_T:
			return reinterpret_cast<const int8_t&>(_data[pos]) < static_cast<int8_t>(val);
		case TYPE::UINT16_T:
			return reinterpret_cast<const uint16_t&>(_data[pos]) < static_cast<uint16_t>(val);
		case TYPE::INT16_T:
			return reinterpret_cast<const int16_t&>(_data[pos]) < static_cast<int16_t>(val);
		case TYPE::UINT32_T:
			return reinterpret_cast<const uint32_t&>(_data[pos]) < static_cast<uint32_t>(val);
		case TYPE::INT32_T:
			return reinterpret_cast<const int32_t&>(_data[pos]) < static_cast<int32_t>(val);
		case TYPE::UINT64_T:
			return reinterpret_cast<const uint64_t&>(_data[pos]) < static_cast<uint64_t>(val);
		case TYPE::INT64_T:
			return reinterpret_cast<const int64_t&>(_data[pos]) < static_cast<int64_t>(val);
		case TYPE::FLOAT:
			return reinterpret_cast<const float&>(_data[pos]) < reinterpret_cast<float&>(val);
		case TYPE::DOUBLE:
			return reinterpret_cast<const double&>(_data[pos]) < reinterpret_cast<double&>(val);
		default:
			return false;
	}
}

bool Column::more(const size_t pos, uint64_t val, const TYPE type) const
{
	if(flag_more(type) == false)
	{
		return false;
	}

	if (this->_type == type)
	{
		if(this->_type == TYPE::STRING)
		{
			std::string* this_str = (std::string*)this->_data[pos];
			return ((*this_str.size()) > (*((std::string*)val).size()));
		}
	}
	else
	{
		if(this->_type == TYPE::STRING)
		{
			return false;
		}
		val = set_type_data(val, type, this->_type);
	}

	switch(this->_type)
	{
		case TYPE::UINT8_T:
			return reinterpret_cast<const uint8_t&>(_data[pos]) > static_cast<uint8_t>(val);
		case TYPE::INT8_T:
			return reinterpret_cast<const int8_t&>(_data[pos]) > static_cast<int8_t>(val);
		case TYPE::UINT16_T:
			return reinterpret_cast<const uint16_t&>(_data[pos]) > static_cast<uint16_t>(val);
		case TYPE::INT16_T:
			return reinterpret_cast<const int16_t&>(_data[pos]) > static_cast<int16_t>(val);
		case TYPE::UINT32_T:
			return reinterpret_cast<const uint32_t&>(_data[pos]) > static_cast<uint32_t>(val);
		case TYPE::INT32_T:
			return reinterpret_cast<const int32_t&>(_data[pos]) > static_cast<int32_t>(val);
		case TYPE::UINT64_T:
			return reinterpret_cast<const uint64_t&>(_data[pos]) > static_cast<uint64_t>(val);
		case TYPE::INT64_T:
			return reinterpret_cast<const int64_t&>(_data[pos]) > static_cast<int64_t>(val);
		case TYPE::FLOAT:
			return reinterpret_cast<const float&>(_data[pos]) > reinterpret_cast<float&>(val);
		case TYPE::DOUBLE:
			return reinterpret_cast<const double&>(_data[pos]) > reinterpret_cast<double&>(val);
		default:
			return false;
	}
}

bool Column::less_or_equal(const size_t pos, uint64_t val, const TYPE type) const
{
	return !(more(pos, val, type);
}

bool Column::more_or_equal(const size_t pos, uint64_t val, const TYPE type) const
{
	return !(less(pos, val, type);
}

void Column::set_flags(const struct flags_column& flags)
{
	this->_flags = flags;
}

void Column::set_flags_default()
{
	set_flags(COLUMN_FLAGS_DEFAULT[static_cast<size_t>(this->type())]);
}

void Column::set_flags_default(const TYPE type)
{
	set_flags(COLUMN_FLAGS_DEFAULT[static_cast<size_t>(type)]);
}

void Column::set_flag_set(const TYPE type, const bool val)
{
	_flags.set[static_cast<size_t>(type)] = val;
}

void Column::set_flag_add(const TYPE type, const bool val)
{
	_flags.add[static_cast<size_t>(type)] = val;
}

void Column::set_flag_sub(const TYPE type, const bool val)
{
	_flags.sub[static_cast<size_t>(type)] = val;
}

void Column::set_flag_mul(const TYPE type, const bool val)
{
	_flags.mul[static_cast<size_t>(type)] = val;
}

void Column::set_flag_div(const TYPE type, const bool val)
{
	_flags.div[static_cast<size_t>(type)] = val;
}

void Column::set_flag_equal(const TYPE type, const bool val)
{
	_flags.equal[static_cast<size_t>(type)] = val;
}

void Column::set_flag_less(const TYPE type, const bool val)
{
	_flags.less[static_cast<size_t>(type)] = val;
}

const struct flags_column& Column::flags() const
{
	return _flags;
}

bool Column::flag_set(const TYPE type) const
{
	return _flags.set[static_cast<size_t>(type)];
}

bool Column::flag_add(const TYPE type) const
{
	return _flags.add[static_cast<size_t>(type)];
}

bool Column::flag_sub(const TYPE type) const
{
	return _flags.sub[static_cast<size_t>(type)];
}

bool Column::flag_mul(const TYPE type) const
{
	return _flags.mul[static_cast<size_t>(type)];
}

bool Column::flag_div(const TYPE type) const
{
	return _flags.div[static_cast<size_t>(type)];
}

bool Column::flag_equal(const TYPE type) const
{
	return _flags.equal[static_cast<size_t>(type)];
}

bool Column::flag_less(const TYPE type) const
{
	return _flags.less[static_cast<size_t>(type)];
}
