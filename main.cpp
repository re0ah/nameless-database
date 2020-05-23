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
	Column cl[]{  Column(TYPE::BOOL),
				  Column(TYPE::UINT8_T),
				  Column(TYPE::INT8_T),
				  Column(TYPE::UINT16_T),
				  Column(TYPE::INT16_T),
				  Column(TYPE::UINT32_T),
				  Column(TYPE::INT32_T),
				  Column(TYPE::UINT64_T),
				  Column(TYPE::INT64_T),
				  Column(TYPE::FLOAT),
				  Column(TYPE::DOUBLE),
				  Column(TYPE::STRING)
	};
	cl[11].push_back((uint64_t)str);
	cl[11].add(0, reinterpret_cast<uint64_t&&>(3.14), TYPE::DOUBLE);
	std::cout << *(cl[11].data<std::string*>(0)) << std::endl;
	return 0;
}
