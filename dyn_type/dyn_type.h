#pragma once
#include <vector>
#include <cstdint>
#include <string>
#include <ostream>

namespace db_col
{
enum class TYPE
{
	NONE     = 0,
	BOOL     = 1,
	UINT8_T  = 2,
	INT8_T   = 3,
	UINT16_T = 4,
	INT16_T  = 5,
	UINT32_T = 6,
	INT32_T  = 7,
	UINT64_T = 8,
	INT64_T  = 9,
	FLOAT    = 10,
	DOUBLE   = 11,
	STRING   = 12
};

std::ostream& operator<< (std::ostream& os, const TYPE& type);

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
		~Column();
		Column& operator=(const Column& col);
		TYPE type() const;
		size_t size() const;
		void reserve(const size_t size);
		void push_back(const uint64_t data);
		void resize(const size_t size);
		void set_data(const size_t pos, const uint64_t data);

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
			if constexpr(type_check<T, std::string>::value)
			{
				return *((T*)_data[pos]);
			}
			else
			{
				return (T)_data[pos];
			}
		};

		void print_stdout(const size_t pos) const;
		void print_type_stdout() const;
		void print_all_stdout() const;
		void print_info_stdout() const;

		/*set ( = )
		  Interaction with types:
		 	None =:
				all types -> nothing to do
---------------------------------------------------------------
			Bool =:
				Uint8_t, int8_t, uint16_t, int16_t, uint32_t,
				int32_t, uint64_t, int64_t -> cast to bool and add

				String, None, float, double -> nothing to do
---------------------------------------------------------------
			Uint8_t, Int8_t, Uint16_t, Int16_t, Uint32_t,
			Int32_t, Uint64_t, Int64_t, float, double =:
				String -> nothing do

				Bool, Uint8_t, int8_t, uint16_t, int16_t, uint32_t,
				int32_t, uint64_t, int64_t, float, double -> cast and set
---------------------------------------------------------------
			String =:
				Bool, Uint8_t, int8_t, uint16_t, int16_t, uint32_t,
				int32_t, uint64_t, int64_t, float, double -> nothing do
				
				String -> set
		*/
		void set(const size_t pos, const uint64_t val, const TYPE type);

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

		/*sub ( -= )
		  Interaction with types:
		 	None, String -=:
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
		*/
		void div(const size_t pos, const uint64_t val, const TYPE type);
		
		/*equal ( == )
		  Interaction with types:
			None ==:
				None -> true
				all other types -> false
---------------------------------------------------------------
			Bool ==:
				None -> false
				all other types -> cast and compare
---------------------------------------------------------------
			Uint8_t, Int8_t, Uint16_t, Int16_t, Uint32_t,
			Int32_t, Uint64_t, Int64_t, Float, Double ==:
				None, String -> false
				
				Bool -> cast and compare

				Uint8_t, Int8_t, Uint16_t, Int16_t,
				Uint32_t, Int32_t, Uint64_t, Int64_t,
				Float, Double -> compare
---------------------------------------------------------------
			String ==:
				None, Uint8_t, Int8_t, Uint16_t, Int16_t,
				Uint32_t, Int32_t, Uint64_t, Int64_t,
				Float, Double -> false

				Bool -> compare, if "true" or "false

				String -> compare
		 */
		bool equal(const size_t pos, const uint64_t val, const TYPE type);
		bool not_equal(const size_t pos, const uint64_t val, const TYPE type);
	private:
		TYPE _type;
		std::vector<uint64_t> _data;
};
}/*end of namespace*/
