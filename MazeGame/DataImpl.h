 #pragma once
#include <vector>
	
#define tp_tpr(x) x*

/**
 * \brief TemplateUtils - обёртки, шаблоны, полезнве интсрументы
 */
namespace tu
{
	///////////////////////////////////////////////////////////////////////////
	/**
	 * \brief даёт сумму вариативных аргументов
	 * \param args вариативные аргументы
	 * \return сумма вариативных аргументов
	 */
	template<typename... _Args>
	constexpr inline int argsum(_Args... args)
	{
		return sizeof...(args);
	}

	///////////////////////////////////////////////////////////////////////////

	template<typename _Value_ptr_type> using Ptr = _Value_ptr_type*;
	template<typename _Value_ptr_type> using Ref = _Value_ptr_type&;

	///////////////////////////////////////////////////////////////////////////
	
	/**
	 * \brief Оболочка для векторов одного типа, для их объединения
	 * \tparam _Value_type тип объекта в векторе
	 */
	template <typename _Value_type>
	class VecOpImpl
	{
	public:
		std::vector<_Value_type>& vector;

		VecOpImpl(Ref<std::vector<_Value_type>> vector) :
			vector(vector)
		{


		}
		void operator << (Ref<VecOpImpl> constructor)
		{
			for (_Value_type it : constructor.vector)
			{
				this->vector.push_back(it);
			}
		}
	};

	typedef int rank_t;

	///////////////////////////////////////////////////////////////////////////
	
	/**
	* \brief создаёт многомерный массив
	* \tparam _Value_type тип объекта
	* \tparam _Dimension_rank число измерений массива
	*/
	template <typename _Value_type, rank_t _Dimension_rank>
	struct multi_dimension_vector_str
	{
		typedef std::vector<typename multi_dimension_vector_str
			<_Value_type, _Dimension_rank - 1>::type> type;
	};

	/**
	 * \brief создаёт многомерный массив
	 * \tparam _Value_type тип объекта
	 */
	template<typename _Value_type>
	struct multi_dimension_vector_str<_Value_type, 0>
	{
		typedef _Value_type type;
	};

	/**
	 * \brief 
	 * \tparam _Value_type 
	 * \tparam _Dimension_rank 
	 */
	template <typename _Value_type, rank_t _Dimension_rank> 
	using multi_dimension_vector = \
	typename multi_dimension_vector_str<_Value_type, _Dimension_rank>::type;
	
	///////////////////////////////////////////////////////////////////////////
	
	/**
	* \brief Превращает вариативные аргументы в массив
	*/
	template<typename _Value_type, _Value_type... _Args>
	struct variadic_args_array_str
	{
		static _Value_type data[argsum(_Args...)];
	};

	/**
	 * \brief Превращает вариативные аргументы в массив
	 */
	template<typename _Value_type, _Value_type... _Args>
	_Value_type variadic_args_array_str<_Value_type, _Args...> \
	::data[argsum(_Args...)] =
	{
		_Args...
	};

	///////////////////////////////////////////////////////////////////////////
	
	template<typename _Value_type, _Value_type _Arg, _Value_type... _Args>
	struct variadic_arg_last
	{
		static const inline _Value_type value = \
			variadic_arg_last<_Value_type, _Args...>::value;
	};
	
	template<typename _Value_type, _Value_type _Arg>
	struct variadic_arg_last<_Value_type, _Arg>
	{
		static const inline _Value_type value = _Arg;
	};
	
	///////////////////////////////////////////////////////////////////////////
	///
	template<typename _Value_type, _Value_type _Arg, _Value_type... _Args>
	struct variadic_arg_first
	{
		static const inline _Value_type value = _Arg;
	};

	template<typename _Value_type, _Value_type _Arg>
	struct variadic_arg_first<_Value_type, _Arg>
	{
		static const inline _Value_type value = _Arg;
	};

	///////////////////////////////////////////////////////////////////////////

	template<typename _Arg>
	constexpr inline _Arg variadic_func_arg_last(_Arg arg)
	{
		return arg;
	};
	
	template<typename _Arg, typename... _Args>
	constexpr inline _Arg variadic_func_arg_last(_Arg arg, _Args... args)
	{
		return variadic_func_arg_last(args...);
	};

	///////////////////////////////////////////////////////////////////////////

	template<typename _Arg>
	constexpr inline _Arg variadic_func_arg_first(_Arg arg)
	{
		return arg;
	};

	template<typename _Arg, typename... _Args>
	constexpr inline _Arg variadic_func_arg_first(_Arg arg, _Args... args)
	{
		return arg;
	};

	///////////////////////////////////////////////////////////////////////////
	
	template<typename _Value_ptr_type, rank_t _Rank>
	struct PtrInfStr
	{
		typedef typename PtrInfStr<Ptr<_Value_ptr_type>, _Rank - 1>::value value;
	};

	template< typename _Value_ptr_type>
	struct PtrInfStr<_Value_ptr_type, NULL>
	{
		typedef _Value_ptr_type value;
	};

	template<typename _Value_ptr_type, rank_t _Rank>
	using PtrInf = typename PtrInfStr<_Value_ptr_type, _Rank>::value;

	template<typename _Value_ptr_type, rank_t _Rank, rank_t _Size>
	using PtrInfInit = typename PtrInfStr<_Value_ptr_type, _Rank - 1>::value[_Size];
	
	template<typename _Value_ptr_type>
	using Ptr1 = typename PtrInfStr<_Value_ptr_type, 1>::value;
	
	template<typename _Value_ptr_type>
	using Ptr2 = typename PtrInfStr<_Value_ptr_type, 2>::value;
	
	template<typename _Value_ptr_type>
	using Ptr3 = typename PtrInfStr<_Value_ptr_type, 3>::value;

	///////////////////////////////////////////////////////////////////////////
	//ArrayGetElement - сверхскоростной доступ к элементам массива 
	
	template<typename _Value_getting_type, rank_t... _Einf>
	struct ArrayGetElement
	{
		
	};

	template<typename _Value_getting_type, rank_t _E0, rank_t... _Einf>
	struct ArrayGetElement<_Value_getting_type, _E0, _Einf...>
	{
		static constexpr inline _Value_getting_type& (at) \
		(Ref<PtrInf<_Value_getting_type, argsum(_Einf...) + 1>> _Data)
		{
			return ArrayGetElement<_Value_getting_type, _Einf...>::at(_Data[_E0]);
		}
	};

	template<typename _Value_getting_type, rank_t _E0>
	struct ArrayGetElement<_Value_getting_type, _E0>
	{
		static constexpr inline _Value_getting_type& (at) \
		(Ref<PtrInf<_Value_getting_type, 1>> _Data)
		{
			return _Data[_E0];
		}
	};
	
	///////////////////////////////////////////////////////////////////////////

	template<typename _Value_getting_type, rank_t... _Einf>
	struct ArrayFill
	{

	};

	template<typename _Value_type, rank_t _Size_0, rank_t... _Size_inf>
	struct ArrayFill<_Value_type, _Size_0, _Size_inf...>
	{
		static constexpr inline void (fill) \
		(Ref<PtrInf<_Value_type, argsum(_Size_inf...) + 1>> _Data)
		{
			for (rank_t i = 0; i < _Size_0; i++)
			{
				_Data[i] = new PtrInf<_Value_type, argsum(_Size_inf...) - 1>[_Size_0];
				ArrayFill<_Value_type, _Size_inf...>::fill(_Data[i]);
			}
		}

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

		static constexpr inline void(fill) \
			(Ref<PtrInf<Ptr<_Value_type>, argsum(_Size_inf...) + 1>> _Data, \
				Ref<PtrInf<_Value_type, 1>> _Data_copy)
		{
			rank_t iter(0);
			fill(_Data, _Data_copy, iter);
		}

		rank_t rank = argsum(_Size_inf...);
		
		typedef typename PtrInf<_Value_type, argsum(_Size_inf...) + 1> type;
		typedef typename PtrInf< Ptr<_Value_type>, argsum(_Size_inf...) + 1> type_ptr;
		typedef typename PtrInf<_Value_type, argsum(_Size_inf...)> \
			type_init[variadic_arg_last<rank_t, _Size_inf...>::value];
		typedef typename PtrInf< Ptr<_Value_type>, argsum(_Size_inf...)> \
			type_init_ptr[variadic_arg_last<rank_t, _Size_inf...>::value];
		
	};

	template<typename _Value_type, rank_t _Size_0>
	struct ArrayFill<_Value_type, _Size_0>
	{
		static constexpr inline void (fill) \
		(Ref<PtrInf<_Value_type, 1>> _Data)
		{
			for (rank_t i = 0; i < _Size_0; i++)
			{
				_Data[i] = _Value_type{};
				
			}
		}

		static constexpr inline void(fill) \
			(Ref<PtrInf<Ptr<_Value_type>, 1>> _Data, \
				Ref<PtrInf<_Value_type, 1>> _Data_copy, Ref<rank_t> iter)
		{
			for (rank_t i = 0; i < _Size_0; i++)
			{
				_Data[i] = &_Data_copy[iter];
				iter++;

			}
		}

		static constexpr inline void(fill) \
			(Ref<PtrInf<Ptr<_Value_type>, 1>> _Data, Ref<PtrInf<_Value_type, 1>> _Data_copy)
		{
			rank_t iter(0);
			fill(_Data, _Data_copy, iter);
		}
		
		rank_t rank = 1;
		typedef typename PtrInf<_Value_type, 1> type;
		typedef typename PtrInf<Ptr<_Value_type>, 1> type_ptr;
		typedef typename PtrInf<_Value_type, 0> type_init[_Size_0];
		typedef typename PtrInf<Ptr<_Value_type>, 0> type_init_ptr[_Size_0];
	};
	
/**
 * \brief Оболочка (обёртка) над обычным одномерным массивом
 * \tparam _Value_type тип элементов
 */
	template<typename _Value_type>
	class DataImpl2 :
	public multi_dimension_vector<PtrInf<_Value_type, 1>, 2>
	{
	private:
		Ptr<PtrInf<_Value_type, 1>> data_ptr_;

	public:
		/**
		 * \brief создаёт оболочку
		 * \param data_ptr указатель на массив
		 * \param _Size_1  размер первого измерения массива
		 * \param _Size_2 размер второго измерения массива
		 */
		explicit DataImpl2(
			_Value_type** _Data_ptr,
			const rank_t _Size_1,
			const rank_t _Size_2) :
			data_ptr_(_Data_ptr),
			size_first(_Size_1),
			size_second(_Size_2)
		{
			for (rank_t i = 0; i < _Size_1; i++)
			{
				this->push_back(multi_dimension_vector<PtrInf<_Value_type, 1>, 1>(_Size_2));
				for (rank_t l = 0; l < _Size_2; l++)
				{
					(*this)[i][l] = &(*_Data_ptr)[i * _Size_2 + l];
				}
			}
		}

		DataImpl2() = default;
		
		/**
		 * \brief получить элемени по данным индексам
		 * \param idx1 индекс первого измерения
		 * \param idx2 индекс второго измерения
		 * \return возвращает значение по данным индексам
		 */
		
		[[nodiscard]] constexpr \
		_Value_type& (get) (const rank_t& idx1, const rank_t& idx2) const
		{
			return *(*this)[idx1][idx2];
		}

	
	public:
		const rank_t size_first;
		const rank_t size_second;
		
	};

	///////////////////////////////////////////////////////////////////////////
	
	template<typename _Value_type, rank_t _Size = 1>
	inline _Value_type fix_array[_Size] = {};
	
	///////////////////////////////////////////////////////////////////////////
	
	template<typename _Value_type, rank_t _Rank>
	class Data
	{
	public:
		template<rank_t... _Sizes>
		class DataImpl
		{
			PtrInf<_Value_type*, _Rank> data_ptr_;

		public:
			explicit DataImpl(Ref<PtrInf<_Value_type, 1>> _Data_ptr) :
				data_ptr_(new PtrInf<_Value_type, _Rank>[variadic_func_arg_last(_Sizes...)])
			{
				ArrayFill<_Value_type, _Sizes...>::fill(data_ptr_, _Data_ptr);
			}

			constexpr inline PtrInf<_Value_type, _Rank> operator [] (const rank_t& idx)
			{
				return data_ptr_[idx];
			}

			template<rank_t... _Idx>
			constexpr inline _Value_type& at()
			{
				return *(ArrayGetElement<_Value_type*, _Idx...>::at(data_ptr_));
			}
			
		};
	};

	///////////////////////////////////////////////////////////////////////////
	
}
