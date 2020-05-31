#pragma once
#include "column/column.h"

namespace db_table
{
class Table
{
	public:
		Table(const std::string& name=std::string("Untitled"));
		Table(const std::string&& name);

		void new_column(const std::string& str, const db_column::TYPE type);
		void new_column(const std::string&& str, const db_column::TYPE type);
		void new_column(const db_column::Column& cl);
		void new_column(const db_column::Column&& cl);

		void print_column_element_stdout(const size_t index_column,
										 const size_t index_element) const;
		void print_column_stdout(const size_t index_column) const;
		void print_all_column_stdout() const;

		size_t size() const;

		void save_file(const std::string& fname) const;
		void save_file(const std::string&& fname) const;

		void load_file(const std::string& fname);
		void load_file(const std::string&& fname);

		db_column::Column& operator[](const size_t index);
	private:
		std::vector<db_column::Column> _data;
		std::string _name;
};
};/*end of namespace db_table*/
