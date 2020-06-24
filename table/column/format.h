#ifndef FORMAT_H_
#define FORMAT_H_
namespace FORMAT
{
/*reference about formats
    all formats begin from '%', and next symbol define the format
    standard formats:
        1.  s8  - int8_t
        2.  s16 - int16_t
        3.  s32 - int32_t
        4.  s64 - int64_t  | only for x86_64
        5.  u8  - uint8_t
        6.  u16 - uint16_t
        7.  u32 - uint32_t 
        8.  u64 - uint64_t | only for x86_64
        9.  f   - float 
        10. d   - double   | only for x86_64
        11. b   - bool
    formats for string:
        1. s - string
    formats for date (are calculated from unix time):
        1. Y - year
        2. M - month
        3. D - day
        4. H - hour
        5. m - minute
        6. S - second
        date also must be output just how number, using standard formats
 */
    namespace STANDARD
    {
        const char s8[]  = "8";
        const char s16[] = "16";
        const char s32[] = "32";
#ifdef ENV64BIT
        const char s64[] = "64";
#endif
        const char u8[]  = "u8";
        const char u16[] = "u16";
        const char u32[] = "u32";
#ifdef ENV64BIT
        const char u64[] = "u64";
#endif
        const char f[]   = "f";
#ifdef ENV64BIT
        const char d[]   = "d";
#endif
        const char b[]   = "b";
    };
    namespace STRING
    {
        const char s[] = "s";
    };
    namespace DATE
    {
        const char Y[] = "Y";
        const char M[] = "M";
        const char D[] = "D";
        const char H[] = "H";
        const char m[] = "m";
        const char S[] = "S";
    };
    enum class FMT
    {
        s8,
        s16,
        s32,
#ifdef ENV64BIT
        s64,
#endif
        u8,
        u16,
        u32,
#ifdef ENV64BIT
        u64,
#endif
        f,
#ifdef ENV64BIT
        d,
#endif
        b,
        s,
        Y,
        M,
        D,
        H,
        m,
        S
    };
    namespace DEFAULT
    {
        const char INT8[]  = "%8";
        const char INT16[] = "%16";
        const char INT32[] = "%32";
#ifdef ENV64BIT
        const char INT64[] = "%64";
#endif

        const char UINT8[]  = "%u8";
        const char UINT16[] = "%u16";
        const char UINT32[] = "%u32";
#ifdef ENV64BIT
        const char UINT64[] = "%u64";
#endif
        const char FLOAT[]  = "%f";
#ifdef ENV64BIT
        const char DOUBLE[] = "%d";
#endif
        const char BOOL[]   = "%b";
        const char STRING[] = "%s";
        const char DATE[]   = "%D-%M-%Y %H:%m:%S";

        const char* FMT[] = {
                            INT8,
                            INT16,
                            INT32,
#ifdef ENV64BIT
                            INT64,
#endif
                            UINT8,
                            UINT16,
                            UINT32,
#ifdef ENV64BIT
                            UINT64,
#endif
                            FLOAT,
#ifdef ENV64BIT
                            DOUBLE,
#endif
                            BOOL,
                            STRING,
                            DATE
        };
    };
};
#endif
