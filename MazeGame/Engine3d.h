#pragma once

/*
 * Пишем, что будем использовать .dll библиотеки
 * (библиотеки динамической компановки)
 */
#ifndef SFML_DYNAMIC
#define SFML_DYNAMIC
#endif

#ifdef V3D_DEVELOPER

 //ОТКРЫТЫЙ МОДИФИКАТОР ДОСТУПА
#define VAILDER_3D_API_ACCESS_MODIFER_1 public

#define VAILDER_3D_API_ACCESS_MODIFER_2 public

#else

 //ПРИВАТНЫЙ МОДИФИКАТОР ДОСТУПА
#define VAILDER_3D_API_ACCESS_MODIFER_1 private

#define VAILDER_3D_API_ACCESS_MODIFER_2 protected

#endif

/*
 * Если конфигурация решения Debug, то выбираем -d .lib файлы
 */
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

#include <SFML/Graphics.hpp>
#include "Math.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <amp.h>

 /*
  * Проверяем тип имени на тип и мени с плавающей точкой
  */
template<typename Type>
constexpr bool is_float_type =
std::is_same<Type, float>::value ||
std::is_same<Type, double>::value ||
std::is_same<Type, long double>::value;

inline float operator "" _deg(const long double deg)
{
	return math::toRad(static_cast<float>(deg));
}

/*
 * Класс Vailder's 3D
 */
class v3d
{
public:

	/*
	 * Менеджер файлов - статический класс для сохранения классов,
	 * унаследовавших абстрактный класс Saveable
	 */

	class FileManager;

	class Saveable
	{
	public:
		virtual compl Saveable() = default;

	VAILDER_3D_API_ACCESS_MODIFER_1:
		friend class FileManager;

	VAILDER_3D_API_ACCESS_MODIFER_2:

		virtual void saveToFile(const std::string& name) = 0;

		virtual void openFromFile(const std::string& name) = 0;
	};

	class FileManager
	{
	public:

		/*
		 * Читает файл и переводит его в std::map<std::string, std::string >
		 */
		static [[nodiscard]] std::map<std::string, std::string>
			ReadConfFile(const std::string& path)
		{
			std::map <std::string, std::string> output;

			std::fstream file;
			file.open(path);

			if (!file.is_open())
			{
				std::cout << "Save >> error;" << std::endl;
			}

			else
			{
				std::string time_t;
				std::string cs;

				while (std::getline(file, time_t))
				{
					cs += time_t;
				}

				std::string t;
				std::string tt;

				for (size_t i = 0; i < cs.length(); i++)
				{
					if (cs[i] != ' ')
					{
						while (cs[i] != '=')
						{
							if (cs[i] != ' ')
							{
								t += cs[i];
							}
							i++;
						}

						i++;

						while (cs[i] != ';')
						{
							if (cs[i] != ' ')
							{
								tt += cs[i];
							}
							i++;
						}

						output[t] = tt;
						t = "";
						tt = "";
					}
				}
			}

			file.close();
			return output;
		}

		/*
		 * std::map<std::string, std::string> переводит в файл
		 */
		static void WriteConfFile(const std::map<std::string,
			std::string>& input, const std::string& path)
		{
			std::fstream file;
			file.open(path);

			if (!file.is_open())
			{
				std::cout << "Open >> error" << std::endl;
			}

			std::string output;

			for (const auto& item : input)
			{
				output += item.first + "=" + item.second + ";";
			}

			file << output;
			file.close();
		}

		static void saveToFile(Saveable* saveable, const std::string& name)
		{
			saveable->saveToFile(name);
		}

		static void openFromFile(Saveable* saveable, const std::string& name)
		{
			saveable->openFromFile(name);
		}
	};

public:

	/*
	 * Короткое имя для матрицы лабиринта / мира и прочего
	 */
	typedef std::vector<std::vector<int>> Matrix;

	/*
	 * Функция, генерируящая матрицу
	 */
	static [[nodiscard]] Matrix generateMatrix(const unsigned int sizeX,
		const unsigned int sizeY, const int val)
	{
		return Matrix(sizeX, std::vector<int>(sizeY, val));
	}

	/*
	 * Статический класс для получения времени
	 */
	template<typename Time>
	class Clock
	{
		static_assert(is_float_type<Time>,
			"template's typename are not <float> or <double> or <long double>");

	public:

		/*
		 * Получить время в секундах
		 */
		static Time seconds()
		{
			return static_cast<Time>(std::clock()) /
				static_cast<Time>(CLOCKS_PER_SEC);
		}

		/*
		 * Получить время в миллисекундах
		 */
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

	/*
	 * Таймер, можно очистить время
	 */
	template<typename Time>
	class Timer
	{
		static_assert(is_float_type<Time>,
			"template's typename are not <float> or <double> or <long double>");

	VAILDER_3D_API_ACCESS_MODIFER_2:
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

		/*
		 * Сбросить время
		 */
		void clear()
		{
			d_time_ = static_cast<Time>(std::clock()) /
				static_cast<Time>CLOCKS_PER_SEC;
		}
	};

	/*
	 * Класс для удобной генерации лабиринта.
	 * SizeX - размер матрицы по оси X,
	 * SizeY - размер матрицы по оси Y,
	 * Seed - ключ генерации - любое число в пределах типа имени unsigned.
	 */
	template<const unsigned int SizeX,
		const unsigned int SizeY,
		const unsigned Seed>
		class Maze : public Saveable
	{
	public:

		unsigned int getSizeX() const
		{
			return SizeX;
		}
		unsigned int getSizeY() const
		{
			return SizeX;
		}
		unsigned int getSeed() const
		{
			return Seed;
		}

	VAILDER_3D_API_ACCESS_MODIFER_1:

		/*
		 * Развёрнутый ключ генерации, имеющий только 4 символа
		 */
		unsigned __int8 seed_[64] = {};

		Matrix matrix_;

	VAILDER_3D_API_ACCESS_MODIFER_1:

		/*
		 * Энумераторы поворота
		 */
		enum class Rot
		{
			LEFT,
			RIGHT,
			DOWN,
			UP,
			RotNull
		};

		/*
		 * Класс, который возвращает функция fnd()
		 */
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

		/*
		 * Ищет все возможные варианты поворота
		 */
		ret fnd(Matrix& matrix, std::vector<sf::Vector2<int>>& maze_way_vec)
		{
			int sdpY(0);
			int sdpX(0);

			/*
			 * Проверка прохода вверх
			 */
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
												return ret(Rot::UP,
													sf::Vector2<int>(sdpX, sdpY));
											}
										}
									}
								}
							}
						}
					}
				}

				/*
				 * Проверка прохода вниз
				 */
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
												return ret(Rot::DOWN,
													sf::Vector2<int>(sdpX, sdpY));
											}
										}
									}
								}
							}
						}
					}
				}

				/*
				 * Проверка прохода влево
				 */
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
												return ret(Rot::LEFT,
													sf::Vector2<int>(sdpX, sdpY));
											}
										}
									}
								}
							}
						}
					}
				}

				/*
				 * Проверка прохода вправо
				 */
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
												return ret(Rot::RIGHT,
													sf::Vector2<int>(sdpX, sdpY));
											}
										}
									}
								}
							}
						}
					}
				}
			}

			/*
			 * Ничего не найдено, возвращает Rot:RotNull,
			 * сообщая этим, что лабиринт сгенегирован
			 */
			return ret();
		}

	public:

		/*
		 * Функция, возвращающая лабиринт
		 */
		Matrix generate()
		{
			/*
			 * Генерируем пустую матрицу
			 */
			class Matrix matrix = generateMatrix(SizeX, SizeY, 1);

			/*
			 * Первоначальная позиция
			 */
			sf::Vector2<int> pos = sf::Vector2<int>
				(static_cast<int>(SizeX / 2), static_cast<int>(SizeY / 2));

			/*
			 * Вектор для хранения пройденного пути
			 */
			std::vector<sf::Vector2<int>> maze_way_vec;

			/*
			 * Генерация развёрнутого ключа генерации
			 */
			for (auto i = 0; i < 64; i++)
			{
				seed_[i] = static_cast<char>(math::rand(0, 3,
					Seed / (i + 1)));
			}

			maze_way_vec.push_back(pos);
			matrix[pos.x][pos.y] = 1;

			/*
			 * Цикл, осуществляющий генерацию лабиринта
			 */
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

				for (unsigned i = maze_time; i < 64; i++)
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

			matrix_ = matrix;
			return matrix_;
		}

	public:

		/*
		 * Сохранить лабиринт в файл
		 */
		void saveToFile(const std::string& name) override
		{
			std::ofstream ofstream;
			ofstream.open("data/saves/camera/" + name);
			if (!ofstream.is_open())
			{
				std::cout << "Camera >> save >> error >> -7" << std::endl;
				exit(-0);
			}
			else
			{
				ofstream.write(reinterpret_cast<char*>(&this->matrix_),
					sizeof(Matrix));
			}
			ofstream.close();
		}

		/*
		 * Открыть лабиринт из файла
		 */
		void openFromFile(const std::string& name) override
		{
			std::ifstream ifstream;
			ifstream.open("data/saves/camera/" + name);
			if (!ifstream.is_open())
			{
				std::cout << "Camera >> open >> error >> -8" << std::endl;
				exit(-6);
			}
			else
			{
				Matrix matrix;
				ifstream.read(reinterpret_cast<char*>(&matrix),
					sizeof(Matrix));
				this->matrix_ = matrix;
			}
			ifstream.close();
		}
	};

	/*
	 * Объявляем класс Camera,
	 * чтобы обозначить его как friend class в других классах.
	 */
	class Camera;

VAILDER_3D_API_ACCESS_MODIFER_1:

	/*
	 * Объявляем класс RayCaster,
	 * чтобы обозначить его как friend class в других классах.
	 */
	class RayCaster;

	/*
	 * Объявляем класс World,
	 * чтобы обозначить его как friend class в других классах.
	 */
	class World;

VAILDER_3D_API_ACCESS_MODIFER_2:

	class CameraSave
	{
	public:
		const float fov;
		const float zoom;
		const float shading_coefficient;
		const float radius;

		explicit CameraSave(const float fov = 60.f,
			const float zoom = 3.f,
			const float shading_coefficient = 40.f,
			const float radius = 16.f) :
			fov(fov),
			zoom(zoom),
			shading_coefficient(shading_coefficient),
			radius(16.f)
		{
		}
	};

public:

	/*
	 * Каласс для удобного проведения процедуры бросания лучей.
	 */
	class RayCaster
	{
	public:

		RayCaster()
		{
		}

		/*
		 * Объявляем класс RayData,
		 * чтобы обозначить его как friend class в других классах.
		 */
		class RayData;

		/*
		 * При удалении класса удаляем массив лучей
		 */
		~RayCaster()
		{
		}

		/*
		 * Энумераторы сторон стены
		 */
		enum class dir
		{
			left,
			right,
			down,
			up,
			none
		};

		/*
		 * Класс для хранения состояния луча
		 */
		class RayData
		{
		public:
			float position_x;
			float position_y;
			float length;
			unsigned int wall_number;
			float rotation;
			dir direction;

			RayData() :
				position_x(0),
				position_y(0),
				length(0),
				wall_number(0),
				rotation(0),
				direction(dir::none)
			{
			}

			RayData(const float pos_x,
				const float pos_y,
				const float len,
				const unsigned int wall_num,
				const float rotation,
				const RayCaster::dir dir) :

				position_x(pos_x),
				position_y(pos_y),
				length(len),
				wall_number(wall_num),
				rotation(rotation),
				direction(dir)
			{
			}

			/*
			 * Перегрузка / переопределение оператора.
			 * Для сравнения длин лучей
			 */
			bool operator < (const RayData& ray_data) const
			{
				if (length < ray_data.length)
				{
					return true;
				}

				return false;
			}
		};

		typedef std::vector<RayData> pixel_depth;
		typedef std::vector<pixel_depth> pixel_depth_vector;

	VAILDER_3D_API_ACCESS_MODIFER_1:

		bool sort_enable_ = true;

	public:

		void setSortEnable(const bool sort_enable)
		{
			sort_enable_ = sort_enable;
		}

		bool getSortEnable() const
		{
			return sort_enable_;
		}

		void __fastcall RayCast(
			Camera* const camera,
			const World* world,
			pixel_depth_vector* t_ray_data_vec) const
		{
			/*Условные обозначения
				up
			 ________
			|		 |
			|		 |
	  left  |		 |  right
			|________|
			   down
			*/

			/*
			 * Отношение угла обзора к количеству лучей
			 */
			float frn = camera->fov_ /
				static_cast<float>(camera->window_size_.x);

			/*
			 * Счётчик лучей
			 */
			int f = static_cast<float>(-camera->window_size_2_.x);

			/*
			 * Процедура бросания лучей
			 * Old API (2020)
			 */
			for (auto r = 0; r < camera->window_size_.x; r++)
			{
				std::vector<RayData> ray_data_vec;
				/*
				 * Вектор состояний лучей.
				 * Для дальнейшей его сортировки
				 */

				 /*
				  * Угол данного луча
				  */
				const float angle = camera->rotation_ +
					(static_cast<float>(f) * (frn));

				/*
				 * Тригонометрические состояния угла
				 */
				const float cos_angle = cosf(angle);
				const float sin_angle = sinf(angle);
				const float tan_angle = tanf(angle);
				const float ctg_angle = math::ctg(angle);

				f++;

				if (cos_angle > 0 && sin_angle > 0)
				{
					//Ver

					const float vA = world->wall_size_.x - (camera
						->position_.x -
						math::getMatrixPos(camera->position_.x,
							world->wall_size_.x) *
						world->wall_size_.x);
					const float vB = vA * tan_angle;
					float vcx = camera->position_.x + vA;
					float vcy = camera->position_.y + vB;
					int vjx = math::getMatrixPos(vcx,
						world->wall_size_.x);
					int vjy = math::getMatrixPos(vcy,
						world->wall_size_.y);
					if (vjx < world->matrix_size_.x && vjx >= 0 &&
						vjy < world->matrix_size_.y && vjy >= 0)
					{
						if (world->matrix_[vjx][vjy] != 0)
						{
							ray_data_vec.emplace_back(vcx, vcy,
								math::fast_hypot(vcx - camera->position_.x,
									vcy - camera->position_.y),
								world->matrix_[vjx][vjy], angle, dir::left);
						}
					}
					const float vAA = world->wall_size_.x;
					const float vBB = vAA * tan_angle;

					//Hor
					const float hB = world->wall_size_.y - (camera
						->position_.y -
						math::getMatrixPos(camera->position_.y,
							world->wall_size_.y) *
						world->wall_size_.y);
					const float hA = hB * ctg_angle;
					float hcx = camera->position_.x + hA;
					float hcy = camera->position_.y + hB;
					int hjx = math::getMatrixPos(hcx,
						world->wall_size_.x);
					int hjy = math::getMatrixPos(hcy,
						world->wall_size_.y);
					if (hjx < world->matrix_size_.x && hjx >= 0 &&
						hjy < world->matrix_size_.y && hjy >= 0)
					{
						if (world->matrix_[hjx][hjy] != 0)
						{
							ray_data_vec.emplace_back(hcx, hcy,
								math::fast_hypot(hcx - camera->position_.x,
									hcy - camera->position_.y),
								world->matrix_[hjx][hjy], angle, dir::up);
						}
					}
					const float hBB = world->wall_size_.y;
					const float hAA = hBB * ctg_angle;
					//RAY LEN SEARCHER
					for (unsigned int i = 0; i < 4194304; i++)
					{
						vcx += vAA;
						vcy += vBB;
						vjx = math::getMatrixPos(vcx,
							world->wall_size_.x);
						vjy = math::getMatrixPos(vcy,
							world->wall_size_.y);
						if (vjx < world->matrix_size_.x && vjx >= 0 &&
							vjy < world->matrix_size_.y && vjy >= 0)
						{
							if (world->matrix_[vjx][vjy] != 0)
							{
								ray_data_vec.emplace_back(vcx, vcy,
									math::fast_hypot(vcx - camera->position_.x,
										vcy - camera->position_.y),
									world->matrix_[vjx][vjy], angle, dir::left);
								break;
							}
						}
						else
						{
							break;
						}
					}
					for (unsigned int i = 0; i < 4194304; i++)
					{
						hcx += hAA;
						hcy += hBB;
						hjx = math::getMatrixPos(hcx,
							world->wall_size_.x);
						hjy = math::getMatrixPos(hcy,
							world->wall_size_.y);
						if (hjx < world->matrix_size_.x && hjx >= 0 &&
							hjy < world->matrix_size_.y && hjy >= 0)
						{
							if (world->matrix_[hjx][hjy] != 0)
							{
								ray_data_vec.emplace_back(hcx, hcy,
									math::fast_hypot(hcx - camera->position_.x,
										hcy - camera->position_.y),
									world->matrix_[hjx][hjy], angle, dir::up);
								break;
							}
						}
						else
						{
							break;
						}
					}

					if (sort_enable_)
					{
						std::sort
						(
							ray_data_vec.begin(),
							ray_data_vec.end(),
							[](RayData const& a, RayData const& b) -> bool
							{
								return a.length < b.length;
							}
						);
					}

					t_ray_data_vec->at(r) = ray_data_vec;
				}

				else if (cos_angle < 0 && sin_angle > 0)
				{
					//Ver
					const float vA = camera->position_.x -
						math::getMatrixPos(camera->position_.x,
							world->wall_size_.x) * world
						->wall_size_.x;
					const float vB = vA * tan_angle;
					float vcx = camera->position_.x - vA;
					float vcy = camera->position_.y - vB;
					int vjx = math::getMatrixPos(vcx - 0.1f,
						world->wall_size_.x);
					int vjy = math::getMatrixPos(vcy, world
						->wall_size_.y);
					if (vjx < world->matrix_size_.x && vjx >= 0 &&
						vjy < world->matrix_size_.y && vjy >= 0)
					{
						if (world->matrix_[vjx][vjy] != 0)
						{
							ray_data_vec.emplace_back(vcx, vcy,
								math::fast_hypot(vcx - camera->position_.x,
									vcy - camera->position_.y),
								world->matrix_[vjx][vjy], angle, dir::right);
						}
					}
					const float vAA = world->wall_size_.x;
					const float vBB = vAA * tan_angle;
					//Hor
					const float hB = world->wall_size_.y - (camera
						->position_.y -
						math::getMatrixPos(camera->position_.y,
							world->wall_size_.y) *
						world->wall_size_.y);
					const float hA = hB * ctg_angle;
					float hcx = camera->position_.x + hA;
					float hcy = camera->position_.y + hB;
					int hjx = math::getMatrixPos(hcx,
						world->wall_size_.x);
					int hjy = math::getMatrixPos(hcy,
						world->wall_size_.y);
					if (hjx < world->matrix_size_.x && hjx >= 0 &&
						hjy < world->matrix_size_.y && hjy >= 0)
					{
						if (world->matrix_[hjx][hjy] != 0)
						{
							ray_data_vec.emplace_back(hcx, hcy,
								math::fast_hypot(hcx - camera->position_.x,
									hcy - camera->position_.y),
								world->matrix_[hjx][hjy], angle, dir::up);
						}
					}
					const float hBB = world->wall_size_.y;
					const float hAA = hBB * ctg_angle;
					//RAY LEN SEARCHER
					for (unsigned int i = 0; i < 4194304; i++)
					{
						vcx -= vAA;
						vcy -= vBB;
						vjx = math::getMatrixPos(vcx - 0.1f,
							world->wall_size_.x);
						vjy = math::getMatrixPos(vcy,
							world->wall_size_.y);
						if (vjx < world->matrix_size_.x && vjx >= 0 &&
							vjy < world->matrix_size_.y && vjy >= 0)
						{
							if (world->matrix_[vjx][vjy] != 0)
							{
								ray_data_vec.emplace_back(vcx, vcy,
									math::fast_hypot(vcx - camera->position_.x,
										vcy - camera->position_.y),
									world->matrix_[vjx][vjy], angle, dir::right);
								break;
							}
						}
						else
						{
							break;
						}
					}
					for (unsigned int i = 0; i < 4194304; i++)
					{
						hcx += hAA;
						hcy += hBB;
						hjx = math::getMatrixPos(hcx,
							world->wall_size_.x);
						hjy = math::getMatrixPos(hcy,
							world->wall_size_.y);
						if (hjx < world->matrix_size_.x && hjx >= 0 &&
							hjy < world->matrix_size_.y && hjy >= 0)
						{
							if (world->matrix_[hjx][hjy] != 0)
							{
								ray_data_vec.emplace_back(hcx, hcy,
									math::fast_hypot(hcx - camera->position_.x,
										hcy - camera->position_.y),
									world->matrix_[hjx][hjy], angle, dir::up);
								break;
							}
						}
						else
						{
							break;
						}
					}

					if (sort_enable_)
					{
						std::sort
						(
							ray_data_vec.begin(),
							ray_data_vec.end(),
							[](RayData const& a, RayData const& b) -> bool
							{
								return a.length < b.length;
							}
						);
					}

					t_ray_data_vec->at(r) = ray_data_vec;
				}

				else if (cos_angle > 0 && sin_angle < 0)
				{
					//Ver
					const float vA = world->wall_size_.x - (camera
						->position_.x -
						math::getMatrixPos(camera->position_.x,
							world->wall_size_.x) *
						world->wall_size_.x);
					const float vB = vA * tan_angle;
					float vcx = camera->position_.x + vA;
					float vcy = camera->position_.y + vB;
					int vjx = math::getMatrixPos(vcx,
						world->wall_size_.x);
					int vjy = math::getMatrixPos(vcy,
						world->wall_size_.y);
					if (vjx < world->matrix_size_.x && vjx >= 0 &&
						vjy < world->matrix_size_.y && vjy >= 0)
					{
						if (world->matrix_[vjx][vjy] != 0)
						{
							ray_data_vec.emplace_back(vcx, vcy,
								math::fast_hypot(vcx - camera->position_.x,
									vcy - camera->position_.y),
								world->matrix_[vjx][vjy], angle, dir::left);
						}
					}
					const float vAA = world->wall_size_.x;
					const float vBB = vAA * tan_angle;

					//Hor
					const float hB = camera->position_.y -
						math::getMatrixPos(camera->position_.y,
							world->wall_size_.y)
						* world->wall_size_.y;
					const float hA = hB * ctg_angle;
					float hcx = camera->position_.x - hA;
					float hcy = camera->position_.y - hB;
					int hjx = math::getMatrixPos(hcx,
						world->wall_size_.x);
					int hjy = math::getMatrixPos(hcy - 0.1f,
						world->wall_size_.y);
					if (hjx < world->matrix_size_.x && hjx >= 0 &&
						hjy < world->matrix_size_.y && hjy >= 0)
					{
						if (world->matrix_[hjx][hjy] != 0)
						{
							ray_data_vec.emplace_back(hcx, hcy,
								math::fast_hypot(hcx - camera->position_.x,
									hcy - camera->position_.y),
								world->matrix_[hjx][hjy], angle, dir::down);
						}
					}
					const float hBB = world->wall_size_.y;
					const float hAA = hBB * ctg_angle;
					//RAY LEN SEARCHER
					for (unsigned int i = 0; i < 4194304; i++)
					{
						vcx += vAA;
						vcy += vBB;
						vjx = math::getMatrixPos(vcx,
							world->wall_size_.x);
						vjy = math::getMatrixPos(vcy,
							world->wall_size_.y);
						if (vjx < world->matrix_size_.x && vjx >= 0 &&
							vjy < world->matrix_size_.y && vjy >= 0)
						{
							if (world->matrix_[vjx][vjy] != 0)
							{
								ray_data_vec.emplace_back(vcx, vcy,
									math::fast_hypot(vcx - camera->position_.x,
										vcy - camera->position_.y),
									world->matrix_[vjx][vjy], angle, dir::left);
								break;
							}
						}
						else
						{
							break;
						}
					}
					for (unsigned int i = 0; i < 4194304; i++)
					{
						hcx -= hAA;
						hcy -= hBB;
						hjx = math::getMatrixPos(hcx,
							world->wall_size_.x);
						hjy = math::getMatrixPos(hcy - 0.1f,
							world->wall_size_.y);
						if (hjx < world->matrix_size_.x && hjx >= 0 &&
							hjy < world->matrix_size_.y && hjy >= 0)
						{
							if (world->matrix_[hjx][hjy] != 0)
							{
								ray_data_vec.emplace_back(hcx, hcy,
									math::fast_hypot(hcx - camera->position_.x,
										hcy - camera->position_.y),
									world->matrix_[hjx][hjy], angle, dir::down);
								break;
							}
						}
						else
						{
							break;
						}
					}

					if (sort_enable_)
					{
						std::sort
						(
							ray_data_vec.begin(),
							ray_data_vec.end(),
							[](RayData const& a, RayData const& b) -> bool
							{
								return a.length < b.length;
							}
						);
					}

					t_ray_data_vec->at(r) = ray_data_vec;
				}

				else if (cos_angle < 0 && sin_angle < 0)
				{
					//Ver
					const float vA = camera->position_.x -
						math::getMatrixPos(camera->position_.x,
							world->wall_size_.x) *
						world->wall_size_.x;
					const float vB = vA * tan_angle;
					float vcx = camera->position_.x - vA;
					float vcy = camera->position_.y - vB;
					int vjx = math::getMatrixPos(vcx - 0.1f,
						world->wall_size_.x);
					int vjy = math::getMatrixPos(vcy,
						world->wall_size_.y);
					if (vjx < world->matrix_size_.x && vjx >= 0 &&
						vjy < world->matrix_size_.y && vjy >= 0)
					{
						if (world->matrix_[vjx][vjy] != 0)
						{
							ray_data_vec.emplace_back(vcx, vcy,
								math::fast_hypot(vcx - camera->position_.x,
									vcy - camera->position_.y),
								world->matrix_[vjx][vjy], angle, dir::right);
						}
					}
					const float vAA = world->wall_size_.x;
					const float vBB = vAA * tan_angle;

					//Hor
					const float hB = camera->position_.y -
						math::getMatrixPos(camera->position_.y,
							world->wall_size_.y) * world->wall_size_.y;
					const float hA = hB * ctg_angle;
					float hcx = camera->position_.x - hA;
					float hcy = camera->position_.y - hB;
					int hjx = math::getMatrixPos(hcx,
						world->wall_size_.x);
					int hjy = math::getMatrixPos(hcy - 0.1f,
						world->wall_size_.y);
					if (hjx < world->matrix_size_.x && hjx >= 0 &&
						hjy < world->matrix_size_.y && hjy >= 0)
					{
						if (world->matrix_[hjx][hjy] != 0)
						{
							ray_data_vec.emplace_back(hcx, hcy,
								math::fast_hypot(hcx - camera->position_.x,
									hcy - camera->position_.y),
								world->matrix_[hjx][hjy], angle, dir::down);
						}
					}
					const float hBB = world->wall_size_.y;
					const float hAA = hBB * ctg_angle;
					//RAY LEN SEARCHER
					for (unsigned int i = 0; i < 4194304; i++)
					{
						vcx -= vAA;
						vcy -= vBB;
						vjx = math::getMatrixPos(vcx - 0.1f,
							world->wall_size_.x);
						vjy = math::getMatrixPos(vcy,
							world->wall_size_.y);
						if (vjx < world->matrix_size_.x && vjx >= 0 &&
							vjy < world->matrix_size_.y && vjy >= 0)
						{
							if (world->matrix_[vjx][vjy] != 0)
							{
								ray_data_vec.emplace_back(vcx, vcy,
									math::fast_hypot(vcx - camera->position_.x,
										vcy - camera->position_.y),
									world->matrix_[vjx][vjy], angle, dir::right);
								break;
							}
						}
						else
						{
							break;
						}
					}
					for (unsigned int i = 0; i < 4194304; i++)
					{
						hcx -= hAA;
						hcy -= hBB;
						hjx = math::getMatrixPos(hcx,
							world->wall_size_.x);
						hjy = math::getMatrixPos(hcy - 0.1f,
							world->wall_size_.y);
						if (hjx < world->matrix_size_.x && hjx >= 0 &&
							hjy < world->matrix_size_.y && hjy >= 0)
						{
							if (world->matrix_[hjx][hjy] != 0)
							{
								ray_data_vec.emplace_back(hcx, hcy,
									math::fast_hypot(hcx - camera->position_.x,
										hcy - camera->position_.y),
									world->matrix_[hjx][hjy], angle, dir::down);
								break;
							}
						}
						else
						{
							break;
						}
					}

					if (sort_enable_)
					{
						//SORT BUFFER
						std::sort
						(
							ray_data_vec.begin(),
							ray_data_vec.end(),
							[](RayData const& a, RayData const& b) -> bool
							{
								return a.length < b.length;
							}
						);
					}

					t_ray_data_vec->at(r) = ray_data_vec;
				}
			}
		}
	};

public:
	class Wall
	{
		friend class World;

	protected:

		sf::Texture texture_;

	public:

		virtual compl Wall() = default;

		explicit Wall(const sf::Texture& texture = {}) :
			texture_(texture)
		{
		}

		/*
		 * Передаёт приватные состояния луча.
		 */

		void setTexture(const sf::Texture& texture)
		{
			texture_ = texture;
		}

		sf::Texture getTexture() const
		{
			return texture_;
		}

		virtual void wall_states(const RayCaster::RayData& data) = 0;

		virtual void wall_states_center_ray(
			const RayCaster::RayData& data,
			const sf::Vector2<float> center_ray) = 0;
	};

	/*
	 * Стена, самая обычная, без свойств, имеет номер 1
	 */
	class MainWall : public Wall
	{
	public:

		explicit MainWall(const std::string& texture_path =
			"data/tex/wall5.png") :
			Wall()
		{
			texture_.loadFromFile(texture_path);
		}

		sf::RenderTexture render_texture;

		void wall_states(const RayCaster::RayData& data) override
		{
			this->texture_ = this->texture_;
		}

		void wall_states_center_ray(
			const RayCaster::RayData& data,
			const sf::Vector2<float> center_ray) override
		{
			this->texture_ = this->texture_;
		}
	};

	/*
	 * Builder, Fabric pattern
	 * Конструктор для создания клиентского класса спрайта
	 */
	class Sprite
	{
		friend class World;

#ifdef V3D_FUNCTIONAL

		//РЕЗУЛЬТАТ ВЫПОЛНЕНИЯ ФУНККЦИИ
#define VAILDER_3D_SPRITE_API_RESULT Sprite&

//ВОЗВРАЩАЕМОЕ ФУНКЦИИ
#define VAILDER_3D_SPRITE_API_RETURN return *this;

#else

		//РЕЗУЛЬТАТ ВЫПОЛНЕНИЯ ФУНККЦИИ
#define VAILDER_3D_SPRITE_API_RESULT void

//ВОЗВРАЩАЕМОЕ ФУНКЦИИ
#define VAILDER_3D_SPRITE_API_RETURN

#endif

	public:

		virtual compl Sprite() = default;

		explicit Sprite(
			const sf::Vector3<float> position = {},
			const sf::Vector2<float> size = {},
			const std::string& texture_path = {}) :

			position_(position),
			size_(size)
		{
			texture_.loadFromFile(texture_path);
		}

		Sprite(
			const float x,
			const float y,
			const float z,
			const float width,
			const float height,
			const std::string& texture_path = {}) :

			position_({ x, y, z }),
			size_({ width, height })
		{
			texture_.loadFromFile(texture_path);
		}

		explicit Sprite(const std::string& texture_path = {})
		{
			texture_.loadFromFile(texture_path);
		}
		
		Sprite() = default;

	VAILDER_3D_API_ACCESS_MODIFER_2:

		/*
		 * X - in the World
		 * Y - in the World
		 * Z - in pseudo 3D World - uplift
		 */
		sf::Vector3<float> position_;

		/*
		 * X - on the Window
		 * Y - on the Window
		 */
		sf::Vector2<float> size_;

		/*
		 * Texture, changeable.
		 * For virtual void sprite_states(const RayCaster::RayData& data);
		 */
		sf::Texture texture_;

	public:

		VAILDER_3D_SPRITE_API_RESULT
			setTexture(const sf::Texture& texture)
		{
			texture_ = texture;
			VAILDER_3D_SPRITE_API_RETURN
		}

		sf::Texture getTexture() const
		{
			return texture_;
		}

		VAILDER_3D_SPRITE_API_RESULT
			setPosition(const sf::Vector3<float> position)
		{
			position_ = position;
			VAILDER_3D_SPRITE_API_RETURN
		}

		VAILDER_3D_SPRITE_API_RESULT
			setPosition(const float x, const float y, const float z)
		{
			position_ = { x, y, z };
			VAILDER_3D_SPRITE_API_RETURN
		}

		sf::Vector3<float> getPosition() const
		{
			return position_;
		}

		VAILDER_3D_SPRITE_API_RESULT
			setSize(const sf::Vector2<float> size)
		{
			size_ = size;
			VAILDER_3D_SPRITE_API_RETURN
		}

		VAILDER_3D_SPRITE_API_RESULT
			setSize(const float x, const float y)
		{
			size_ = { x, y };
			VAILDER_3D_SPRITE_API_RETURN
		}

		sf::Vector2<float> getSize() const
		{
			return size_;
		}

		virtual void sprite_states(const RayCaster::RayData& data) = 0;

		// Pattern builder
		virtual Sprite* build()
		{
			return this;
		}
	};

	class MainSprite : public Sprite
	{
	public:
		MainSprite() = default;

		explicit MainSprite
		(
			const sf::Vector3<float> position = {},
			const sf::Vector2<float> size = {},
			const std::string& texture_path = {}
		) :
			Sprite(position, size, texture)
		{
			
		}

		MainSprite
		(
			const float x,
			const float y,
			const float z,
			const float width,
			const float height,
			const sf::Texture& texture = {}
		) :
			Sprite({ x, y, z }, {width, height}, texture)
		{
			
		}

		void sprite_states(const RayCaster::RayData& data) override
		{
			/*
			 * ...code...
			 */
		}
	};

	/*
	 * Самый главный класс.
	 * Хранит все свойства псевдо-3D мира.
	 * Можно отрисовать средствами самого SFMl.
	 * Подчиняется класслу FileManager
	 */
	class World :
		public sf::Drawable,
		public sf::Transformable
	{
		friend void __fastcall RayCaster::RayCast(
			Camera* const camera,
			const World* world,
			RayCaster::pixel_depth_vector* t_ray_data_vec) const;

	public:

#ifdef V3D_FUNCTIONAL

		//РЕЗУЛЬТАТ ВЫПОЛНЕНИЯ ФУНККЦИИ
#define VAILDER_3D_WORLD_API_RESULT World&

//ВОЗВРАЩАЕМОЕ ФУНКЦИИ
#define VAILDER_3D_WORLD_API_RETURN return *this;

#else

		//РЕЗУЛЬТАТ ВЫПОЛНЕНИЯ ФУНККЦИИ
#define VAILDER_3D_WORLD_API_RESULT void

//ВОЗВРАЩАЕМОЕ ФУНКЦИИ
#define VAILDER_3D_WORLD_API_RETURN

#endif

		sf::Vector2<float> getSize() const
		{
			return this->size_;
		}

		sf::Vector2<float> getWallSize() const
		{
			return this->wall_size_;
		}

		sf::Vector2<int> getMatrixSize() const
		{
			return this->matrix_size_;
		}

		Matrix getMatrix() const
		{
			return this->matrix_;
		}

		std::vector<int>& operator [] (const int& x)
		{
			return this->matrix_[x];
		}

		int& operator [] (const sf::Vector2<int>& position_matrix)
		{
			return this->matrix_[position_matrix.x][position_matrix.y];
		}

		VAILDER_3D_WORLD_API_RESULT
			setCamera(Camera* camera)
		{
			camera_ = camera;

			VAILDER_3D_WORLD_API_RETURN
		}

		VAILDER_3D_WORLD_API_RESULT
			setBackgroundTexture(sf::Texture* texture)
		{
			background_texture_ = texture;

			VAILDER_3D_WORLD_API_RETURN
		}

	VAILDER_3D_API_ACCESS_MODIFER_2:

		sf::Vector2<float> size_;

		Matrix matrix_;

		sf::Vector2<int> matrix_size_;

		sf::Vector2<float> wall_size_;

		/*
		 * Ксласс бросателя лучей
		 */
		RayCaster ray_caster_api_{};

		/*
		 * Вектор,
		 * хранящий все классы,
		 * которые унаследовали абстрактный класс Wall.
		 */
		std::vector<Wall*> walls_{};

		std::vector<Sprite*> sprites_{};

		Camera* camera_ = nullptr;

		/*
		 * Цвет пола
		 */
		sf::Color floor_color_;

		/*
		 * Текстура заднего плана
		 */
		sf::Texture background_texture_p_;

		sf::Texture* background_texture_ = nullptr;

	public:

		explicit World(sf::RenderTarget& target,
			const sf::Vector2<float> size =
			sf::Vector2<float>(1024, 1024),
			Matrix matrix = generateMatrix(32, 32, 1)) :

			size_(size),
			matrix_(matrix),
			matrix_size_({ static_cast<int>(matrix.size()),
			static_cast<int>(matrix[0].size()) }),
			wall_size_({ size.x / static_cast<float>(matrix_size_.x),
			size.y / static_cast<float>(matrix_size_.y) }),
			floor_color_(0, 150, 0)
		{
			walls_.push_back(new MainWall);
			walls_.push_back(new MainWall);

			background_texture_p_.loadFromFile("data/tex/bg.jpg");
			setBackgroundTexture(&background_texture_p_);

			ray_caster_api_.setSortEnable(true);
		}

		/*
		 * Добавление новой стены в мир
		 */
		VAILDER_3D_WORLD_API_RESULT
			addWallType(Wall* wall)
		{
			walls_.push_back(wall);

			VAILDER_3D_WORLD_API_RETURN
		}

		VAILDER_3D_WORLD_API_RESULT
			addSpriteType(Sprite* sprite)
		{
			sprites_.push_back(sprite);

			VAILDER_3D_WORLD_API_RETURN
		}

		void addWallTypes(std::initializer_list<Wall*> wall_list)
		{
			for (Wall* const& wall : wall_list)
			{
				walls_.push_back(wall);
			}
		}

		void addSpriteTypes(std::initializer_list<Sprite*> sprite_list)
		{
			for (Sprite* const& sprite : sprite_list)
			{
				sprites_.push_back(sprite);
			}
		}

		/*
		 * Перегрузка / переопределение оператора для вышеописанной функции
		 */
		World& operator << (Wall* wall)
		{
			addWallType(wall);
			return *this;
		}

		World& operator << (Sprite* sprite)
		{
			addSpriteType(sprite);
			return *this;
		}

		/*
		 * Переопределение виртуальной функции.
		 * Для отрисовки средстсвами SFML.
		 */

	VAILDER_3D_API_ACCESS_MODIFER_1:
		RayCaster::pixel_depth_vector* ray_data_ =
			new RayCaster::pixel_depth_vector;

	VAILDER_3D_API_ACCESS_MODIFER_2:

		void draw(sf::RenderTarget& target,
			sf::RenderStates states) const override
		{
			if (camera_->window_size_.x != ray_data_->size())
			{
				ray_data_->resize(camera_->window_size_.x);
			}

			ray_caster_api_.RayCast(camera_, this, ray_data_);

			/*
			* Вектор использованых номеров стен
			*/
			std::vector<unsigned int> used_walls_;

			/*
			 * Вектор уникальных номеров использованых номером стен
			 */
			std::vector<unsigned int> unique_walls_;

			for (int i = 1; i < camera_->window_size_.x; i++)
			{
				used_walls_.push_back(ray_data_->at(i)[0].wall_number);
			}

			/*
			 * Процедура отбора уникальных номеров стен,
			 * для уменьшения количества вызовов виртуальных функций стен
			 * и их сортировка средствами STL
			 */
			std::unique_copy(
				used_walls_.begin(),
				used_walls_.end(),
				std::back_inserter(unique_walls_)
			);

			/*
			 * Вызов виртуальных функций стен
			 */
			for (int i = 0; i < unique_walls_.size(); i++)
			{
				walls_[unique_walls_[i]]->wall_states(ray_data_->at(i)[0]);
			}

			float d{};

			/*
			 * В зависимости от того, на какую сторону падает луч,
			 * находим столб текстуры
			 */

			switch (ray_data_->at(static_cast<int>(camera_
				->window_size_2_.x))[0].direction) {
			case RayCaster::dir::left:
				d = fmodf(ray_data_->at(static_cast<int>(camera_
					->window_size_2_.x))[0].position_y,
					wall_size_.y);
				break;

			case RayCaster::dir::right:
				d = (wall_size_.y - fmodf(ray_data_->at(static_cast<int>(camera_
					->window_size_2_.x))[0].position_y,
					wall_size_.y));
				break;

			case RayCaster::dir::down:
				d = (wall_size_.x - fmodf(ray_data_->at(static_cast<int>(camera_
					->window_size_2_.x))[0].position_x,
					wall_size_.x));
				break;

			case RayCaster::dir::up:
				d = fmodf(ray_data_->at(static_cast<int>(camera_
					->window_size_2_.x))[0].position_x,
					wall_size_.x);
				break;

			case RayCaster::dir::none:
				break;

			default:
				break;
			}

			/*
			 * Определение позиции курсора на текстуре.
			 * Позиция конвертикуется в позицию курсора на текстуре.
			 * И вызывает виртуальную функцию классам,
			 * которые унаследовали абстрактный класс - Wall.
			 */
			walls_[ray_data_->at(static_cast<int>(camera_->window_size_2_.x))
				[0].wall_number]->wall_states_center_ray(
					ray_data_->at(static_cast<int>(camera_->window_size_2_.x))
					[0], {
					static_cast<float>(d * ((static_cast<float>(walls_
					[ray_data_->at(
							static_cast<int>(camera_->window_size_2_.x))
						[0].wall_number]->texture_.getSize().x) /
						wall_size_
						.x))),
					static_cast<float>(walls_
						[ray_data_->at(
							static_cast<int>(camera_->window_size_2_.x))
						[0].wall_number]
									   ->texture_.getSize().y) -
					((camera_->window_size_2_.y -
						(camera_->window_delta_y_ -
							(camera_->render_constant_ / (ray_data_->at(
							static_cast<int>(camera_->window_size_2_.x))
							[0].length *
							cosf(ray_data_->at(
								static_cast<int>(camera_->window_size_2_.x))
								[0].rotation - camera_->
								rotation_))) / 2.f)) *
						(static_cast<float>(walls_
							[ray_data_->
						at(static_cast<int>(camera_->window_size_2_.x))
							[0].wall_number]->texture_.getSize().y) / (
						camera_->render_constant_ /
								(ray_data_->at(
									static_cast<int>(camera_->window_size_2_.x))
									[0].length * cosf(
								ray_data_->at(
									static_cast<int>(camera_->window_size_2_.x))
										[0].rotation - camera_->
								rotation_)))))
					});

			unique_walls_.clear();

			/*
			 * Отрисовка пола
			 */
			sf::RectangleShape rectangle_shape_floor_;

			rectangle_shape_floor_.setSize({ static_cast<float>(camera_
				->window_size_.x),1 });

			const float floor_delta = camera_->window_size_.y - camera_
				->window_delta_y_;

			const float delta_color_r(static_cast<float>(floor_color_.r)
				/ floor_delta);

			const float delta_color_g(static_cast<float>(floor_color_.g)
				/ floor_delta);

			const float delta_color_b(static_cast<float>(floor_color_.b)
				/ floor_delta);

			for (auto i = camera_->window_delta_y_;
				i < camera_->window_size_.y; i++)
			{
				rectangle_shape_floor_.setPosition(0, i);

				const float it = (i - camera_->window_delta_y_);

				rectangle_shape_floor_.setFillColor(
					sf::Color(it * delta_color_r,
						it * delta_color_g, it * delta_color_b));

				target.draw(rectangle_shape_floor_,
					this->getTransform());
			}

			/*
			 * Отрисовка заднего плана
			 */
			sf::Sprite background_sprite{};
			background_sprite.setTexture(*background_texture_);

			background_sprite.setScale(1,
				(static_cast<float>(camera_->window_size_.y)
					+ camera_->window_size_2_.y) /
				static_cast<float>(background_texture_->getSize().y));

			float a;

			for (int i = 0; i < camera_->window_size_.x; i++)
			{
				if (ray_data_->at(i)[0].rotation < 0)
				{
					a = camera_->background_repeating_fov_
						- abs(fmodf(ray_data_->at(i)[0].rotation,
							camera_->background_repeating_fov_));
				}
				else
				{
					a = abs(fmodf(ray_data_->at(i)[0].rotation,
						camera_->background_repeating_fov_));
				}

				background_sprite.setPosition(sf::Vector2f(i,
					-static_cast<float>(camera_->window_size_.y)
					+ camera_->window_delta_y_ -
					camera_->window_size_2_.y));

				background_sprite.setTextureRect(
					sf::IntRect(a *
						(static_cast<float>(background_texture_->getSize().x)
							/ camera_->background_repeating_fov_),
						0, 1,
						background_texture_->getSize().y));

				target.draw(background_sprite, this->getTransform());
			}

			/*
			 * Отрисовка стен
			 */
			sf::Sprite sprite;

			for (auto i = 0; i < camera_->window_size_.x; i++)
			{
				/*
				 * В зависимости от того, на какую сторону падает луч,
				 * находим столб текстуры
				 */

				switch (ray_data_->at(i)[0].direction) {
				case RayCaster::dir::left:
					d = fmodf(ray_data_->at(i)[0].position_y,
						wall_size_.y);
					break;

				case RayCaster::dir::right:
					d = (wall_size_.y - fmodf(ray_data_->at(i)[0].position_y,
						wall_size_.y));
					break;

				case RayCaster::dir::down:
					d = (wall_size_.x - fmodf(ray_data_->at(i)[0].position_x,
						wall_size_.x));
					break;

				case RayCaster::dir::up:
					d = fmodf(ray_data_->at(i)[0].position_x,
						wall_size_.x);
					break;

				case RayCaster::dir::none:
					break;

				default:
					break;
				}

				/*
				 * Берём текстуру у стены.
				 */
				sprite.setTexture(walls_[ray_data_->at(i)[0].wall_number]->texture_);

				/*
				 * Высота стены.
				 */
				const float wall_height = camera_->render_constant_ /
					(ray_data_->at(i)[0].length * cosf(
						ray_data_->at(i)[0].rotation -
						camera_->rotation_));

				/*
				 * Позиция столба стены на экране по координате Y.
				 */
				const float sprite_position_y = camera_->window_delta_y_
					- wall_height / 2.f;

				/*
				 * Ставим столб стены на данную позицию.
				 */
				sprite.setPosition(i, sprite_position_y);

				const float tex_wall_scale = (static_cast<float>
					(walls_[ray_data_->at(i)[0].wall_number]->texture_.getSize().x)
					/ wall_size_.x);

				const int texture_position_x = d * tex_wall_scale;

				sprite.setTextureRect(sf::IntRect(
					texture_position_x,
					0, 1,
					walls_[ray_data_->at(i)[0].wall_number]
					->texture_.getSize().y));

				const float scale = 1.f / tex_wall_scale;

				sprite.setScale(1, wall_height /
					static_cast<float>(walls_[ray_data_->at(i)[0].wall_number]
						->texture_.getSize().y));

				/*
				 * Определяем цвет стены,
				 * в зависимости от её расстояния до камеры.
				 */
				const sf::Uint8 wall_clr = 255.f / (1.f +
					powf(ray_data_->at(i)[0].length, 2.f) *
					camera_->shading_coefficient_);

				sprite.setColor(sf::Color(wall_clr,
					wall_clr, wall_clr));
				/*
				 * Отрисовка данного столба
				 */
				target.draw(sprite, this->getTransform());


				
				  /*
				    ╔═══╗░░░░░░╔╗░░░░░░░░░░░░
					║╔═╗║░░░░░╔╝╚╗░░░░░░░░░░░
					║╚══╦══╦═╦╬╗╔╬══╗ ╔══╦══╦╗
					╚══╗║╔╗║╔╬╣║║║║═╣ ║╔╗║╔╗╠╣
					║╚═╝║╚╝║║║║║╚╣║═╣ ║╔╗║╚╝║║
					╚═══╣╔═╩╝╚╝╚═╩══╝ ╚╝╚╣╔═╩╝
					░░░░║║░░░░░░░░░░░░░░ ║║░░░
					░░░░╚╝░░░░░░░░░░░░░░ ╚╝░░░
					(SPRITE SCRIPT)
				   */

				for (auto i = 0; i < sprites_.size(); i++)
				{
					if (ray_data_->at(i)[0].rotation < 0)
					{
						a = camera_->background_repeating_fov_
							- abs(fmodf(ray_data_->at(i)[0].rotation,
								camera_->background_repeating_fov_));
					}
					else
					{
						a = abs(fmodf(ray_data_->at(i)[0].rotation,
							camera_->background_repeating_fov_));
					}

					std::cout << a << std::endl;
				}
				
			}
		}

		/*
		 * Передвинуть камеру,
		 * в соответствии с матрицей мира
		 */
	public:
		void camera_move_with_cls(Camera& camera, const float step_x,
			const float step_y) const
		{
			if (matrix_[math::getMatrixPos(((step_x > 0) ?
				(camera_->position_.x + camera_->radius_) :
				(camera_->position_.x - camera_->radius_)) + step_x,
				wall_size_.x)]
				[math::getMatrixPos(((step_y > 0) ?
					(camera_->position_.y + camera_->radius_) :
					(camera_->position_.y - camera_->radius_)),
					wall_size_.y)] == 0)
			{
				camera_->move(step_x, 0);
			}

			if (matrix_[math::getMatrixPos(((step_x > 0) ?
				(camera_->position_.x + camera_->radius_) :
				(camera_->position_.x - camera_->radius_)),
				wall_size_.x)]
				[math::getMatrixPos(((step_y > 0) ?
					(camera_->position_.y + camera_->radius_) :
					(camera_->position_.y - camera_->radius_)) + step_y,
					wall_size_.y)] == 0)
			{
				camera_->move(0, step_y);
			}
		}
	};

	/*
	 * Класс камеры.
	 */

	class Camera : protected Saveable
	{
		/*
		 * Дружелюбные классы
		 */

		friend class RayCaster;

		friend class World;

#ifdef V3D_FUNCTIONAL

		//РЕЗУЛЬТАТ ВЫПОЛНЕНИЯ ФУНККЦИИ
#define VAILDER_3D_CAMERA_API_RESULT Camera&

//ВОЗВРАЩАЕМОЕ ФУНКЦИИ
#define VAILDER_3D_CAMERA_API_RETURN return *this;

#else

		//РЕЗУЛЬТАТ ВЫПОЛНЕНИЯ ФУНККЦИИ
#define VAILDER_3D_CAMERA_API_RESULT void

//ВОЗВРАЩАЕМОЕ ФУНКЦИИ
#define VAILDER_3D_CAMERA_API_RETURN

#endif

	VAILDER_3D_API_ACCESS_MODIFER_1:

		/*
		 * Переопределение размеров холста
		 */
		void resizeTarget(const sf::Vector2<unsigned> target_size)
		{
			window_size_.x = static_cast<int>(target_size.x);
			window_size_.y = static_cast<int>(target_size.y);
			window_size_pi_.x = static_cast<float>(target_size.x) *
				background_repeating_fov_;
			window_size_pi_.y = static_cast<float>(target_size.y) *
				background_repeating_fov_;
			window_size_2_.x = static_cast<float>(target_size.x) / 2.0F;
			window_size_2_.y = static_cast<float>(target_size.y) / 2.0F;
		}

		/*
		* Важная константа.
		* Используется для определения размера стены
		*/
		void change_render_constant()
		{
			this->render_constant_ = static_cast<float>(this->window_size_.x)
				/ fabs(2 * (tanf(this->fov_)))
				* static_cast<float>(this->window_size_.y) * this->zoom_;
		}

	VAILDER_3D_API_ACCESS_MODIFER_1:

		/*
		 * FOV - Field Of View.
		 * FOV - Угол Обзора.
		 */
		sf::Vector2<unsigned> window_size_;
		sf::Vector2<float> window_size_pi_;
		sf::Vector2<float> window_size_2_;
		sf::Vector2<float> position_;
		float rotation_;
		float fov_;
		float zoom_;
		float render_constant_{};
		float wwf_;
		float shading_coefficient_;
		float background_repeating_fov_ = math::PI_2;

	public:

		/**
		 * \brief Создаёт камеру
		 * \param target it is sf::Window or sf::RenderTexture
		 * \param position float position of camera (x, y)
		 * \param fov угол обзора камеры
		 * \param rotation угол поворота камеры по координате X
		 * \param zoom увеличение видимости
		 * \param shading_coefficient коэффициент затенения
		 */
		explicit Camera(const sf::RenderTarget& target,
			const sf::Vector2<float> position = {},
			const float fov = 60.0_deg,
			const float rotation = 0.10_deg,
			const float zoom = 0.075F,
			const float shading_coefficient = 40.0F) :

			window_size_(target.getSize()),
			window_size_pi_({ static_cast<float>(target.getSize().x)
				* math::PI_2 ,
				static_cast<float>(target.getSize().y) * math::PI_2 }),
			window_size_2_({ static_cast<float>(target.getSize().x) / 2.0F,
				static_cast<float>(target.getSize().y) / 2 }),
			position_(position),
			rotation_(rotation),
			fov_(fov),
			zoom_(zoom),
			wwf_(static_cast<float>(window_size_.x) / fov),
			shading_coefficient_(shading_coefficient * 0.00001F)
		{
			change_render_constant();
		}

		Camera() = default;

	VAILDER_3D_API_ACCESS_MODIFER_1:
		
		float radius_ = 5;

		float window_delta_y_ = 400;

	public:

			/**
			 * \brief Переопределить окно или другой холст
			 * \param target сам холст или окно
			 * \return Если определён препроцессор V3D_FUNCTIONAL,
			 * то возвращает класс Camera, иначе ничего
			 */
			VAILDER_3D_CAMERA_API_RESULT
			setRenderTarget(const sf::RenderTarget& target)
		{
			resizeTarget(target.getSize());

			this->wwf_ = this->window_size_.x / this->fov_;

			change_render_constant();

			VAILDER_3D_CAMERA_API_RETURN
		}


		/**
		 * \brief Установить коэффициент затенения
		 * \param shading_coefficient коэффициент затенения
		 * \return Если определён препроцессор V3D_FUNCTIONAL,
		 * то возвращает класс Camera, иначе ничего
		 */
			VAILDER_3D_CAMERA_API_RESULT
			setShadingCoefficient(const float shading_coefficient)
		{
			this->shading_coefficient_ = shading_coefficient * 0.00001f;

			VAILDER_3D_CAMERA_API_RETURN
		}

		/**
		 * \brief Получить коэффициент затенения
		 * \return float коэффициент затенения
		 */
		[[nodiscard]]
		float getShadingCoefficient() const
		{
			return this->shading_coefficient_;
		}

			/**
			* \brief Установить угол растяжения заднего фона
			* на определённый угол
			* \param background_repeating_fov угол растяжения заднего фона
			* \return Если определён препроцессор V3D_FUNCTIONAL,
			* то возвращает класс Camera, иначе ничего
			*/
			VAILDER_3D_CAMERA_API_RESULT
			setBackgroundRepeatingFov(const float background_repeating_fov)
		{
			background_repeating_fov_ = background_repeating_fov;
			resizeTarget(window_size_);

			VAILDER_3D_CAMERA_API_RETURN
		}

		/**
		* \brief Получить угол растяжения заднего фона
		* \return float угол растяжения заднего фона
		*/
		[[nodiscard]]
		float getBackgroundRepeatingFov() const
		{
			return background_repeating_fov_;
		}


		/**
		* \brief Установить угол поворота камеры
		* \param rotation угол поворота камеры
		* \return Если определён препроцессор V3D_FUNCTIONAL,
		* то возвращает класс Camera, иначе ничего
		*/

		VAILDER_3D_CAMERA_API_RESULT
			setRotation(const float rotation)
		{
			this->rotation_ = rotation;

			VAILDER_3D_CAMERA_API_RETURN
		}


		/**
		* \brief Получить угол поворота камеры
		* \return float угол поворота камеры
		*/
		[[nodiscard]]
		float getRotation() const
		{
			return this->rotation_;
		}


		/**
		* \brief Повернуть камеру
		* \param rotation угол поворота камеры
		* \return Если определён препроцессор V3D_FUNCTIONAL,
		* то возвращает класс Camera, иначе ничего
		*/
		VAILDER_3D_CAMERA_API_RESULT
			rotate(const float rotation)
		{
			this->rotation_ += rotation;

			VAILDER_3D_CAMERA_API_RETURN
		}

		/**
		* \brief Установить позицию камеры
		* \param position позиция камеры
		* \return Если определён препроцессор V3D_FUNCTIONAL,
		* то возвращает класс Camera, иначе ничего
		*/
		VAILDER_3D_CAMERA_API_RESULT
			setPosition(const sf::Vector2<float> position)
		{
			this->position_ = position;

			VAILDER_3D_CAMERA_API_RETURN
		}


		/**
		* \brief Установить позицию камеры
		* \param x позиция по координате Х
		* \param y позиция по координате У
		* \return Если определён препроцессор V3D_FUNCTIONAL,
		* то возвращает класс Camera, иначе ничего
		*/
		VAILDER_3D_CAMERA_API_RESULT
			setPosition(const float x, const float y)
		{
			this->position_.x = x;
			this->position_.y = y;

			VAILDER_3D_CAMERA_API_RETURN
		}


		/**
		* \brief Получить позицию камеры
		* \return float позиция камеры
		*/

		[[nodiscard]]
		sf::Vector2<float> getPosition() const
		{
			return this->position_;
		}


		/**
		* \brief Сдвинуть камеру
		* \param x сдвиг по координате Х
		* \param y сдвиг по координате У
		* \return Если определён препроцессор V3D_FUNCTIONAL,
		* то возвращает класс Camera, иначе ничего
		*/
		VAILDER_3D_CAMERA_API_RESULT
			move(const float x, const float y)
		{
			this->position_.x += x;
			this->position_.y += y;

			VAILDER_3D_CAMERA_API_RETURN
		}


		/**
		* \brief Сдвинуть камеру
		* \param position сдвиг
		* \return Если определён препроцессор V3D_FUNCTIONAL,
		* то возвращает класс Camera, иначе ничего
		*/
		VAILDER_3D_CAMERA_API_RESULT
			move(const sf::Vector2<float> position)
		{
			move(position.x, position.y);

			VAILDER_3D_CAMERA_API_RETURN
		}


		/**
		* \brief Установить угол обзора
		* \param fov угол обзора
		* \return Если определён препроцессор V3D_FUNCTIONAL,
		* то возвращает класс Camera, иначе ничего
		*/
		VAILDER_3D_CAMERA_API_RESULT
			setFov(const float fov)
		{
			this->fov_ = fov;
			this->wwf_ = this->window_size_.x / fov_;
			change_render_constant();

			VAILDER_3D_CAMERA_API_RETURN
		}


		/**
		* \brief Получить угол обзора камеры
		* \return float угол обзора камеры
		*/
		[[nodiscard]]
		float getFov() const
		{
			return this->fov_;
		}


		/**
		* \brief увеличить угол обзора
		* \param fov угол обзора
		* \return Если определён препроцессор V3D_FUNCTIONAL,
		* то возвращает класс Camera, иначе ничего
		*/
		VAILDER_3D_CAMERA_API_RESULT
			fov_unary(const float fov)
		{
			setFov(this->fov_ + fov);

			VAILDER_3D_CAMERA_API_RETURN
		}


		/**
		* \brief Установить зум камеры
		* \param zoom зум
		* \return Если определён препроцессор V3D_FUNCTIONAL,
		* то возвращает класс Camera, иначе ничего
		*/
		VAILDER_3D_CAMERA_API_RESULT
			setZoom(const float zoom)
		{
			this->zoom_ = zoom;
			change_render_constant();

			VAILDER_3D_CAMERA_API_RETURN
		}


		/**
		* \brief Получить зум камеры
		* \return float зум камеры
		*/
		[[nodiscard]]
		float getZoom() const
		{
			return this->zoom_;
		}


		/**
		* \brief Увеличить зум камеры
		* \param zoom зум
		* \return Если определён препроцессор V3D_FUNCTIONAL,
		* то возвращает класс Camera, иначе ничего
		*/
		VAILDER_3D_CAMERA_API_RESULT
			zoom_unary(const float zoom)
		{
			setZoom(this->zoom_ + zoom);

			VAILDER_3D_CAMERA_API_RETURN
		}


		/**
		* \brief Установить радиус камеры, влияет на коллизию
		* \param radius радиус
		* \return Если определён препроцессор V3D_FUNCTIONAL,
		* то возвращает класс Camera, иначе ничего
		*/
		VAILDER_3D_CAMERA_API_RESULT
			setRadius(const float radius)
		{
			radius_ = radius;

			VAILDER_3D_CAMERA_API_RETURN
		}

		 /**
		 * \brief Получить радиус камеры
		 * \return float радиус камеры
		 */
		[[nodiscard]]
		float getRadius() const
		{
			return radius_;
		}


		/**
		* \brief Установить угол поворота камеры по оси Y
		* на экране
		* \param window_delta_y угол по оси Y
		* \return Если определён препроцессор V3D_FUNCTIONAL,
		* то возвращает класс Camera, иначе ничего
		*/
		VAILDER_3D_CAMERA_API_RESULT
			setWindowDeltaY(const float window_delta_y)
		{
			window_delta_y_ = window_delta_y;

			VAILDER_3D_CAMERA_API_RETURN
		}

		 /**
		 * \brief Получить угол поворота камеры по оси Y
		 * на экране
		 * \return float угол поворота камеры по оси Y
		 * на экране
		 */
		[[nodiscard]]
		float getWindowDeltaY() const
		{
			return window_delta_y_;
		}

		/**
		* \brief Увеличить угол поворота камеры по оси Y
		* на экране
		* \param window_delta_y угол по оси Y
		* \return Если определён препроцессор V3D_FUNCTIONAL,
		* то возвращает класс Camera, иначе ничего
		*/
		VAILDER_3D_CAMERA_API_RESULT
			windowDeltaY_unary(const float window_delta_y)
		{
			window_delta_y_ += window_delta_y;

			VAILDER_3D_CAMERA_API_RETURN
		}

	VAILDER_3D_API_ACCESS_MODIFER_2:

		/**
		 * \brief загрузить характеристики
		 * и настройки камеры
		 * в файл по адресу
		 * "data/saves/camera/
		 * в вашем проекте
		 * \param name имя файла,
		 * который размещён по адресу
		 * "data/saves/camera/
		 * в вашем проекте"
		 */
		virtual void saveToFile(const std::string& name) override
		{
			std::ofstream ofstream;
			ofstream.open("data/saves/camera/" + name);

			if (!ofstream.is_open())
			{
				std::cout << "Camera >> save >> error >> -0" << std::endl;
				exit(-0);
			}
			else
			{
				CameraSave camera_save(
					this->fov_,
					this->zoom_,
					this->shading_coefficient_,
					this->radius_);
				ofstream.write(reinterpret_cast<char*>(&camera_save),
					sizeof(CameraSave));
			}

			ofstream.close();
		}


		/**
		 * \brief загрузить характеристики
		 * и настройки камеры
		 * из файла по адресу
		 * "data/saves/camera/
		 * в вашем проекте
		 * \param name имя файла,
		 * который размещён по адресу
		 * "data/saves/camera/
		 * в вашем проекте"
		 */
		virtual void openFromFile(const std::string& name) override
		{
			std::ifstream ifstream;
			ifstream.open("data/saves/camera/" + name);

			if (!ifstream.is_open())
			{
				std::cout << "Camera >> open >> error >> -6" << std::endl;
				exit(-6);
			}
			else
			{
				CameraSave camera_save;

				ifstream.read(reinterpret_cast<char*>(&camera_save),
					sizeof(CameraSave));

				this->fov_ = camera_save.fov;
				this->shading_coefficient_ = camera_save.shading_coefficient;
				this->zoom_ = camera_save.zoom;
				this->radius_ = camera_save.radius;

				change_render_constant();

				this->wwf_ = math::scale(
					static_cast<float>(this->window_size_.x),
					this->fov_);
			}
			ifstream.close();
		}

	public:

#ifdef V3D_FUNCTIONAL

		/**
		 * \brief построить камеру,
		 * возможно использовать только если
		 * определён препроцессор V3D_FUNCTIONAL.
		 * Сама функцию относится к типу
		 * паттернов проектирования,
		 * а именно паттерну-строителю.
		 * Функция является статической
		 * \return Возвращат результат
		 * постройки камеры
		 */
		static Camera build()
		{
			return Camera();
		}
		
		/**
		 * \brief скопировать эту камеру,
		 * возможно использовать только если
		 * определён препроцессор V3D_FUNCTIONAL.
		 * Сама функцию относится к типу
		 * паттернов проектирования,
		 * а именно паттерну-строителю.
		 * Функция является статической
		 * \return Возвращат результат
		 * постройки камеры
		 */
		[[nodiscard]]
		Camera copy() const
		{
			return Camera(*this);
		}

#endif
	};
};
