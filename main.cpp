#include "table/table.h"

int main(int argc, char** argv)
{
	using db_column::TYPE;
	/*cout settings*/
	std::cout << std::boolalpha;
	/***************/
	db_table::Table tb = db_table::Table();
	tb.new_column(std::string("CL1"), TYPE::UINT16_T);
	tb[0].set({TYPE::UINT32_T, 64}, 12);
	tb[0].set_type_column(TYPE::STRING);
	std::cout << tb[0].flags().set[TYPE::STRING] << std::endl;
	std::cout << tb[0].flags().set[TYPE::UINT8_T] << std::endl;
//	tb[0].set_flag_set({TYPE::STRING, true});
//	tb[0].print_flags_set_stdout();
//	tb["test"].print_all_stdout();
//	tb[0].print_all_stdout();
	return 0;
}
