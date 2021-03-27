#pragma once
#ifndef SFML_DYNAMIC
#define SFML_DYNAMIC
#include <fstream>
#include <functional>
#endif
#ifdef _DEBUG
#pragma comment(lib, "lib/sfml-graphics-d.lib")
#pragma comment(lib, "lib/sfml-audio-d.lib")
#pragma comment(lib, "lib/sfml-network-d.lib")
#pragma comment(lib, "lib/sfml-system-d.lib")
#pragma comment(lib, "lib/sfml-window-d.lib")
#else
#pragma comment(lib, "lib/sfml-graphics.lib")
#pragma comment(lib, "lib/sfml-audio.lib")
#pragma comment(lib, "lib/sfml-network.lib")
#pragma comment(lib, "lib/sfml-system.lib")
#pragma comment(lib, "lib/sfml-window.lib")
#endif

#include "include/my/binary.h"
#include <SFML/Graphics.hpp>
#include "Math.h"

template<typename Type>
constexpr bool IsFloatType = 
std::is_same<Type, float>::value || 
std::is_same<Type, double>::value || 
std::is_same<Type, long double>::value;



class v3d
{
public:
	
	typedef std::vector<std::vector<int>> Matrix;

	static Matrix generateMatrix(const unsigned int sizeX, 
		const unsigned int sizeY, const int val)
	{
		return Matrix(sizeX, std::vector<int>(sizeY, val));
	}
	
	template<typename T>
	class Quad
	{
	public:
		T a;
		T b;
		T c;
		T d;

		Quad() :
			a(T()),
			b(T()),
			c(T()),
			d(T())
		{

		}

		Quad(const T a, const T b, const T c, const T d) :
			a(a),
			b(b),
			c(c),
			d(d)
		{

		}
	};

	template<typename Time>
	class Clock
	{
		static_assert(IsFloatType<Time>, "template's typename are not <float> or <double> or <long double>");

	public:

		static Time seconds()
		{
			return std::clock() / CLOCKS_PER_SEC;
		}

		static Time milliseconds()
		{
			return seconds() * 1000;
		}

		static Time microseconds()
		{
			return seconds() * 1000000;
		}

		static Time nanoseconds()
		{
			return seconds() * 1000000000;
		}
	};

	template<typename Time>
	class Timer
	{
		static_assert(IsFloatType<Time>, 
			"template's typename are not <float> or <double> or <long double>");
	
	protected:
		Time d_time_;

	public:

		Timer() :
		d_time_(Clock<Time>::seconds())
		{
			
		}
		
		Time seconds()
		{
			return static_cast<Time>(std::clock()) / 
				static_cast<Time>CLOCKS_PER_SEC - d_time_;
		}
		
		Time milliseconds()
		{
			return seconds() * 1000;
		}
		
		Time microseconds()
		{
			return seconds() * 1000000;
		}
		
		Time nanoseconds()
		{
			return seconds() * 1000000000;
		}

		void clear()
		{
			d_time_ = static_cast<Time>(std::clock()) / 
				static_cast<Time>CLOCKS_PER_SEC;
		}
	};

public:

	//Система сохранений
	class FileSaveable
	{
	public:
		virtual void save(std::string name) = 0;
		virtual ~FileSaveable() = default;
	};
	
	//Менеджер файлов
	class FileManager
	{
	public:	
		static void save(FileSaveable *states, const std::string& name)
		{
			states->save(name);
		}
	};
	
	//Генератор лабиринта
	template<const unsigned int SizeX,
	const unsigned int SizeY, const unsigned Seed>
	class Maze : public FileSaveable
	{
	private:
		
		//Сид, по которому будет генерироваться определённый узор лабиринта
		unsigned __int8 seed_[64] = {};
		
		//Варианты поворота
		enum class Rot
		{
			LEFT,
			RIGHT,
			DOWN,
			UP,
			RotNull
		};

		//Класс для функции, ищущей варианты поворота
		class ret
		{
		public:
			Rot rot;
			sf::Vector2<int> pos;
			ret() :
				rot(Rot::RotNull),
				pos(sf::Vector2<int>())
			{

			}
			ret(const Rot rot, const sf::Vector2<int> pos) :
				rot(rot),
				pos(pos)
			{

			}
		};

		//Эта функция находит возможный вариант поворота лабиринта в определённую сторону
		ret fnd(Matrix& matrix, std::vector<sf::Vector2<int>>& maze_way_vec)
		{
			int sdpY(0);
			int sdpX(0);
			for (size_t i = 0; i < maze_way_vec.size(); i++)
			{
				//0
				sdpY = maze_way_vec[i].y - 1;
				sdpX = maze_way_vec[i].x;
				if (sdpY > 0)
				{
					if (sdpY < SizeY - 1)
					{
						if (sdpX > 0)
						{
							if (sdpX < SizeX - 1)
							{
								if (matrix[sdpX][sdpY] != 0)
								{
									if (matrix[sdpX][sdpY - 1] != 0)
									{
										if (matrix[sdpX + 1][sdpY] != 0)
										{
											if (matrix[sdpX - 1][sdpY] != 0)
											{
												return ret(Rot::UP, sf::Vector2<int>(sdpX, sdpY));
											}
										}
									}
								}
							}
						}
					}
				}
				//1
				sdpY = maze_way_vec[i].y + 1;
				sdpX = maze_way_vec[i].x;
				if (sdpY > 0)
				{
					if (sdpY < SizeY - 1)
					{
						if (sdpX > 0)
						{
							if (sdpX < SizeX - 1)
							{
								if (matrix[sdpX][sdpY] != 0)
								{
									if (matrix[sdpX][sdpY + 1] != 0)
									{
										if (matrix[sdpX + 1][sdpY] != 0)
										{
											if (matrix[sdpX - 1][sdpY] != 0)
											{
												return ret(Rot::DOWN, sf::Vector2<int>(sdpX, sdpY));
											}
										}
									}
								}
							}
						}
					}
				}
				//2
				sdpY = maze_way_vec[i].y;
				sdpX = maze_way_vec[i].x - 1;
				if (sdpY > 0)
				{
					if (sdpY < SizeY - 1)
					{
						if (sdpX > 0)
						{
							if (sdpX < SizeX - 1)
							{
								if (matrix[sdpX][sdpY] != 0)
								{
									if (matrix[sdpX][sdpY - 1] != 0)
									{
										if (matrix[sdpX][sdpY + 1] != 0)
										{
											if (matrix[sdpX - 1][sdpY] != 0)
											{
												return ret(Rot::LEFT, sf::Vector2<int>(sdpX, sdpY));
											}
										}
									}
								}
							}
						}
					}
				}
				//3
				sdpY = maze_way_vec[i].y;
				sdpX = maze_way_vec[i].x + 1;
				if (sdpY > 0)
				{
					if (sdpY < SizeY - 1)
					{
						if (sdpX > 0)
						{
							if (sdpX < SizeX - 1)
							{
								if (matrix[sdpX][sdpY] != 0)
								{
									if (matrix[sdpX][sdpY - 1] != 0)
									{
										if (matrix[sdpX][sdpY + 1] != 0)
										{
											if (matrix[sdpX + 1][sdpY] != 0)
											{
												return ret(Rot::RIGHT, sf::Vector2<int>(sdpX, sdpY));
											}
										}
									}
								}
							}
						}
					}
				}
			}
			return ret();
		}
	
	public:
		
		//Функция, возвращающая лабиринт
		Matrix generate()
		{
			//Создаём матрицу лабиринта
			class Matrix matrix = generateMatrix(SizeX, SizeY, 1);

			//Объявляем первоначальную точку строителя лабиринта в матрице
			sf::Vector2<int> pos = sf::Vector2<int>
			(static_cast<int>(SizeX / 2), static_cast<int>(SizeY / 2));

			//Путь строителя для записи его ходов
			std::vector<sf::Vector2<int>> maze_way_vec;

			//Генерируем настоящий сид лабиринта
			for (auto i = 0; i < 64; i++)
			{
				seed_[i] = static_cast<char>(math::rand(0, 3, 
					Seed /(i + 1)));
			}

			//Записываем в путь первоначальную позицию строителя
			maze_way_vec.push_back(pos);

			//объявляем доступнцю точку в матрице лабиринта по данной позиции
			matrix[pos.x][pos.y] = 1;

			//площадь матрицы лабиринта
			const int for_num = SizeX * SizeY;
			for (size_t l = 0; l < for_num; l++)
			{
				const class ret fnd_ret = fnd(matrix, maze_way_vec);
				if (fnd_ret.rot == Rot::RotNull)
				{
					return matrix;
				}
				pos = fnd_ret.pos;
				matrix[pos.x][pos.y] = 0;
				maze_way_vec.emplace_back(pos.x, pos.y);

				unsigned maze_time = 0;

				for (unsigned i = maze_time; i < 4; i++)
				{
					if (seed_[i] == 0)
					{
						if (pos.y - 1 > 0)
						{
							if (matrix[pos.x][pos.y - 1] != 0)
							{
								const int mpY = pos.y - 1;
								const int mpX = pos.x;
								if (matrix[mpX][mpY - 1] != 0)
								{
									if (matrix[mpX + 1][mpY] != 0)
									{
										if (matrix[mpX - 1][mpY] != 0)
										{
											pos.y -= 1;
											matrix[pos.x][pos.y] = 0;
											maze_way_vec.emplace_back(pos.x, pos.y);
										}
									}
								}
							}
						}
					}
					else if (seed_[i] == 1)
					{
						if (pos.y + 1 < SizeY - 1)
						{
							if (matrix[pos.y + 1][pos.x] != 0)
							{
								const int mpY = pos.y + 1;
								const int mpX = pos.x;
								if (matrix[mpX + 1][mpY] != 0)
								{
									if (matrix[mpX - 1][mpY] != 0)
									{
										if (matrix[mpX][mpY + 1] != 0)
										{
											pos.y += 1;
											matrix[pos.x][pos.y] = 0;
											maze_way_vec.emplace_back(pos.x, pos.y);
										}
									}
								}
							}
						}
					}
					else if (seed_[i] == 2)
					{
						if (pos.x - 1 > 0)
						{
							if (matrix[pos.x - 1][pos.y] != 0)
							{
								const int mpY = pos.y;
								const int mpX = pos.x - 1;
								if (matrix[mpX][mpY - 1] != 0)
								{
									if (matrix[mpX][mpY + 1] != 0)
									{
										if (matrix[mpX - 1][mpY] != 0)
										{
											pos.x -= 1;
											matrix[pos.x][pos.y] = 0;
											maze_way_vec.emplace_back(pos.x, pos.y);
										}
									}
								}
							}
						}
					}
					else if (seed_[i] == 3)
					{
						if (pos.x + 1 < SizeX - 1)
						{
							if (matrix[pos.x][pos.y] != 0)
							{
								const int mpY = pos.y;
								const int mpX = pos.x + 1;
								if (matrix[mpX][mpY + 1] != 0)
								{
									if (matrix[mpX][mpY - 1] != 0)
									{
										if (matrix[mpX + 1][mpY] != 0)
										{
											pos.x += 1;
											matrix[pos.x][pos.y] = 1;
											maze_way_vec.emplace_back(pos.x, pos.y);
										}
									}
								}
							}
						}
					}
					maze_time++;
					if (maze_time == 64)
					{
						maze_time = 0;
					}
				}
			}
			return matrix;
		}
		
		void save(const std::string name) override
		{
			std::cout << "SAVE MAZE" << std::endl;
		}
	};

	class Camera;

private:

	class RayCaster_api;
	
	class MainWorldStates
	{
	protected:
		sf::Vector2<float> size_;
		Matrix matrix_;
		sf::Vector2<int> matrix_size_;
		sf::Vector2<float> wall_size_;
		unsigned int render_distance_;
		int ray_num_;
		int ray_num_2_;

		friend class RayCaster_api;
		
		MainWorldStates(const sf::Vector2<float> size, 
			Matrix& matrix,
			const  sf::Vector2<int> matrix_size, 
			const  sf::Vector2<float> wall_size, 
			const  unsigned int render_distance,
			sf::RenderWindow& window) :
			size_(size),
			matrix_(matrix),
			matrix_size_(matrix_size),
			wall_size_(wall_size),
			render_distance_(render_distance),
			ray_num_(window.getSize().x),
			ray_num_2_(ray_num_ / 2)
		{
		}
	};

public:
	class RayCaster_api : public MainWorldStates
	{
	public:

		class  RayData;
		
		RayData* t_ray_data_vec{};
		
		RayCaster_api(const MainWorldStates& states) :
		MainWorldStates(states),
			t_ray_data_vec(static_cast<RayData*>(malloc((states.ray_num_) * sizeof(RayData))))
		{

		}

		~RayCaster_api()
		{
			free(t_ray_data_vec);
		}
		
		void ResetStates(const MainWorldStates& states)
		{
			*this = states;
			if (realloc(t_ray_data_vec, (states.ray_num_) * sizeof(RayData)) == NULL)
			{
				exit(-4);
			}
		}

		enum class dir
		{
			left,
			right,
			down,
			up,
			none
		};
		
		class RayData
		{
		public:			
			float pos_x;
			float pos_y;
			float len;
			unsigned int wall_num;
			dir dir;

			RayData() :
				pos_x(0),
				pos_y(0),
				len(0),
				wall_num(0),
				dir(dir::none)
			{
				
			}

			RayData(const float pos_x, const float pos_y, const float len,
				const unsigned int wall_num,
				const RayCaster_api::dir dir) :
				pos_x(pos_x),
				pos_y(pos_y),
				len(len),
				wall_num(wall_num),
				dir(dir)
			{
				
			}

			bool operator < (const RayData& ray_data) const
			{
				if (len < ray_data.len)
				{
					return true;
				}

				return false;
			}
		};

		RayData* RayCast(Camera& camera)
		{
			/*
				0
			_________
			|		|
			|		|
		3	|		|	1
			|_______|

				2
			*/
			float frn = camera.fov_ / static_cast<float>(ray_num_);
			int f = -ray_num_2_;
			for (auto r = 0; r < ray_num_; r++)
			{
				std::vector<RayData> ray_data_vec;
				const float angle = camera.rotation_ + (f * (frn));
				f++;
				const float cos_angle = math::cos180(angle);
				const float sin_angle = math::sin180(angle);
				const float tan_angle = math::tan180(angle);
				const float ctg_angle = math::ctg180(angle);

				if (cos_angle > 0 && sin_angle > 0)
				{
					//Ver
					
					const float vA = wall_size_.x - (camera.position_.x - math::getMatrixPos(camera.position_.x, wall_size_.x) * wall_size_.x);
					const float vB = vA * tan_angle;
					float vcx = camera.position_.x + vA;
					float vcy = camera.position_.y + vB;
					int vjx = math::getMatrixPos(vcx, wall_size_.x);
					int vjy = math::getMatrixPos(vcy, wall_size_.y);
					if (vjx < matrix_size_.x && vjx >= 0 && vjy < matrix_size_.y && vjy >= 0)
					{
						if (matrix_[vjx][vjy] != 0)
						{
							ray_data_vec.emplace_back(vcx, vcy, hypot(vcx - camera.position_.x, vcy - camera.position_.y), matrix_[vjx][vjy], dir::left);
						}
					}
					const float vAA = wall_size_.x;
					const float vBB = vAA * tan_angle;

					//Hor
					const float hB = wall_size_.y - (camera.position_.y - math::getMatrixPos(camera.position_.y, wall_size_.y) * wall_size_.y);
					const float hA = hB * ctg_angle;
					float hcx = camera.position_.x + hA;
					float hcy = camera.position_.y + hB;
					int hjx = math::getMatrixPos(hcx, wall_size_.x);
					int hjy = math::getMatrixPos(hcy, wall_size_.y);
					if (hjx < matrix_size_.x && hjx >= 0 && hjy < matrix_size_.y && hjy >= 0)
					{
						if (matrix_[hjx][hjy] != 0)
						{
							ray_data_vec.emplace_back(hcx, hcy, hypot(hcx - camera.position_.x, hcy - camera.position_.y), matrix_[hjx][hjy], dir::left);
						}
					}
					const float hBB = wall_size_.y;
					const float hAA = hBB * ctg_angle;
					//RAY LEN SEARCHER
					for (unsigned int i = 0; i < render_distance_; i++)
					{
						vcx += vAA;
						vcy += vBB;
						vjx = math::getMatrixPos(vcx, wall_size_.x);
						vjy = math::getMatrixPos(vcy, wall_size_.y);
						if (vjx < matrix_size_.x && vjx >= 0 && vjy < matrix_size_.y && vjy >= 0)
						{
							if (matrix_[vjx][vjy] != 0)
							{
								ray_data_vec.emplace_back(vcx, vcy, hypotf(vcx - camera.position_.x, vcy - camera.position_.y), matrix_[vjx][vjy], dir::left);
								break;
							}
						}
						else
						{
							break;
						}
					}
					for (unsigned int i = 0; i < render_distance_; i++)
					{
						hcx += hAA;
						hcy += hBB;
						hjx = math::getMatrixPos(hcx, wall_size_.x);
						hjy = math::getMatrixPos(hcy, wall_size_.y);
						if (hjx < matrix_size_.x && hjx >= 0 && hjy < matrix_size_.y && hjy >= 0)
						{
							if (matrix_[hjx][hjy] != 0)
							{
								ray_data_vec.emplace_back(hcx, hcy, hypot(hcx - camera.position_.x, hcy - camera.position_.y), matrix_[hjx][hjy], dir::left);
								break;
							}
						}
						else
						{
							break;
						}
					}

					std::sort
					(
						ray_data_vec.begin(),
						ray_data_vec.end(),
						[](RayData const& a, RayData const& b) -> bool
						{
							return a.len < b.len;
						}
					);
					t_ray_data_vec[r] = ray_data_vec[0];
				}
				else if (cos_angle < 0 && sin_angle > 0)
				{
					//Ver
					const float vA = camera.position_.x - math::getMatrixPos(camera.position_.x, wall_size_.x) * wall_size_.x;
					const float vB = vA * tan_angle;
					float vcx = camera.position_.x - vA;
					float vcy = camera.position_.y - vB;
					int vjx = math::getMatrixPos(vcx - 0.1f, wall_size_.x);
					int vjy = math::getMatrixPos(vcy, wall_size_.y);
					if (vjx < matrix_size_.x && vjx >= 0 && vjy < matrix_size_.y && vjy >= 0)
					{
						if (matrix_[vjx][vjy] != 0)
						{
							ray_data_vec.emplace_back(vcx, vcy, hypot(vcx - camera.position_.x, vcy - camera.position_.y), matrix_[vjy][vjx], dir::left);
						}
					}
					const float vAA = wall_size_.x;
					const float vBB = vAA * tan_angle;
					//Hor
					const float hB = wall_size_.y - (camera.position_.y - math::getMatrixPos(camera.position_.y, wall_size_.y) * wall_size_.y);
					const float hA = hB * ctg_angle;
					float hcx = camera.position_.x + hA;
					float hcy = camera.position_.y + hB;
					int hjx = math::getMatrixPos(hcx, wall_size_.x);
					int hjy = math::getMatrixPos(hcy, wall_size_.y);
					if (hjx < matrix_size_.x && hjx >= 0 && hjy < matrix_size_.y && hjy >= 0)
					{
						if (matrix_[hjx][hjy] != 0)
						{
							ray_data_vec.emplace_back(hcx, hcy, hypot(hcx - camera.position_.x, hcy - camera.position_.y), matrix_[hjx][hjy], dir::left);
						}
					}
					const float hBB = wall_size_.y;
					const float hAA = hBB * ctg_angle;
					//RAY LEN SEARCHER
					for (unsigned int i = 0; i < render_distance_; i++)
					{
						vcx -= vAA;
						vcy -= vBB;
						vjx = math::getMatrixPos(vcx - 0.1f, wall_size_.x);
						vjy = math::getMatrixPos(vcy, wall_size_.y);
						if (vjx < matrix_size_.x && vjx >= 0 && vjy < matrix_size_.y && vjy >= 0)
						{
							if (matrix_[vjx][vjy] != 0)
							{
								ray_data_vec.emplace_back(vcx, vcy, hypot(vcx - camera.position_.x, vcy - camera.position_.y), matrix_[vjx][vjy], dir::left);
								break;
							}
						}
						else
						{
							break;
						}
					}
					for (unsigned int i = 0; i < render_distance_; i++)
					{
						hcx += hAA;
						hcy += hBB;
						hjx = math::getMatrixPos(hcx, wall_size_.x);
						hjy = math::getMatrixPos(hcy, wall_size_.y);
						if (hjx < matrix_size_.x && hjx >= 0 && hjy < matrix_size_.y && hjy >= 0)
						{
							if (matrix_[hjx][hjy] != 0)
							{
								ray_data_vec.emplace_back(hcx, hcy, hypot(hcx - camera.position_.x, hcy - camera.position_.y), matrix_[hjx][hjy], dir::left);
								break;
							}
						}
						else
						{
							break;
						}
					}

					std::sort
					(
						ray_data_vec.begin(),
						ray_data_vec.end(),
						[](RayData const& a, RayData const& b) -> bool
						{
							return a.len < b.len;
						}
					);

					t_ray_data_vec[r] = ray_data_vec[0];
					
				}
				else if (cos_angle > 0 && sin_angle < 0)
				{
					//Ver
					const float vA = wall_size_.x - (camera.position_.x - math::getMatrixPos(camera.position_.x, wall_size_.x) * wall_size_.x);
					const float vB = vA * tan_angle;
					float vcx = camera.position_.x + vA;
					float vcy = camera.position_.y + vB;
					int vjx = math::getMatrixPos(vcx, wall_size_.x);
					int vjy = math::getMatrixPos(vcy, wall_size_.y);
					if (vjx < matrix_size_.x && vjx >= 0 && vjy < matrix_size_.y && vjy >= 0)
					{
						if (matrix_[vjx][vjy] != 0)
						{
							ray_data_vec.emplace_back(vcx, vcy, hypot(vcx - camera.position_.x, vcy - camera.position_.y), matrix_[vjy][vjx], dir::left);
						}
					}
					const float vAA = wall_size_.x;
					const float vBB = vAA * tan_angle;

					//Hor
					const float hB = camera.position_.y - math::getMatrixPos(camera.position_.y, wall_size_.y) * wall_size_.y;
					const float hA = hB * ctg_angle;
					float hcx = camera.position_.x - hA;
					float hcy = camera.position_.y - hB;
					int hjx = math::getMatrixPos(hcx, wall_size_.x);
					int hjy = math::getMatrixPos(hcy - 0.1f, wall_size_.y);
					if (hjx < matrix_size_.x && hjx >= 0 && hjy < matrix_size_.y && hjy >= 0)
					{
						if (matrix_[hjx][hjy] != 0)
						{
							ray_data_vec.emplace_back(hcx, hcy, hypot(hcx - camera.position_.x, hcy - camera.position_.y), matrix_[hjx][hjy], dir::left);
						}
					}
					const float hBB = wall_size_.y;
					const float hAA = hBB * ctg_angle;
					//RAY LEN SEARCHER
					for (unsigned int i = 0; i < render_distance_; i++)
					{
						vcx += vAA;
						vcy += vBB;
						vjx = math::getMatrixPos(vcx, wall_size_.x);
						vjy = math::getMatrixPos(vcy, wall_size_.y);
						if (vjx < matrix_size_.x && vjx >= 0 && vjy < matrix_size_.y && vjy >= 0)
						{
							if (matrix_[vjx][vjy] != 0)
							{
								ray_data_vec.emplace_back(vcx, vcy, hypot(vcx - camera.position_.x, vcy - camera.position_.y), matrix_[vjx][vjy], dir::left);
								break;
							}
						}
						else
						{
							break;
						}
					}
					for (unsigned int i = 0; i < render_distance_; i++)
					{
						hcx -= hAA;
						hcy -= hBB;
						hjx = math::getMatrixPos(hcx, wall_size_.x);
						hjy = math::getMatrixPos(hcy - 0.1f, wall_size_.y);
						if (hjx < matrix_size_.x && hjx >= 0 && hjy < matrix_size_.y && hjy >= 0)
						{
							if (matrix_[hjx][hjy] != 0)
							{
								ray_data_vec.emplace_back(hcx, hcy, hypot(hcx - camera.position_.x, hcy - camera.position_.y), matrix_[hjx][hjy], dir::left);
								break;
							}
						}
						else
						{
							break;
						}
					}

					std::sort
					(
						ray_data_vec.begin(),
						ray_data_vec.end(),
						[](RayData const& a, RayData const& b) -> bool
						{
							return a.len < b.len;
						}
					);

					t_ray_data_vec[r] = ray_data_vec[0];
					
				}
				else if (cos_angle < 0 && sin_angle < 0)
				{
					//Ver
					const float vA = camera.position_.x - math::getMatrixPos(camera.position_.x, wall_size_.x) * wall_size_.x;
					const float vB = vA * tan_angle;
					float vcx = camera.position_.x - vA;
					float vcy = camera.position_.y - vB;
					int vjx = math::getMatrixPos(vcx - 0.1f, wall_size_.x);
					int vjy = math::getMatrixPos(vcy, wall_size_.y);
					if (vjx < matrix_size_.x && vjx >= 0 && vjy < matrix_size_.y && vjy >= 0)
					{
						if (matrix_[vjx][vjy] != 0)
						{
							ray_data_vec.emplace_back(vcx, vcy, hypot(vcx - camera.position_.x, vcy - camera.position_.y), matrix_[vjy][vjx], dir::left);
						}
					}
					const float vAA = wall_size_.x;
					const float vBB = vAA * tan_angle;

					//Hor
					const float hB = camera.position_.y - math::getMatrixPos(camera.position_.y, wall_size_.y) * wall_size_.y;
					const float hA = hB * ctg_angle;
					float hcx = camera.position_.x - hA;
					float hcy = camera.position_.y - hB;
					int hjx = math::getMatrixPos(hcx, wall_size_.x);
					int hjy = math::getMatrixPos(hcy - 0.1f, wall_size_.y);
					if (hjx < matrix_size_.x && hjx >= 0 && hjy < matrix_size_.y && hjy >= 0)
					{
						if (matrix_[hjx][hjy] != 0)
						{
							ray_data_vec.emplace_back(hcx, hcy, hypot(hcx - camera.position_.x, hcy - camera.position_.y), matrix_[hjx][hjy], dir::left);
						}
					}
					const float hBB = wall_size_.y;
					const float hAA = hBB * ctg_angle;
					//RAY LEN SEARCHER
					for (unsigned int i = 0; i < render_distance_; i++)
					{
						vcx -= vAA;
						vcy -= vBB;
						vjx = math::getMatrixPos(vcx - 0.1f, wall_size_.x);
						vjy = math::getMatrixPos(vcy, wall_size_.y);
						if (vjx < matrix_size_.x && vjx >= 0 && vjy < matrix_size_.y && vjy >= 0)
						{
							if (matrix_[vjx][vjy] != 0)
							{
								ray_data_vec.emplace_back(vcx, vcy, hypot(vcx - camera.position_.x, vcy - camera.position_.y), matrix_[vjx][vjy], dir::left);
								break;
							}
						}
						else
						{
							break;
						}
					}
					for (unsigned int i = 0; i < render_distance_; i++)
					{
						hcx -= hAA;
						hcy -= hBB;
						hjx = math::getMatrixPos(hcx, wall_size_.x);
						hjy = math::getMatrixPos(hcy - 0.1f, wall_size_.y);
						if (hjx < matrix_size_.x && hjx >= 0 && hjy < matrix_size_.y && hjy >= 0)
						{
							if (matrix_[hjx][hjy] != 0)
							{
								ray_data_vec.emplace_back(hcx, hcy, hypot(hcx - camera.position_.x, hcy - camera.position_.y), matrix_[hjx][hjy], dir::left);
								break;
							}
						}
						else
						{
							break;
						}
					}
					//SORT BUFFER
					std::sort
					(
						ray_data_vec.begin(),
						ray_data_vec.end(),
						[](RayData const& a, RayData const& b) -> bool
						{
							return a.len < b.len;
						}
					);
					//t_ray_data_vec[r] = ray_data_vec[0];
				}
			}
			return t_ray_data_vec;
		}
	};

public:

	class WallAPI
	{
	public:
		//virtual voids
	};

	class World : public MainWorldStates, public sf::Drawable, public sf::Transformable, public FileSaveable
	{
	private:
		
		RayCaster_api ray_caster_api_;
		
		RayCaster_api::RayData* ray_data_{};

	public:
		
		World(sf::RenderWindow& window, const sf::Vector2<float> size =
			sf::Vector2<float>(1024, 1024),
			Matrix matrix = generateMatrix(32, 32, 1), 
			unsigned int render_distance = 32 * 32) :
		
			MainWorldStates(size,
				matrix,
				sf::Vector2<int>(static_cast<int>(matrix.size()), 
					static_cast<int>(matrix[0].size())),
				sf::Vector2<float>(size.x / 
					static_cast<float>(matrix.size()),
					size.y / static_cast<float>(matrix[0].size())),
				render_distance,
				window),
			ray_caster_api_(*this)
		{

		}

		void render(Camera& camera)
		{
			ray_data_ = (ray_caster_api_.RayCast(camera));
			
		}

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override
		{
			for(auto i = 0; i < ray_num_; i++)
			{
				
			}
		}

		void save(std::string name) override
		{
			
		};
		
	};


	class Camera : public FileSaveable
	{
	private:
		//FOV - Field Of View
		sf::Vector2<float> window_size_;
		sf::Vector2<float> window_size_2_;
		sf::Vector2<float> position_;
		float rotation_;
		float fov_;
		float zoom_;
		float render_constant_{};
		float wwf_;
		float shading_coefficient_;

		friend class RayCaster_api;
		
		void window_init(const sf::Vector2<unsigned> window_size)
		{
			this->window_size_.x = window_size.x;
			this->window_size_.y = window_size.y;
			this->window_size_2_.x = static_cast<float>(window_size.x) / 2.f;
			this->window_size_2_.y = static_cast<float>(window_size.y) / 2.f;
		}
		
		void change_render_constant()
		{
			this->render_constant_ = static_cast<float>(this->window_size_.x) / abs(2 * (tanf(this->fov_) * math::PI_180))
				* static_cast<float>(this->window_size_.y) * this->zoom_;
		}

	public:

		explicit Camera(const sf::RenderWindow& window) :
			window_size_(window.getSize()),
			window_size_2_(sf::Vector2<int>(window.getSize().x / 2, window.getSize().y / 2)),
			position_(sf::Vector2<float>(0.f, 0.f)),
			rotation_(0),
			fov_(60),
			zoom_(1.f),
			wwf_(window_size_.x / fov_),
			shading_coefficient_(40.f * 0.00001f)
		{
			change_render_constant();
		}

		Camera(const sf::RenderWindow& window, const sf::Vector2<float> position, const float fov,
			const float rotation, const float zoom = 4.f, const float shading_coefficient = 40.f) :
			window_size_(window.getSize()),
			window_size_2_(sf::Vector2<int>(window.getSize().x / 2, window.getSize().y / 2)),
			position_(position),
			rotation_(rotation),
			fov_(fov),
			zoom_(zoom),
			wwf_(window_size_.x / fov),
			shading_coefficient_(shading_coefficient * 0.00001f)
		{
			change_render_constant();
		}

		void setWindow(const sf::RenderWindow& window)
		{
			window_init(window.getSize());
			wwf_ = window_size_.x / fov_;
			change_render_constant();
		}

		void setShadingCoefficient(const float shading_coefficient)
		{
			shading_coefficient_ = shading_coefficient * 0.00001f;
		}

		float getShadingCoefficient() const
		{
			return shading_coefficient_;
		}

		void setRotation(const float rotation)
		{
			rotation_ = rotation;
			math::delimit_angle(this->rotation_);
		}

		float getRotation() const
		{
			return rotation_;
		}

		void rotate(const float rotation)
		{
			rotation_ += rotation;
			math::delimit_angle(this->rotation_);
		}

		void setPosition(const sf::Vector2<float> position)
		{
			position_ = position;
		}

		void setPosition(const float x, const float y)
		{
			position_.x = x;
			position_.y = y;
		}

		sf::Vector2<float> getPosition() const
		{
			return position_;
		}

		void move(const float x, const float y)
		{
			position_.x += x;
			position_.y += y;
		}

		void move(const sf::Vector2<float> position)
		{
			move(position.x, position.y);
		}

		void setFov(const float fov)
		{
			fov_ = fov;
			wwf_ = window_size_.x / fov;
			change_render_constant();
		}

		float getFov() const
		{
			return fov_;
		}

		void fov_unary(const float fov)
		{
			setFov(fov_ + fov);
		}

		void setZoom(const float zoom)
		{
			zoom_ = zoom;
			change_render_constant();
		}

		float getZoom() const
		{
			return zoom_;
		}

		void zoom_unary(const float zoom)
		{
			setZoom(zoom_ + zoom);
		}

		void save(std::string name) override
		{
			std::cout << "camera saved" << std::endl;
		};
		
	};
	
	
};
