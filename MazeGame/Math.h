#pragma once
#include <iostream>

//Trigonometric math

namespace math
{
	const float PI = acosf(-1.f);
	const float PI2 = PI / 2.f;
	const float PI180 = PI / 180.f;
	const float PI_180 = 180.f / PI;

	inline float toDeg(const float rad)
	{
		return rad * PI_180;
	}

	inline float toRad(const float deg)
	{
		return deg * PI180;
	}

	//Разница между двамя углами
	inline float getSignedAngle(const float Angle1, const float Angle2)
	{
		const float sa = std::fmodf((Angle1 - Angle2) + 180, 360.f) - 180;
		if (sa < -180)
		{
			return 360 + sa;
		}
		return sa;
	}

	//Котангенс float
	inline float ctg(const float angle)
	{
		return std::tanf(PI2 - angle);
	}

	//ГСПЧ алгоритм рандома по сиду
	inline int rand(const int min, const int max, unsigned int seed)
	{
		seed = 8253729 * seed + 2396403;
		int rand = (8253729 * seed + 2396403) % (max + min + 1) - min;
		int i = 2;
		//Редко, но бывает, что не генерируется нужное число, поэтому:
		while (rand < min || rand > max)
		{
			rand = (8253729 * seed * i + 2396403) % (max + min) - min;
			i++;
		}
		return rand;
	}

	//ГСПЧ алгоритм рандома без сида
	inline int rand(const int min, const int max)
	{
		return rand(min, max, static_cast<int>(8253729 * (std::clock() / static_cast<float>(CLOCKS_PER_SEC))));
	}

	inline float cos180(const float deg)
	{
		return std::cosf(math::toRad(deg));
	}

	inline float sin180(const float deg)
	{
		return std::sinf(math::toRad(deg));
	}

	inline float tan180(const float deg)
	{
		return std::tanf(math::toRad(deg));
	}

	inline float ctg180(const float deg)
	{
		return math::ctg(math::toRad(deg));
	}

	inline float delimit_angle(float& angle)
	{
		if (angle > 180)
		{
			angle = -179.9f;
		}
		else if (angle < -180)
		{
			angle = 179.9f;
		}
		return angle;
	}

	inline int getMatrixPos(const float pos, const float m_matrix_width)
	{
		return static_cast<int>(pos / m_matrix_width);
	}

	inline float fast_hypot(const float a, const float b)
	{
		return sqrtf(a * a + b * b);
	}
};
