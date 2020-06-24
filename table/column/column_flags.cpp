#include "column_flags.h"

Column_flags::flag16::flag16(std::initializer_list<bool> list)
{
    auto it = list.begin();
    for(size_t i = 0; i < list.size(); i++)
    {
        _data |= ((*it) << i);
        it++;
    }
}

bool Column_flags::flag16::get(const Type_cl type) const
{
    return static_cast<bool>(_data & (1 << type.size()));
}

void Column_flags::flag16::set(const Type_cl type, const bool val)
{
    _data &= ~(1 << type.size());
    _data |= (val << type.size());
}

