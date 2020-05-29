#include <iostream>
#include <string>
#include <cstring>
#include <unordered_map>
#include "table/table.h"

using namespace db_col;

std::pair<TYPE, uint16_t>& operator+(std::pair<TYPE, uint16_t>& lhs,
									 const std::pair<TYPE, uint16_t>& rhs)
{
	lhs.second += rhs.second;
	return lhs;
}

int main(int argc, char** argv)
{
	/*cout settings*/
	std::cout << std::boolalpha;
	/***************/
	Table tb = Table();
	tb.new_column(std::string("CL1"), TYPE::UINT16_T);
	tb.new_column(std::string("CL2"), TYPE::INT64_T);
	tb.new_column(std::string("CL3"), TYPE::UINT32_T);
	tb.new_column(std::string("CL4"), TYPE::UINT32_T);
	tb.new_column(std::string("CL5"), TYPE::UINT32_T);
	tb.new_column(Column());
	for(int i = 0; i < 5; i++)
	{
		tb[0].push_back({TYPE::UINT64_T, i});
		tb[1].push_back({TYPE::UINT64_T, i * i});
		tb[3].push_back({TYPE::UINT64_T, i * i * i * i});
		tb[4].push_back({TYPE::UINT64_T, i * i * i * i * i});
	}
	tb.new_column(tb[0]);
	tb[6].set_name("CL322");
//	tb[2].set_data(8, 322);
	tb[3] = tb[4];
	tb[4].set_type_column(TYPE::STRING);
	tb[4].set_type_column(TYPE::UINT32_T);
	tb[0].add({TYPE::UINT32_T, 87}, 8);
	tb.print_all_column_stdout();
	std::cout << std::endl;
//	tb[0].set_flag_set({TYPE::STRING, true});
//	tb[0].print_flags_set_stdout();
//	tb["test"].print_all_stdout();
//	tb[0].print_all_stdout();
	return 0;
}
