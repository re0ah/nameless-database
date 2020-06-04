#include "column.h"

namespace db_column
{
Column::Column(const TYPE type, const size_t reserve)
	: _type{type}
    , _data{std::vector<uint64_t>(reserve)}
	, _flags{COLUMN_FLAGS_DEFAULT_NONE}
    , _name{QString("Untitled")}
{}

Column::Column(const QString& name,
			   const TYPE type,
			   const size_t reserve)
	: _type{type}
    , _data{std::vector<uint64_t>()}
	, _flags{COLUMN_FLAGS_DEFAULT[static_cast<size_t>(type)]}
	, _name{name}
{
	_data.reserve(reserve);
}

Column::Column(const QString&& name,
			   const TYPE type,
			   const size_t reserve)
	: _type{type}
    , _data{std::vector<uint64_t>()}
	, _flags{COLUMN_FLAGS_DEFAULT[static_cast<size_t>(type)]}
	, _name{std::move(name)}
{
	_data.reserve(reserve);
}

Column::Column(const Column& col)
	: _type{col.type()}
	, _data{col._data}
	, _flags{col._flags}
	, _name{col._name}
{}

Column::Column(Column&& col)
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
            delete (QString*)_data[i];
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

Column& Column::operator=(Column&& col)
{
	this->_type  = col.type();
    this->_data  = col.all_data();
	this->_flags = std::move(col.flags());
	return *this;
}

std::pair<TYPE, uint64_t&> Column::operator[](const size_t index)
{
	return {type(), _data.at(index)};
}

const std::pair<TYPE, uint64_t> Column::operator[](const size_t index) const
{
	return {type(), _data.at(index)};
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
	if(flags().set[data.type] == false)
    {
		return;
    }
	if (this->_type == data.type)
    {
		if(this->_type == TYPE::STRING)
		{
            QString* new_str = new QString(*((QString*)data.value));
			_data.push_back(std::move((uint64_t)new_str));
			return;
        }
        _data.push_back(data.value);
	}
	else
    {
		if(this->_type == TYPE::STRING)
		{
            QString* new_str = new QString(type_to_string(data));
			_data.push_back(std::move((uint64_t)new_str));
			return;
		}
        _data.push_back(std::move(set_type_data(data, this->_type)));
	}
#undef type
#undef value
}

void Column::resize(const size_t size)
{
    if(size < this->_data.size())
    {
        for(size_t i = size; i < this->_data.size(); i++)
        {
            delete (QString*)_data[i];
        }
        _data.resize(size, 0);
        return;
    }

	_data.resize(size, 0);
    if(this->type() == TYPE::STRING)
    {
        for(size_t i = 0; i < size; i++)
        {
            _data[i] = (uint64_t)(new QString(""));
        }
    }
}

void Column::set_type_column(const TYPE type)
{
	for (size_t i = 0, size = _data.size(); i < size; i++)
	{
		_data[i] = set_type_data({this->_type, _data[i]}, type);
	}
	this->_type = type;
	_flags.set_default(type);
}

std::vector<uint64_t> Column::all_data() const
{
	return this->_data;
}

bool Column::check_size(const size_t pos) const
{
	return (this->_data.size() < pos);
}

void Column::check_and_resize(const size_t pos)
{
	if (check_size(pos))
	{
		resize(pos + 1);
	}
}

void Column::set(std::pair<TYPE, uint64_t> data, const size_t pos)
{
	if(flags().set[data.first] == false)
    {
		return;
    }
	check_and_resize(pos);

    std::pair<TYPE, uint64_t&> this_pair({this->type(), this->_data[pos]});
	this_pair = data;
}

void Column::add(std::pair<TYPE, uint64_t> data, const size_t pos)
{
	if(flags().add[data.first] == false)
	{
		return;
	}
	check_and_resize(pos);

    std::pair<TYPE, uint64_t&> this_pair({this->type(), this->_data[pos]});
	this_pair += data;
}

void Column::sub(std::pair<TYPE, uint64_t> data, const size_t pos)
{
	if((flags().sub[data.first] == false) ||
	   (this->_type == TYPE::STRING))
	{
		return;
	}
	check_and_resize(pos);

    std::pair<TYPE, uint64_t&> this_pair({this->type(), this->_data[pos]});
	this_pair -= data;
}

void Column::mul(std::pair<TYPE, uint64_t> data, const size_t pos)
{
	if((flags().mul[data.first] == false) ||
	   (this->_type == TYPE::STRING))
	{
		return;
	}
	check_and_resize(pos);

    std::pair<TYPE, uint64_t&> this_pair({this->type(), this->_data[pos]});
	this_pair *= data;
}

void Column::div(std::pair<TYPE, uint64_t> data, const size_t pos)
{
	if((flags().div[data.first] == false) ||
	   (this->_type == TYPE::STRING))
	{
		return;
	}
	check_and_resize(pos);

    std::pair<TYPE, uint64_t&> this_pair({this->type(), this->_data[pos]});
	this_pair /= data;
}

bool Column::equal(std::pair<TYPE, uint64_t> data, const size_t pos) const
{
    if((_flags.equal[data.first] == false) ||
	   (check_size(pos) == true))
	{
		return false;
	}
	std::pair<TYPE, const uint64_t&> this_pair(this->type(), this->_data[pos]);
	return this_pair == data;
}

bool Column::not_equal(std::pair<TYPE, uint64_t> data, const size_t pos) const
{
	return !equal(data, pos);
}

bool Column::less(std::pair<TYPE, uint64_t> data, const size_t pos) const
{
    if((_flags.less[data.first] == false) ||
	   (check_size(pos) == true))
	{
		return false;
	}

	std::pair<TYPE, const uint64_t&> this_pair(this->type(), this->_data[pos]);
	return this_pair < data;
}

bool Column::more(std::pair<TYPE, uint64_t> data, const size_t pos) const
{
    if((_flags.less[data.first] == false) ||
	   (check_size(pos) == true))
	{
		return false;
	}

	std::pair<TYPE, const uint64_t&> this_pair(this->type(), this->_data[pos]);
	return this_pair > data;
}

bool Column::less_or_equal(std::pair<TYPE, uint64_t> data, const size_t pos) const
{
	return !(more(data, pos));
}

bool Column::more_or_equal(std::pair<TYPE, uint64_t> data, const size_t pos) const
{
	return !(less(data, pos));
}

struct Column::flags_column& Column::flags()
{
	return _flags;
}

void Column::flags_column::set_all(const struct flags_column& flags)
{
	*this = flags;
}

void Column::flags_column::set_default(const TYPE type)
{
	*this = COLUMN_FLAGS_DEFAULT[static_cast<size_t>(type)];
}

void Column::set_name(const QString& name)
{
	_name = name;
}

void Column::set_name(const QString&& name)
{
	_name = std::move(name);
}

void Column::set_name(const char* const name)
{
	_name = name;
}

QString Column::name() const
{
	return _name;
}

};/*end of namespace db_column*/
