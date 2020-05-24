#pragma once
#include <vector>
#include <cstdint>

namespace db_col
{
enum class TYPE
{
	NONE,
	BOOL,
	UINT8_T,
	INT8_T,
	UINT16_T,
	INT16_T,
	UINT32_T,
	INT32_T,
	UINT64_T,
	INT64_T,
	FLOAT,
	DOUBLE,
	STRING
};

/*snatch from https://stackoverflow.com/questions/17032310/how-to-make-a-variadic-is-same
  compare all variadic types with T - if no match then return false*/
template <class T, class... Ts>
struct type_check : std::disjunction<std::is_same<T, Ts>...> {};

/*class Column store vector<uint64_t> and must be
  store data of any type, for which declare
  interaction with another types. All types in this
  vector are same, so it Column in table, within 1
  col type only 1*/
class Column
{
	public:
		Column();
		Column(const TYPE type);
		Column(const Column& col);
		Column& operator=(const Column& col);
		~Column();
		TYPE type() const;
		void reserve(const size_t size);
		void push_back(const uint64_t data);

		/*set type
		  all data in Column will be transfer to needed type.
		  Interaction with types:
		  	None:
				all types -> nothing do
---------------------------------------------------------------
			Bool:	
				Uint8_t, Int8_t, Uint16_t, Int16_t, Uint32_t,
				Int32_t, Uint64_t, Int64_t, Float, Double -> nothing do

				String -> if true then "true", same for false
---------------------------------------------------------------
			Uint8_t, Int8_t, Uint16_t, Int16_t, Uint32_t,
			Int32_t, Uint64_t, Int64_t, Float, Double:
				None, Bool -> nothing do

				Uint8_t, int8_t, uint16_t, int16_t,
				uint32_t, int32_t, uint64_t, int64_t,
				float, double -> cast, for (signed -> unsigned) lost sign
				
				String -> transfer, allocate str and save
---------------------------------------------------------------
			String:
				None -> nothing do
				
				Bool -> if "true" then true, same for "false"
				
				Uint8_t, int8_t, uint16_t, int16_t,
				uint32_t, int32_t, uint64_t, int64_t,
				float, double -> transfer
		*/
		void set_type(const TYPE);
		void set_data();
		const std::vector<uint64_t>& all_data() const;
		uint64_t data(const size_t pos) const;
		template <typename T>
		T data(const size_t pos) const
		{
			return *(T*)(&_data[pos]);
		};

		/*add ( += )
		  Interaction with types:
		 	None +=:
				all types -> nothing to do
---------------------------------------------------------------
			Bool +=:
				Uint8_t, int8_t, uint16_t, int16_t, uint32_t,
				int32_t, uint64_t, int64_t -> cast to bool and add
				
				None, Float, Double, String -> nothing do
---------------------------------------------------------------
			Uint8_t, Int8_t, Uint16_t, Int16_t, Uint32_t,
			Int32_t, Uint64_t, Int64_t +=:
				None -> nothing do

				Uint8_t, int8_t, uint16_t, int16_t,
				uint32_t, int32_t, uint64_t, int64_t,
				float, double -> cast to T and add
				
				String -> transfer string to needed type and add
---------------------------------------------------------------
			String +=:
				None     -> nothing do

				String   -> concate strings
		*/
		void add(const size_t pos, const uint64_t val, const TYPE type);

		/*set ( = )
		  Interaction with types:
		 	None =:
				all types -> nothing to do
---------------------------------------------------------------
			Bool =:
				Uint8_t, int8_t, uint16_t, int16_t, uint32_t,
				int32_t, uint64_t, int64_t -> cast to bool and add

				String -> if "false" then bool = false, same for "true"

				None, float, double -> nothing to do
---------------------------------------------------------------
			Uint8_t, Int8_t, Uint16_t, Int16_t, Uint32_t,
			Int32_t, Uint64_t, Int64_t, float, double =:
				Bool, Uint8_t, int8_t, uint16_t, int16_t, uint32_t,
				int32_t, uint64_t, int64_t, float, double -> cast and set
				
				String -> convert string to num and set
---------------------------------------------------------------
			String =:
				Bool -> bool == true then str = "true", same for false
				
				Uint8_t, int8_t, uint16_t, int16_t, uint32_t,
				int32_t, uint64_t, int64_t, float, double -> convert and set
				
				String -> set
		*/
		void set(const size_t pos, const uint64_t val, const TYPE type);

		/*sub ( -= )
		  Interaction with types:
		 	None -=:
				all types -> nothing to do
---------------------------------------------------------------
			Bool -=:
				Uint8_t, int8_t, uint16_t, int16_t, uint32_t,
				int32_t, uint64_t, int64_t -> cast to bool and sub
				
				None, Float, Double, String -> nothing do
---------------------------------------------------------------
			Uint8_t, Int8_t, Uint16_t, Int16_t, Uint32_t,
			Int32_t, Uint64_t, Int64_t, Float, Double -=:
				None -> nothing do

				Uint8_t, int8_t, uint16_t, int16_t,
				uint32_t, int32_t, uint64_t, int64_t,
				float, double -> cast to T and sub
				
				String -> transfer string to needed type and sub
---------------------------------------------------------------
			String -=:
				all types -> nothing do
		*/
		void sub(const size_t pos, const uint64_t val, const TYPE type);

		/*mul ( *= )
		  Interaction with types:
		 	None, Bool *=:
				all types -> nothing to do
---------------------------------------------------------------
			Uint8_t, Int8_t, Uint16_t, Int16_t, Uint32_t,
			Int32_t, Uint64_t, Int64_t, float, double *=:
				None -> nothing do

				Uint8_t, int8_t, uint16_t, int16_t,
				uint32_t, int32_t, uint64_t, int64_t,
				float, double -> cast to T and mul
				
				String -> transfer string to needed type and mul
		*/
		void mul(const size_t pos, const uint64_t val, const TYPE type);

		/*div ( /= )
		  Interaction with types:
		 	None, Bool, String /=:
				all types -> nothing to do
---------------------------------------------------------------
			Uint8_t, Int8_t, Uint16_t, Int16_t, Uint32_t,
			Int32_t, Uint64_t, Int64_t, Float, Double /=:
				None -> nothing do

				Uint8_t, int8_t, uint16_t, int16_t,
				uint32_t, int32_t, uint64_t, int64_t,
				float, double -> cast to T and div
				
				String -> transfer string to need type and div
		*/
		void div(const size_t pos, const uint64_t val, const TYPE type);
		
	private:
		TYPE _type;
		std::vector<uint64_t> _data;
};
}/*end of namespace*/
