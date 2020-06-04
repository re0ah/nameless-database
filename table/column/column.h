#pragma once
#include <bitset>
#include "type.h"

namespace db_column
{
/*class Column store vector<uint64_t> and must be
  store data of any type, for which declare
  interaction with another types. All types in this
  vector are same, so it Column in table, within 1
  col type only 1*/
class Column
{
	public:
		struct flags_column
		{
			struct flag_type set;
			struct flag_type add;
			struct flag_type sub;
			struct flag_type mul;
			struct flag_type div;
			struct flag_type equal;
			struct flag_type less;

			void set_all(const struct flags_column& flags);
			void set_default(const TYPE type);
		};

		Column(const TYPE type=TYPE::NONE,
			   const size_t reserve=16);
        Column(const QString& name,
			   const TYPE type=TYPE::NONE,
			   const size_t reserve=16);
        Column(const QString&& name,
			   const TYPE type=TYPE::NONE,
			   const size_t reserve=16);
		Column(const Column& col);
        Column(Column&& col);
		~Column();
		void swap(Column& lhs, Column& rhs);
		Column& operator=(Column col);
        Column& operator=(Column&& col);
		std::pair<TYPE, uint64_t&> operator[](const size_t index);
		const std::pair<TYPE, uint64_t> operator[](const size_t index) const;
		TYPE type() const;
		size_t size() const;
		void reserve(const size_t size);
		void push_back(const std::pair<TYPE, uint64_t> data);
		void resize(const size_t size);
		void set_type_column(const TYPE);

		std::vector<uint64_t> all_data() const;

		bool check_size(const size_t pos) const;
		void check_and_resize(const size_t pos);

		void set(std::pair<TYPE, uint64_t> data, const size_t pos);
		void add(std::pair<TYPE, uint64_t> data, const size_t pos);
		void sub(std::pair<TYPE, uint64_t> data, const size_t pos);
		void mul(std::pair<TYPE, uint64_t> data, const size_t pos);
		void div(std::pair<TYPE, uint64_t> data, const size_t pos);
		
		bool equal(std::pair<TYPE, uint64_t> data, const size_t pos) const;
		bool not_equal(std::pair<TYPE, uint64_t> data, const size_t pos) const;
		bool less(std::pair<TYPE, uint64_t> data, const size_t pos) const;
		bool more(std::pair<TYPE, uint64_t> data, const size_t pos) const;
		bool less_or_equal(std::pair<TYPE, uint64_t> data, const size_t pos) const;
		bool more_or_equal(std::pair<TYPE, uint64_t> data, const size_t pos) const;
		
        struct flags_column& flags();

        TIME_TYPE& flag_time();

        void set_name(const QString& name);
        void set_name(const QString&& name);
        void set_name(const char* const name);
        QString name() const;
	private:
        TYPE _type;
		std::vector<uint64_t> _data;
		struct flags_column _flags;
        QString _fmt;
        QString _name;
};
const struct Column::flags_column COLUMN_FLAGS_DEFAULT_NONE = {/*
|--------------------------------------------------------------------------|
|-set|------------------------------------------------------------------|--|
|-set|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-set|*/{false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-set|-----------|----------|----------|----------|----------|----------|--|
|-set|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-set|*/ false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-set|-----------|----------|----------|----------|----------|----------|--|
|-set|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-set|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-add|------------------------------------------------------------------|--|
|-add|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-add|*/{false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-add|-----------|----------|----------|----------|----------|----------|--|
|-add|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-add|*/ false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-add|-----------|----------|----------|----------|----------|----------|--|
|-add|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-add|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-sub|------------------------------------------------------------------|--|
|-sub|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-sub|*/{false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-sub|-----------|----------|----------|----------|----------|----------|--|
|-sub|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-sub|*/ false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-sub|-----------|----------|----------|----------|----------|----------|--|
|-sub|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-sub|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-mul|------------------------------------------------------------------|--|
|-mul|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-mul|*/{false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-mul|-----------|----------|----------|----------|----------|----------|--|
|-mul|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-mul|*/ false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-mul|-----------|----------|----------|----------|----------|----------|--|
|-mul|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-mul|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-div|------------------------------------------------------------------|--|
|-div|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-div|*/{false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-div|-----------|----------|----------|----------|----------|----------|--|
|-div|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-div|*/ false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-div|-----------|----------|----------|----------|----------|----------|--|
|-div|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-div|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-eq-|------------------------------------------------------------------|--|
|-eq-|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-eq-|*/{false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-eq-|-----------|----------|----------|----------|----------|----------|--|
|-eq-|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-eq-|*/ false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-eq-|-----------|----------|----------|----------|----------|----------|--|
|-eq-|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-eq-|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|less|------------------------------------------------------------------|--|
|less|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|less|*/{false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|less|-----------|----------|----------|----------|----------|----------|--|
|less|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|less|*/ false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|less|-----------|----------|----------|----------|----------|----------|--|
|less|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|less|*/ false   ,   false  ,   false  ,   false} /*         |          |  |
|--------------------------------------------------------------------------|
*/};

const struct Column::flags_column COLUMN_FLAGS_DEFAULT_BOOL = {/*
|--------------------------------------------------------------------------|
|-set|------------------------------------------------------------------|--|
|-set|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-set|*/{false   ,   true   ,   false  ,   false  ,   false  ,  false,  /* |
|-set|-----------|----------|----------|----------|----------|----------|--|
|-set|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-set|*/ false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-set|-----------|----------|----------|----------|----------|----------|--|
|-set|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-set|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-add|------------------------------------------------------------------|--|
|-add|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-add|*/{false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-add|-----------|----------|----------|----------|----------|----------|--|
|-add|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-add|*/ false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-add|-----------|----------|----------|----------|----------|----------|--|
|-add|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-add|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-sub|------------------------------------------------------------------|--|
|-sub|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-sub|*/{false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-sub|-----------|----------|----------|----------|----------|----------|--|
|-sub|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-sub|*/ false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-sub|-----------|----------|----------|----------|----------|----------|--|
|-sub|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-sub|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-mul|------------------------------------------------------------------|--|
|-mul|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-mul|*/{false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-mul|-----------|----------|----------|----------|----------|----------|--|
|-mul|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-mul|*/ false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-mul|-----------|----------|----------|----------|----------|----------|--|
|-mul|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-mul|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-div|------------------------------------------------------------------|--|
|-div|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-div|*/{false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-div|-----------|----------|----------|----------|----------|----------|--|
|-div|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-div|*/ false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-div|-----------|----------|----------|----------|----------|----------|--|
|-div|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-div|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-eq-|------------------------------------------------------------------|--|
|-eq-|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-eq-|*/{false   ,   true   ,   false  ,   false  ,   false  ,  false,  /* |
|-eq-|-----------|----------|----------|----------|----------|----------|--|
|-eq-|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-eq-|*/ false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-eq-|-----------|----------|----------|----------|----------|----------|--|
|-eq-|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-eq-|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|less|------------------------------------------------------------------|--|
|less|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|less|*/{false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|less|-----------|----------|----------|----------|----------|----------|--|
|less|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|less|*/ false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|less|-----------|----------|----------|----------|----------|----------|--|
|less|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|less|*/ false   ,   false  ,   false  ,   false} /*         |          |  |
|--------------------------------------------------------------------------|
*/};

const struct Column::flags_column COLUMN_FLAGS_DEFAULT_UINT8_T = {/*
|--------------------------------------------------------------------------|
|-set|------------------------------------------------------------------|--|
|-set|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-set|*/{false   ,   false  ,   true   ,   true   ,   true   ,  true,   /* |
|-set|-----------|----------|----------|----------|----------|----------|--|
|-set|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-set|*/ true    ,   true   ,   true   ,   true   ,   false  ,  false,  /* |
|-set|-----------|----------|----------|----------|----------|----------|--|
|-set|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-set|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-add|------------------------------------------------------------------|--|
|-add|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-add|*/{false   ,   false  ,   true   ,   true   ,   true   ,  true,   /* |
|-add|-----------|----------|----------|----------|----------|----------|--|
|-add|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-add|*/ true    ,   true   ,   true   ,   true   ,   false  ,  false,  /* |
|-add|-----------|----------|----------|----------|----------|----------|--|
|-add|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-add|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-sub|------------------------------------------------------------------|--|
|-sub|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-sub|*/{false   ,   false  ,   true   ,   true   ,   true   ,  true,   /* |
|-sub|-----------|----------|----------|----------|----------|----------|--|
|-sub|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-sub|*/ true    ,   true   ,   true   ,   true   ,   false  ,  false,  /* |
|-sub|-----------|----------|----------|----------|----------|----------|--|
|-sub|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-sub|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-mul|------------------------------------------------------------------|--|
|-mul|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-mul|*/{false   ,   false  ,   true   ,   true   ,   true   ,  true,   /* |
|-mul|-----------|----------|----------|----------|----------|----------|--|
|-mul|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-mul|*/ true    ,   true   ,   true   ,   true   ,   false  ,  false,  /* |
|-mul|-----------|----------|----------|----------|----------|----------|--|
|-mul|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-mul|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-div|------------------------------------------------------------------|--|
|-div|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-div|*/{false   ,   false  ,   true   ,   true   ,   true   ,  true,   /* |
|-div|-----------|----------|----------|----------|----------|----------|--|
|-div|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-div|*/ true    ,   true   ,   true   ,   true   ,   false  ,  false,  /* |
|-div|-----------|----------|----------|----------|----------|----------|--|
|-div|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-div|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-eq-|------------------------------------------------------------------|--|
|-eq-|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-eq-|*/{false   ,   false  ,   true   ,   true   ,   true   ,  true,   /* |
|-eq-|-----------|----------|----------|----------|----------|----------|--|
|-eq-|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-eq-|*/ true    ,   true   ,   true   ,   true   ,   false  ,  false,  /* |
|-eq-|-----------|----------|----------|----------|----------|----------|--|
|-eq-|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-eq-|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|less|------------------------------------------------------------------|--|
|less|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|less|*/{false   ,   false  ,   true   ,   true   ,   true   ,  true,   /* |
|less|-----------|----------|----------|----------|----------|----------|--|
|less|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|less|*/ true    ,   true   ,   true   ,   true   ,   false  ,  false,  /* |
|less|-----------|----------|----------|----------|----------|----------|--|
|less|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|less|*/ false   ,   false  ,   false  ,   false} /*         |          |  |
|--------------------------------------------------------------------------|
*/};

const struct Column::flags_column COLUMN_FLAGS_DEFAULT_INT8_T = db_column::COLUMN_FLAGS_DEFAULT_UINT8_T;

const struct Column::flags_column COLUMN_FLAGS_DEFAULT_UINT16_T = db_column::COLUMN_FLAGS_DEFAULT_UINT8_T;

const struct Column::flags_column COLUMN_FLAGS_DEFAULT_INT16_T = db_column::COLUMN_FLAGS_DEFAULT_UINT8_T;

const struct Column::flags_column COLUMN_FLAGS_DEFAULT_UINT32_T = db_column::COLUMN_FLAGS_DEFAULT_UINT8_T;

const struct Column::flags_column COLUMN_FLAGS_DEFAULT_INT32_T = db_column::COLUMN_FLAGS_DEFAULT_UINT8_T;

const struct Column::flags_column COLUMN_FLAGS_DEFAULT_UINT64_T = db_column::COLUMN_FLAGS_DEFAULT_UINT8_T;

const struct Column::flags_column COLUMN_FLAGS_DEFAULT_INT64_T = db_column::COLUMN_FLAGS_DEFAULT_UINT8_T;

const struct Column::flags_column COLUMN_FLAGS_DEFAULT_FLOAT = {/*
|--------------------------------------------------------------------------|
|-set|------------------------------------------------------------------|--|
|-set|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-set|*/{false   ,   false  ,   true   ,   true   ,   true   ,  true,   /* |
|-set|-----------|----------|----------|----------|----------|----------|--|
|-set|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-set|*/ true    ,   true   ,   true   ,   true   ,   true   ,  true,  /* |
|-set|-----------|----------|----------|----------|----------|----------|--|
|-set|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-set|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-add|------------------------------------------------------------------|--|
|-add|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-add|*/{false   ,   false  ,   true   ,   true   ,   true   ,  true,   /* |
|-add|-----------|----------|----------|----------|----------|----------|--|
|-add|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-add|*/ true    ,   true   ,   true   ,   true   ,   true   ,  true,  /* |
|-add|-----------|----------|----------|----------|----------|----------|--|
|-add|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-add|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-sub|------------------------------------------------------------------|--|
|-sub|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-sub|*/{false   ,   false  ,   true   ,   true   ,   true   ,  true,   /* |
|-sub|-----------|----------|----------|----------|----------|----------|--|
|-sub|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-sub|*/ true    ,   true   ,   true   ,   true   ,   true   ,  true,  /* |
|-sub|-----------|----------|----------|----------|----------|----------|--|
|-sub|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-sub|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-mul|------------------------------------------------------------------|--|
|-mul|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-mul|*/{false   ,   false  ,   true   ,   true   ,   true   ,  true,   /* |
|-mul|-----------|----------|----------|----------|----------|----------|--|
|-mul|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-mul|*/ true    ,   true   ,   true   ,   true   ,   true   ,  true,  /* |
|-mul|-----------|----------|----------|----------|----------|----------|--|
|-mul|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-mul|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-div|------------------------------------------------------------------|--|
|-div|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-div|*/{false   ,   false  ,   true   ,   true   ,   true   ,  true,   /* |
|-div|-----------|----------|----------|----------|----------|----------|--|
|-div|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-div|*/ true    ,   true   ,   true   ,   true   ,   true   ,  true,   /* |
|-div|-----------|----------|----------|----------|----------|----------|--|
|-div|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-div|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-eq-|------------------------------------------------------------------|--|
|-eq-|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-eq-|*/{false   ,   false  ,   true   ,   true   ,   true   ,  true,   /* |
|-eq-|-----------|----------|----------|----------|----------|----------|--|
|-eq-|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-eq-|*/ true    ,   true   ,   true   ,   true   ,   true   ,  true,   /* |
|-eq-|-----------|----------|----------|----------|----------|----------|--|
|-eq-|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-eq-|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|less|------------------------------------------------------------------|--|
|less|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|less|*/{false   ,   false  ,   true   ,   true   ,   true   ,  true,   /* |
|less|-----------|----------|----------|----------|----------|----------|--|
|less|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|less|*/ true    ,   true   ,   true   ,   true   ,   true   ,  true,   /* |
|less|-----------|----------|----------|----------|----------|----------|--|
|less|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|less|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|
*/};

const struct Column::flags_column COLUMN_FLAGS_DEFAULT_DOUBLE = db_column::COLUMN_FLAGS_DEFAULT_FLOAT;

const struct Column::flags_column COLUMN_FLAGS_DEFAULT_STRING = {/*
|--------------------------------------------------------------------------|
|-set|------------------------------------------------------------------|--|
|-set|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-set|*/{false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-set|-----------|----------|----------|----------|----------|----------|--|
|-set|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-set|*/ false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-set|-----------|----------|----------|----------|----------|----------|--|
|-set|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-set|*/ true    ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-add|------------------------------------------------------------------|--|
|-add|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-add|*/{false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-add|-----------|----------|----------|----------|----------|----------|--|
|-add|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-add|*/ false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-add|-----------|----------|----------|----------|----------|----------|--|
|-add|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-add|*/ true    ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-sub|------------------------------------------------------------------|--|
|-sub|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-sub|*/{false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-sub|-----------|----------|----------|----------|----------|----------|--|
|-sub|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-sub|*/ false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-sub|-----------|----------|----------|----------|----------|----------|--|
|-sub|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-sub|*/ true    ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-mul|------------------------------------------------------------------|--|
|-mul|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-mul|*/{false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-mul|-----------|----------|----------|----------|----------|----------|--|
|-mul|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-mul|*/ false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-mul|-----------|----------|----------|----------|----------|----------|--|
|-mul|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-mul|*/ true    ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-div|------------------------------------------------------------------|--|
|-div|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-div|*/{false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-div|-----------|----------|----------|----------|----------|----------|--|
|-div|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-div|*/ false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-div|-----------|----------|----------|----------|----------|----------|--|
|-div|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-div|*/ true    ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-eq-|------------------------------------------------------------------|--|
|-eq-|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-eq-|*/{false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-eq-|-----------|----------|----------|----------|----------|----------|--|
|-eq-|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-eq-|*/ false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-eq-|-----------|----------|----------|----------|----------|----------|--|
|-eq-|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-eq-|*/ true    ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|less|------------------------------------------------------------------|--|
|less|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|less|*/{false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|less|-----------|----------|----------|----------|----------|----------|--|
|less|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|less|*/ false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|less|-----------|----------|----------|----------|----------|----------|--|
|less|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|less|*/ true    ,   false  ,   false  ,   false} /*         |          |  |
|--------------------------------------------------------------------------|
*/};

const struct Column::flags_column COLUMN_FLAGS_DEFAULT_TIME = {/*
|--------------------------------------------------------------------------|
|-set|------------------------------------------------------------------|--|
|-set|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-set|*/{false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-set|-----------|----------|----------|----------|----------|----------|--|
|-set|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-set|*/ false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-set|-----------|----------|----------|----------|----------|----------|--|
|-set|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-set|*/ false   ,   true   ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-add|------------------------------------------------------------------|--|
|-add|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-add|*/{false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-add|-----------|----------|----------|----------|----------|----------|--|
|-add|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-add|*/ false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-add|-----------|----------|----------|----------|----------|----------|--|
|-add|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-add|*/ false   ,   true   ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-sub|------------------------------------------------------------------|--|
|-sub|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-sub|*/{false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-sub|-----------|----------|----------|----------|----------|----------|--|
|-sub|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-sub|*/ false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-sub|-----------|----------|----------|----------|----------|----------|--|
|-sub|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-sub|*/ false   ,   true   ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-mul|------------------------------------------------------------------|--|
|-mul|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-mul|*/{false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-mul|-----------|----------|----------|----------|----------|----------|--|
|-mul|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-mul|*/ false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-mul|-----------|----------|----------|----------|----------|----------|--|
|-mul|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-mul|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-div|------------------------------------------------------------------|--|
|-div|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-div|*/{false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-div|-----------|----------|----------|----------|----------|----------|--|
|-div|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-div|*/ false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-div|-----------|----------|----------|----------|----------|----------|--|
|-div|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-div|*/ false   ,   false  ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|-eq-|------------------------------------------------------------------|--|
|-eq-|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|-eq-|*/{false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-eq-|-----------|----------|----------|----------|----------|----------|--|
|-eq-|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|-eq-|*/ false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|-eq-|-----------|----------|----------|----------|----------|----------|--|
|-eq-|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|-eq-|*/ false   ,   true   ,   false  ,   false},/*         |          |  |
|--------------------------------------------------------------------------|

|--------------------------------------------------------------------------|
|less|------------------------------------------------------------------|--|
|less|    NONE   |   BOOL   |    U8T   |    S8T   |   U16T   |   S16T   |  |
|less|*/{false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|less|-----------|----------|----------|----------|----------|----------|--|
|less|    U32T   |   S32T   |   U64T   |   S64T   |   FLOAT  |  DOUBLE  |  |
|less|*/ false   ,   false  ,   false  ,   false  ,   false  ,  false,  /* |
|less|-----------|----------|----------|----------|----------|----------|--|
|less|   STRING  |   TIME   |   RESRV  |   RESRV  |          |          |  |
|less|*/ false   ,   true   ,   false  ,   false} /*         |          |  |
|--------------------------------------------------------------------------|
*/};

const struct Column::flags_column COLUMN_FLAGS_DEFAULT[] = {
									 COLUMN_FLAGS_DEFAULT_NONE,
									 COLUMN_FLAGS_DEFAULT_BOOL,
									 COLUMN_FLAGS_DEFAULT_UINT8_T,
									 COLUMN_FLAGS_DEFAULT_INT8_T,
									 COLUMN_FLAGS_DEFAULT_UINT16_T,
									 COLUMN_FLAGS_DEFAULT_INT16_T,
									 COLUMN_FLAGS_DEFAULT_UINT32_T,
									 COLUMN_FLAGS_DEFAULT_INT32_T,
									 COLUMN_FLAGS_DEFAULT_UINT64_T,
									 COLUMN_FLAGS_DEFAULT_INT64_T,
									 COLUMN_FLAGS_DEFAULT_FLOAT,
									 COLUMN_FLAGS_DEFAULT_DOUBLE,
                                     COLUMN_FLAGS_DEFAULT_STRING,
                                     COLUMN_FLAGS_DEFAULT_TIME
};
}/*end of namespace*/
