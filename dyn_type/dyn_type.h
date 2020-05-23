#pragma once
#include <string>
#include <vector>
#include <iostream>

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
		void set_type();
		void set_data();
		const std::vector<uint64_t>& all_data() const;
		uint64_t data(const size_t pos) const;

		/*unite 2 functions: data and data_cast, allow write shorter.
		  Example: need return and output string to ostream
			   
				if use basic data (which return just uint64_t) need will write:
					std::cout << *(data_cast<std::string*>(class_name.data(n));
				with this template can write:
					std::cout << *(cl.data<std::string*>(n);
				
				what, on my opinion, better
		 */
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
				
				String -> transfer string to needed type 
						  (signed or unsigned) and add
---------------------------------------------------------------
			Float, Double +=:
				None -> nothing do

				Uint8_t, int8_t, uint16_t, int16_t, uint32_t,
				int32_t, uint64_t, int64_t -> cast to T and add

				Float  -> transfer with data_cast to float and add

				Double -> transfer with data_cast to double and add

				String -> transfer string to float or double and add
---------------------------------------------------------------
			String +=:
				None     -> nothing do

				String   -> concate strings

				Depending on what how can interpret string there 2 option:
					1. If string must be interpret how number 
					   (float or int, not matter) then string
					   will be transfer to number and to him
					   will be add argument who was pass. After
					   transfer this number back to string.

					2. If string must be interpret how string,
					not number, then argument will be transfer
					to string and concate.
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
	private:
		TYPE _type;
		std::vector<uint64_t> _data;
};
}/*end of namespace*/
