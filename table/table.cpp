#include "table.h"

using namespace db_column;

namespace db_table
{
Table::Table(const QString& name)
	: _name{name}
{}

Table::Table(const QString&& name)
	: _name{std::move(name)}
{}

void Table::new_column(const QString& str, const TYPE type)
{
    _data.push_back(Column(str, type));
}

void Table::new_column(const QString&& str, const TYPE type)
{
    _data.push_back(Column(std::move(str), type));
}

void Table::new_column(const Column& cl)
{
    _data.push_back(Column(cl));
}

void Table::new_column(const Column&& cl)
{
	_data.push_back(std::move(cl));
}

size_t Table::size() const
{
	return _data.size();
}

void Table::save_file(const QString& fname) const
{
	
}

Column& Table::operator[](const size_t index)
{
	return _data[index];
}
};/*end of namespace db_table*/
