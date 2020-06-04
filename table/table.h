#pragma once
#include "column/column.h"

namespace db_table
{
class Table
{
	public:
        Table(const QString& name=QString("Untitled"));
        Table(const QString&& name);

        void new_column(const QString& str, const db_column::TYPE type);
        void new_column(const QString&& str, const db_column::TYPE type);
		void new_column(const db_column::Column& cl);
		void new_column(const db_column::Column&& cl);

		size_t size() const;

        void save_file(const QString& fname) const;
        void save_file(const QString&& fname) const;

        void load_file(const QString& fname);
        void load_file(const QString&& fname);

		db_column::Column& operator[](const size_t index);
	private:
		std::vector<db_column::Column> _data;
        QString _name;
};
};/*end of namespace db_table*/
