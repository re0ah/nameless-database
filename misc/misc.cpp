#include "misc.h"

bool check_num_in_str(const QString* const str)
{
	/*come down*/
	char* ptr;
    strtod(str->toStdString().c_str(), &ptr);
	return *ptr == 0;
}
