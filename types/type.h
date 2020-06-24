#ifndef TYPE_H_
#define TYPE_H_
#include "string.h"
#include "date.h"

/*not RAII object*/
template <typename T>
struct Type
{
    T data;

    Type()=delete;
    ~Type()=delete;

    template <typename K>
    struct Type<T>& operator=(const K& val);
    struct Type<T>& operator=(const struct string& val);
    struct Type<T>& operator=(const struct date& val);

    template <typename K>
    struct Type<T>& operator+=(const K& val);
    struct Type<T>& operator+=(const struct string& val);
    struct Type<T>& operator+=(const struct date& val);

    template <typename K>
    struct Type<T>& operator-=(const K& val);
    struct Type<T>& operator-=(const struct string& val);
    struct Type<T>& operator-=(const struct date& val);

    template <typename K>
    struct Type<T>& operator*=(const K& val);
    struct Type<T>& operator*=(const struct string& val);
    struct Type<T>& operator*=(const struct date& val);

    template <typename K>
    struct Type<T>& operator/=(const K& val);
    struct Type<T>& operator/=(const struct string& val);
    struct Type<T>& operator/=(const struct date& val);

    template <typename K>
    bool operator==(const K& val);
    bool operator==(const struct string& val);
    bool operator==(const struct date& val);

    template <typename K>
    bool operator!=(const K& val);
    bool operator!=(const struct string& val);
    bool operator!=(const struct date& val);

    template <typename K>
    bool operator<(const K& val);
    bool operator<(const struct string& val);
    bool operator<(const struct date& val);

    template <typename K>
    bool operator>(const K& val);
    bool operator>(const struct string& val);
    bool operator>(const struct date& val);

    template <typename K>
    bool operator<=(const K& val);
    bool operator<=(const struct string& val);
    bool operator<=(const struct date& val);

    template <typename K>
    bool operator>=(const K& val);
    bool operator>=(const struct string& val);
    bool operator>=(const struct date& val);
};

#define arithm_op(op)\
template <typename T>\
template <typename K>\
struct Type<T>& Type<T>::operator op(const K& val)\
{\
    this->data op val.data;\
    return *this;\
}\
\
template <typename T>\
struct Type<T>& Type<T>::operator op(const struct string& val)\
{\
    if(val.is_number())\
    {\
        this->data op val.to_number<T>();\
    }\
    return *this;\
}\
\
template <typename T>\
struct Type<T>& Type<T>::operator op(const struct date& val)\
{\
    this->data op val.data;\
    return *this;\
}
arithm_op(=);
arithm_op(+=);
arithm_op(-=);
arithm_op(/=);
arithm_op(*=);
#undef arithm_op

#define compare_op(op)\
template <typename T>\
template <typename K>\
bool Type<T>::operator op(const K& val)\
{\
    return this->data op val.data;\
}\
template <typename T>\
bool Type<T>::operator op(const struct string& val)\
{\
    if(val.is_number())\
    {\
        return this->data op val.to_number<T>();\
    }\
    return false;\
}\
\
template <typename T>\
bool Type<T>::operator op(const struct date& val)\
{\
    return this->data op val.data;\
}

compare_op(==);
compare_op(!=);
compare_op(<);
compare_op(>);
compare_op(<=);
compare_op(>=);
#undef compare_op

#endif
