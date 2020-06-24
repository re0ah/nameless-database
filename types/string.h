#ifndef STRING_H_
#define STRING_H_
#include "date.h"
#include <ostream>
#include <cstring>
#include "../misc/xalloc.h"

/*not RAII object*/
struct string
{
    char* data;
#ifdef ENV64BIT
    uint32_t alloc;
    uint32_t size;
#else
    uint16_t alloc;
    uint16_t size;
#endif

    static const size_t ALLOC_DEFAULT = 16;

    string()=delete;
    ~string()=delete;

    int is_number() const;

    template<typename T>
    T to_number() const;
#ifdef ENV32BIT
    void to_string(int32_t val);
    void to_string(uint32_t val);
    void to_string(float val);
#endif
#ifdef ENV64BIT
    void to_string(int64_t val);
    void to_string(uint64_t val);
    void to_string(double val);
#endif
    void to_string(const struct time& val);

    template <typename T>
    struct string& operator=(const T& val);
    struct string& operator=(const struct date& date);
    struct string& operator=(const struct string& str);
    struct string& operator=(const char* const str);

    template <typename T>
    struct string& operator+=(const T& val);
    struct string& operator+=(const struct string& str);
    struct string& operator+=(const char* const str);

    template <typename T>
    bool operator==(const T& str) const;
    bool operator==(const struct string& str) const;
    bool operator==(const char* const str) const;

    template <typename T>
    bool operator!=(const T& str) const;
    bool operator!=(const struct string& str) const;
    bool operator!=(const char* const str) const;

    template <typename T>
    bool operator<(const T& str) const;
    bool operator<(const struct string& str) const;
    bool operator<(const char* const str) const;

    template <typename T>
    bool operator>(const T& str) const;
    bool operator>(const struct string& str) const;
    bool operator>(const char* const str) const;

    template <typename T>
    bool operator<=(const T& str) const;
    bool operator<=(const struct string& str) const;
    bool operator<=(const char* const str) const;

    template <typename T>
    bool operator>=(const T& str) const;
    bool operator>=(const struct string& str) const;
    bool operator>=(const char* const str) const;

    char& operator[](const size_t pos);
    const char& operator[](const size_t pos) const;
};

template<typename T>
T string::to_number() const
{
    return (T)0;
}

template <typename T>
struct string& string::operator=(const T& val)
{
    return *this;
}

template <typename T>
struct string& string::operator+=(const T& val)
{
    return *this;
}

template <typename T>
bool string::operator==(const T& val) const
{
    return false;
}

template <typename T>
bool string::operator!=(const T& val) const
{
    return false;
}

template <typename T>
bool string::operator<(const T& val) const
{
    return false;
}

template <typename T>
bool string::operator>(const T& val) const
{
    return false;
}

template <typename T>
bool string::operator<=(const T& val) const
{
    return false;
}

template <typename T>
bool string::operator>=(const T& val) const
{
    return false;
}

std::ostream& operator<<(std::ostream& os, const struct string& value);

#endif
