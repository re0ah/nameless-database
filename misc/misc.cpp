#include "misc.h"

bool check_num_in_str(const std::string* const str)
{
	/*come down*/
	char* ptr;
	strtod(str->c_str(), &ptr);
	return *ptr == 0;
}
