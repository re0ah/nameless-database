#ifndef TYPE_CL_H
#define TYPE_CL_H
#include <stdint.h>
#include <ostream>
#include "../../types/type.h"

#if _WIN32 || _WIN64
    #if _WIN64
        #define ENV64BIT
    #else
        #define ENV32BIT
    #endif
#endif

#if __GNUC__
    #if __x86_64__ || __ppc64__
        #define ENV64BIT
    #else
        #define ENV32BIT
    #endif
#endif

/*type of Table_data*/
struct Type_cl
{
#ifdef ENV64BIT
    enum class GENERAL : uint8_t
    {
        INT8   = 0,
        INT16  = 1,
        INT32  = 2,
        INT64  = 3,
        UINT8  = 4,
        UINT16 = 5,
        UINT32 = 6,
        UINT64 = 7,
        FLOAT  = 8,
        DOUBLE = 9,
        BOOL   = 10,
        STRING = 11,
        DATE   = 12
    };
#else
    enum class GENERAL : uint8_t
    {
        INT8   = 0,
        INT16  = 1,
        INT32  = 2,
        UINT8  = 3,
        UINT16 = 4,
        UINT32 = 5,
        FLOAT  = 6,
        BOOL   = 7,
        STRING = 8,
        DATE   = 9
    };
#endif
    enum class ERROR
    {
        UNDEFINED_TYPE = 255
    };

    Type_cl(const struct Type_cl& cl);
    Type_cl(const enum GENERAL val);
    Type_cl(const enum ERROR   val);
    ~Type_cl();

    Type_cl& operator=(const struct Type_cl type);
    /*Yes, I know what constructor can create Type_cl from all that's enum's
      and call previous functuon. But it's slow*/
    Type_cl& operator=(const enum GENERAL type);
    Type_cl& operator=(const enum ERROR type);

    bool operator==(const Type_cl type) const;
    bool operator==(const enum GENERAL type) const;
    bool operator==(const enum ERROR type) const;

    bool operator!=(const Type_cl type) const;
    bool operator!=(const enum GENERAL type) const;
    bool operator!=(const enum ERROR type) const;

    enum GENERAL data() const;
    size_t size() const;

    template <typename T>
    static enum GENERAL type_check();
    private:
        uint8_t data_;
};

/*static method for identif type in templates.
  Used like: type_check<T>()
*/
template <typename T>
enum Type_cl::GENERAL Type_cl::type_check()
{
#define check(type, ret_value) \
    if constexpr(std::is_same<T, type>::value) \
        return ret_value;

    check(struct Type<int8_t>,  GENERAL::INT8);
    check(int8_t,  GENERAL::INT8);

    check(struct Type<int16_t>, GENERAL::INT16);
    check(int16_t, GENERAL::INT16);

    check(struct Type<int32_t>, GENERAL::INT32);
    check(int32_t, GENERAL::INT32);

    check(struct Type<uint8_t>,  GENERAL::UINT8);
    check(uint8_t,  GENERAL::UINT8);

    check(struct Type<uint16_t>, GENERAL::UINT16);
    check(uint16_t, GENERAL::UINT16);

    check(struct Type<uint32_t>, GENERAL::UINT32);
    check(uint32_t, GENERAL::UINT32);

    check(struct Type<float>, GENERAL::FLOAT);
    check(float, GENERAL::FLOAT);

    check(struct Type<bool>, GENERAL::BOOL);
    check(bool, GENERAL::BOOL);

    check(struct string, GENERAL::STRING);
    check(struct date,   GENERAL::DATE);

#ifdef ENV64BIT
    check(struct Type<int64_t>,  GENERAL::INT64);
    check(int64_t,  GENERAL::INT64);

    check(struct Type<uint64_t>, GENERAL::UINT64);
    check(uint64_t, GENERAL::UINT64);
    check(double,   GENERAL::DOUBLE);
#endif

    /*to here is impossible to get, if were transfed allowed types.
      It's type check trap*/
    assert(0 && "Passed not allowed type");
#undef check
}

std::ostream& operator<<(std::ostream& os, const Type_cl& value);

#endif
