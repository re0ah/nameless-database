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
	std::string* str = new std::string("true ");
	union C
	{
		uint64_t u;
		double d;
	};
	C c1{.d{4.7}};
	Column cl[11]{Column(TYPE::UINT8_T),
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
	for(int i = 0; i < 10; i++)
	{
		cl[i].push_back('a');
		cl[i].add(0, 4, TYPE::INT8_T);
	}
	cl[10].push_back((uint64_t)str);
	cl[10].set(0, 322, TYPE::UINT64_T);
	std::cout << *(cl[10].data<std::string*>(0)) << std::endl;
	return 0;
}
