#ifndef COLUMN_FLAGS_H_
#define COLUMN_FLAGS_H_
#include "type_cl.h"

struct Column_flags
{
    struct flag16
    {
        flag16()=default;
        flag16(std::initializer_list<bool> list);

        void set(const Type_cl type, const bool val);
        bool get(const Type_cl type) const;
        private:
            uint16_t _data;
    };
    struct flag16 mov;
    struct flag16 add;
    struct flag16 sub;
    struct flag16 mul;
    struct flag16 div;
    struct flag16 eq;
    struct flag16 less;
    /*empty 2 bytes, align on 16 bytes*/
};

const struct Column_flags COLUMN_FLAGS_UNDEFINED = {{false},{false},
                                                    {false},{false},
                                                    {false},{false},
                                                    {false}};

#ifdef ENV64BIT
const struct Column_flags COLUMN_FLAGS_DEFAULT_INT8 = {/*
|--------------------------------------------------------------------------|
|-mov|------------------------------------------------------------------|--|
|-mov|   S8T     |   S16T   |   S32T   |   S64T   |   U8T    |  U16T    |  |
|-mov|*/{true    ,   true   ,   true   ,   true   ,   true   ,  true ,  /* |
|-mov|-----------|----------|----------|----------|----------|----------|--|
|-mov|   U32T    |   U64T   |   FLOAT  |   DOUBLE |   BOOL   |  STRING  |  |
|-mov|*/ true    ,   true   ,   true   ,   true   ,   false  ,  false,  /* |
|-mov|-----------|----------|----------|----------|----------|----------|--|
|-mov|   DATE    |   RESRV  |   RESRV  |   RESRV  |          |          |  |
|-mov|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-add|------------------------------------------------------------------|--|
|-add|   S8T     |   S16T   |   S32T   |   S64T   |   U8T    |  U16T   |  |
|-add|*/{true    ,   true   ,   true   ,   true   ,   true   ,  true ,  /* |
|-add|-----------|----------|----------|----------|----------|----------|--|
|-add|   U32T    |   U64T   |   FLOAT  |   DOUBLE |   BOOL   |  STRING  |  |
|-add|*/ true    ,   true   ,   true   ,   true   ,   false  ,  false,  /* |
|-add|-----------|----------|----------|----------|----------|----------|--|
|-add|   DATE    |   RESRV  |   RESRV  |   RESRV  |          |          |  |
|-add|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-sub|------------------------------------------------------------------|--|
|-sub|   S8T     |   S16T   |   S32T   |   S64T   |   U8T    |  U16T    |  |
|-sub|*/{true    ,   true   ,   true   ,   true   ,   true   ,  true ,  /* |
|-sub|-----------|----------|----------|----------|----------|----------|--|
|-sub|   U32T    |   U64T   |   FLOAT  |   DOUBLE |   BOOL   |  STRING  |  |
|-sub|*/ true    ,   true   ,   true   ,   true   ,   false  ,  false,  /* |
|-sub|-----------|----------|----------|----------|----------|----------|--|
|-sub|   DATE    |   RESRV  |   RESRV  |   RESRV  |          |          |  |
|-sub|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-mul|------------------------------------------------------------------|--|
|-mul|   S8T     |   S16T   |   S32T   |   S64T   |   U8T    |  U16T    |  |
|-mul|*/{true    ,   true   ,   true   ,   true   ,   true   ,  true ,  /* |
|-mul|-----------|----------|----------|----------|----------|----------|--|
|-mul|   U32T    |   U64T   |   FLOAT  |   DOUBLE |   BOOL   |  STRING  |  |
|-mul|*/ true    ,   true   ,   true   ,   true   ,   false  ,  false,  /* |
|-mul|-----------|----------|----------|----------|----------|----------|--|
|-mul|   DATE    |   RESRV  |   RESRV  |   RESRV  |          |          |  |
|-mul|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-div|------------------------------------------------------------------|--|
|-div|   S8T     |   S16T   |   S32T   |   S64T   |   U8T    |  U16T    |  |
|-div|*/{true    ,   true   ,   true   ,   true   ,   true   ,  true ,  /* |
|-div|-----------|----------|----------|----------|----------|----------|--|
|-div|   U32T    |   U64T   |   FLOAT  |   DOUBLE |   BOOL   |  STRING  |  |
|-div|*/ true    ,   true   ,   true   ,   true   ,   false  ,  false,  /* |
|-div|-----------|----------|----------|----------|----------|----------|--|
|-div|   DATE    |   RESRV  |   RESRV  |   RESRV  |          |          |  |
|-div|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-eq-|------------------------------------------------------------------|--|
|-eq-|   S8T     |   S16T   |   S32T   |   S64T   |   U8T    |  U16T   |  |
|-eq-|*/{true    ,   true   ,   true   ,   true   ,   true   ,  true ,  /* |
|-eq-|-----------|----------|----------|----------|----------|----------|--|
|-eq-|   U32T    |   U64T   |   FLOAT  |   DOUBLE |   BOOL   |  STRING  |  |
|-eq-|*/ true    ,   true   ,   true   ,   true   ,   false  ,  false,  /* |
|-eq-|-----------|----------|----------|----------|----------|----------|--|
|-eq-|   DATE    |   RESRV  |   RESRV  |   RESRV  |          |          |  |
|-eq-|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|less|------------------------------------------------------------------|--|
|less|   S8T     |   S16T   |   S32T   |   S64T   |   U8T    |  U16T    |  |
|less|*/{true    ,   true   ,   true   ,   true   ,   true   ,  true ,  /* |
|less|-----------|----------|----------|----------|----------|----------|--|
|less|   U32T    |   U64T   |   FLOAT  |   DOUBLE |   BOOL   |  STRING  |  |
|less|*/ true    ,   true   ,   true   ,   true   ,   false  ,  false,  /* |
|less|-----------|----------|----------|----------|----------|----------|--|
|less|   DATE    |   RESRV  |   RESRV  |   RESRV  |          |          |  |
|less|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|
*/};

const struct Column_flags COLUMN_FLAGS_DEFAULT_INT16 =
                                        COLUMN_FLAGS_DEFAULT_INT8;

const struct Column_flags COLUMN_FLAGS_DEFAULT_INT32 =
                                        COLUMN_FLAGS_DEFAULT_INT8;

const struct Column_flags COLUMN_FLAGS_DEFAULT_INT64 =
                                        COLUMN_FLAGS_DEFAULT_INT8;

const struct Column_flags COLUMN_FLAGS_DEFAULT_UINT8 =
                                        COLUMN_FLAGS_DEFAULT_INT8;

const struct Column_flags COLUMN_FLAGS_DEFAULT_UINT16 =
                                        COLUMN_FLAGS_DEFAULT_INT8;

const struct Column_flags COLUMN_FLAGS_DEFAULT_UINT32 =
                                        COLUMN_FLAGS_DEFAULT_INT8;

const struct Column_flags COLUMN_FLAGS_DEFAULT_UINT64 =
                                        COLUMN_FLAGS_DEFAULT_INT8;

const struct Column_flags COLUMN_FLAGS_DEFAULT_FLOAT =
                                        COLUMN_FLAGS_DEFAULT_INT8;

const struct Column_flags COLUMN_FLAGS_DEFAULT_DOUBLE =
                                        COLUMN_FLAGS_DEFAULT_INT8;

const struct Column_flags COLUMN_FLAGS_DEFAULT_BOOL =
                                        COLUMN_FLAGS_DEFAULT_INT8;

const struct Column_flags COLUMN_FLAGS_DEFAULT_STRING =
                                        COLUMN_FLAGS_DEFAULT_INT8;

const struct Column_flags COLUMN_FLAGS_DEFAULT_DATE =
                                        COLUMN_FLAGS_DEFAULT_INT8;

const struct Column_flags COLUMN_FLAGS_DEFAULT[] = {
                                        COLUMN_FLAGS_DEFAULT_INT8,
                                        COLUMN_FLAGS_DEFAULT_INT16,
                                        COLUMN_FLAGS_DEFAULT_INT32,
                                        COLUMN_FLAGS_DEFAULT_INT64,
                                        COLUMN_FLAGS_DEFAULT_UINT8,
                                        COLUMN_FLAGS_DEFAULT_UINT16,
                                        COLUMN_FLAGS_DEFAULT_UINT32,
                                        COLUMN_FLAGS_DEFAULT_UINT64,
                                        COLUMN_FLAGS_DEFAULT_FLOAT,
                                        COLUMN_FLAGS_DEFAULT_DOUBLE,
                                        COLUMN_FLAGS_DEFAULT_BOOL,
                                        COLUMN_FLAGS_DEFAULT_STRING,
                                        COLUMN_FLAGS_DEFAULT_DATE
};
#endif
#ifdef ENV32BIT
const struct Column_flags COLUMN_FLAGS_DEFAULT_INT8 = {/*
|--------------------------------------------------------------------------|
|-mov|------------------------------------------------------------------|--|
|-mov|   S8T     |   S16T   |   S32T   |   U8T    |   U16T   |  U32T    |  |
|-mov|*/{true    ,   true   ,   true   ,   true   ,   true   ,  true ,  /* |
|-mov|-----------|----------|----------|----------|----------|----------|--|
|-mov|   FLOAT   |   BOOL   |   STRING |   DATE   |   RESRV  |  RESRV   |  |
|-mov|*/ true    ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-mov|-----------|----------|----------|----------|----------|----------|--|
|-mov|   RESRV   |   RESRV  |   RESRV  |   RESRV  |          |          |  |
|-mov|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-add|------------------------------------------------------------------|--|
|-add|   S8T     |   S16T   |   S32T   |   U8T    |   U16T   |  U32T    |  |
|-add|*/{true    ,   true   ,   true   ,   true   ,   true   ,  true ,  /* |
|-add|-----------|----------|----------|----------|----------|----------|--|
|-add|   FLOAT   |   BOOL   |  STRING  |   DATE   |   RESRV  |  RESRV   |  |
|-add|*/ true    ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-add|-----------|----------|----------|----------|----------|----------|--|
|-add|   RESRV   |   RESRV  |   RESRV  |   RESRV  |          |          |  |
|-add|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-sub|------------------------------------------------------------------|--|
|-sub|   S8T     |   S16T   |   S32T   |   U8T    |   U16T   |  U32T    |  |
|-sub|*/{true    ,   true   ,   true   ,   true   ,   true   ,  true ,  /* |
|-sub|-----------|----------|----------|----------|----------|----------|--|
|-sub|   FLOAT   |   BOOL   |   STRING |   DATE   |   RESRV  |  RESRV   |  |
|-sub|*/ true    ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-sub|-----------|----------|----------|----------|----------|----------|--|
|-sub|   RESRV   |   RESRV  |   RESRV  |   RESRV  |          |          |  |
|-sub|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-mul|------------------------------------------------------------------|--|
|-mul|   S8T     |   S16T   |   S32T   |   U8T    |   U16T   |  U32T    |  |
|-mul|*/{true    ,   true   ,   true   ,   true   ,   true   ,  true ,  /* |
|-mul|-----------|----------|----------|----------|----------|----------|--|
|-mul|   FLOAT   |   BOOL   |   STRING |   DATE   |   RESRV  |  RESRV   |  |
|-mul|*/ true    ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-mul|-----------|----------|----------|----------|----------|----------|--|
|-mul|   RESRV   |   RESRV  |   RESRV  |   RESRV  |          |          |  |
|-mul|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-div|------------------------------------------------------------------|--|
|-div|   S8T     |   S16T   |   S32T   |   U8T    |   U16T   |  U32T    |  |
|-div|*/{true    ,   true   ,   true   ,   true   ,   true   ,  true ,  /* |
|-div|-----------|----------|----------|----------|----------|----------|--|
|-div|   FLOAT   |   BOOL   |   STRING |   DATE   |   RESRV  |  RESRV   |  |
|-div|*/ true    ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-div|-----------|----------|----------|----------|----------|----------|--|
|-div|   RESRV   |   RESRV  |   RESRV  |   RESRV  |          |          |  |
|-div|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-eq-|------------------------------------------------------------------|--|
|-eq-|   S8T     |   S16T   |   S32T   |   U8T    |   U16T   |  U32T    |  |
|-eq-|*/{true    ,   true   ,   true   ,   true   ,   true   ,  true ,  /* |
|-eq-|-----------|----------|----------|----------|----------|----------|--|
|-eq-|   FLOAT   |   BOOL   |   STRING |   DATE   |   RESRV  |  RESRV   |  |
|-eq-|*/ true    ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-eq-|-----------|----------|----------|----------|----------|----------|--|
|-eq-|   RESRV   |   RESRV  |   RESRV  |   RESRV  |          |          |  |
|-eq-|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|less|------------------------------------------------------------------|--|
|less|   S8T     |   S16T   |   S32T   |   U8T    |   U16T   |  U32T    |  |
|less|*/{true    ,   true   ,   true   ,   true   ,   true   ,  true ,  /* |
|less|-----------|----------|----------|----------|----------|----------|--|
|less|   FLOAT   |   BOOL   |   STRING |   DATE   |   RESRV  |  RESRV   |  |
|less|*/ true    ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|less|-----------|----------|----------|----------|----------|----------|--|
|less|   RESRV   |   RESRV  |   RESRV  |   RESRV  |          |          |  |
|less|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|
*/};

const struct Column_flags COLUMN_FLAGS_DEFAULT_INT16 =
                                        COLUMN_FLAGS_DEFAULT_INT8;

const struct Column_flags COLUMN_FLAGS_DEFAULT_INT32 =
                                        COLUMN_FLAGS_DEFAULT_INT8;

const struct Column_flags COLUMN_FLAGS_DEFAULT_UINT8 =
                                        COLUMN_FLAGS_DEFAULT_INT8;

const struct Column_flags COLUMN_FLAGS_DEFAULT_UINT16 =
                                        COLUMN_FLAGS_DEFAULT_INT8;

const struct Column_flags COLUMN_FLAGS_DEFAULT_UINT32 =
                                        COLUMN_FLAGS_DEFAULT_INT8;

const struct Column_flags COLUMN_FLAGS_DEFAULT_FLOAT =
                                        COLUMN_FLAGS_DEFAULT_INT8;

const struct Column_flags COLUMN_FLAGS_DEFAULT_BOOL =
                                        COLUMN_FLAGS_DEFAULT_INT8;

const struct Column_flags COLUMN_FLAGS_DEFAULT_STRING =
                                        COLUMN_FLAGS_DEFAULT_INT8;

const struct Column_flags COLUMN_FLAGS_DEFAULT_DATE =
                                        COLUMN_FLAGS_DEFAULT_INT8;

const struct Column_flags COLUMN_FLAGS_DEFAULT[] = {
                                        COLUMN_FLAGS_DEFAULT_INT8,
                                        COLUMN_FLAGS_DEFAULT_INT16,
                                        COLUMN_FLAGS_DEFAULT_INT32,
                                        COLUMN_FLAGS_DEFAULT_UINT8,
                                        COLUMN_FLAGS_DEFAULT_UINT16,
                                        COLUMN_FLAGS_DEFAULT_UINT32,
                                        COLUMN_FLAGS_DEFAULT_FLOAT,
                                        COLUMN_FLAGS_DEFAULT_BOOL,
                                        COLUMN_FLAGS_DEFAULT_STRING,
                                        COLUMN_FLAGS_DEFAULT_DATE
};
#endif
#endif
