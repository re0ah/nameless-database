#ifndef TABLE_DATA_H_
#define TABLE_DATA_H_
#include "column_flags.h"

/*element of data_ in table
  stored type and pointer to element of data_
  defines operations and interaction between types
 */
class Table_data
{
    public:
        template <typename T>
        Table_data(const T& val);

        Table_data(const Column_flags& flags,
                   const char* const fmt,
                   uint8_t* const val,
                   const Type_cl type);
        ~Table_data();

        struct Type_cl type() const;
        uint8_t* data();
        const uint8_t* data() const;

        const struct Column_flags& flags() const;
        const char* const fmt() const;

        Table_data& operator=(const Table_data& value);
        Table_data& operator+=(const Table_data& value);
        Table_data& operator-=(const Table_data& value);
        Table_data& operator*=(const Table_data& value);
        Table_data& operator/=(const Table_data& value);

        bool operator==(const Table_data& value);
        bool operator!=(const Table_data& value);
        bool operator< (const Table_data& value);
        bool operator> (const Table_data& value);
        bool operator<=(const Table_data& value);
        bool operator>=(const Table_data& value);
    private:
        const struct Column_flags& flags_;
        const char* const fmt_;
        uint8_t* const data_;
        struct Type_cl type_;
};

template <typename T>
Table_data::Table_data(const T& val)
    : flags_{COLUMN_FLAGS_DEFAULT[static_cast<size_t>(Type_cl::type_check<T>())]}
    , fmt_{nullptr}
    , data_{(uint8_t*)&val}
    , type_{Type_cl::type_check<T>()}
{}

std::ostream& operator<<(std::ostream& os, const Table_data& value);
#endif
