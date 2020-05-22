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
	std::string* str = new std::string("true");
	union C
	{
		uint64_t u;
		double d;
	};
	C c1{.d{4.7}};
	Column cl = Column(TYPE::STRING);
	cl.push_back((uint64_t)str);
	cl.set(0, (uint64_t)str, TYPE::STRING);
	cl.set(0, (uint64_t)str, TYPE::STRING);
	std::cout << *(cl.data<std::string*>(0)) << std::endl;
	return 0;
}
