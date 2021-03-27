#pragma once
#include <SFML/Graphics.hpp>
class ray_casting_api
{
	explicit ray_casting_api(const int render_distance) :
	render_distance(render_distance)
	{
		
	}
	
	class RAYRET
	{
	public:
		sf::Vector2<int> matrixpos;
		sf::Vector2<float> pos;
		float hypot;
		unsigned __int8 walldir;
	};

	int render_distance;
	float vA = 0;
	float vB = 0;
	float vcx = 0;
	float vcy = 0;
	float vcxm = 0;
	float vAA = 0;
	float vBB = 0;
	float hA = 0;
	float hB = 0;
	float hcx = 0;
	float hcy = 0;
	float hcym = 0;
	float hAA = 0;
	float hBB = 0;
	float hypot = 0;
	int jx = 0;
	int jy = 0;
	RAYRET* LENRAYBUFF = new RAYRET[render_distance * 2];
	
	/*class _optimization_utils
	{
	public:
		unsigned __int8 _getMatrixPos(__int16 pos)
		{
			return pos / game::world::wall_size;
		}
	};
	
	//VARS
	class _any_utils
	{
		bool check_cls_walls_arr(unsigned __int16 j, vars::cls_walls_class* _cls_walls, unsigned __int8 size = 0)
		{
			for (unsigned __int8 i = 0; i < size; i++)
			{
				if (j == _cls_walls[i].matrixnum)
				{
					return false;
				}
			}
			return true;
		}
	}

	RAYRET ray_cast_v1(float rayangle)
	{
		float cosA = cos(engine::math::toRad(rayangle));
		float sinA = sin(engine::math::toRad(rayangle));
		for (float l = 0; l < game::world::size; l += game::render::precision)
		{
			float cx = player::getFLOATMatrixPos().x + l * cosA;
			float cy = player::getFLOATMatrixPos().y + l * sinA;
			if (game::world::MATRIX[int(cy)][int(cx)] == 1)
			{
				engine::FLOATPOINT clspos = engine::FLOATPOINT({ cx * game::world::wall_size, cy * game::world::wall_size });
				return RAYRET
				(
					{
						__INT8POINT
						(
							{
								static_cast<unsigned __int8>(cx),
								static_cast<unsigned __int8>(cy)
							}
						),
						clspos,
						hypot(clspos.x - player::getFLOATMatrixPos().x, clspos.y - player::getFLOATMatrixPos().y)
					}
				);
				break;
			}
			return RAYRET();
		}
	}*/
	inline RAYRET ray_cast_v2(float degrayangle, float cosDegAngle, float sinDegAngle, float tanrayangle, float tanrayangle__90)
	{
		unsigned __int8 time = 0;
		float radedangle = math::toRad(degrayangle);
		/*
			0
		_________
		|		|
		|		|
	3	|		|	1
		|_______|
			2
		*/
		if (cosDegAngle > 0 && sinDegAngle > 0)
		{
			//Ver
			_vars::vA = game::world::wall_size - (game::player::pos.x - engine::player::getINTMatrixPos().x * game::world::wall_size);
			_vars::vB = _vars::vA * tanrayangle;
			_vars::vcx = game::player::pos.x + _vars::vA;
			_vars::vcy = game::player::pos.y + _vars::vB;
			_vars::jx = _optimization_utils::_getMatrixPos(_vars::vcx);
			_vars::jy = _optimization_utils::_getMatrixPos(_vars::vcy);
			if (_vars::jx < game::world::size && _vars::jy < game::world::size)
			{
				if (game::world::MATRIX[_vars::jy][_vars::jx] == 1)
				{
					_vars::LENRAYBUFF[time].pos.x = _vars::vcx;
					_vars::LENRAYBUFF[time].pos.y = _vars::vcy;
					_vars::LENRAYBUFF[time].hypot = hypot(_vars::vcx - game::player::pos.x, _vars::vcy - game::player::pos.y);
					_vars::LENRAYBUFF[time].matrixpos.x = _vars::jx;
					_vars::LENRAYBUFF[time].matrixpos.y = _vars::jy;
					_vars::LENRAYBUFF[time].walldir = 3;
					time++;
				}
			}
			_vars::vAA = game::world::wall_size;
			_vars::vBB = _vars::vAA * tanrayangle;

			//Hor
			_vars::hB = game::world::wall_size - (game::player::pos.y - engine::player::getINTMatrixPos().y * game::world::wall_size);
			_vars::hA = _vars::hB * tanrayangle__90;
			_vars::hcx = game::player::pos.x + _vars::hA;
			_vars::hcy = game::player::pos.y + _vars::hB;
			_vars::jx = _optimization_utils::_getMatrixPos(_vars::hcx);
			_vars::jy = _optimization_utils::_getMatrixPos(_vars::hcy);
			if (_vars::jx < game::world::size && _vars::jy < game::world::size)
			{
				if (game::world::MATRIX[_vars::jy][_vars::jx] == 1)
				{
					_vars::LENRAYBUFF[time].pos.x = _vars::hcx;
					_vars::LENRAYBUFF[time].pos.y = _vars::hcy;
					_vars::LENRAYBUFF[time].hypot = hypot(_vars::hcx - game::player::pos.x, _vars::hcy - game::player::pos.y);
					_vars::LENRAYBUFF[time].matrixpos.x = _vars::jx;
					_vars::LENRAYBUFF[time].matrixpos.y = _vars::jy;
					_vars::LENRAYBUFF[time].walldir = 2;
					time++;
				}
			}
			_vars::hBB = game::world::wall_size;
			_vars::hAA = _vars::hBB * tanrayangle__90;
			//RAY LEN SEARCHER
			for (unsigned __int8 i = 0; i < game::render::any::render_distance2__2; i++)
			{
				_vars::vcx += _vars::vAA;
				_vars::vcy += _vars::vBB;
				_vars::jx = _optimization_utils::_getMatrixPos(_vars::vcx);
				_vars::jy = _optimization_utils::_getMatrixPos(_vars::vcy);
				if (_vars::jx < game::world::size && _vars::jy < game::world::size)
				{
					if (game::world::MATRIX[_vars::jy][_vars::jx] == 1)
					{
						//cout << (game::world::MATRIX[_vars::jy][_vars::jx] == 1) << endl;
						_vars::LENRAYBUFF[time].pos.x = _vars::vcx;
						_vars::LENRAYBUFF[time].pos.y = _vars::vcy;
						_vars::LENRAYBUFF[time].hypot = hypot(_vars::vcx - game::player::pos.x, _vars::vcy - game::player::pos.y);
						_vars::LENRAYBUFF[time].matrixpos.x = _vars::jx;
						_vars::LENRAYBUFF[time].matrixpos.y = _vars::jy;
						_vars::LENRAYBUFF[time].walldir = 3;
						time++;
						break;
					}
				}
				else
				{
					break;
				}
			}
			for (unsigned __int8 i = 0; i < game::render::any::render_distance2__2; i++)
			{
				_vars::hcx += _vars::hAA;
				_vars::hcy += _vars::hBB;
				_vars::jx = _optimization_utils::_getMatrixPos(_vars::hcx);
				_vars::jy = _optimization_utils::_getMatrixPos(_vars::hcy);
				if (_vars::jx < game::world::size && _vars::jy < game::world::size)
				{
					if (game::world::MATRIX[_vars::jy][_vars::jx] == 1)
					{
						_vars::LENRAYBUFF[time].pos.x = _vars::hcx;
						_vars::LENRAYBUFF[time].pos.y = _vars::hcy;
						_vars::LENRAYBUFF[time].hypot = hypot(_vars::hcx - game::player::pos.x, _vars::hcy - game::player::pos.y);
						_vars::LENRAYBUFF[time].matrixpos.x = _vars::jx;
						_vars::LENRAYBUFF[time].matrixpos.y = _vars::jy;
						_vars::LENRAYBUFF[time].walldir = 2;
						time++;
						break;
					}
				}
				else
				{
					break;
				}
			}
			//SORT BUFFER
			unsigned __int8 ft = time;
			for (unsigned __int8 i = 0; i < ft; i++)
			{
				for (unsigned __int8 l = 0; l < ft; l++)
				{
					if (_vars::LENRAYBUFF[l].hypot > _vars::LENRAYBUFF[l + 1].hypot)
					{
						swap(_vars::LENRAYBUFF[l], _vars::LENRAYBUFF[l + 1]);
						break;
					}
				}
			}
			return _vars::LENRAYBUFF[0];
		}
		else if (cosDegAngle < 0 && sinDegAngle > 0)
		{
			//Ver
			_vars::vA = game::player::pos.x - engine::player::getINTMatrixPos().x * game::world::wall_size;
			_vars::vB = _vars::vA * tanrayangle;
			_vars::vcx = game::player::pos.x - _vars::vA;
			_vars::vcy = game::player::pos.y - _vars::vB;
			_vars::jx = _optimization_utils::_getMatrixPos(_vars::vcx - 0.1);
			_vars::jy = _optimization_utils::_getMatrixPos(_vars::vcy);
			if (_vars::jx < game::world::size && _vars::jy < game::world::size)
			{
				if (game::world::MATRIX[_vars::jy][_vars::jx] == 1)
				{
					_vars::LENRAYBUFF[time].pos.x = _vars::vcx;
					_vars::LENRAYBUFF[time].pos.y = _vars::vcy;
					_vars::LENRAYBUFF[time].hypot = hypot(_vars::vcx - game::player::pos.x, _vars::vcy - game::player::pos.y);
					_vars::LENRAYBUFF[time].matrixpos.x = _vars::jx;
					_vars::LENRAYBUFF[time].matrixpos.y = _vars::jy;
					_vars::LENRAYBUFF[time].walldir = 1;
					time++;
				}
			}
			_vars::vAA = game::world::wall_size;
			_vars::vBB = _vars::vAA * tanrayangle;

			//Hor
			_vars::hB = game::world::wall_size - (game::player::pos.y - engine::player::getINTMatrixPos().y * game::world::wall_size);
			_vars::hA = _vars::hB * tanrayangle__90;
			_vars::hcx = game::player::pos.x + _vars::hA;
			_vars::hcy = game::player::pos.y + _vars::hB;
			_vars::jx = _optimization_utils::_getMatrixPos(_vars::hcx);
			_vars::jy = _optimization_utils::_getMatrixPos(_vars::hcy);
			if (_vars::jx < game::world::size && _vars::jy < game::world::size)
			{
				if (game::world::MATRIX[_vars::jy][_vars::jx] == 1)
				{
					_vars::LENRAYBUFF[time].pos.x = _vars::hcx;
					_vars::LENRAYBUFF[time].pos.y = _vars::hcy;
					_vars::LENRAYBUFF[time].hypot = hypot(_vars::hcx - game::player::pos.x, _vars::hcy - game::player::pos.y);
					_vars::LENRAYBUFF[time].matrixpos.x = _vars::jx;
					_vars::LENRAYBUFF[time].matrixpos.y = _vars::jy;
					_vars::LENRAYBUFF[time].walldir = 2;
					time++;
				}
			}
			_vars::hBB = game::world::wall_size;
			_vars::hAA = _vars::hBB * tanrayangle__90;
			//RAY LEN SEARCHER
			for (unsigned __int8 i = 0; i < game::render::any::render_distance2__2; i++)
			{
				_vars::vcx -= _vars::vAA;
				_vars::vcy -= _vars::vBB;
				_vars::jx = _optimization_utils::_getMatrixPos(_vars::vcx - 0.1);
				_vars::jy = _optimization_utils::_getMatrixPos(_vars::vcy);
				if (_vars::jx < game::world::size && _vars::jy < game::world::size)
				{
					if (game::world::MATRIX[_vars::jy][_vars::jx] == 1)
					{
						//cout << (game::world::MATRIX[_vars::jy][_vars::jx] == 1) << endl;
						_vars::LENRAYBUFF[time].pos.x = _vars::vcx;
						_vars::LENRAYBUFF[time].pos.y = _vars::vcy;
						_vars::LENRAYBUFF[time].hypot = hypot(_vars::vcx - game::player::pos.x, _vars::vcy - game::player::pos.y);
						_vars::LENRAYBUFF[time].matrixpos.x = _vars::jx;
						_vars::LENRAYBUFF[time].matrixpos.y = _vars::jy;
						_vars::LENRAYBUFF[time].walldir = 1;
						time++;
						break;
					}
				}
				else
				{
					break;
				}
			}
			for (unsigned __int8 i = 0; i < game::render::any::render_distance2__2; i++)
			{
				_vars::hcx += _vars::hAA;
				_vars::hcy += _vars::hBB;
				_vars::jx = _optimization_utils::_getMatrixPos(_vars::hcx);
				_vars::jy = _optimization_utils::_getMatrixPos(_vars::hcy);
				if (_vars::jx < game::world::size && _vars::jy < game::world::size)
				{
					if (game::world::MATRIX[_vars::jy][_vars::jx] == 1)
					{
						_vars::LENRAYBUFF[time].pos.x = _vars::hcx;
						_vars::LENRAYBUFF[time].pos.y = _vars::hcy;
						_vars::LENRAYBUFF[time].hypot = hypot(_vars::hcx - game::player::pos.x, _vars::hcy - game::player::pos.y);
						_vars::LENRAYBUFF[time].matrixpos.x = _vars::jx;
						_vars::LENRAYBUFF[time].matrixpos.y = _vars::jy;
						_vars::LENRAYBUFF[time].walldir = 2;
						time++;
						break;
					}
				}
				else
				{
					break;
				}
			}
			//SORT BUFFER
			unsigned __int8 ft = time;
			for (unsigned __int8 i = 0; i < ft; i++)
			{
				for (unsigned __int8 l = 0; l < ft; l++)
				{
					if (_vars::LENRAYBUFF[l].hypot > _vars::LENRAYBUFF[l + 1].hypot)
					{
						swap(_vars::LENRAYBUFF[l], _vars::LENRAYBUFF[l + 1]);
						break;
					}
				}
			}
			return _vars::LENRAYBUFF[0];
		}
		else if (cosDegAngle > 0 && sinDegAngle < 0)
		{
			//Ver
			_vars::vA = game::world::wall_size - (game::player::pos.x - engine::player::getINTMatrixPos().x * game::world::wall_size);
			_vars::vB = _vars::vA * tanrayangle;
			_vars::vcx = game::player::pos.x + _vars::vA;
			_vars::vcy = game::player::pos.y + _vars::vB;
			_vars::jx = _optimization_utils::_getMatrixPos(_vars::vcx);
			_vars::jy = _optimization_utils::_getMatrixPos(_vars::vcy);
			if (_vars::jx < game::world::size && _vars::jy < game::world::size)
			{
				if (game::world::MATRIX[_vars::jy][_vars::jx] == 1)
				{
					_vars::LENRAYBUFF[time].pos.x = _vars::vcx;
					_vars::LENRAYBUFF[time].pos.y = _vars::vcy;
					_vars::LENRAYBUFF[time].hypot = hypot(_vars::vcx - game::player::pos.x, _vars::vcy - game::player::pos.y);
					_vars::LENRAYBUFF[time].matrixpos.x = _vars::jx;
					_vars::LENRAYBUFF[time].matrixpos.y = _vars::jy;
					_vars::LENRAYBUFF[time].walldir = 3;
					time++;
				}
			}
			_vars::vAA = game::world::wall_size;
			_vars::vBB = _vars::vAA * tanrayangle;

			//Hor
			_vars::hB = game::player::pos.y - engine::player::getINTMatrixPos().y * game::world::wall_size;
			_vars::hA = _vars::hB * tanrayangle__90;
			_vars::hcx = game::player::pos.x - _vars::hA;
			_vars::hcy = game::player::pos.y - _vars::hB;
			_vars::jx = _optimization_utils::_getMatrixPos(_vars::hcx);
			_vars::jy = _optimization_utils::_getMatrixPos(_vars::hcy - 0.1);
			if (_vars::jx < game::world::size && _vars::jy < game::world::size)
			{
				if (game::world::MATRIX[_vars::jy][_vars::jx] == 1)
				{
					_vars::LENRAYBUFF[time].pos.x = _vars::hcx;
					_vars::LENRAYBUFF[time].pos.y = _vars::hcy;
					_vars::LENRAYBUFF[time].hypot = hypot(_vars::hcx - game::player::pos.x, _vars::hcy - game::player::pos.y);
					_vars::LENRAYBUFF[time].matrixpos.x = _vars::jx;
					_vars::LENRAYBUFF[time].matrixpos.y = _vars::jy;
					_vars::LENRAYBUFF[time].walldir = 0;
					time++;
				}
			}
			_vars::hBB = game::world::wall_size;
			_vars::hAA = _vars::hBB * tanrayangle__90;
			//RAY LEN SEARCHER
			for (unsigned __int8 i = 0; i < game::render::any::render_distance2__2; i++)
			{
				_vars::vcx += _vars::vAA;
				_vars::vcy += _vars::vBB;
				_vars::jx = _optimization_utils::_getMatrixPos(_vars::vcx);
				_vars::jy = _optimization_utils::_getMatrixPos(_vars::vcy);
				if (_vars::jx < game::world::size && _vars::jy < game::world::size)
				{
					if (game::world::MATRIX[_vars::jy][_vars::jx] == 1)
					{
						//cout << (game::world::MATRIX[_vars::jy][_vars::jx] == 1) << endl;
						_vars::LENRAYBUFF[time].pos.x = _vars::vcx;
						_vars::LENRAYBUFF[time].pos.y = _vars::vcy;
						_vars::LENRAYBUFF[time].hypot = hypot(_vars::vcx - game::player::pos.x, _vars::vcy - game::player::pos.y);
						_vars::LENRAYBUFF[time].matrixpos.x = _vars::jx;
						_vars::LENRAYBUFF[time].matrixpos.y = _vars::jy;
						_vars::LENRAYBUFF[time].walldir = 3;
						time++;
						break;
					}
				}
				else
				{
					break;
				}
			}
			for (unsigned __int8 i = 0; i < game::render::any::render_distance2__2; i++)
			{
				_vars::hcx -= _vars::hAA;
				_vars::hcy -= _vars::hBB;
				_vars::jx = _optimization_utils::_getMatrixPos(_vars::hcx);
				_vars::jy = _optimization_utils::_getMatrixPos(_vars::hcy - 0.1);
				if (_vars::jx < game::world::size && _vars::jy < game::world::size)
				{
					if (game::world::MATRIX[_vars::jy][_vars::jx] == 1)
					{
						_vars::LENRAYBUFF[time].pos.x = _vars::hcx;
						_vars::LENRAYBUFF[time].pos.y = _vars::hcy;
						_vars::LENRAYBUFF[time].hypot = hypot(_vars::hcx - game::player::pos.x, _vars::hcy - game::player::pos.y);
						_vars::LENRAYBUFF[time].matrixpos.x = _vars::jx;
						_vars::LENRAYBUFF[time].matrixpos.y = _vars::jy;
						_vars::LENRAYBUFF[time].walldir = 0;
						time++;
						break;
					}
				}
				else
				{
					break;
				}
			}
			//SORT BUFFER
			unsigned __int8 ft = time;
			for (unsigned __int8 i = 0; i < ft; i++)
			{
				for (unsigned __int8 l = 0; l < ft; l++)
				{
					if (_vars::LENRAYBUFF[l].hypot > _vars::LENRAYBUFF[l + 1].hypot)
					{
						swap(_vars::LENRAYBUFF[l], _vars::LENRAYBUFF[l + 1]);
						break;
					}
				}
			}
			return _vars::LENRAYBUFF[0];
		}
		else if (cosDegAngle < 0 && sinDegAngle < 0)
		{
			//Ver
			_vars::vA = game::player::pos.x - engine::player::getINTMatrixPos().x * game::world::wall_size;
			_vars::vB = _vars::vA * tanrayangle;
			_vars::vcx = game::player::pos.x - _vars::vA;
			_vars::vcy = game::player::pos.y - _vars::vB;
			_vars::jx = _optimization_utils::_getMatrixPos(_vars::vcx - 0.1);
			_vars::jy = _optimization_utils::_getMatrixPos(_vars::vcy);
			if (_vars::jx < game::world::size && _vars::jy < game::world::size)
			{
				if (game::world::MATRIX[_vars::jy][_vars::jx] == 1)
				{
					_vars::LENRAYBUFF[time].pos.x = _vars::vcx;
					_vars::LENRAYBUFF[time].pos.y = _vars::vcy;
					_vars::LENRAYBUFF[time].hypot = hypot(_vars::vcx - game::player::pos.x, _vars::vcy - game::player::pos.y);
					_vars::LENRAYBUFF[time].matrixpos.x = _vars::jx;
					_vars::LENRAYBUFF[time].matrixpos.y = _vars::jy;
					_vars::LENRAYBUFF[time].walldir = 1;
					time++;
				}
			}
			_vars::vAA = game::world::wall_size;
			_vars::vBB = _vars::vAA * tanrayangle;

			//Hor
			_vars::hB = game::player::pos.y - engine::player::getINTMatrixPos().y * game::world::wall_size;
			_vars::hA = _vars::hB * tanrayangle__90;
			_vars::hcx = game::player::pos.x - _vars::hA;
			_vars::hcy = game::player::pos.y - _vars::hB;
			_vars::jx = _optimization_utils::_getMatrixPos(_vars::hcx);
			_vars::jy = _optimization_utils::_getMatrixPos(_vars::hcy - 0.1);
			if (_vars::jx < game::world::size && _vars::jy < game::world::size)
			{
				if (game::world::MATRIX[_vars::jy][_vars::jx] == 1)
				{
					_vars::LENRAYBUFF[time].pos.x = _vars::hcx;
					_vars::LENRAYBUFF[time].pos.y = _vars::hcy;
					_vars::LENRAYBUFF[time].hypot = hypot(_vars::hcx - game::player::pos.x, _vars::hcy - game::player::pos.y);
					_vars::LENRAYBUFF[time].matrixpos.x = _vars::jx;
					_vars::LENRAYBUFF[time].matrixpos.y = _vars::jy;
					_vars::LENRAYBUFF[time].walldir = 0;
					time++;
				}
			}
			_vars::hBB = game::world::wall_size;
			_vars::hAA = _vars::hBB * tanrayangle__90;
			//RAY LEN SEARCHER
			for (unsigned __int8 i = 0; i < game::render::any::render_distance2__2; i++)
			{
				_vars::vcx -= _vars::vAA;
				_vars::vcy -= _vars::vBB;
				_vars::jx = _optimization_utils::_getMatrixPos(_vars::vcx - 0.1);
				_vars::jy = _optimization_utils::_getMatrixPos(_vars::vcy);
				if (_vars::jx < game::world::size && _vars::jy < game::world::size)
				{
					if (game::world::MATRIX[_vars::jy][_vars::jx] == 1)
					{
						//cout << (game::world::MATRIX[_vars::jy][_vars::jx] == 1) << endl;
						_vars::LENRAYBUFF[time].pos.x = _vars::vcx;
						_vars::LENRAYBUFF[time].pos.y = _vars::vcy;
						_vars::LENRAYBUFF[time].hypot = hypot(_vars::vcx - game::player::pos.x, _vars::vcy - game::player::pos.y);
						_vars::LENRAYBUFF[time].matrixpos.x = _vars::jx;
						_vars::LENRAYBUFF[time].matrixpos.y = _vars::jy;
						_vars::LENRAYBUFF[time].walldir = 1;
						time++;
						break;
					}
				}
				else
				{
					break;
				}
			}
			for (unsigned __int8 i = 0; i < game::render::any::render_distance2__2; i++)
			{
				_vars::hcx -= _vars::hAA;
				_vars::hcy -= _vars::hBB;
				_vars::jx = _optimization_utils::_getMatrixPos(_vars::hcx);
				_vars::jy = _optimization_utils::_getMatrixPos(_vars::hcy - 0.01);
				if (_vars::jx < game::world::size && _vars::jy < game::world::size)
				{
					if (game::world::MATRIX[_vars::jy][_vars::jx] == 1)
					{
						_vars::LENRAYBUFF[time].pos.x = _vars::hcx;
						_vars::LENRAYBUFF[time].pos.y = _vars::hcy;
						_vars::LENRAYBUFF[time].hypot = hypot(_vars::hcx - game::player::pos.x, _vars::hcy - game::player::pos.y);
						_vars::LENRAYBUFF[time].matrixpos.x = _vars::jx;
						_vars::LENRAYBUFF[time].matrixpos.y = _vars::jy;
						_vars::LENRAYBUFF[time].walldir = 0;
						time++;
						break;
					}
				}
				else
				{
					break;
				}
			}
			//SORT BUFFER
			unsigned __int8 ft = time;
			for (unsigned __int8 i = 0; i < ft; i++)
			{
				for (unsigned __int8 l = 0; l < ft; l++)
				{
					if (_vars::LENRAYBUFF[l].hypot > _vars::LENRAYBUFF[l + 1].hypot)
					{
						swap(_vars::LENRAYBUFF[l], _vars::LENRAYBUFF[l + 1]);
						break;
					}
				}
			}
			return _vars::LENRAYBUFF[0];
		}
		//MAIN RAYRET
		return RAYRET();
	}
}