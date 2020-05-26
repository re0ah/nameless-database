#pragma once
#include <vector>
#include <cstdint>
#include "type.h"

namespace db_col
{
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
		Column(const Column&& col);
		~Column();
		Column& operator=(const Column& col);
		Column& operator=(const Column&& col);
		TYPE type() const;
		size_t size() const;
		void reserve(const size_t size);
		void push_back(const uint64_t data);
		void resize(const size_t size);
		void set_data(const size_t pos, const uint64_t data);
		void set_type_column(const TYPE);
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
				Bool -> set

				all other types -> nothing to do
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
		void set(const size_t pos, uint64_t val, const TYPE type);

		/*add ( += )
		  Interaction with types:
		 	None, Bool +=:
				all types -> nothing to do
---------------------------------------------------------------
			Uint8_t, Int8_t, Uint16_t, Int16_t, Uint32_t,
			Int32_t, Uint64_t, Int64_t +=:
				Uint8_t, int8_t, uint16_t, int16_t,
				uint32_t, int32_t, uint64_t, int64_t,
				float, double -> cast to T and add
---------------------------------------------------------------
			String +=:
				String   -> concate strings
		*/
		void add(const size_t pos, uint64_t val, const TYPE type);

		/*sub ( -= )
		  Interaction with types:
		 	None, String, Bool -=:
				all types -> nothing to do
---------------------------------------------------------------
			Uint8_t, Int8_t, Uint16_t, Int16_t, Uint32_t,
			Int32_t, Uint64_t, Int64_t, Float, Double -=:
				None -> nothing do

				Uint8_t, int8_t, uint16_t, int16_t,
				uint32_t, int32_t, uint64_t, int64_t,
				float, double -> cast to T and sub
		*/
		void sub(const size_t pos, uint64_t val, const TYPE type);

		/*mul ( *= )
		  Interaction with types:
		 	None, Bool, String *=:
				all types -> nothing to do
---------------------------------------------------------------
			Uint8_t, Int8_t, Uint16_t, Int16_t, Uint32_t,
			Int32_t, Uint64_t, Int64_t, float, double *=:
				None -> nothing do

				Uint8_t, int8_t, uint16_t, int16_t,
				uint32_t, int32_t, uint64_t, int64_t,
				float, double -> cast to T and mul
		*/
		void mul(const size_t pos, uint64_t val, const TYPE type);

		/*div ( /= )
		  Interaction with types:
		 	None, Bool, String /=:
				all types -> nothing to do
---------------------------------------------------------------
			Uint8_t, Int8_t, Uint16_t, Int16_t, Uint32_t,
			Int32_t, Uint64_t, Int64_t, float, double /=:
				Uint8_t, int8_t, uint16_t, int16_t,
				uint32_t, int32_t, uint64_t, int64_t,
				float, double-> cast to T and div
				
		*/
		void div(const size_t pos, uint64_t val, const TYPE type);
		
		/*equal ( == )
		  Interaction with types:
			None ==:
				None -> true
				all other types -> false
---------------------------------------------------------------
			Bool ==:
				Bool -> compare

				all other types -> false
---------------------------------------------------------------
			Uint8_t, Int8_t, Uint16_t, Int16_t, Uint32_t,
			Int32_t, Uint64_t, Int64_t, Float, Double ==:
				None, String, Bool, Float, Double -> false

				Uint8_t, Int8_t, Uint16_t, Int16_t,
				Uint32_t, Int32_t, Uint64_t, Int64_t -> compare
---------------------------------------------------------------
			String ==:
				None, Bool, Uint8_t, Int8_t, Uint16_t, Int16_t,
				Uint32_t, Int32_t, Uint64_t, Int64_t,
				Float, Double -> false

				String -> compare
		 */
		bool equal(const size_t pos, const uint64_t val, const TYPE type) const;
		bool not_equal(const size_t pos, const uint64_t val, const TYPE type) const;
	private:
		TYPE _type;
		std::vector<uint64_t> _data;
};
}/*end of namespace*/
