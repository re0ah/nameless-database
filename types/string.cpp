#include "string.h"

/*
   return 0, if not number
   return 1, if integer
   return 2, if floating
*/
int string::is_number() const
{
    /*supported symbols: '0'-'9' | '.' | ','
      first symbol must be '0'-'9'
      in string must be only 1 '.' or ','
    */
    const char* ptr = &data[0];
    if((*ptr >= '0') && (*ptr <= '9'))
    {
        ++ptr;
    }
    else
    {
        return false;
    }

    bool is_float = false;
    while(*ptr)
    {
        if((*ptr >= '0') && (*ptr <= '9'))
        {}
        else if ((*ptr == '.') || (*ptr == ','))
        {
            if(is_float) /*in string must be only 1 '.' or ','*/
            {
                return false;
            }
            is_float = true;
         }
        else
        {
            return false;
        }
        ++ptr;
    }
    return is_float + 1;
}

struct string& string::operator=(const struct date& date)
{
    return *this;
}

struct string& string::operator=(const struct string& str)
{
    if(this->alloc < str.alloc)
    {
        this->data = (char*)xrealloc(this->data, str.alloc);
        this->alloc = str.alloc;
    }
    strcpy(this->data, str.data);
    this->size = str.size;
    return *this;
};

struct string& string::operator=(const char* const str)
{
    const size_t str_size = strlen(str);
    if(this->alloc <= str_size)
    {
        const size_t alloc = str_size * 2;
        this->data = (char*)xrealloc(this->data, alloc);
        this->alloc = alloc;
    }
    strcpy(this->data, str);
    this->size = str_size;
    return *this;
};

struct string& string::operator+=(const struct string& str)
{
    const size_t result_size = this->size + str.size;
    if(this->alloc <= result_size)
    {
        const size_t alloc = result_size * 2;
        this->data = (char*)xrealloc(this->data, alloc);
        this->alloc = alloc;
    }
    strcat(this->data, str.data);
    this->size = result_size;
    return *this;
}

struct string& string::operator+=(const char* const str)
{
    const size_t result_size = this->size + strlen(str);
    if(this->alloc <= result_size)
    {
        const size_t alloc = result_size * 2;
        this->data = (char*)xrealloc(this->data, alloc);
        this->alloc = alloc;
    }
    strcat(this->data, str);
    this->size = result_size;
    return *this;
}

bool string::operator==(const struct string& str) const
{
    const int result = strcmp(this->data, str.data);
    return result == 0;
}

bool string::operator==(const char* const str) const
{
    const int result = strcmp(this->data, str);
    return result == 0;
}

bool string::operator!=(const struct string& str) const
{
    return !(*this == str);
}

bool string::operator!=(const char* const str) const
{
    return !(*this == str);
}

bool string::operator<(const struct string& str) const
{
    const int result = strcmp(this->data, str.data);
    return result < 0;
}

bool string::operator<(const char* const str) const
{
    const int result = strcmp(this->data, str);
    return result < 0;
}

bool string::operator>(const struct string& str) const
{
    const int result = strcmp(this->data, str.data);
    return result > 0;
}

bool string::operator>(const char* const str) const
{
    const int result = strcmp(this->data, str);
    return result > 0;
}


bool string::operator<=(const struct string& str) const
{
    return !(*this > str.data);
}

bool string::operator<=(const char* const str) const
{
    return !(*this > str);
}

bool string::operator>=(const struct string& str) const
{
    return !(*this < str.data);
}

bool string::operator>=(const char* const str) const
{
    return !(*this < str);
}

char& string::operator[](const size_t pos)
{
    return this->data[pos];
}

const char& string::operator[](const size_t pos) const
{
    return this->data[pos];
}

std::ostream& operator<<(std::ostream& os, const struct string& value)
{
    os << value.data;
    return os;
}
