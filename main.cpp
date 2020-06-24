#include <iostream>
#include <string>
#include "table/table.h"

int32_t main(int32_t argc, char* argv[])
{
	Table tb = Table();
//	tb.test();
	tb.push_back_column(4, Type_cl::GENERAL::DATE);
	tb[0].set_fmt("%Y-%M-%D %H:%m:%S, unix=%u64");
	struct string* tst = (struct string*)malloc(sizeof(struct string));
	tst->data = (char*)malloc(5);
	tst->alloc = 5;
	tst->size = 4;
	strcpy(tst->data, "test");
	struct string* tst2 = (struct string*)malloc(sizeof(struct string));
	tst2->data = (char*)malloc(8);
	tst2->alloc = 8;
	tst2->size = 7;
	strcpy(tst2->data, "qwerty");
/*
	tb[0][3] = *tst;
	tb[0][2] = tb[0][3];
	tb[0][0] = tb[0][3];
	tb[0][0] += tb[0][2];
	tb[0][1] = *tst2;
	std::cout << "TEST" << tb[0][3] << std::endl;
*/	for(size_t i = 0; i < tb.size(); i++)
	{
		for(size_t j = 0; j < tb[i].size(); j++)
		{
			tb[i][j] = (i + 256461) * (j + 1534546);
			std::cout << "tb[" << i << "][" << j << "] = " << tb[i][j] << std::endl;
		}
	}
	return 0;
}
