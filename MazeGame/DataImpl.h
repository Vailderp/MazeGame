 #pragma once
#include <vector>
//#define get_array_element_def(X)
namespace tu
{

	//////////////////////////////////////////////////////////////////////////////

	/**
	 * \brief value type of rank
	 */
	typedef int rank_t;

	//////////////////////////////////////////////////////////////////////////////

	/**
	 * \brief typename -> typename*
	 * \tparam _Value_ptr_type value type / тип величины
	 */
	template<typename _Value_ptr_type> using Ptr = _Value_ptr_type*;

	/**
	 * \brief typename -> typename&
	 * \tparam _Value_ptr_type value type / тип величины
	 */
	template<typename _Value_ptr_type> using Ref = _Value_ptr_type&;

	
	//////////////////////////////////////////////////////////////////////////////

	/**
	* \brief get the sum of arguments / получить количество аргументов
	* \param _Args args / аргументы
	* \return the sum of arguments / сумма аргкментов
	*/
	template<typename... _Args>
	constexpr inline rank_t argsum(_Args... args)
	{
		return sizeof...(args);
	}
	
	//////////////////////////////////////////////////////////////////////////////
	
	/**
	 * \brief multi-dimension vector / многомерный вектор
	 * \tparam _Value_type value type
	 * \tparam _Dimension_rank rank
	 */
	template <typename _Value_type, rank_t _Dimension_rank>
	struct multi_dimension_vector_str
	{
		/**
		 * \brief multi-dimension vector / многомерный вектор
		 */
		typedef std::vector<typename multi_dimension_vector_str
			<_Value_type, _Dimension_rank - 1>::type> type;
	};

	/**
	 * \brief multi-dimension vector / многомерный вектор
	 * \tparam _Value_type value type
	 * \tparam _Dimension_rank rank
	 */
	template<typename _Value_type>
	struct multi_dimension_vector_str<_Value_type, 0>
	{
		/**
		 * \brief multi-dimension vector / многомерный вектор
		 */
		typedef _Value_type type;
	};

	
	/**
	 * \brief multi-dimension vector / многомерный вектор
	 * \tparam _Value_type value type
	 * \tparam _Dimension_rank rank
	 */
	template <typename _Value_type, rank_t _Dimension_rank> 
	using multi_dimension_vector = \
	typename multi_dimension_vector_str<_Value_type, _Dimension_rank>::type;

	/**
	* \brief variadic args to array / массив вариативных аргументов
	*/
	template<typename _Value_type, _Value_type... _Args>
	struct variadic_args_array_str
	{
		/**
		* \brief array of variadic args / массив вариативных аргументов
		*/
		static _Value_type data[argsum(_Args...)];
	};
	
	/**
	 * \brief array of variadic args / массив вариативных аргументов
	 */
	template<typename _Value_type, _Value_type... _Args>
	_Value_type variadic_args_array_str<_Value_type, _Args...> \
	::data[argsum(_Args...)] =
	{
		_Args...
	};

	//////////////////////////////////////////////////////////////////////////////

	/**
	 * \brief return last arg
	 * \param arg first arg
	 * \param args other args
	 */
	template<typename _Value_type, _Value_type _Arg, _Value_type... _Args>
	struct variadic_arg_last
	{
		/**
		 * \brief last arg
		 */
		static const inline _Value_type value = \
			variadic_arg_last<_Value_type, _Args...>::value;
	};

	/**
	 * \brief return last arg
	 * \param arg first arg
	 * \param args other args
	 */
	template<typename _Value_type, _Value_type _Arg>
	struct variadic_arg_last<_Value_type, _Arg>
	{
		/**
		 * \brief last arg
		 */
		static const inline _Value_type value = _Arg;
	};

	/**
	 * \brief return first arg
	 * \param arg first arg
	 * \param args other args
	 */
	template<typename _Value_type, _Value_type _Arg, _Value_type... _Args>
	struct variadic_arg_first
	{
		/**
		 * \brief first arg
		 */
		static const inline _Value_type value = _Arg;
	};
	
	/**
	 * \brief return first arg
	 * \param arg first arg
	 * \param args other args
	 */
	template<typename _Value_type, _Value_type _Arg>
	struct variadic_arg_first<_Value_type, _Arg>
	{
		/**
		 * \brief first arg
		 */
		static const inline _Value_type value = _Arg;
	};

	//////////////////////////////////////////////////////////////////////////////
	
	/**
	 * \brief return last arg
	 * \param arg first arg
	 * \param args other args
	 * \return last arg
	 */
	template<typename _Arg>
	constexpr inline _Arg variadic_func_arg_last(_Arg arg)
	{
		return arg;
	};

	/**
	 * \brief return last arg
	 * \param arg first arg
	 * \param args other args
	 * \return last arg
	 */
	template<typename _Arg, typename... _Args>
	constexpr inline _Arg variadic_func_arg_last(_Arg arg, _Args... args)
	{
		return variadic_func_arg_last(args...);
	};

	/**
	 * \brief return first arg
	 * \param arg first arg
	 * \param args other args
	 * \return first arg
	 */
	template<typename _Arg>
	constexpr inline _Arg variadic_func_arg_first(_Arg arg)
	{
		return arg;
	};
	
	/**
	 * \brief return first arg
	 * \param arg first arg
	 * \param args other args
	 * \return first arg
	 */
	template<typename _Arg, typename... _Args>
	constexpr inline _Arg variadic_func_arg_first(_Arg arg, _Args... args)
	{
		return arg;
	};

	//////////////////////////////////////////////////////////////////////////////

	/**
	 * \brief returns a given number of pointers to pointers of type
	 * \brief возвращает заданное количество указателей на указатели типа
	 * \tparam _Value_ptr_type value type
	 * \tparam _Rank rank
	 */
	template<typename _Value_ptr_type, rank_t _Rank>
	struct PtrInfStr
	{
		typedef typename PtrInfStr<Ptr<_Value_ptr_type>, _Rank - 1>::value value;
	};

	
	/**
	 * \brief value = value type
	 * \tparam _Value_ptr_type value type
	 */
	template< typename _Value_ptr_type>
	struct PtrInfStr<_Value_ptr_type, NULL>
	{
		typedef _Value_ptr_type value;
	};
	
	/**
	 * \brief returns a given number of pointers to pointers of type
	 * \brief возвращает заданное количество указателей на указатели типа
	 * \tparam _Value_ptr_type value type
	 * \tparam _Rank rank
	 */
	template<typename _Value_ptr_type, rank_t _Rank>
	using PtrInf = typename PtrInfStr<_Value_ptr_type, _Rank>::value;


	/**
	 * \brief init pointer-type
	 * \tparam _Value_ptr_type value type
	 * \tparam _Rank rank
	 * \tparam _Size size
	 */
	template<typename _Value_ptr_type, rank_t _Rank, rank_t _Size>
	using PtrInfInit = typename PtrInfStr<_Value_ptr_type, _Rank - 1>::value[_Size];

	/**
	 * \brief return _Value_ptr_type*
	 * \tparam _Value_ptr_type value type
	 */
	template<typename _Value_ptr_type>
	using Ptr1 = typename PtrInfStr<_Value_ptr_type, 1>::value;

	/**
	 * \brief return _Value_ptr_type**
	 * \tparam _Value_ptr_type value type
	 */
	template<typename _Value_ptr_type>
	using Ptr2 = typename PtrInfStr<_Value_ptr_type, 2>::value;
	
	/**
	 * \brief return _Value_ptr_type***
	 * \tparam _Value_ptr_type value type
	 */
	template<typename _Value_ptr_type>
	using Ptr3 = typename PtrInfStr<_Value_ptr_type, 3>::value;

	//////////////////////////////////////////////////////////////////////////////
	
	///---
	template<typename _Value_getting_type, rank_t... _Einf>
	struct ArrayGetElement
	{};

	/**
	 * \brief Fast getting element from array / быстрое получение элемента из массива
	 * \tparam _Value_getting_type type of element / тип элемента
	 * \tparam _E0 first array coordinate / первая координата массива
	 * \tparam _Einf array coordinates / остальные координаты массива
	 */
	template<typename _Value_getting_type, rank_t _E0, rank_t... _Einf>
	struct ArrayGetElement<_Value_getting_type, _E0, _Einf...>
	{
		/**
		 * \brief get an element in your array / получить элемент в вашем массиве
		 * \param _Data your array / ваш массив
		 * \return element of array/ элемент массива
		 */
		static constexpr inline _Value_getting_type& (at) \
			(Ref<PtrInf<_Value_getting_type, argsum(_Einf...)+ 1>> _Data)
		{
			return ArrayGetElement<_Value_getting_type, _Einf...>::at(_Data[_E0]);
		}
	};
	
	/**
	 * \brief Fast getting element from array / быстрое получение элемента из массива
	 * \tparam _Value_getting_type type of element / тип элемента
	 * \tparam _E0 first array coordinate / первая координата массива
	 */
	template<typename _Value_getting_type, rank_t _E0>
	struct ArrayGetElement<_Value_getting_type, _E0>
	{
		/**
		 * \brief get an element in your array / получить элемент в вашем массиве
		 * \param _Data your array / ваш массив
		 * \return element of array/ элемент массива
		 */
		static constexpr inline _Value_getting_type& (at) \
		(Ref<PtrInf<_Value_getting_type, 1>> _Data)
		{
			return _Data[_E0];
		}
	};


	
	//////////////////////////////////////////////////////////////////////////////
	
	///---
	template<typename _Value_getting_type, rank_t... _Einf>
	struct ArrayFill {};
	
	/**
	 * \brief filling your array | coping your array in your pointer-array
	 * \brief заполнение вашего массива | копирование вашего массива в ваш массив указателей
	 * \tparam _Value_type value type of array / тип значений массива
	 * \tparam _Size_0 first size of array / размер первого ранга(измерения)
	 * \tparam _Size_inf sizes of array / размеры рангов(измерений)
	 */
	template<typename _Value_type, rank_t _Size_0, rank_t... _Size_inf>
	struct ArrayFill<_Value_type, _Size_0, _Size_inf...>
	{
		/**
		 * \brief filling your array
		 * \brief заполнение вашего массива
		 * \param _Data your array / ваш массив
		 */
		static constexpr inline void (fill) \
		(Ref<PtrInf<_Value_type, argsum(_Size_inf...) + 1>> _Data)
		{
			for (rank_t i = 0; i < _Size_0; i++)
			{
				_Data[i] = new PtrInf<_Value_type, argsum(_Size_inf...) - 1>[_Size_0];
				ArrayFill<_Value_type, _Size_inf...>::fill(_Data[i]); 
			}
		}

		/**
		 * \brief coping your array in your pointer-array;
		 * utility function
		 * \brief копирование вашего массива в ваш массив указателей;
		 * служебная функция
		 * \param _Data your array / ваш массив
		 * \param _Data_copy your pointer-array / ваш массив указателей
		 * \param iter итератор
		 */
		static constexpr inline void(fill) \
			(Ref<PtrInf<Ptr<_Value_type>, argsum(_Size_inf...) + 1>> _Data, \
				Ref<PtrInf<_Value_type, 1>> _Data_copy, Ref<rank_t> iter)
		{
			for (rank_t i = 0; i < _Size_0; i++)
			{
				_Data[i] = new PtrInf<_Value_type, argsum(_Size_inf...)>[_Size_0];
				ArrayFill<_Value_type, _Size_inf...>::fill(_Data[i], _Data_copy, iter);
			}
		}

		/**
		 * \brief coping your array in your pointer-array
		 * \brief копирование вашего массива в ваш массив указателей
		 * \param _Data your array / ваш массив
		 * \param _Data_copy your pointer-array / ваш массив указателей
		 */
		static constexpr inline void(fill) \
			(Ref<PtrInf<Ptr<_Value_type>, argsum(_Size_inf...) + 1>> _Data, \
				Ref<PtrInf<_Value_type, 1>> _Data_copy)
		{
			rank_t iter(0);
			fill(_Data, _Data_copy, iter);
		}

		/**
		 * \brief rank of array
		 * \brief ранг или же количество измерений массива
		 */
		rank_t rank = argsum(_Size_inf...);

		/**
		 * \brief this type
		 * \brief данный тип
		 */
		typedef typename PtrInf<_Value_type, argsum(_Size_inf...) + 1> type;

		/**
		 * \brief this type-pointer
		 * \brief данный тип-указатель
		 */
		typedef typename PtrInf< Ptr<_Value_type>, argsum(_Size_inf...) + 1> type_ptr;

		/**
		 * \brie this type
		 * \brie данный тип для инициализации
		 */
		typedef typename PtrInf<_Value_type, argsum(_Size_inf...)> \
		type_init[variadic_arg_last<rank_t, _Size_inf...>::value];

		/**
		 * \brief this pointer-type
		 * \briefданный тип-указатель для инициализации
		 */
		typedef typename PtrInf<Ptr<_Value_type>, argsum(_Size_inf...)> \
		type_init_ptr[variadic_arg_last<rank_t, _Size_inf...>::value];
	};
	
	/**
	 * \brief filling your array | coping your array in your pointer-array
	 * \brief заполнение вашего массива | копирование вашего массива в ваш массив указателей
	 * \tparam _Value_type value type of array / тип значений массива
	 * \tparam _Size_0 first size of array / размер первого ранга(измерения)
	 */
	template<typename _Value_type, rank_t _Size_0>
	struct ArrayFill<_Value_type, _Size_0>
	{
		/**
		 * \brief filling your array
		 * \brief заполнение вашего массива
		 * \param _Data your array / ваш массив
		 */
		static constexpr inline void (fill) \
		(Ref<PtrInf<_Value_type, 1>> _Data)
		{
			for (rank_t i = 0; i < _Size_0; i++)
			{
				_Data[i] = _Value_type{};
				
			}
		}

		/**
		 * \brief coping your array in your pointer-array;
		 * utility function
		 * \brief копирование вашего массива в ваш массив указателей;
		 * служебная функция
		 * \param _Data your array / ваш массив
		 * \param _Data_copy your pointer-array / ваш массив указателей
		 * \param iter итератор
		 */
		static constexpr inline void (fill) \
			(Ref<PtrInf<Ptr<_Value_type>, 1>> _Data, \
				Ref<PtrInf<_Value_type, 1>> _Data_copy, Ref<rank_t> iter)
		{
			for (rank_t i = 0; i < _Size_0; i++)
			{
				_Data[i] = &_Data_copy[iter];
				iter++;

			}
		}
		
		/**
		 * \brief coping your array in your pointer-array
		 * \brief копирование вашего массива в ваш массив указателей
		 * \param _Data your array / ваш массив
		 * \param _Data_copy your pointer-array / ваш массив указателей
		 */
		static constexpr inline void (fill) \
			(Ref<PtrInf<Ptr<_Value_type>, 1>> _Data, Ref<PtrInf<_Value_type, 1>> _Data_copy)
		{
			rank_t iter(0);
			fill(_Data, _Data_copy, iter);
		}
		
		/**
		 * \brief rank of array
		 * \brief ранг или же количество измерений массива
		 */
		rank_t rank = 1;

		/**
		 * \brief this type
		 * \brief данный тип
		 */
		typedef typename PtrInf<_Value_type, 1> type;
		
		/**
		 * \brief this type-pointer
		 * \brief данный тип-указатель
		 */
		typedef typename PtrInf<Ptr<_Value_type>, 1> type_ptr;
		
		/**
		 * \brie this type
		 * \brie данный тип для инициализации
		 */
		typedef typename PtrInf<_Value_type, 0> type_init[_Size_0];
		
		/**
		 * \brief this pointer-type
		 * \briefданный тип-указатель для инициализации
		 */
		typedef typename PtrInf<Ptr<_Value_type>, 0> type_init_ptr[_Size_0];
	};

	//////////////////////////////////////////////////////////////////////////////
	
	template<typename _Value_type, rank_t _Size = 1>
	inline _Value_type fix_array[_Size] = {};
	
	//////////////////////////////////////////////////////////////////////////////
	
	template<typename _Value_type, rank_t _Rank>
	class Data
	{
	public:
		/**
		 * \brief Implementing a multidimensional array over a one-dimensional array
		 * \brief Осуществление многомерного массива над одномерным массивом
		 * \tparam _Sizes Each rank size
		 * / Размеры каждого ранга (измерения)
		 */
		template<rank_t... _Sizes>
		class DataImpl
		{
		public:
			
			/**
			 * \brief Multidimensional array with pointers to one-dimensional array
			 * \brief Многомерный массив с указателями на одномерный массив
			 */
			PtrInf<Ptr<_Value_type>, _Rank> data_ptr;
			
			/**
			 * \brief 
			 * \param Data_src The one-dimensional array on which the implementation is taking place
			 * / Одномерный массив, над которым произойдёт осуществление
			 */
			explicit DataImpl(PtrInf<_Value_type, 1> Data_src) :
				data_ptr(new PtrInf<_Value_type, _Rank>[variadic_func_arg_last(_Sizes...)])
			{
				ArrayFill<_Value_type, _Sizes...>::fill(data_ptr, Data_src);
			}
		};

	//////////////////////////////////////////////////////////////////////////////
		
		static inline rank_t dimension = _Rank;
		static inline rank_t rank = _Rank;

	//////////////////////////////////////////////////////////////////////////////

		template<rank_t _Size0, rank_t... _Sizes>
		class DataImpl2
		{
		public:
			DataImpl2<_Sizes...> tuple;
		};
		
		template<rank_t _Size0>
		class DataImpl2<_Size0>
		{
		public:
			PtrInf<Ptr<_Value_type>, _Rank> data_ptr;
			constexpr inline  _Value_type& (at) (rank_t idx)
			{
				return *data_ptr[idx];
			}
		};		
	};

	//////////////////////////////////////////////////////////////////////////////
};
