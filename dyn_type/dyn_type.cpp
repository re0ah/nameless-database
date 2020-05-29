#include "dyn_type.h"

using namespace db_col;

Column::Column(const TYPE type)
	: _type{type}
	, _flags{COLUMN_FLAGS_DEFAULT_NONE}
	, _name{std::move(std::string("Untitled"))}
{}

Column::Column(const std::string& name,
			   const TYPE type)
	: _type{type}
	, _flags{COLUMN_FLAGS_DEFAULT[static_cast<size_t>(type)]}
	, _name{name}
{}

Column::Column(const std::string&& name,
			   const TYPE type)
	: _type{type}
	, _flags{COLUMN_FLAGS_DEFAULT[static_cast<size_t>(type)]}
	, _name{std::move(name)}
{}

Column::Column(const Column& col)
	: _type{col.type()}
	, _data{col._data}
	, _flags{col._flags}
	, _name{col._name}
{}

Column::Column(const Column&& col)
	: _type{col.type()}
	, _data{std::move(col._data)}
	, _flags{std::move(col._flags)}
	, _name{std::move(col._name)}
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

void Column::swap(Column& lhs, Column& rhs)
{
	std::swap(lhs._type,  rhs._type);
	std::swap(lhs._data,  rhs._data);
	std::swap(lhs._flags, rhs._flags);
}

Column& Column::operator=(Column col)
{
	swap(*this, col);
	return *this;
}

Column& Column::operator=(const Column&& col)
{
	this->_type  = col.type();
	this->_data  = std::move(col.all_data());
	this->_flags = std::move(col.flags());
	return *this;
}

const uint64_t& Column::operator[](const size_t index) const
{
	return _data[index];
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

void Column::push_back(const std::pair<TYPE, uint64_t> data)
{
#define type  first
#define value second
	if ((this->_type != data.type) &&
		(flag_set(data.type) == false))
	{
		return;
	}
	_data.push_back(data.value);
#undef type
#undef value
}

void Column::resize(const size_t size)
{
	_data.resize(size, 0);
}

void Column::set_data(const std::pair<TYPE, uint64_t> data, const size_t pos)
{
#define type  first
#define value second
	if ((this->_type != data.type) &&
		(flag_set(data.type) == false))
	{
		return;
	}
//КАК ПРОСНЕШЬСЯ СДЕЛАЙ ПРОВЕРКУ СТРОКА = ???
	if (_data.size() > pos)
	{
		_data[pos] = data.value;
	}
	else
	{
		resize(pos + 1);
		_data[pos] = data.value;
	}
#undef type
#undef value
}

void Column::set_type_column(const TYPE type)
{
	for (size_t i = 0, size = _data.size(); i < size; i++)
	{
		_data[i] = set_type_data({this->_type, _data[i]}, type);
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
			std::cout << static_cast<int>(static_cast<uint8_t>(_data[pos]));
			break;
		case TYPE::INT8_T:
			std::cout << static_cast<int>(static_cast<int8_t>(_data[pos]));
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
		std::cout << '\t' << i << ": ";
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

void Column::set(std::pair<TYPE, uint64_t> data, const size_t pos)
{
#define type  first
#define value second
	if(flag_set(data.type) == false)
	{
		return;
	}

	if (this->_type == data.type)
	{
		if(this->_type == TYPE::STRING)
		{
			std::string* this_str = (std::string*)this->_data[pos];
			*this_str = *((std::string*)data.value);
			return;
		}
	}
	else
	{
		if(this->_type == TYPE::STRING)
		{
			std::string* this_str = (std::string*)this->_data[pos];
			*this_str = std::move(type_to_string(data));
			return;
		}
		data.value = set_type_data(data, this->_type);
	}

	_data[pos] = data.value;
#undef type
#undef value
}

void Column::add(std::pair<TYPE, uint64_t> data, const size_t pos)
{
#define type  first
#define value second
	if(flag_add(data.type) == false)
	{
		return;
	}

	if (this->_type == data.type)
	{
		if(this->_type == TYPE::STRING)
		{
			std::string* this_str = (std::string*)this->_data[pos];
			*this_str += *((std::string*)data.value);
			return;
		}
	}
	else
	{
		if(this->_type == TYPE::STRING)
		{
			std::string* this_str = (std::string*)this->_data[pos];
			*this_str += std::move(type_to_string(data));
			return;
		}
		data.value = set_type_data(data, this->_type);
	}

	switch(this->_type)
	{
		case TYPE::UINT8_T:
			reinterpret_cast<uint8_t&>(_data[pos]) += static_cast<uint8_t>(data.value);
			break;
		case TYPE::INT8_T:
			reinterpret_cast<int8_t&>(_data[pos]) += static_cast<int8_t>(data.value);
			break;
		case TYPE::UINT16_T:
			reinterpret_cast<uint16_t&>(_data[pos]) += static_cast<uint16_t>(data.value);
			break;
		case TYPE::INT16_T:
			reinterpret_cast<int16_t&>(_data[pos]) += static_cast<int16_t>(data.value);
			break;
		case TYPE::UINT32_T:
			reinterpret_cast<uint32_t&>(_data[pos]) += static_cast<uint32_t>(data.value);
			break;
		case TYPE::INT32_T:
			reinterpret_cast<int32_t&>(_data[pos]) += static_cast<int32_t>(data.value);
			break;
		case TYPE::UINT64_T:
			reinterpret_cast<uint64_t&>(_data[pos]) += static_cast<uint64_t>(data.value);
			break;
		case TYPE::INT64_T:
			reinterpret_cast<int64_t&>(_data[pos]) += static_cast<int64_t>(data.value);
			break;
		case TYPE::FLOAT:
			reinterpret_cast<float&>(_data[pos]) += reinterpret_cast<float&>(data.value);
			break;
		case TYPE::DOUBLE:
			reinterpret_cast<double&>(_data[pos]) += reinterpret_cast<double&>(data.value);
			break;
		default:
			break;
	}
#undef type
#undef value
}

void Column::sub(std::pair<TYPE, uint64_t> data, const size_t pos)
{
#define type  first
#define value second
	if((flag_sub(data.type) == false) ||
	   (this->_type == TYPE::STRING))
	{
		return;
	}

	if (this->_type != data.type)
	{
		data.value = set_type_data(data, this->_type);
	}
	switch(this->_type)
	{
		case TYPE::UINT8_T:
			reinterpret_cast<uint8_t&>(_data[pos]) -= static_cast<uint8_t>(data.value);
			break;
		case TYPE::INT8_T:
			reinterpret_cast<int8_t&>(_data[pos]) -= static_cast<int8_t>(data.value);
			break;
		case TYPE::UINT16_T:
			reinterpret_cast<uint16_t&>(_data[pos]) -= static_cast<uint16_t>(data.value);
			break;
		case TYPE::INT16_T:
			reinterpret_cast<int16_t&>(_data[pos]) -= static_cast<int16_t>(data.value);
			break;
		case TYPE::UINT32_T:
			reinterpret_cast<uint32_t&>(_data[pos]) -= static_cast<uint32_t>(data.value);
			break;
		case TYPE::INT32_T:
			reinterpret_cast<int32_t&>(_data[pos]) -= static_cast<int32_t>(data.value);
			break;
		case TYPE::UINT64_T:
			reinterpret_cast<uint64_t&>(_data[pos]) -= static_cast<uint64_t>(data.value);
			break;
		case TYPE::INT64_T:
			reinterpret_cast<int64_t&>(_data[pos]) -= static_cast<int64_t>(data.value);
			break;
		case TYPE::FLOAT:
			reinterpret_cast<float&>(_data[pos]) -= reinterpret_cast<float&>(data.value);
			break;
		case TYPE::DOUBLE:
			reinterpret_cast<double&>(_data[pos]) -= reinterpret_cast<double&>(data.value);
			break;
		default:
			break;
	}
#undef type
#undef value
}

void Column::mul(std::pair<TYPE, uint64_t> data, const size_t pos)
{
#define type  first
#define value second
	if((flag_mul(data.type) == false) ||
	   (this->_type == TYPE::STRING))
	{
		return;
	}

	if (this->_type != data.type)
	{
		data.value = set_type_data(data, this->_type);
	}
	switch(this->_type)
	{
		case TYPE::UINT8_T:
			reinterpret_cast<uint8_t&>(_data[pos]) *= static_cast<uint8_t>(data.value);
			break;
		case TYPE::INT8_T:
			reinterpret_cast<int8_t&>(_data[pos]) *= static_cast<int8_t>(data.value);
			break;
		case TYPE::UINT16_T:
			reinterpret_cast<uint16_t&>(_data[pos]) *= static_cast<uint16_t>(data.value);
			break;
		case TYPE::INT16_T:
			reinterpret_cast<int16_t&>(_data[pos]) *= static_cast<int16_t>(data.value);
			break;
		case TYPE::UINT32_T:
			reinterpret_cast<uint32_t&>(_data[pos]) *= static_cast<uint32_t>(data.value);
			break;
		case TYPE::INT32_T:
			reinterpret_cast<int32_t&>(_data[pos]) *= static_cast<int32_t>(data.value);
			break;
		case TYPE::UINT64_T:
			reinterpret_cast<uint64_t&>(_data[pos]) *= static_cast<uint64_t>(data.value);
			break;
		case TYPE::INT64_T:
			reinterpret_cast<int64_t&>(_data[pos]) *= static_cast<int64_t>(data.value);
			break;
		case TYPE::FLOAT:
			reinterpret_cast<float&>(_data[pos]) *= reinterpret_cast<float&>(data.value);
			break;
		case TYPE::DOUBLE:
			reinterpret_cast<double&>(_data[pos]) *= reinterpret_cast<double&>(data.value);
			break;
		default:
			break;
	}
#undef type
#undef value
}

void Column::div(std::pair<TYPE, uint64_t> data, const size_t pos)
{
#define type  first
#define value second
	if((flag_div(data.type) == false) ||
	   (this->_type == TYPE::STRING))
	{
		return;
	}

	if(this->_type != data.type)
	{
		data.value = set_type_data(data, this->_type);
	}
	switch(this->_type)
	{
		case TYPE::UINT8_T:
			reinterpret_cast<uint8_t&>(_data[pos]) /= static_cast<uint8_t>(data.value);
			break;
		case TYPE::INT8_T:
			reinterpret_cast<int8_t&>(_data[pos]) /= static_cast<int8_t>(data.value);
			break;
		case TYPE::UINT16_T:
			reinterpret_cast<uint16_t&>(_data[pos]) /= static_cast<uint16_t>(data.value);
			break;
		case TYPE::INT16_T:
			reinterpret_cast<int16_t&>(_data[pos]) /= static_cast<int16_t>(data.value);
			break;
		case TYPE::UINT32_T:
			reinterpret_cast<uint32_t&>(_data[pos]) /= static_cast<uint32_t>(data.value);
			break;
		case TYPE::INT32_T:
			reinterpret_cast<int32_t&>(_data[pos]) /= static_cast<int32_t>(data.value);
			break;
		case TYPE::UINT64_T:
			reinterpret_cast<uint64_t&>(_data[pos]) /= static_cast<uint64_t>(data.value);
			break;
		case TYPE::INT64_T:
			reinterpret_cast<int64_t&>(_data[pos]) /= static_cast<int64_t>(data.value);
			break;
		case TYPE::FLOAT:
			reinterpret_cast<float&>(_data[pos]) /= reinterpret_cast<float&>(data.value);
			break;
		case TYPE::DOUBLE:
			reinterpret_cast<double&>(_data[pos]) /= reinterpret_cast<double&>(data.value);
			break;
		default:
			break;
	}
#undef type
#undef value
}

bool Column::equal(std::pair<TYPE, uint64_t> data, const size_t pos) const
{
#define type  first
#define value second
	if(flag_equal(data.type) == false)
	{
		return false;
	}

	if (this->_type == data.type)
	{
		if(this->_type == TYPE::STRING)
		{
			std::string* this_str = (std::string*)this->_data[pos];
			return (*this_str == *((std::string*)data.value));
		}
	}
	else
	{
		if(this->_type == TYPE::STRING)
		{
			return false;
		}
		data.value = set_type_data(data, this->_type);
	}

	switch(this->_type)
	{
		case TYPE::NONE:
			return data.type == TYPE::NONE;
		case TYPE::UINT8_T:
			return reinterpret_cast<const uint8_t&>(_data[pos]) == static_cast<uint8_t>(data.value);
		case TYPE::INT8_T:
			return reinterpret_cast<const int8_t&>(_data[pos]) == static_cast<int8_t>(data.value);
		case TYPE::UINT16_T:
			return reinterpret_cast<const uint16_t&>(_data[pos]) == static_cast<uint16_t>(data.value);
		case TYPE::INT16_T:
			return reinterpret_cast<const int16_t&>(_data[pos]) == static_cast<int16_t>(data.value);
		case TYPE::UINT32_T:
			return reinterpret_cast<const uint32_t&>(_data[pos]) == static_cast<uint32_t>(data.value);
		case TYPE::INT32_T:
			return reinterpret_cast<const int32_t&>(_data[pos]) == static_cast<int32_t>(data.value);
		case TYPE::UINT64_T:
			return reinterpret_cast<const uint64_t&>(_data[pos]) == static_cast<uint64_t>(data.value);
		case TYPE::INT64_T:
			return reinterpret_cast<const int64_t&>(_data[pos]) == static_cast<int64_t>(data.value);
		case TYPE::FLOAT:
			return reinterpret_cast<const float&>(_data[pos]) == reinterpret_cast<float&>(data.value);
		case TYPE::DOUBLE:
			return reinterpret_cast<const double&>(_data[pos]) == reinterpret_cast<double&>(data.value);
		default:
			return false;
	}
#undef type
#undef value
}

bool Column::not_equal(std::pair<TYPE, uint64_t> data, const size_t pos) const
{
	return !equal(data, pos);
}

bool Column::less(std::pair<TYPE, uint64_t> data, const size_t pos) const
{
#define type  first
#define value second
	if(flag_less(data.type) == false)
	{
		return false;
	}

	if (this->_type == data.type)
	{
		if(this->_type == TYPE::STRING)
		{
			std::string* this_str = (std::string*)this->_data[pos];
			return (this_str->size() < (((std::string*)data.value)->size()));
		}
	}
	else
	{
		if(this->_type == TYPE::STRING)
		{
			return false;
		}
		data.value = set_type_data(data, this->_type);
	}

	switch(this->_type)
	{
		case TYPE::UINT8_T:
			return reinterpret_cast<const uint8_t&>(_data[pos]) < static_cast<uint8_t>(data.value);
		case TYPE::INT8_T:
			return reinterpret_cast<const int8_t&>(_data[pos]) < static_cast<int8_t>(data.value);
		case TYPE::UINT16_T:
			return reinterpret_cast<const uint16_t&>(_data[pos]) < static_cast<uint16_t>(data.value);
		case TYPE::INT16_T:
			return reinterpret_cast<const int16_t&>(_data[pos]) < static_cast<int16_t>(data.value);
		case TYPE::UINT32_T:
			return reinterpret_cast<const uint32_t&>(_data[pos]) < static_cast<uint32_t>(data.value);
		case TYPE::INT32_T:
			return reinterpret_cast<const int32_t&>(_data[pos]) < static_cast<int32_t>(data.value);
		case TYPE::UINT64_T:
			return reinterpret_cast<const uint64_t&>(_data[pos]) < static_cast<uint64_t>(data.value);
		case TYPE::INT64_T:
			return reinterpret_cast<const int64_t&>(_data[pos]) < static_cast<int64_t>(data.value);
		case TYPE::FLOAT:
			return reinterpret_cast<const float&>(_data[pos]) < reinterpret_cast<float&>(data.value);
		case TYPE::DOUBLE:
			return reinterpret_cast<const double&>(_data[pos]) < reinterpret_cast<double&>(data.value);
		default:
			return false;
	}
#undef type
#undef value
}

bool Column::more(std::pair<TYPE, uint64_t> data, const size_t pos) const
{
#define type  first
#define value second
	if(flag_less(data.type) == false)
	{
		return false;
	}

	if (this->_type == data.type)
	{
		if(this->_type == TYPE::STRING)
		{
			std::string* this_str = (std::string*)this->_data[pos];
			return (this_str->size() > (((std::string*)data.value)->size()));
		}
	}
	else
	{
		if(this->_type == TYPE::STRING)
		{
			return false;
		}
		data.value = set_type_data(data, this->_type);
	}

	switch(this->_type)
	{
		case TYPE::UINT8_T:
			return reinterpret_cast<const uint8_t&>(_data[pos]) > static_cast<uint8_t>(data.value);
		case TYPE::INT8_T:
			return reinterpret_cast<const int8_t&>(_data[pos]) > static_cast<int8_t>(data.value);
		case TYPE::UINT16_T:
			return reinterpret_cast<const uint16_t&>(_data[pos]) > static_cast<uint16_t>(data.value);
		case TYPE::INT16_T:
			return reinterpret_cast<const int16_t&>(_data[pos]) > static_cast<int16_t>(data.value);
		case TYPE::UINT32_T:
			return reinterpret_cast<const uint32_t&>(_data[pos]) > static_cast<uint32_t>(data.value);
		case TYPE::INT32_T:
			return reinterpret_cast<const int32_t&>(_data[pos]) > static_cast<int32_t>(data.value);
		case TYPE::UINT64_T:
			return reinterpret_cast<const uint64_t&>(_data[pos]) > static_cast<uint64_t>(data.value);
		case TYPE::INT64_T:
			return reinterpret_cast<const int64_t&>(_data[pos]) > static_cast<int64_t>(data.value);
		case TYPE::FLOAT:
			return reinterpret_cast<const float&>(_data[pos]) > reinterpret_cast<float&>(data.value);
		case TYPE::DOUBLE:
			return reinterpret_cast<const double&>(_data[pos]) > reinterpret_cast<double&>(data.value);
		default:
			return false;
	}
#undef type
#undef value
}

bool Column::less_or_equal(std::pair<TYPE, uint64_t> data, const size_t pos) const
{
	return !(more(data, pos));
}

bool Column::more_or_equal(std::pair<TYPE, uint64_t> data, const size_t pos) const
{
	return !(less(data, pos));
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

void Column::set_flag_set(const std::pair<TYPE, bool> val)
{
	_flags.set[static_cast<size_t>(val.first)] = val.second;
}

void Column::set_flag_add(const std::pair<TYPE, bool> val)
{
	_flags.add[static_cast<size_t>(val.first)] = val.second;
}

void Column::set_flag_sub(const std::pair<TYPE, bool> val)
{
	_flags.sub[static_cast<size_t>(val.first)] = val.second;
}

void Column::set_flag_mul(const std::pair<TYPE, bool> val)
{
	_flags.mul[static_cast<size_t>(val.first)] = val.second;
}

void Column::set_flag_div(const std::pair<TYPE, bool> val)
{
	_flags.div[static_cast<size_t>(val.first)] = val.second;
}

void Column::set_flag_equal(const std::pair<TYPE, bool> val)
{
	_flags.equal[static_cast<size_t>(val.first)] = val.second;
}

void Column::set_flag_less(const std::pair<TYPE, bool> val)
{
	_flags.less[static_cast<size_t>(val.first)] = val.second;
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

void Column::set_name(const std::string& name)
{
	_name = name;
}

void Column::set_name(const std::string&& name)
{
	_name = std::move(name);
}

void Column::set_name(const char* const name)
{
	_name = name;
}

std::string Column::name() const
{
	return _name;
}
