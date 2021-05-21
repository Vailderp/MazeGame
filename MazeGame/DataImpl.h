#pragma once
#include <vector>


/**
 * \brief TemplateUtils
 */
namespace tu
{
	/**
	 * \brief даёт сумму вариативных аргументов
	 * \param args вариативные аргументы
	 * \return сумма вариативных аргументов
	 */
	template<typename... Args>
	constexpr int argsum(Args... args)
	{
		return sizeof...(args);
	}

	/**
	 * \brief Оболочка для векторов одного типа, для их объединения
	 * \tparam _Value_type тип объекта в векторе
	 */
	template <typename _Value_type>
	class VecOpImpl
	{
	public:
		std::vector<_Value_type>& vector;

		VecOpImpl(std::vector<_Value_type>& vector) :
			vector(vector)
		{


		}
		void operator << (VecOpImpl& constructor)
		{
			for (_Value_type it : constructor.vector)
			{
				this->vector.push_back(it);
			}
		}
	};

	typedef int rank_t;

	/**
	* \brief создаёт многомерный массив
	* \tparam _Value_type тип объекта
	* \tparam _Dimension_rank число измерений массива
	*/
	template <typename _Value_type, rank_t _Dimension_rank>
	struct multi_dimension_vector_
	{
		typedef std::vector<typename multi_dimension_vector_
			<_Value_type, _Dimension_rank - 1>::type> type;
	};

	/**
	 * \brief создаёт многомерный массив
	 * \tparam _Value_type тип объекта
	 */
	template<typename _Value_type>
	struct multi_dimension_vector_<_Value_type, 0>
	{
		typedef _Value_type type;
	};

	/**
	* \brief Превращает вариативные аргументы в массив
	*/
	template<typename _Value_type, _Value_type... _Args>
	struct variadic_args_array
	{
		static _Value_type data[argsum(_Args...)];
	};

	/**
	 * \brief Превращает вариативные аргументы в массив
	 */
	template<typename _Value_type, _Value_type... _Args>
	_Value_type variadic_args_array<_Value_type, _Args...>
	::data[argsum(_Args...)] =
	{
		_Args...
	};

	template <typename _Value_type>
	struct TemplArgs
	{
		template <_Value_type...>
		struct Shove
		{
			enum
			{
				value = _Value_type()
			};
		};

		template <>
		struct Shove<>
		{
			enum
			{
				value = _Value_type()
			};
		};

		template <
			_Value_type _First,
		_Value_type... _Other>
		struct Shove<_First, _Other...>
		{
			enum
			{
				value = Shove<_Other...>::value << 1
			};
		};
	};
	
	///////////////////////////////////////////////////////////////////////////////

	/**
	 * \brief
	 * \tparam _Value_type тип элементов
	 * \tparam _Rank вариативный аргумент,
	 * указывающий количество указателей
	 * на элементы массива в каждом измерении
	 */
	 /*
	template <typename _Value_type, rank_t... _Rank>
	class DataImpl
	{
	public:
		static const rank_t dimension = argsum(_Rank...);

	private:
		const rank_t* dimensions_ =
			variadic_args_array<rank_t, _Rank...>::data;

		_Value_type** data_ptr_;

		multi_dimension_vector<_Value_type, dimension> vec_;

	public:
		explicit DataImpl(_Value_type** data_ptr) :
			data_ptr_(data_ptr)
		{
			rank_t* sizes = new rank_t[dimension];
			for (int i = 0; i < dimension; i++)
			{
				//std::cout << "i: " << i << std::endl;
				const int size = dimension - i;
				rank_t* max_size_arr = new rank_t[size];
				for (int l = dimension - i - 1; l > -1; --l)
				{
					//std::cout << "l: " << l << std::endl;
					max_size_arr[l] = dimensions_[l];
				}

				rank_t max_size = 1;

				for (int l = 0; l < size; l ++)
				{
					std::cout << "max: " << max_size_arr[l] << std::endl;
					max_size *= max_size_arr[l];
				}
				std::cout << "size_" << i << ": " << max_size / 10 << std::endl;
				delete[] max_size_arr;
			}
			delete[] sizes;
		}

		template<rank_t... _Cell_rank>
		_Value_type cell(rank_t... _Cell_rank)
		{
			rank_t* rank = array_<rank_t, _Cell_rank...>::value;
			return (*data_ptr_)[n1];
		}

	};
	*/
/**
 * \brief Оболочка (обёртка) над обычным одномерным массивом
 * \tparam _Value_type тип элементов
 */
	template<typename _Value_type>
	class DataImpl2 :
	public multi_dimension_vector_<_Value_type*, 2>::type
	{
	private:
		_Value_type** data_ptr_;

	public:
		/**
		 * \brief создаёт оболочку
		 * \param data_ptr указатель на массив
		 * \param _Size_1
		 * \param _Size_2
		 */
		explicit DataImpl2(_Value_type** data_ptr, 
			const rank_t _Size_1, const rank_t _Size_2) :
			data_ptr_(data_ptr)
		{
			for (rank_t i = 0; i < _Size_1; i++)
			{
				this->push_back(multi_dimension_vector_<_Value_type*, 1>
					::type(_Size_2));
				for (rank_t l = 0; l < _Size_2; l++)
				{
					this->at(i).at(l) = &((*data_ptr)[i * _Size_2 + l]);
				}
			}
		}
		
		/**
		 * \brief получить элемени по данным индексам
		 * \param idx1 индекс первого измерения
		 * \param idx2 индекс второго измерения
		 * \return возвращает значение по данным индексам
		 */
		inline constexpr _Value_type get(const rank_t idx1, const rank_t idx2)
		{
			return *(this->at(idx1).at(idx2));
		}

		
		
	};
}