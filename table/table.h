#ifndef TABLE_H_
#define TABLE_H_
#include "column/column.h"

class Table
{
    public:
        enum class ERROR
        {
            SUCCESS  = 0,
            BAD_SIZE = 1,
            BAD_TYPE = 2
        };

        Table(const size_t alloc_data=2048,
              const size_t alloc_cls=32);
        ~Table();

        size_t alloc_size() const; /*return size in bytes*/
        size_t alloc_cls() const;  /*return size of columns in amounts*/
        size_t size() const;       /*return size in amounts*/

        enum ERROR push_back_column(const size_t size,
                                    const Type_cl type);

        void append(const size_t size,
                    const Type_cl type,
                    const size_t pos);

        Column& operator[](const size_t pos);
        const Column& operator[](const size_t pos) const;
    private:
        size_t  alloc_data_;
        uint8_t* data_;

        size_t  size_cls_;
        size_t  alloc_cls_;
        Column* cls_;
};
#endif
