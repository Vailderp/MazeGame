#pragma once

#include <amp.h>
#include <amp_math.h>
#include "DataImpl.h"

typedef int rank_t;

//Вот над этим я работал час)))
template<typename _Value_type, rank_t... _Einf>
struct GPGPUHOST
{
	concurrency::array_view<_Value_type, sizeof...(_Einf)> host_array;
	GPGPUHOST() = default;
	GPGPUHOST(_Value_type*& _Src) :
		host_array(concurrency::array_view \
			<_Value_type, sizeof...(_Einf)>(_Einf..., _Src))
	{}
};

struct GPGPUPROC
{
	
};

