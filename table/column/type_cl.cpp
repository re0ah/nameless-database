#include "type_cl.h"

Type_cl::Type_cl(const struct Type_cl& cl)
    : data_{static_cast<uint8_t>(cl.data())}
{}

Type_cl::Type_cl(const enum Type_cl::GENERAL val)
    : data_{static_cast<uint8_t>(val)}
{}

Type_cl::Type_cl(const enum Type_cl::ERROR val)
    : data_{static_cast<uint8_t>(val)}
{}

Type_cl::~Type_cl()
{}

Type_cl& Type_cl::operator=(const struct Type_cl type)
{
    data_ = static_cast<uint8_t>(type.data());
    return *this;
}

Type_cl& Type_cl::operator=(const enum GENERAL type)
{
    data_ = static_cast<uint8_t>(type);
    return *this;
}

Type_cl& Type_cl::operator=(const enum ERROR type)
{
    data_ = static_cast<uint8_t>(type);
    return *this;
}

bool Type_cl::operator==(const Type_cl type) const
{
    return (this->data_ == type.data_);
}

bool Type_cl::operator==(const enum GENERAL type) const
{
    return (this->data_ == static_cast<uint8_t>(type));
}

bool Type_cl::operator==(const enum ERROR type) const
{
    return (this->data_ == static_cast<uint8_t>(type));
}

bool Type_cl::operator!=(const Type_cl type) const
{
    return (this->data_ != type.data_);
}

bool Type_cl::operator!=(const enum GENERAL type) const
{
    return (this->data_ != static_cast<uint8_t>(type));
}

bool Type_cl::operator!=(const enum ERROR type) const
{
    return (this->data_ != static_cast<uint8_t>(type));
}

enum Type_cl::GENERAL Type_cl::data() const
{
    return Type_cl::GENERAL(data_);
}

#ifdef ENV64BIT
size_t Type_cl::size() const
{
    switch(data())
    {
        case GENERAL::INT8:
            return sizeof(struct Type<uint8_t>);
        case GENERAL::INT16:
            return sizeof(struct Type<int16_t>);
        case GENERAL::INT32:
            return sizeof(struct Type<int32_t>);
        case GENERAL::INT64:
            return sizeof(struct Type<int64_t>);
        case GENERAL::UINT8:
            return sizeof(struct Type<uint8_t>);
        case GENERAL::UINT16:
            return sizeof(struct Type<uint16_t>);
        case GENERAL::UINT32:
            return sizeof(struct Type<uint32_t>);
        case GENERAL::UINT64:
            return sizeof(struct Type<uint64_t>);
        case GENERAL::FLOAT:
            return sizeof(struct Type<float>);
        case GENERAL::DOUBLE:
            return sizeof(struct Type<double>);
        case GENERAL::BOOL:
            return sizeof(struct Type<bool>);
        case GENERAL::STRING:
            return sizeof(struct string);
        case GENERAL::DATE:
            return sizeof(struct date);
    }
    return 0; /*it's error value*/
}
#endif

#ifdef ENV32BIT
size_t Type_cl::size() const
{
    switch(data())
    {
        case GENERAL::INT8:
            return sizeof(struct Type<int8_t>);
        case GENERAL::INT16:
            return sizeof(struct Type<int16_t>);
        case GENERAL::INT32:
            return sizeof(struct Type<int32_t>);
        case GENERAL::UINT8:
            return sizeof(struct Type<uint8_t>);
        case GENERAL::UINT16:
            return sizeof(struct Type<uint16_t>);
        case GENERAL::UINT32:
            return sizeof(struct Type<uint32_t>);
        case GENERAL::FLOAT:
            return sizeof(struct Type<float>);
        case GENERAL::BOOL:
            return sizeof(struct Type<bool>);
        case GENERAL::STRING:
            return sizeof(struct string);
        case GENERAL::DATE:
            return sizeof(struct date);
    }
    return 0; /*it's error value*/
}
#endif

#ifdef ENV64BIT
std::ostream& operator<<(std::ostream& os, const Type_cl& value)
{
    static const char* const type_names[] = {"INT8",
                                             "INT16",
                                             "INT32",
                                             "INT64",
                                             "UINT8",
                                             "UINT16",
                                             "UINT32",
                                             "UINT64",
                                             "float",
                                             "double",
                                             "bool",
                                             "string",
                                             "date"};
    os << type_names[static_cast<size_t>(value.data())];
    return os;
}
#endif

#ifdef ENV32BIT
std::ostream& operator<<(std::ostream& os, const Type_cl& value)
{
    static const char* const type_names[] = {"INT8",
                                             "INT16",
                                             "INT32",
                                             "UINT8",
                                             "UINT16",
                                             "UINT32",
                                             "float",
                                             "bool",
                                             "string",
                                             "date"};
    os << type_names[static_cast<size_t>(value.data())];
    return os;
}
#endif
