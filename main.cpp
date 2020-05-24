#include <iostream>
#include <stdint.h>
#include <string>
#include <cstring>
#include <vector>
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
	cl[11].push_back(reinterpret_cast<uint64_t&&>(str));
	cl[11].set_type(TYPE::UINT64_T);
	std::cout << cl[11].data<uint64_t>(0) << std::endl;
//	cl[4].mul(0, (uint64_t)str, TYPE::INT32_T);
//	std::cout << cl[0].data<int16_t>(0) << std::endl;
	return 0;
}
