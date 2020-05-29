#pragma once
#include "column/column.h"
using namespace db_col;

class Table
{
	public:
		Table(const std::string& name=std::string("Untitled"));
		Table(const std::string&& name);

		void new_column(const std::string& str, const TYPE type);
		void new_column(const std::string&& str, const TYPE type);
		void new_column(const Column& cl);
		void new_column(const Column&& cl);

		void print_column_element_stdout(const size_t index_column,
										 const size_t index_element) const;
		void print_column_stdout(const size_t index_column) const;
		void print_all_column_stdout() const;

		size_t size() const;

		Column& operator[](const size_t index);
	private:
		std::vector<Column> _data;
		std::string _name;
};
