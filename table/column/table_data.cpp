#include "table_data.h"
#include "format.h"

Table_data::Table_data(const Column_flags& flags,
                       const char* const fmt,
                       uint8_t* const val,
                       const Type_cl type)
    : flags_{flags}
    , fmt_{fmt}
    , data_{val}
    , type_{type}
{}

Table_data::~Table_data()
{}

struct Type_cl Table_data::type() const
{
    return type_;
}

uint8_t* Table_data::data()
{
    return data_;
}

const uint8_t* Table_data::data() const
{
    return data_;
}

const struct Column_flags& Table_data::flags() const
{
    return flags_;
}

const char* const Table_data::fmt() const
{
    return fmt_;
}


template <typename lhs_type, typename rhs_type>
inline void mov_op(uint8_t* lhs, const uint8_t* const rhs)
{
    *((lhs_type*)lhs) = *((rhs_type*)rhs);
}

template <typename lhs_type, typename rhs_type>
inline void add_op(uint8_t* lhs, const uint8_t* const rhs)
{
    *((lhs_type*)lhs) += *((rhs_type*)(rhs));
}

template <typename lhs_type, typename rhs_type>
inline void sub_op(uint8_t* lhs, const uint8_t* const rhs)
{
//  *((lhs_type*)lhs) -= *((rhs_type*)(rhs));
}

template <typename lhs_type, typename rhs_type>
inline void mul_op(uint8_t* lhs, const uint8_t* const rhs)
{
//  *((lhs_type*)lhs) *= *((rhs_type*)(rhs));
}

template <typename lhs_type, typename rhs_type>
inline void div_op(uint8_t* lhs, const uint8_t* const rhs)
{
//  *((lhs_type*)lhs) /= *((rhs_type*)(rhs));
}

template <typename lhs_type, typename rhs_type>
inline bool eq_op(uint8_t* lhs, const uint8_t* const rhs)
{
//  return *((lhs_type*)lhs) == *((rhs_type*)(rhs));
}

template <typename lhs_type, typename rhs_type>
inline bool not_eq_op(uint8_t* lhs, const uint8_t* const rhs)
{
//  return *((lhs_type*)lhs) != *((rhs_type*)(rhs));
}

template <typename lhs_type, typename rhs_type>
inline bool less_op(uint8_t* lhs, const uint8_t* const rhs)
{
//  return *((lhs_type*)lhs) < *((rhs_type*)(rhs));
}

template <typename lhs_type, typename rhs_type>
inline bool more_op(uint8_t* lhs, const uint8_t* const rhs)
{
//  return *((lhs_type*)lhs) > *((rhs_type*)(rhs));
}

template <typename lhs_type, typename rhs_type>
inline bool less_eq_op(uint8_t* lhs, const uint8_t* const rhs)
{
//  return *((lhs_type*)lhs) <= *((rhs_type*)(rhs));
}

template <typename lhs_type, typename rhs_type>
inline bool more_eq_op(uint8_t* lhs, const uint8_t* const rhs)
{
//  return *((lhs_type*)lhs) >= *((rhs_type*)(rhs));
}

#define MOV_OP 0
#define ADD_OP 1
#define SUB_OP 2
#define MUL_OP 3
#define DIV_OP 4
#define EQ_OP  5
#define NOT_EQ_OP  6
#define LESS_OP 7
#define MORE_OP 8
#define LESS_EQ_OP 9
#define MORE_EQ_OP 10
template <typename lhs_type, int OP_TYPE, typename rhs_type>
inline void op(uint8_t* lhs, const uint8_t* const rhs)
{
    if constexpr(OP_TYPE == MOV_OP)
        mov_op<lhs_type, rhs_type>(lhs, rhs);
    if constexpr(OP_TYPE == ADD_OP)
        add_op<lhs_type, rhs_type>(lhs, rhs);
    if constexpr(OP_TYPE == SUB_OP)
        sub_op<lhs_type, rhs_type>(lhs, rhs);
    if constexpr(OP_TYPE == DIV_OP)
        mul_op<lhs_type, rhs_type>(lhs, rhs);
    if constexpr(OP_TYPE == MUL_OP)
        div_op<lhs_type, rhs_type>(lhs, rhs);
}

template <typename lhs_type, int OP_TYPE, typename rhs_type>
inline bool bool_op(uint8_t* lhs, const uint8_t* const rhs)
{
    if constexpr(OP_TYPE == EQ_OP)
        return eq_op<lhs_type, rhs_type>(lhs, rhs);
    if constexpr(OP_TYPE == NOT_EQ_OP)
        return not_eq_op<lhs_type, rhs_type>(lhs, rhs);
    if constexpr(OP_TYPE == LESS_OP)
        return less_op<lhs_type, rhs_type>(lhs, rhs);
    if constexpr(OP_TYPE == MORE_OP)
        return more_op<lhs_type, rhs_type>(lhs, rhs);
    if constexpr(OP_TYPE == LESS_EQ_OP)
        return less_eq_op<lhs_type, rhs_type>(lhs, rhs);
    if constexpr(OP_TYPE == MORE_EQ_OP)
        return more_eq_op<lhs_type, rhs_type>(lhs, rhs);
}

template <typename T, int OP>
inline void to_op(uint8_t* lhs,
                  const uint8_t* const rhs, const enum Type_cl::GENERAL type)
{
    switch(type)
    {
        case Type_cl::GENERAL::INT8:
            op<T, OP, struct Type<int8_t>>(lhs, rhs);
            break;
        case Type_cl::GENERAL::INT16:
            op<T, OP, struct Type<int16_t>>(lhs, rhs);
            break;
        case Type_cl::GENERAL::INT32:
            op<T, OP, struct Type<int32_t>>(lhs, rhs);
            break;
        case Type_cl::GENERAL::UINT8:
            op<T, OP, struct Type<uint8_t>>(lhs, rhs);
            break;
        case Type_cl::GENERAL::UINT16:
            op<T, OP, struct Type<uint16_t>>(lhs, rhs);
            break;
        case Type_cl::GENERAL::UINT32:
            op<T, OP, struct Type<uint32_t>>(lhs, rhs);
            break;
        case Type_cl::GENERAL::FLOAT:
            op<T, OP, struct Type<float>>(lhs, rhs);
            break;
        case Type_cl::GENERAL::BOOL:
            op<T, OP, struct Type<bool>>(lhs, rhs);
            break;
        case Type_cl::GENERAL::STRING:
            op<T, OP, struct string>(lhs, rhs);
            break;
        case Type_cl::GENERAL::DATE:
            op<T, OP, struct date>(lhs, rhs);
            break;
    #ifdef ENV64BIT
        case Type_cl::GENERAL::INT64:
            op<T, OP, struct Type<int64_t>>(lhs, rhs);
            break;
        case Type_cl::GENERAL::UINT64:
            op<T, OP, struct Type<uint64_t>>(lhs, rhs);
            break;
        case Type_cl::GENERAL::DOUBLE:
            op<T, OP, struct Type<double>>(lhs, rhs);
            break;
    #endif
    }
}

template <typename T, int OP>
inline bool to_bool_op(uint8_t* lhs,
                       const uint8_t* const rhs, const enum Type_cl::GENERAL type)
{
    switch(type)
    {
        case Type_cl::GENERAL::INT8:
            return bool_op<T, OP, struct Type<int8_t>>(lhs, rhs);
        case Type_cl::GENERAL::INT16:
            return bool_op<T, OP, struct Type<int16_t>>(lhs, rhs);
        case Type_cl::GENERAL::INT32:
            return bool_op<T, OP, struct Type<int32_t>>(lhs, rhs);
        case Type_cl::GENERAL::UINT8:
            return bool_op<T, OP, struct Type<uint8_t>>(lhs, rhs);
        case Type_cl::GENERAL::UINT16:
            return bool_op<T, OP, struct Type<uint16_t>>(lhs, rhs);
        case Type_cl::GENERAL::UINT32:
            return bool_op<T, OP, struct Type<uint32_t>>(lhs, rhs);
        case Type_cl::GENERAL::FLOAT:
            return bool_op<T, OP, struct Type<float>>(lhs, rhs);
        case Type_cl::GENERAL::BOOL:
            return bool_op<T, OP, struct Type<bool>>(lhs, rhs);
        case Type_cl::GENERAL::STRING:
            return bool_op<T, OP, struct string>(lhs, rhs);
        case Type_cl::GENERAL::DATE:
            return bool_op<T, OP, struct date>(lhs, rhs);
    #ifdef ENV64BIT
        case Type_cl::GENERAL::INT64:
            return bool_op<T, OP, struct Type<int64_t>>(lhs, rhs);
        case Type_cl::GENERAL::UINT64:
            return bool_op<T, OP, struct Type<uint64_t>>(lhs, rhs);
        case Type_cl::GENERAL::DOUBLE:
            return bool_op<T, OP, struct Type<double>>(lhs, rhs);
    #endif
    }
    return false;
}

#ifdef ENV64BIT
#define table_data_operator_function(op_symb, flag_type, op_name) \
Table_data& Table_data::operator op_symb(const Table_data& value)\
{\
    if(this->flags().flag_type.get(value.type_.data()) == false)\
    {\
        return *this;\
    }\
    switch(this->type().data())\
    {\
        case Type_cl::GENERAL::INT8:\
            to_op<struct Type<int8_t>, op_name>(data_, value.data_, value.type_.data());\
            break;\
        case Type_cl::GENERAL::INT16:\
            to_op<struct Type<int16_t>, op_name>(data_, value.data_, value.type_.data());\
            break;\
        case Type_cl::GENERAL::INT32:\
            to_op<struct Type<int32_t>, op_name>(data_, value.data_, value.type_.data());\
            break;\
        case Type_cl::GENERAL::INT64:\
            to_op<struct Type<int64_t>, op_name>(data_, value.data_, value.type_.data());\
            break;\
        case Type_cl::GENERAL::UINT8:\
            to_op<struct Type<uint8_t>, op_name>(data_, value.data_, value.type_.data());\
            break;\
        case Type_cl::GENERAL::UINT16:\
            to_op<struct Type<uint16_t>, op_name>(data_, value.data_, value.type_.data());\
            break;\
        case Type_cl::GENERAL::UINT32:\
            to_op<struct Type<uint32_t>, op_name>(data_, value.data_, value.type_.data());\
            break;\
        case Type_cl::GENERAL::UINT64:\
            to_op<struct Type<uint64_t>, op_name>(data_, value.data_, value.type_.data());\
            break;\
        case Type_cl::GENERAL::FLOAT:\
            to_op<struct Type<float>, op_name>(data_, value.data_, value.type_.data());\
            break;\
        case Type_cl::GENERAL::DOUBLE:\
            to_op<struct Type<double>, op_name>(data_, value.data_, value.type_.data());\
            break;\
        case Type_cl::GENERAL::BOOL:\
            to_op<struct Type<bool>, op_name>(data_, value.data_, value.type_.data());\
            break;\
        case Type_cl::GENERAL::STRING:\
            to_op<struct string, op_name>(data_, value.data_, value.type_.data());\
            break;\
        case Type_cl::GENERAL::DATE:\
            to_op<struct date, op_name>(data_, value.data_, value.type_.data());\
            break;\
    }\
    return *this;\
}
#endif
#ifdef ENV32BIT
#define table_data_operator_function(op_symb, flag_type, op_name) \
Table_data& Table_data::operator op_symb(const Table_data& value)\
{\
    if(this->flags().flag_type.get(value.type_.data()) == false)\
    {\
        return *this;\
    }\
    switch(this->type().data())\
    {\
        case Type_cl::GENERAL::INT8:\
            to_op<struct Type<int8_t>, op_name>(data_, value.data_, value.type_.data());\
            break;\
        case Type_cl::GENERAL::INT16:\
            to_op<struct Type<int16_t>, op_name>(data_, value.data_, value.type_.data());\
            break;\
        case Type_cl::GENERAL::INT32:\
            to_op<struct Type<int32_t>, op_name>(data_, value.data_, value.type_.data());\
            break;\
        case Type_cl::GENERAL::UINT8:\
            to_op<struct Type<uint8_t>, op_name>(data_, value.data_, value.type_.data());\
            break;\
        case Type_cl::GENERAL::UINT16:\
            to_op<struct Type<uint16_t>, op_name>(data_, value.data_, value.type_.data());\
            break;\
        case Type_cl::GENERAL::UINT32:\
            to_op<struct Type<uint32_t>, op_name>(data_, value.data_, value.type_.data());\
            break;\
        case Type_cl::GENERAL::FLOAT:\
            to_op<struct Type<float>, op_name>(data_, value.data_, value.type_.data());\
            break;\
        case Type_cl::GENERAL::BOOL:\
            to_op<struct Type<bool>, op_name>(data_, value.data_, value.type_.data());\
            break;\
        case Type_cl::GENERAL::STRING:\
            to_op<struct string, op_name>(data_, value.data_, value.type_.data());\
            break;\
        case Type_cl::GENERAL::DATE:\
            to_op<struct date, op_name>(data_, value.data_, value.type_.data());\
            break;\
    }\
    return *this;\
}
#endif
#ifdef ENV64BIT
#define table_data_bool_operator_function(op_symb, flag_type, op_name) \
bool Table_data::operator op_symb(const Table_data& value)\
{\
    if(this->flags().flag_type.get(value.type_.data()) == false)\
    {\
        return false;\
    }\
    switch(this->type().data())\
    {\
        case Type_cl::GENERAL::INT8:\
            return to_bool_op<struct Type<int8_t>, op_name>(data_, value.data_, value.type_.data());\
        case Type_cl::GENERAL::INT16:\
            return to_bool_op<struct Type<int16_t>, op_name>(data_, value.data_, value.type_.data());\
        case Type_cl::GENERAL::INT32:\
            return to_bool_op<struct Type<int32_t>, op_name>(data_, value.data_, value.type_.data());\
        case Type_cl::GENERAL::INT64:\
            return to_bool_op<struct Type<int64_t>, op_name>(data_, value.data_, value.type_.data());\
        case Type_cl::GENERAL::UINT8:\
            return to_bool_op<struct Type<uint8_t>, op_name>(data_, value.data_, value.type_.data());\
        case Type_cl::GENERAL::UINT16:\
            return to_bool_op<struct Type<uint16_t>, op_name>(data_, value.data_, value.type_.data());\
        case Type_cl::GENERAL::UINT32:\
            return to_bool_op<struct Type<uint32_t>, op_name>(data_, value.data_, value.type_.data());\
        case Type_cl::GENERAL::UINT64:\
            return to_bool_op<struct Type<uint64_t>, op_name>(data_, value.data_, value.type_.data());\
        case Type_cl::GENERAL::FLOAT:\
            return to_bool_op<struct Type<float>, op_name>(data_, value.data_, value.type_.data());\
        case Type_cl::GENERAL::DOUBLE:\
            return to_bool_op<struct Type<double>, op_name>(data_, value.data_, value.type_.data());\
        case Type_cl::GENERAL::BOOL:\
            return to_bool_op<struct Type<bool>, op_name>(data_, value.data_, value.type_.data());\
        case Type_cl::GENERAL::STRING:\
            return to_bool_op<struct string, op_name>(data_, value.data_, value.type_.data());\
        case Type_cl::GENERAL::DATE:\
            return to_bool_op<struct date, op_name>(data_, value.data_, value.type_.data());\
    }\
    return false;\
}
#endif
#ifdef ENV32BIT
#define table_data_bool_operator_function(op_symb, flag_type, op_name) \
bool Table_data::operator op_symb(const Table_data& value)\
{\
    if(this->flags().flag_type.get(value.type_.data()) == false)\
    \
        return false;\
    }\
    switch(this->type().data())\
    {\
        case Type_cl::GENERAL::INT8:\
            return to_op<struct Type<int8_t>, op_name>(data_, value.data_, value.type_.data());\
        case Type_cl::GENERAL::INT16:\
            return to_op<struct Type<int16_t>, op_name>(data_, value.data_, value.type_.data());\
        case Type_cl::GENERAL::INT32:\
            return to_op<struct Type<int32_t>, op_name>(data_, value.data_, value.type_.data());\
        case Type_cl::GENERAL::UINT8:\
            return to_op<struct Type<uint8_t>, op_name>(data_, value.data_, value.type_.data());\
        case Type_cl::GENERAL::UINT16:\
            return to_op<struct Type<uint16_t>, op_name>(data_, value.data_, value.type_.data());\
        case Type_cl::GENERAL::UINT32:\
            return to_op<struct Type<uint32_t>, op_name>(data_, value.data_, value.type_.data());\
        case Type_cl::GENERAL::FLOAT:\
            return to_op<struct Type<float>, op_name>(data_, value.data_, value.type_.data());\
        case Type_cl::GENERAL::BOOL:\
            return to_op<struct Type<bool>, op_name>(data_, value.data_, value.type_.data());\
        case Type_cl::GENERAL::STRING:\
            return to_op<struct string, op_name>(data_, value.data_, value.type_.data());\
        case Type_cl::GENERAL::DATE:\
            return to_op<struct date, op_name>(data_, value.data_, value.type_.data());\
    }\
    return false;\
}
#endif
table_data_operator_function(=,  mov,  MOV_OP);
table_data_operator_function(+=, add,  ADD_OP);
table_data_operator_function(-=, sub,  SUB_OP);
table_data_operator_function(*=, mul,  MUL_OP);
table_data_operator_function(/=, div,  DIV_OP);
table_data_bool_operator_function(==, eq ,  EQ_OP);
table_data_bool_operator_function(!=, eq ,  NOT_EQ_OP);
table_data_bool_operator_function(<,  less, LESS_OP);
table_data_bool_operator_function(>,  less, MORE_OP);
table_data_bool_operator_function(<=, less, LESS_EQ_OP);
table_data_bool_operator_function(>=, less, MORE_EQ_OP);
#undef MOV_OP
#undef ADD_OP
#undef SUB_OP
#undef MUL_OP
#undef DIV_OP
#undef EQ_OP
#undef NOT_EQ_OP
#undef LESS_OP
#undef MORE_OP
#undef LESS_EQ_OP
#undef MORE_EQ_OP
#undef table_data_operator_function

std::ostream& std_output(std::ostream& os, const Table_data& value)
{
    switch(value.type().data())
    {
        case Type_cl::GENERAL::INT8:
            os << (int)*((const int8_t*)value.data());
            break;
        case Type_cl::GENERAL::INT16:
            os << *((const int16_t*)value.data());
            break;
        case Type_cl::GENERAL::INT32:
            os << *((const int32_t*)value.data());
            break;
        case Type_cl::GENERAL::UINT8:
            os << (int)*((const uint8_t*)value.data());
            break;
        case Type_cl::GENERAL::UINT16:
            os << *((const uint16_t*)value.data());
            break;
        case Type_cl::GENERAL::UINT32:
            os << *((const uint32_t*)value.data());
            break;
        case Type_cl::GENERAL::FLOAT:
            os << *((const float*)value.data());
            break;
        case Type_cl::GENERAL::BOOL:
            os << *((const bool*)value.data());
            break;
        case Type_cl::GENERAL::STRING:
            os << ((const struct string*)value.data())->data;
            break;
        case Type_cl::GENERAL::DATE:
            {const struct date* ptr = (const struct date*)value.data();
            os << ptr->day()   << '-'
               << ptr->month() << '-'
               << ptr->year()  << ' '
               << ptr->hour()   << ':'
               << ptr->minute() << ':'
               << ptr->second();}
            break;
    #ifdef ENV64BIT
        case Type_cl::GENERAL::INT64:
            os << *((const int64_t*)value.data());
            break;
        case Type_cl::GENERAL::UINT64:
            os << *((const uint64_t*)value.data());
            break;
        case Type_cl::GENERAL::DOUBLE:
            os << *((const float*)value.data());
            break;
    #endif
    }
    return os;
}

template <typename T>
inline std::ostream& _format_output(std::ostream& os,
                                    const size_t fmt,
                                    const Table_data& val)
{
    T* ptr;
    if constexpr(std::is_same<T, struct date>::value)
    {
        ptr = (T*)val.data();
    }
    else
    {
        ptr = (T*)val.data();
    }
    switch(fmt)
    {
        case (size_t)FORMAT::FMT::s8:
            os << (int)((int8_t)ptr->data);
            break;
        case (size_t)FORMAT::FMT::s16:
            os << (int16_t)ptr->data;
            break;
        case (size_t)FORMAT::FMT::s32:
            os << (int32_t)ptr->data;
            break;
#ifdef ENV64BIT
        case (size_t)FORMAT::FMT::s64:
            os << (int64_t)ptr->data;
            break;
#endif
        case (size_t)FORMAT::FMT::u8:
            os << (int)((uint8_t)ptr->data);
            break;
        case (size_t)FORMAT::FMT::u16:
            os << (uint16_t)ptr->data;
            break;
        case (size_t)FORMAT::FMT::u32:
            os << (uint32_t)ptr->data;
            break;
#ifdef ENV64BIT
        case (size_t)FORMAT::FMT::u64:
            os << (uint64_t)ptr->data;
            break;
#endif
        case (size_t)FORMAT::FMT::f:
            os << (uint64_t)ptr->data;
            break;
        case (size_t)FORMAT::FMT::b:
            os << (bool)ptr->data;
            break;
#ifdef ENV64BIT
        case (size_t)FORMAT::FMT::d:
            os << (double)ptr->data;
            break;
#endif
        case (size_t)FORMAT::FMT::Y:
            os << date::year(ptr->data);
            break;
        case (size_t)FORMAT::FMT::M:
            os << date::month(ptr->data);
            break;
        case (size_t)FORMAT::FMT::D:
            os << date::day(ptr->data);
            break;
        case (size_t)FORMAT::FMT::H:
            os << date::hour(ptr->data);
            break;
        case (size_t)FORMAT::FMT::m:
            os << date::minute(ptr->data);
            break;
        case (size_t)FORMAT::FMT::S:
            os << date::second(ptr->data);
            break;
    }
    return os;
}

std::ostream& format_output(std::ostream& os,
                            const size_t fmt,
                            const Table_data& val)
{
    switch(val.type().data())
    {
        case Type_cl::GENERAL::INT8:
            return _format_output<struct Type<int8_t>>(os, fmt, val);
        case Type_cl::GENERAL::INT16:
            return _format_output<struct Type<int16_t>>(os, fmt, val);
        case Type_cl::GENERAL::INT32:
            return _format_output<struct Type<int32_t>>(os, fmt, val);
        case Type_cl::GENERAL::UINT8:
            return _format_output<struct Type<uint8_t>>(os, fmt, val);
        case Type_cl::GENERAL::UINT16:
            return _format_output<struct Type<uint16_t>>(os, fmt, val);
        case Type_cl::GENERAL::UINT32:
            return _format_output<struct Type<uint32_t>>(os, fmt, val);
        case Type_cl::GENERAL::FLOAT:
            return _format_output<struct Type<float>>(os, fmt, val);
        case Type_cl::GENERAL::BOOL:
            return _format_output<struct Type<bool>>(os, fmt, val);
        case Type_cl::GENERAL::STRING:
            {struct string* str = (struct string*)val.data();
             os << str->data;}
            break;
        case Type_cl::GENERAL::DATE:
            return _format_output<struct date>(os, fmt, val);
    #ifdef ENV64BIT
        case Type_cl::GENERAL::INT64:
            return _format_output<struct Type<int64_t>>(os, fmt, val);
        case Type_cl::GENERAL::UINT64:
            return _format_output<struct Type<uint64_t>>(os, fmt, val);
        case Type_cl::GENERAL::DOUBLE:
            return _format_output<struct Type<double>>(os, fmt, val);
    #endif
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Table_data& value)
{
    static const char* FMT[] = {
                        FORMAT::STANDARD::s8,
                        FORMAT::STANDARD::s16,
                        FORMAT::STANDARD::s32,
#ifdef ENV64BIT
                        FORMAT::STANDARD::s64,
#endif
                        FORMAT::STANDARD::u8,
                        FORMAT::STANDARD::u16,
                        FORMAT::STANDARD::u32,
#ifdef ENV64BIT
                        FORMAT::STANDARD::u64,
#endif
                        FORMAT::STANDARD::f,
#ifdef ENV64BIT
                        FORMAT::STANDARD::d,
#endif
                        FORMAT::STANDARD::b,
                        FORMAT::STRING::s,
                        FORMAT::DATE::Y,
                        FORMAT::DATE::M,
                        FORMAT::DATE::D,
                        FORMAT::DATE::H,
                        FORMAT::DATE::m,
                        FORMAT::DATE::S
    };

    if(value.fmt() == nullptr)
    {
        std_output(os, value);
        return os;
    }

    const char* ptr = value.fmt();
    /*for format*/
    char tmp[4];
    while(*ptr)
    {
again:  int tmp_it = 0;
        if(*ptr == '%')
        {
            while(true)
            {
                ++ptr;
                tmp[tmp_it] = *ptr;
                ++tmp_it;
                tmp[tmp_it] = '\0';
                size_t j = 0;
                while(j < (sizeof(FMT) / sizeof(FMT[0])))
                {
                    if(strcmp(tmp, FMT[j]) == 0)
                    {
                        ++ptr;
                        format_output(os, j, value);
                        goto again;
                    }
                    j++;
                }
                if (tmp_it == 4)
                {
                    return os;
                }
            }
        }
        else
        {
            os << *ptr;
        }
        ++ptr;
    }
    return os;
}
