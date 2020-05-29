#include "table.h"
using namespace db_col;

Table::Table(const std::string& name)
	: _name{name}
{}

Table::Table(const std::string&& name)
	: _name{std::move(name)}
{}

void Table::new_column(const std::string& str, const TYPE type)
{
	_data.push_back(std::move(Column(str, type)));
}

void Table::new_column(const std::string&& str, const TYPE type)
{
	_data.push_back(std::move(Column(std::move(str), type)));
}

void Table::new_column(const Column& cl)
{
	_data.push_back(std::move(Column(cl)));
}

void Table::new_column(const Column&& cl)
{
	_data.push_back(std::move(cl));
}

void Table::print_column_element_stdout(const size_t index_column,
								 const size_t index_element) const
{
	std::cout << "Table: " << _name << ", "
		      << "Column: " << _data[index_column].name() << '\n'
			  << "value[" << index_element << "]: ";
	_data[index_column].print_stdout(index_element);
	std::cout << std::endl;
}

void Table::print_column_stdout(const size_t index_column) const
{
	std::cout << "Table: " << _name << ", "
		      << "Column: " << _data[index_column].name() << ", "
			  << "type: " << "\"" << _data[index_column].type() << "\", ";
	const size_t column_size = _data[index_column].size();
	if (column_size == 0)
	{
		std::cout << "column is empty";
	}
	else
	{
		std::cout << "elements[0-" << column_size - 1 << "]:\n";
		_data[index_column].print_all_stdout();
	}
	std::cout << std::endl;
}

void Table::print_all_column_stdout() const
{
	std::cout << "Table: " << _name << '\n';
	const size_t table_size = _data.size();
	for(size_t i = 0; i < table_size; i++)
	{
		const size_t column_size = _data[i].size();
		std::cout << "Column[" << i << "] \"" << _data[i].name() << "\", "
				  << "type: " << "\"" << _data[i].type() << "\", ";
		if (column_size == 0)
		{
			std::cout << "column is empty\n";
		}
		else
		{
			std::cout << "elements[0-" << column_size - 1 << "]:\n";
			_data[i].print_all_stdout();
		}
	}
	std::cout << std::endl;
}

size_t Table::size() const
{
	return _data.size();
}

Column& Table::operator[](const size_t index)
{
	return _data[index];
}
