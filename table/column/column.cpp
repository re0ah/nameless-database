#include "column.h"

namespace settings
{
	size_t COLUMN_ALLOC_FMT_SIZE = 64;
}

Column::Column(const Column_flags& clf,
               char* fmt,
               const size_t size,
               uint8_t* const data,
               const size_t pos,
               const struct Type_cl type)
    : flags_{clf}
    , fmt_{fmt}
    , data_size_{size * type.size()}
    , size_{size}
    , data_{data}
    , pos_{pos}
    , type_{type}
{}

Column::~Column()
{}

size_t Column::data_size() const
{
    return data_size_;
}

size_t Column::size() const
{
    return size_;
}

size_t Column::pos() const
{
    return pos_;
}

Type_cl Column::type() const
{
    return type_;
}

bool Column::is_valid() const
{
    return ((type_ != Type_cl::ERROR::UNDEFINED_TYPE) ||
            (data_ != nullptr)
           );
}

Column_flags& Column::flags()
{
    return flags_;
}

const char* const Column::fmt() const
{
    return fmt_;
}

void Column::set_fmt(const char* str)
{
    if(strlen(str) >= settings::COLUMN_ALLOC_FMT_SIZE)
    {
        return;
    }
    if(!if_alloc_fmt())
    {
        fmt_ = (char*)malloc(settings::COLUMN_ALLOC_FMT_SIZE);
    }
    strcpy(fmt_, str);
}

void Column::set_default_fmt()
{
    if(if_alloc_fmt())
    {
        free(fmt_);
    }
    fmt_ = nullptr;
}

bool Column::if_alloc_fmt() const
{
    return fmt_ != nullptr;
}

Table_data Column::operator[](const size_t pos)
{
    if (pos >= size_)
    {
        return Table_data(COLUMN_FLAGS_UNDEFINED,
                          fmt_,
                          nullptr,
                          Type_cl::ERROR::UNDEFINED_TYPE);
    }
    return Table_data(flags_,
                      fmt_,
                      &data_[pos * type_.size()],
                      type_);
}

const Table_data Column::operator[](const size_t pos) const
{
    return (*(const_cast<Column*>(this)))[pos]; /*call non-const[]*/
}
