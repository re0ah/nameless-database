#include "table.h"

Table::Table(const size_t alloc_data,
             const size_t alloc_cls)
    : alloc_data_{alloc_data}
    , data_{(uint8_t*)xcalloc(alloc_data, sizeof(uint8_t))}
    , size_cls_{0}
    , alloc_cls_{alloc_cls}
    , cls_{(Column*)xcalloc(alloc_cls, sizeof(Column))}
{}

Table::~Table()
{
    xfree(data_);
    xfree(cls_);
}

size_t Table::alloc_size() const
{
    return alloc_data_;
}

size_t Table::alloc_cls() const
{
    return alloc_cls_;
}

size_t Table::size() const
{
    return size_cls_;
}

enum Table::ERROR Table::push_back_column(const size_t size,
                                          const Type_cl type)
{
    if(type == Type_cl::ERROR::UNDEFINED_TYPE)
    {
        return ERROR::BAD_TYPE;
    }

    if (size_cls_ == alloc_cls_)
    {
        alloc_cls_ *= 1.5;
        cls_ = (Column*)xrealloc(cls_, alloc_cls_ * sizeof(Column));
    }

    /*start of column data_ in table data_*/
    size_t pos_data = ((size_cls_ > 0)
                      ? (cls_[size_cls_ - 1].pos() + cls_[size_cls_ - 1].data_size())
                      : (0));

    /*to which _data pos will take the place this column*/
    const size_t front_pos = pos_data + (size * type.size());
    if (alloc_size() <= front_pos)
    {
        const size_t mul = ((size_t)(front_pos / alloc_data_));
        alloc_data_ *= mul * 1.5;
        data_ = (uint8_t*)xrealloc(data_, alloc_data_);
    }

    cls_[size_cls_] = Column(COLUMN_FLAGS_DEFAULT[static_cast<size_t>(type.data())],
                             nullptr,
                             size,
                             &data_[pos_data],
                             pos_data,
                             type);
    ++size_cls_;
    return ERROR::SUCCESS;
}

void Table::append(const size_t size,
                   const Type_cl type,
                   const size_t pos)
{
    if(pos >= size_cls_)
    {
        if(pos >= alloc_cls_) /*true -> need alloc more columns*/
        {
            const size_t mul = ((size_t)(pos / alloc_cls_));
            alloc_cls_ *= mul * 1.5;
            cls_ = (Column*)xrealloc(cls_, alloc_cls_ * sizeof(Column));
        }
        /*front memory it is pos in data_ where Column end*/
        const size_t front_memory = cls_[size_cls_ - 1].pos() + (size * type.size());
        if(front_memory > alloc_data_) /*true -> need alloc more data*/
        {
            const size_t mul = ((size_t)(front_memory / alloc_data_));
            alloc_data_ *= mul * 1.5;
            data_ = (uint8_t*)xrealloc(data_, alloc_data_);
        }
        const size_t mem_start = cls_[size_cls_ - 1].pos() + cls_[size_cls_ - 1].size();
        for(; size_cls_ < pos; ++size_cls_) /*if need create empty columns*/
        {
            cls_[size_cls_] = Column(COLUMN_FLAGS_DEFAULT_INT32,
                                     nullptr,
                                     0,
                                     &data_[mem_start],
                                     mem_start,
                                     Type_cl::GENERAL::INT32);
        }
        cls_[size_cls_] = Column(COLUMN_FLAGS_DEFAULT[static_cast<size_t>(type.data())],
                                 nullptr,
                                 size,
                                 &data_[mem_start],
                                 mem_start,
                                 type);
        ++size_cls_;
    }
    else if(pos < size_cls_)
    {

    }
}

Column& Table::operator[](const size_t pos)
{
    if (size_cls_ <= pos)
    {
        cls_[0] = Column();
        return cls_[0];
    }
    return cls_[pos];
}

const Column& Table::operator[](const size_t pos) const
{
    return (*(const_cast<Table*>(this)))[pos]; /*call non-const[]*/
}
