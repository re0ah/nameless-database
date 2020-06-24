#ifndef COLUMN_H_
#define COLUMN_H_
#include "table_data.h"

/*Column stored: pointer to (data_ + pos_) of table class
                 size of this column in bytes
                 size of this column in elements
                 pos in data_ pointer*/
class Column
{
    public:
        Column(const Column_flags& clf=COLUMN_FLAGS_UNDEFINED,
               char* fmt=nullptr,
               const size_t size=0,
               uint8_t* const data=nullptr,
               const size_t pos=0,
               const struct Type_cl type=Type_cl::ERROR::UNDEFINED_TYPE);
        ~Column();

        size_t data_size() const; /*return size in bytes*/
        size_t size() const;      /*return amount of elements*/
        size_t pos() const;       /*return pos on data_ in table*/
        Type_cl type() const;

        bool is_valid() const;

        Column_flags& flags();

        const char* const fmt() const;
        void set_fmt(const char* str);
        void set_default_fmt();
        bool if_alloc_fmt() const;

        Table_data operator[](const size_t pos);
        const Table_data operator[](const size_t pos) const;
    private:
        Column_flags flags_;
        char* fmt_;
        size_t data_size_;
        size_t size_;
        uint8_t* data_;
        size_t pos_;
        Type_cl type_;
};
#endif
