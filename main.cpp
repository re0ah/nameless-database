#include <iostream>
#include <string>
#include <cstring>
#include <map>
#include "dyn_type/dyn_type.h"

using namespace db_col;

class Table
{
	public:
		std::map<std::string, Column> field;
		Table()
		{
			
		};
		void set_new_name(const std::string name)
		{
			field[name] = Column(TYPE::NONE);
		}
};

int main(int argc, char** argv)
{
	/*cout settings*/
	std::cout << std::boolalpha;
	/***************/
	std::string* str = new std::string("54");
	Column cl[]{  Column(TYPE::BOOL),     /*0*/
				  Column(TYPE::UINT8_T),  /*1*/
				  Column(TYPE::INT8_T),   /*2*/
				  Column(TYPE::UINT16_T), /*3*/
				  Column(TYPE::INT16_T),  /*4*/
				  Column(TYPE::UINT32_T), /*5*/
				  Column(TYPE::INT32_T),  /*6*/
				  Column(TYPE::UINT64_T), /*7*/
				  Column(TYPE::INT64_T),  /*8*/
				  Column(TYPE::FLOAT),    /*9*/
				  Column(TYPE::DOUBLE),   /*10*/
				  Column(TYPE::STRING)    /*11*/
	};
	cl[0].push_back(1);
	cl[0].push_back(1);
	cl[0].push_back(0);
	cl[0].push_back(0);
	cl[0].push_back(0);
	cl[0].push_back(1);
	cl[0].push_back(0);
	cl[0].push_back(0);
	cl[0].push_back(0);
	cl[0].push_back(1);
	cl[0].push_back(0);
	cl[0].push_back(0);
	cl[0].push_back(0);
	cl[0].push_back(1);
	cl[11].push_back((uint64_t)str);
	cl[11].set_flag_add(TYPE::FLOAT, true);
	cl[11].set_flag_set(TYPE::FLOAT, true);
	cl[11].set(0, reinterpret_cast<uint64_t&&>(3.14f), TYPE::FLOAT);
	cl[11].print_all_stdout();
//	cl[4].mul(0, (uint64_t)str, TYPE::INT32_T);
	return 0;
}
