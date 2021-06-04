#pragma once

/*
 * Пишем, что будем использовать .dll библиотеки
 * (библиотеки динамической компановки)
 */
#ifndef SFML_DYNAMIC
#define SFML_DYNAMIC
#endif

#ifndef V3D_GPU_RAYCASTING_MAX_PIXEL_DEPTH
#define V3D_GPU_RAYCASTING_MAX_PIXEL_DEPTH (((1 << 4) * (1 << 4)) * (1 << 1))
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
#pragma comment(lib, "winmm.lib")
#endif

#include <SFML/Graphics.hpp>
#include "Math.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <utility>
#include <amp.h>
#include "DataImpl.h"

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
	
	class Camera;
	class World;
	class CPUWorld;
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

	/*
	 * Короткое имя для матрицы лабиринта / мира и прочего
	 */
	typedef std::vector<std::vector<int>> Matrix;

	/*
	 * Функция, генерируящая матрицу
	 */
	static
	[[nodiscard]]
	Matrix generateMatrix(const unsigned int sizeX,
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
	//class Camera;

	/*
	 * Объявляем класс RayCaster,
	 * чтобы обозначить его как friend class в других классах.
	 */
	//class RayCaster;

	/*
	 * Объявляем класс World,
	 * чтобы обозначить его как friend class в других классах.
	 */
	//class World;

	
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

	/*
	 * Каласс для удобного проведения процедуры бросания лучей.
	 */

	 /*
  * Класс для хранения состояния луча
  */

	enum class dir
	{
		left,
		right,
		down,
		up,
		none
	};

	struct ray_data
	{
		float position_x;
		float position_y;
		float length;
		int wall_number;
		float rotation;
		dir direction;
	};
	



	class RayCaster
	{
	protected:
		const World* world_ = nullptr;
		const Camera* camera_ = nullptr;
		bool sort_enable_ = true;
		bool break_acceleration_enable_ = true;
		
		unsigned window_width_ = NULL;
		ray_data** buffer_ray_data_;
		tu::DataImpl2<ray_data> buffer_ray_data_impl2_;
		
		unsigned max_pixel_depth_ = V3D_GPU_RAYCASTING_MAX_PIXEL_DEPTH;
	
	public:	
		virtual ~RayCaster() = default;

		explicit RayCaster(const unsigned window_width, ray_data** buffer_ray_data) :
			window_width_(window_width),
			buffer_ray_data_(buffer_ray_data),
			buffer_ray_data_impl2_(tu::DataImpl2<ray_data>(buffer_ray_data_, window_width_, V3D_GPU_RAYCASTING_MAX_PIXEL_DEPTH))
		{
			
		}

		/*void setSortEnable(const bool sort_enable)
		{
			sort_enable_ = sort_enable;
		}*/

		[[nodiscard]]
		bool getSortEnable() const
		{
			return sort_enable_;
		}

		virtual void __fastcall startRayCasting
		(
			v3d::Camera* const camera,
			const World* world
		) = 0;
		
	};
	
	class CPURayCaster final:
		public RayCaster
	{

	public:

		explicit CPURayCaster(const unsigned window_width, ray_data** buffer_ray_data) :
			RayCaster(window_width, buffer_ray_data)
		{
			
		}
		
		void startRayCasting
		(
			v3d::Camera* const camera,
			const World* world
		) override
		{
			this->world_ = world;
			this->camera_ = camera;

			for (tu::rank_t i = 0; i < buffer_ray_data_impl2_.size_first; i++)
			{
				for (tu::rank_t l = 0; l < buffer_ray_data_impl2_.size_second; l++)
				{
					(*buffer_ray_data_impl2_[i][l]).length = 0;
				}
			}

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
			 * Процедура бросания лучей
			 * API 2020 year
			 */
			float angle_per_ray = camera_->fov_ /
				static_cast<float>(camera_->window_size_.x);
			for (auto r = 0; r < camera_->window_size_.x; r++)
			{
				unsigned buffer_ray_data_impl2_i = 0;
				/*
				 * Вектор состояний лучей.
				 * Для дальнейшей его сортировки
				 */
				
				 /*
				  * Угол данного луча
				  */
				const float angle = camera_->rotation_ +
					(static_cast<float>(r) * (angle_per_ray)) - camera_->fov_ / 2;

				/*
				 * Тригонометрические состояния угла
				 */
				const float cos_angle = cosf(angle);
				const float sin_angle = sinf(angle);
				const float tan_angle = tanf(angle);
				const float ctg_angle = math::ctg(angle);

				//std::cout << r << std::endl;

				if (cos_angle > 0 && sin_angle > 0)
				{
					//Ver

					const float vA = world_->wall_size_.x - (camera_
						->position_.x -
						math::getMatrixPos(camera_->position_.x,
							world_->wall_size_.x) *
						world_->wall_size_.x);
					const float vB = vA * tan_angle;
					float vcx = camera_->position_.x + vA;
					float vcy = camera_->position_.y + vB;
					int vjx = math::getMatrixPos(vcx,
						world_->wall_size_.x);
					int vjy = math::getMatrixPos(vcy,
						world_->wall_size_.y);
					if (vjx < world_->matrix_size_.x && vjx >= 0 &&
						vjy < world_->matrix_size_.y && vjy >= 0)
					{
						if (world_->matrix_[vjx][vjy] != 0)
						{
							if (buffer_ray_data_impl2_i < max_pixel_depth_)
							{
								buffer_ray_data_impl2_.get(r, buffer_ray_data_impl2_i) =
									ray_data
								{
									vcx,
									vcy,
									math::fast_hypot(vcx - camera_->position_.x,
									vcy - camera_->position_.y),
									world_->matrix_[vjx][vjy],
									angle,
									dir::left
								};
								buffer_ray_data_impl2_i++;
							}
							else
							{
								break;
							}
						}
					}
					const float vAA = world_->wall_size_.x;
					const float vBB = vAA * tan_angle;

					//Hor
					const float hB = world_->wall_size_.y - (camera_
						->position_.y -
						math::getMatrixPos(camera_->position_.y,
							world_->wall_size_.y) *
						world_->wall_size_.y);
					const float hA = hB * ctg_angle;
					float hcx = camera_->position_.x + hA;
					float hcy = camera_->position_.y + hB;
					int hjx = math::getMatrixPos(hcx,
						world_->wall_size_.x);
					int hjy = math::getMatrixPos(hcy,
						world_->wall_size_.y);
					if (hjx < world_->matrix_size_.x && hjx >= 0 &&
						hjy < world_->matrix_size_.y && hjy >= 0)
					{
						if (world_->matrix_[hjx][hjy] != 0)
						{
							if (buffer_ray_data_impl2_i < max_pixel_depth_)
							{
								buffer_ray_data_impl2_.get(r, buffer_ray_data_impl2_i) =
									ray_data
								{
									hcx,
									hcy,
									math::fast_hypot(hcx - camera_->position_.x,
									hcy - camera_->position_.y),
									world_->matrix_[hjx][hjy],
									angle,
									dir::up
								};
								buffer_ray_data_impl2_i++;
							}
							else
							{
								break;
							}
						}
					}
					const float hBB = world_->wall_size_.y;
					const float hAA = hBB * ctg_angle;
					//RAY LEN SEARCHER
					for (unsigned int i = 0; i < 4194304; i++)
					{
						vcx += vAA;
						vcy += vBB;
						vjx = math::getMatrixPos(vcx,
							world_->wall_size_.x);
						vjy = math::getMatrixPos(vcy,
							world_->wall_size_.y);
						if (vjx < world_->matrix_size_.x && vjx >= 0 &&
							vjy < world_->matrix_size_.y && vjy >= 0)
						{
							if (world_->matrix_[vjx][vjy] != 0)
							{
								if (buffer_ray_data_impl2_i < max_pixel_depth_)
								{
									buffer_ray_data_impl2_.get(r, buffer_ray_data_impl2_i) =
										ray_data
									{
										vcx,
										vcy,
										math::fast_hypot(vcx - camera_->position_.x,
										vcy - camera_->position_.y),
										world_->matrix_[vjx][vjy],
										angle,
										dir::left
									};
									buffer_ray_data_impl2_i++;

									if (!break_acceleration_enable_)
									{
										break;
									}
								}
								else
								{
									break;
								}
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
							world_->wall_size_.x);
						hjy = math::getMatrixPos(hcy,
							world_->wall_size_.y);
						if (hjx < world_->matrix_size_.x && hjx >= 0 &&
							hjy < world_->matrix_size_.y && hjy >= 0)
						{
							if (world_->matrix_[hjx][hjy] != 0)
							{
								if (buffer_ray_data_impl2_i < max_pixel_depth_)
								{
									buffer_ray_data_impl2_.get(r, buffer_ray_data_impl2_i) =
										ray_data
									{
										hcx,
										hcy,
										math::fast_hypot(hcx - camera_->position_.x,
										hcy - camera_->position_.y),
										world_->matrix_[hjx][hjy],
										angle,
										dir::up
									};
									buffer_ray_data_impl2_i++;

									if (!break_acceleration_enable_)
									{
										break;
									}
								}
								else
								{
									break;
								}
							}
						}
						else
						{
							break;
						}
					}

					if (sort_enable_)
					{
						/*std::sort
						(
							buffer_ray_data_impl2_[r].begin(),
							buffer_ray_data_impl2_[r].end(),
							[](ray_data * first, ray_data * second) -> bool
							{
								if (first->length == 0 || second->length == 0)
								{
									return false;
								}
								return first->length < second->length;
							}
						);*/
					}

				}

				else if (cos_angle < 0 && sin_angle > 0)
				{
					//Ver
					const float vA = camera_->position_.x -
						math::getMatrixPos(camera_->position_.x,
							world_->wall_size_.x) * world_
						->wall_size_.x;
					const float vB = vA * tan_angle;
					float vcx = camera_->position_.x - vA;
					float vcy = camera_->position_.y - vB;
					int vjx = math::getMatrixPos(vcx - 0.1f,
						world_->wall_size_.x);
					int vjy = math::getMatrixPos(vcy, world_
						->wall_size_.y);
					if (vjx < world_->matrix_size_.x && vjx >= 0 &&
						vjy < world_->matrix_size_.y && vjy >= 0)
					{
						if (world_->matrix_[vjx][vjy] != 0)
						{
							if (buffer_ray_data_impl2_i < max_pixel_depth_)
							{
								buffer_ray_data_impl2_.get(r, buffer_ray_data_impl2_i) =
									ray_data
								{
									vcx,
									vcy,
									math::fast_hypot(vcx - camera_->position_.x,
									vcy - camera_->position_.y),
									world_->matrix_[vjx][vjy],
									angle,
									dir::right
								};
								buffer_ray_data_impl2_i++;
							}
						}
					}
					const float vAA = world_->wall_size_.x;
					const float vBB = vAA * tan_angle;
					//Hor
					const float hB = world_->wall_size_.y - (camera_
						->position_.y -
						math::getMatrixPos(camera_->position_.y,
							world_->wall_size_.y) *
						world_->wall_size_.y);
					const float hA = hB * ctg_angle;
					float hcx = camera_->position_.x + hA;
					float hcy = camera_->position_.y + hB;
					int hjx = math::getMatrixPos(hcx,
						world_->wall_size_.x);
					int hjy = math::getMatrixPos(hcy,
						world_->wall_size_.y);
					if (hjx < world_->matrix_size_.x && hjx >= 0 &&
						hjy < world_->matrix_size_.y && hjy >= 0)
					{
						if (world_->matrix_[hjx][hjy] != 0)
						{
							if (buffer_ray_data_impl2_i < max_pixel_depth_)
							{
								buffer_ray_data_impl2_.get(r, buffer_ray_data_impl2_i) =
									ray_data
								{
									hcx,
									hcy,
									math::fast_hypot(hcx - camera_->position_.x,
									hcy - camera_->position_.y),
									world_->matrix_[hjx][hjy],
									angle,
									dir::up
								};
								buffer_ray_data_impl2_i++;
							}
							else
							{
								break;
							}
						}
					}
					const float hBB = world_->wall_size_.y;
					const float hAA = hBB * ctg_angle;
					//RAY LEN SEARCHER
					for (unsigned int i = 0; i < 4194304; i++)
					{
						vcx -= vAA;
						vcy -= vBB;
						vjx = math::getMatrixPos(vcx - 0.1f,
							world_->wall_size_.x);
						vjy = math::getMatrixPos(vcy,
							world_->wall_size_.y);
						if (vjx < world_->matrix_size_.x && vjx >= 0 &&
							vjy < world_->matrix_size_.y && vjy >= 0)
						{
							if (world_->matrix_[vjx][vjy] != 0)
							{
								if (buffer_ray_data_impl2_i < max_pixel_depth_)
								{
									buffer_ray_data_impl2_.get(r, buffer_ray_data_impl2_i) =
										ray_data
									{
										vcx,
										vcy,
										math::fast_hypot(vcx - camera_->position_.x,
										vcy - camera_->position_.y),
										world_->matrix_[vjx][vjy],
										angle,
										dir::right
									};
									buffer_ray_data_impl2_i++;

									if (!break_acceleration_enable_)
									{
										break;
									}
								}
								else
								{
									break;
								}
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
							world_->wall_size_.x);
						hjy = math::getMatrixPos(hcy,
							world_->wall_size_.y);
						if (hjx < world_->matrix_size_.x && hjx >= 0 &&
							hjy < world_->matrix_size_.y && hjy >= 0)
						{
							if (world_->matrix_[hjx][hjy] != 0)
							{
								if (buffer_ray_data_impl2_i < max_pixel_depth_)
								{
									buffer_ray_data_impl2_.get(r, buffer_ray_data_impl2_i) =
										ray_data
									{
										hcx,
										hcy,
										math::fast_hypot(hcx - camera_->position_.x,
										hcy - camera_->position_.y),
										world_->matrix_[hjx][hjy],
										angle,
										dir::up
									};
									buffer_ray_data_impl2_i++;

									if (!break_acceleration_enable_)
									{
										break;
									}
								}
								else
								{
									break;
								}
							}
						}
						else
						{
							break;
						}
					}

					if (sort_enable_)
					{
						/*std::sort
						(
							ray_data_vec.begin(),
							ray_data_vec.end(),
							[](RayData const& a, RayData const& b) -> bool
							{
								return a.length < b.length;
							}
						);*/
					}

					//t_ray_data_vec->at(r) = ray_data_vec;
				}

				else if (cos_angle > 0 && sin_angle < 0)
				{
					//Ver
					const float vA = world_->wall_size_.x - (camera_
						->position_.x -
						math::getMatrixPos(camera_->position_.x,
							world_->wall_size_.x) *
						world_->wall_size_.x);
					const float vB = vA * tan_angle;
					float vcx = camera_->position_.x + vA;
					float vcy = camera_->position_.y + vB;
					int vjx = math::getMatrixPos(vcx,
						world_->wall_size_.x);
					int vjy = math::getMatrixPos(vcy,
						world_->wall_size_.y);
					if (vjx < world_->matrix_size_.x && vjx >= 0 &&
						vjy < world_->matrix_size_.y && vjy >= 0)
					{
						if (world_->matrix_[vjx][vjy] != 0)
						{
							if (buffer_ray_data_impl2_i < max_pixel_depth_)
							{
								buffer_ray_data_impl2_.get(r, buffer_ray_data_impl2_i) =
									ray_data
								{
									vcx,
									vcy,
									math::fast_hypot(vcx - camera_->position_.x,
									vcy - camera_->position_.y),
									world_->matrix_[vjx][vjy],
									angle,
									dir::left
								};
								buffer_ray_data_impl2_i++;
							}
							else
							{
								break;
							}
						}
					}
					const float vAA = world_->wall_size_.x;
					const float vBB = vAA * tan_angle;

					//Hor
					const float hB = camera_->position_.y -
						math::getMatrixPos(camera_->position_.y,
							world_->wall_size_.y)
						* world_->wall_size_.y;
					const float hA = hB * ctg_angle;
					float hcx = camera_->position_.x - hA;
					float hcy = camera_->position_.y - hB;
					int hjx = math::getMatrixPos(hcx,
						world_->wall_size_.x);
					int hjy = math::getMatrixPos(hcy - 0.1f,
						world_->wall_size_.y);
					if (hjx < world_->matrix_size_.x && hjx >= 0 &&
						hjy < world_->matrix_size_.y && hjy >= 0)
					{
						if (world_->matrix_[hjx][hjy] != 0)
						{
							if (buffer_ray_data_impl2_i < max_pixel_depth_)
							{
								buffer_ray_data_impl2_.get(r, buffer_ray_data_impl2_i) =
									ray_data
								{
									hcx,
									hcy,
									math::fast_hypot(hcx - camera_->position_.x,
									hcy - camera_->position_.y),
									world_->matrix_[hjx][hjy],
									angle,
									dir::down
								};
								buffer_ray_data_impl2_i++;
							}
							else
							{
								break;
							}
						}
					}
					const float hBB = world_->wall_size_.y;
					const float hAA = hBB * ctg_angle;
					//RAY LEN SEARCHER
					for (unsigned int i = 0; i < 4194304; i++)
					{
						vcx += vAA;
						vcy += vBB;
						vjx = math::getMatrixPos(vcx,
							world_->wall_size_.x);
						vjy = math::getMatrixPos(vcy,
							world_->wall_size_.y);
						if (vjx < world_->matrix_size_.x && vjx >= 0 &&
							vjy < world_->matrix_size_.y && vjy >= 0)
						{
							if (world_->matrix_[vjx][vjy] != 0)
							{
								if (buffer_ray_data_impl2_i < max_pixel_depth_)
								{
									buffer_ray_data_impl2_.get(r, buffer_ray_data_impl2_i) =
										ray_data
									{
										vcx,
										vcy,
										math::fast_hypot(vcx - camera_->position_.x,
										vcy - camera_->position_.y),
										world_->matrix_[vjx][vjy],
										angle,
										dir::left
									};
									buffer_ray_data_impl2_i++;

									if (!break_acceleration_enable_)
									{
										break;
									}
								}
								else
								{
									break;
								}
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
							world_->wall_size_.x);
						hjy = math::getMatrixPos(hcy - 0.1f,
							world_->wall_size_.y);
						if (hjx < world_->matrix_size_.x && hjx >= 0 &&
							hjy < world_->matrix_size_.y && hjy >= 0)
						{
							if (world_->matrix_[hjx][hjy] != 0)
							{
								if (buffer_ray_data_impl2_i < max_pixel_depth_)
								{
									buffer_ray_data_impl2_.get(r, buffer_ray_data_impl2_i) =
										ray_data
									{
										hcx,
										hcy,
										math::fast_hypot(hcx - camera_->position_.x,
										hcy - camera_->position_.y),
										world_->matrix_[hjx][hjy],
										angle,
										dir::down
									};
									buffer_ray_data_impl2_i++;

									if (!break_acceleration_enable_)
									{
										break;
									}
								}
								else
								{
									break;
								}
							}
						}
						else
						{
							break;
						}
					}

					if (sort_enable_)
					{
						/*std::sort
						(
							ray_data_vec.begin(),
							ray_data_vec.end(),
							[](RayData const& a, RayData const& b) -> bool
							{
								return a.length < b.length;
							}
						);*/
					}

					//t_ray_data_vec->at(r) = ray_data_vec;
				}

				else if (cos_angle < 0 && sin_angle < 0)
				{
					//Ver
					const float vA = camera_->position_.x -
						math::getMatrixPos(camera_->position_.x,
							world_->wall_size_.x) *
						world_->wall_size_.x;
					const float vB = vA * tan_angle;
					float vcx = camera_->position_.x - vA;
					float vcy = camera_->position_.y - vB;
					int vjx = math::getMatrixPos(vcx - 0.1f,
						world_->wall_size_.x);
					int vjy = math::getMatrixPos(vcy,
						world_->wall_size_.y);
					if (vjx < world_->matrix_size_.x && vjx >= 0 &&
						vjy < world_->matrix_size_.y && vjy >= 0)
					{
						if (world_->matrix_[vjx][vjy] != 0)
						{
							if (buffer_ray_data_impl2_i < max_pixel_depth_)
							{
								buffer_ray_data_impl2_.get(r, buffer_ray_data_impl2_i) =
									ray_data
								{
									vcx,
									vcy,
									math::fast_hypot(vcx - camera_->position_.x,
									vcy - camera_->position_.y),
									world_->matrix_[vjx][vjy],
									angle,
									dir::right
								};
								buffer_ray_data_impl2_i++;
							}
							else
							{
								break;
							}
						}
					}
					const float vAA = world_->wall_size_.x;
					const float vBB = vAA * tan_angle;

					//Hor
					const float hB = camera_->position_.y -
						math::getMatrixPos(camera_->position_.y,
							world_->wall_size_.y) * world_->wall_size_.y;
					const float hA = hB * ctg_angle;
					float hcx = camera_->position_.x - hA;
					float hcy = camera_->position_.y - hB;
					int hjx = math::getMatrixPos(hcx,
						world_->wall_size_.x);
					int hjy = math::getMatrixPos(hcy - 0.1f,
						world_->wall_size_.y);
					if (hjx < world_->matrix_size_.x && hjx >= 0 &&
						hjy < world_->matrix_size_.y && hjy >= 0)
					{
						if (world_->matrix_[hjx][hjy] != 0)
						{
							if (buffer_ray_data_impl2_i < max_pixel_depth_)
							{
								buffer_ray_data_impl2_.get(r, buffer_ray_data_impl2_i) =
									ray_data
								{
									hcx,
									hcy,
									math::fast_hypot(hcx - camera_->position_.x,
									hcy - camera_->position_.y),
									world_->matrix_[hjx][hjy],
									angle,
									dir::down
								};
								buffer_ray_data_impl2_i++;
							}
							else
							{
								break;
							}
						}
					}
					const float hBB = world_->wall_size_.y;
					const float hAA = hBB * ctg_angle;
					//RAY LEN SEARCHER
					for (unsigned int i = 0; i < 4194304; i++)
					{
						vcx -= vAA;
						vcy -= vBB;
						vjx = math::getMatrixPos(vcx - 0.1f,
							world_->wall_size_.x);
						vjy = math::getMatrixPos(vcy,
							world_->wall_size_.y);
						if (vjx < world_->matrix_size_.x && vjx >= 0 &&
							vjy < world_->matrix_size_.y && vjy >= 0)
						{
							if (world_->matrix_[vjx][vjy] != 0)
							{
								if (buffer_ray_data_impl2_i < max_pixel_depth_)
								{
									buffer_ray_data_impl2_.get(r, buffer_ray_data_impl2_i) =
										ray_data
									{
										vcx,
										vcy,
										math::fast_hypot(vcx - camera_->position_.x,
										vcy - camera_->position_.y),
										world_->matrix_[vjx][vjy],
										angle,
										dir::right
									};
									buffer_ray_data_impl2_i++;

									if (!break_acceleration_enable_)
									{
										break;
									}
								}
								else
								{
									break;
								}
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
							world_->wall_size_.x);
						hjy = math::getMatrixPos(hcy - 0.1f,
							world_->wall_size_.y);
						if (hjx < world_->matrix_size_.x && hjx >= 0 &&
							hjy < world_->matrix_size_.y && hjy >= 0)
						{
							if (world_->matrix_[hjx][hjy] != 0)
							{
								if (buffer_ray_data_impl2_i < max_pixel_depth_)
								{
									buffer_ray_data_impl2_.get(r, buffer_ray_data_impl2_i) =
										ray_data
									{
										hcx,
										hcy,
										math::fast_hypot(hcx - camera_->position_.x,
										hcy - camera_->position_.y),
										world_->matrix_[hjx][hjy],
										angle,
										dir::down
									};
									buffer_ray_data_impl2_i++;

									if (!break_acceleration_enable_)
									{
										break;
									}
								}
								else
								{
									break;
								}
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
						/*std::sort
						(
							ray_data_vec.begin(),
							ray_data_vec.end(),
							[](RayData const& a, RayData const& b) -> bool
							{
								return a.length < b.length;
							}
						);*/
					}

					//t_ray_data_vec->at(r) = ray_data_vec;
				}
			}

			//buffer_ray_data_impl2_i = 0;

		}
		
	};

	class PARALLELGPGPUPROCRAYCASTER
	{
	private:
		concurrency::array_view<ray_data, 1> host_mem_array_pixel_depth_;
		concurrency::array_view<float, 1> host_mem_array_camera_states_;
		concurrency::array_view<float, 1> host_mem_array_world_states_;
		
	public:

		explicit PARALLELGPGPUPROCRAYCASTER(
			const unsigned window_width,
			ray_data*& data)
		:
			host_mem_array_pixel_depth_(V3D_GPU_RAYCASTING_MAX_PIXEL_DEPTH * window_width, data),
			host_mem_array_camera_states_(4),
			host_mem_array_world_states_(1)
		
		{
		}

		void update(const Camera* camera, const World* world) const
		{
			host_mem_array_camera_states_[0] = camera->position_.x;
			host_mem_array_camera_states_[1] = camera->position_.y;
			host_mem_array_camera_states_[2] = camera->fov_;
			host_mem_array_camera_states_[3] = camera->rotation_;
		}
		
		void operator()(const concurrency::index<1> idx) const restrict(amp)
		{
			host_mem_array_pixel_depth_[idx].rotation += 1;
		}

		void discard_data() const
		{
			host_mem_array_pixel_depth_.discard_data();
		}
		
		void operator ~() const
		{
			host_mem_array_pixel_depth_.discard_data();
		}
		
		void synchronize() const
		{
			host_mem_array_pixel_depth_.synchronize();
		}

		concurrency::extent<1> extent() const
		{
			return host_mem_array_pixel_depth_.extent;
		}
	};
	
	/*class GPU_RayCaster
	{
	private:
		std::thread* thread_;

		const unsigned window_width_;
		
		Camera* const camera_;
		const World* world_;

		const unsigned int pixel_depth_ = V3D_GPU_RAYCASTING_MAX_PIXEL_DEPTH;

		const unsigned int size_;

		ray_data** mem_array_pixel_depth_;

		ray_data* data_;
		
		bool stopped_ = false;

		concurrency::array<ray_data, 2>* data_ptr_;

		PARALLELGPGPUPROCRAYCASTER parallelgpgpuprocraycaster_;

	
	public:
		GPU_RayCaster(
			Camera* const camera,
			const World* world,
			const unsigned window_width
		)
			:
			thread_(nullptr),
			window_width_(window_width),
			camera_(camera),
			world_(world),
			size_(pixel_depth_* window_width_),
			mem_array_pixel_depth_(size_),
			data_(new ray_data[size_]),
			parallelgpgpuprocraycaster_(window_width_, data_)
		{
			//host_t_ray_data_vec_.discard_data();
		}

		~GPU_RayCaster()
		{
			//host_t_ray_data_vec_[1][1].rotation;
		}

		void start_gpgpu_parallel_raycasting()
		{
			
			thread_ = new std::thread
			(
				[&]()->void
				{
					
					for (int i = 0; i < size_; i++)
					{
						data_[i] = ray_data(1, 1, 1, 1, 1, dir::right);
					}

					//concurrency::array_view<RayData, 1> host_mem_array_pixel_depth(pixel_depth_* window_width_, data_);
					//concurrency::array_view<float, 1> host_mem_array_camera_states(4);
					
					while (!stopped_)
					{
						
						//host_mem_array_camera_states[0] = camera_->position_.x;
						//host_mem_array_camera_states[1] = camera_->position_.y;
						//host_mem_array_camera_states[2] = camera_->fov_;
						//host_mem_array_camera_states[3] = camera_->rotation_;

						//host_mem_array_pixel_depth.discard_data();

						parallelgpgpuprocraycaster_.update(camera_, world_);
						
						~parallelgpgpuprocraycaster_;
						
						concurrency::parallel_for_each
						(
							parallelgpgpuprocraycaster_.extent(),
							parallelgpgpuprocraycaster_
						);
						
						parallelgpgpuprocraycaster_.synchronize();
						
						Sleep(1);
					}
				}
			);
			thread_->detach();
		}

		v3d::ray_data* getDataPtr()
		{
			return this->data_;
		}
		
	};*/

	class Wall
	{
		friend class CPUWorld;
		friend class World;

	protected:

		sf::Texture texture_;

		bool is_transparent_ = true;

		bool is_illusory_ = false;

	public:

		virtual compl Wall() = default;

		explicit Wall(const sf::Texture& texture = {}) :
			texture_(texture)
		{

		}
		
		explicit Wall(const bool is_transparent, const bool is_illusory = false, const sf::Texture& texture = {}) :
			texture_(texture),
			is_transparent_(is_transparent),
			is_illusory_(is_illusory)
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

		virtual void wall_states(const ray_data& data) = 0;

		virtual void wall_states_center_ray(
			const ray_data& data,
			const sf::Vector2<float> center_ray) = 0;
	};

	/*
	 * Стена, самая обычная, без свойств, имеет номер 1
	 */
	class MainWall final: public Wall
	{
	public:

		explicit MainWall( 
			const bool is_transparent = true,
			const bool is_illusory = false,
			const _STD string& texture_path = "data/tex/wall5.png") :
			Wall(is_transparent, is_illusory)
		{
			texture_.loadFromFile(texture_path);
		}

		MainWall(
			const _STD string& texture_path) :
			Wall(true, false)
		{
			texture_.loadFromFile(texture_path);
		}

		sf::RenderTexture render_texture;

		void wall_states(const  ray_data& data) override
		{
			//this->texture_ = this->texture_;
		}

		void wall_states_center_ray(
			const  ray_data& data,
			const sf::Vector2<float> center_ray) override
		{
			//this->texture_ = this->texture_;
		}
	};

	/*
	 * Builder, Fabric pattern
	 * Конструктор для создания клиентского класса спрайта
	 */
	class Sprite
	{
		friend class CPUWorld;

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
			const _STD string& texture_path = {}) :

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
			const _STD string& texture_path = {}) :

			position_({ x, y, z }),
			size_({ width, height })
		{
			texture_.loadFromFile(texture_path);
		}

		explicit Sprite(const _STD string& texture_path = {})
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

		virtual void sprite_states(const ray_data& data) = 0;

		// Pattern builder
		virtual Sprite* build()
		{
			return this;
		}
	};

	class MainSprite final: public Sprite
	{
	public:
		MainSprite() = default;

		explicit MainSprite
		(
			const sf::Vector3<float> position = {},
			const sf::Vector2<float> size = {},
			const _STD string& texture_path = {}
		) :
			Sprite(position, size, texture_path)
		{
			
		}

		MainSprite
		(
			const float x,
			const float y,
			const float z,
			const float width,
			const float height,
			const _STD string& texture_path = {}
		) :
			Sprite({ x, y, z },
				{width, height},
				texture_path)
		{
			
		}

		void sprite_states(const ray_data& data) override
		{
			/*
			 * ...code...
			 */
		}
	};

	/*
	 * \brief Самый главный класс.
	 * Хранит все свойства псевдо-3D мира.
	 * Можно отрисовать средствами самого SFML.
	 */
	class World :
		public sf::Drawable
	{

		friend class v3d::CPURayCaster;

		friend void PARALLELGPGPUPROCRAYCASTER::update
		(
			const Camera* camera,
			const World* world
		) const;

		
	public:
		World
		(
			Camera* camera, 
			ray_data** buffer_ray_data,
			const sf::Vector2<float> size =
			sf::Vector2<float>(1024.0F, 1024.0F),
			Matrix matrix = 
			generateMatrix(32, 32, 1)
		)
			:
			size_(size),
			matrix_(matrix),
			matrix_size_({
				static_cast<int>(matrix.size()),
				static_cast<int>(matrix[0].size())
			}),
			wall_size_({
				size.x / static_cast<float>(matrix_size_.x),
				size.y / static_cast<float>(matrix_size_.y)
			}),
			camera_(camera),
			floor_color_(0, 150, 0),
			buffer_ray_data_(buffer_ray_data),
			buffer_ray_data_impl2_(
				tu::DataImpl2<ray_data>(
					buffer_ray_data_,
					camera_->window_size_.x,
					V3D_GPU_RAYCASTING_MAX_PIXEL_DEPTH))
		{

			image_png->loadFromFile("data/tex/floor1.png");
			image_pixel_dat->create(camera_->window_size_.x, camera_->window_size_.y );
		}

	protected:
		unsigned pixel_depth_max_ = 1;
		bool* multi_threading_enable_ = new bool(false);
		unsigned long long* render_step_ = new unsigned long long(0);
		sf::Vector2<float> size_;
		Matrix matrix_;
		sf::Vector2<int> matrix_size_;
		sf::Vector2<float> wall_size_;
	_STD vector<Wall*> walls_{};
		_STD vector<Sprite*> sprites_{};
		Camera* camera_ = nullptr;
		sf::Color floor_color_;
		sf::Texture background_texture_p_;
		sf::Texture* background_texture_ = nullptr;
		ray_data** buffer_ray_data_;
		tu::DataImpl2<ray_data> buffer_ray_data_impl2_;
		sf::Image *image_png = new sf::Image;
		sf::Image *image_pixel_dat = new sf::Image;


	public:

		virtual void draw(sf::RenderTarget& target,
			sf::RenderStates states) const override
		{

		}
		
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
			setPixelDepthMax(const unsigned pixel_depth_max)
		{
			pixel_depth_max_ = pixel_depth_max;
			VAILDER_3D_WORLD_API_RETURN
		}

		unsigned getPixelDepthMax() const
		{
			return pixel_depth_max_;
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

		/*template<typename... Args>
		constexpr void addWalls(Args... args)
		{
			tu::rank_t argsum = tu::argsum(args...);
			auto arr = tu::variadic_args_array<Wall*, args...>::data;
			//walls_.push_back(args);
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

		void camera_move_with_cls(
			Camera& camera,
			const float step_x,
			const float step_y) const
		{
			if (walls_[matrix_[math::getMatrixPos(((step_x > 0) ?
				(camera_->position_.x + camera_->radius_) :
				(camera_->position_.x - camera_->radius_)) + step_x,
				wall_size_.x)]
				[math::getMatrixPos(((step_y > 0) ?
					(camera_->position_.y + camera_->radius_) :
					(camera_->position_.y - camera_->radius_)),
					wall_size_.y)]]->is_illusory_)
			{
				camera_->move(step_x, 0);
			}

			if (walls_[matrix_[math::getMatrixPos(((step_x > 0) ?
				(camera_->position_.x + camera_->radius_) :
				(camera_->position_.x - camera_->radius_)),
				wall_size_.x)]
				[math::getMatrixPos(((step_y > 0) ?
					(camera_->position_.y + camera_->radius_) :
					(camera_->position_.y - camera_->radius_)) + step_y,
					wall_size_.y)]]->is_illusory_)
			{
				camera_->move(0, step_y);
			}
		}

	};
	
	class CPUWorld final :
		public World
	{
		
	public:
		explicit CPUWorld(World&& world)
		:
			World(world)
		{
			walls_.push_back(new MainWall(false, true));
			walls_.push_back(new MainWall(false, false));

			background_texture_p_.loadFromFile("data/tex/bg.jpg");
			setBackgroundTexture(&background_texture_p_);
		}

	VAILDER_3D_API_ACCESS_MODIFER_1:

	VAILDER_3D_API_ACCESS_MODIFER_2:

		void draw(sf::RenderTarget& target,
			sf::RenderStates states) const override
		{

			/*
			* Вектор использованых номеров стен
			*/
			std::vector<int> used_walls;

			/*
			 * Вектор уникальных номеров использованых номером стен
			 */
			std::vector<int> unique_walls;

			for (int i = 1; i < camera_->window_size_.x; i++)
			{
				//used_walls.push_back((*buffer_ray_data_impl2_[i][0]).wall_number);
				used_walls.push_back(buffer_ray_data_impl2_.get(i, 0).wall_number);
			}

			/*
			 * Процедура отбора уникальных номеров стен,
			 * для уменьшения количества вызовов виртуальных функций стен
			 * и их сортировка средствами STL
			 */
			std::unique_copy(
				used_walls.begin(),
				used_walls.end(),
				std::back_inserter(unique_walls)
			);

			/*
			 * Вызов виртуальных функций стен
			 */
			for (int i = 0; i < unique_walls.size(); i++)
			{
				walls_[unique_walls[i]]->wall_states(buffer_ray_data_impl2_.get(i, 0));
			}

			float d{};

			/*
			 * В зависимости от того, на какую сторону падает луч,
			 * находим столб текстуры
			 */

			switch (buffer_ray_data_impl2_.get(static_cast<int>(camera_
				->window_size_2_.x), 0).direction) {
			case dir::left:
				d = fmodf(buffer_ray_data_impl2_.get(static_cast<int>(camera_
					->window_size_2_.x), 0).position_y,
					wall_size_.y);
				break;

			case dir::right:
				d = (wall_size_.y - fmodf(buffer_ray_data_impl2_.get(static_cast<int>(camera_\
					->window_size_2_.x), 0).position_y,
					wall_size_.y));
				break;

			case dir::down:
				d = (wall_size_.x - fmodf(buffer_ray_data_impl2_.get(static_cast<int>(camera_
					->window_size_2_.x), 0).position_x,
					wall_size_.x));
				break;

			case dir::up:
				d = fmodf(buffer_ray_data_impl2_.get(static_cast<int>(camera_
					->window_size_2_.x), 0).position_x,
					wall_size_.x);
				break;

			case dir::none:
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
			walls_[buffer_ray_data_impl2_.get(static_cast<int>(camera_->window_size_2_.x), 0).wall_number]
			->wall_states_center_ray(buffer_ray_data_impl2_.get(static_cast<int>(camera_->window_size_2_.x), 0), 
				{
					static_cast<float>(d * ((static_cast<float>(walls_
					[buffer_ray_data_impl2_.get(static_cast<int>(camera_->window_size_2_.x), 0).wall_number]->texture_.getSize().x) / wall_size_.x))),
					static_cast<float>(walls_[buffer_ray_data_impl2_.get(static_cast<int>(camera_->window_size_2_.x), 0).wall_number]->texture_.getSize().y) -
					((camera_->window_size_2_.y -
						(camera_->window_delta_y_ -
							(camera_->render_constant_ / (buffer_ray_data_impl2_.get(static_cast<int>(camera_->window_size_2_.x), 0).length *
							cosf(buffer_ray_data_impl2_.get(static_cast<int>(camera_->window_size_2_.x), 0).rotation - camera_->
								rotation_))) / 2.f)) *
						(static_cast<float>(walls_
							[buffer_ray_data_impl2_.get(static_cast<int>(camera_->window_size_2_.x), 0).wall_number]->texture_.getSize().y) / (
						camera_->render_constant_ /
								(buffer_ray_data_impl2_.get(static_cast<int>(camera_->window_size_2_.x), 0).length * cosf(
								buffer_ray_data_impl2_.get(static_cast<int>(camera_->window_size_2_.x), 0).rotation - camera_->
								rotation_)))))
					});

			unique_walls.clear();

			/*
			 * Отрисовка пола
			 */
/*			sf::RectangleShape rectangle_shape_floor_;

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

				target.draw(rectangle_shape_floor_);
			}*/


			
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
				if (buffer_ray_data_impl2_.get(i, 0).rotation < 0)
				{
					a = camera_->background_repeating_fov_
						- abs(fmodf(buffer_ray_data_impl2_.get(i, 0).rotation,
							camera_->background_repeating_fov_));
				}
				else
				{
					a = abs(fmodf(buffer_ray_data_impl2_.get(i, 0).rotation,
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

				target.draw(background_sprite);
			}

			

			/*
			 * Отрисовка стен
			 */
			sf::Sprite sprite;

			//_STD cout << cos(camera_->rotation_) - sin(camera_->rotation_) << _STD endl;

			double posX = camera_->position_.x / this->wall_size_.x, posY = camera_->position_.y / this->wall_size_.y; //x and y start position
			double dirX = cos(camera_->rotation_) * camera_->fov_, dirY = sin(camera_->rotation_) * camera_->fov_; //initial direction vector
			double planeX = -sin(camera_->rotation_) * camera_->fov_ * (camera_->fov_ / 2), planeY = cos(camera_->rotation_) * camera_->fov_ * (camera_->fov_ / 2); //the 2d raycaster version of camera plane

			int window_delta_y = camera_->window_delta_y_;
			int window_size_y = camera_->window_size_.y;
			if (window_delta_y < 0)
			{

				window_delta_y = 0;
			}
			
			for (int y = window_delta_y + 1; y < window_size_y; ++y)
			{
				
				float rayDirX0 = dirX - planeX;
				float rayDirY0 = dirY - planeY;
				float rayDirX1 = dirX + planeX;
				float rayDirY1 = dirY + planeY;
				
				const int p = y - camera_->window_delta_y_;
				const float posZ = camera_->window_size_2_.y  - 250 * (1050 / 600);
				const float rowDistance = posZ / p * 2;

				float floorStepX = rowDistance * \
				(rayDirX1 - rayDirX0) / camera_->window_size_.x;
				float floorStepY = rowDistance * \
				(rayDirY1 - rayDirY0) / camera_->window_size_.x;

				float floorX = posX + rowDistance * rayDirX0;
				float floorY = posY + rowDistance * rayDirY0;
				
				for (int xx = 0; xx < camera_->window_size_.x; xx++)
				{
					// the cell coord is simply got from the integer parts of floorX and floorY
					int cellX = (int)(floorX);
					int cellY = (int)(floorY);

					// get the texture coordinate from the fractional part
					int tx = (int)(image_png->getSize().x * \
						(floorX - cellX)) % (image_png->getSize().x - 1);
					int ty = (int)(image_png->getSize().y * \
						(floorY - cellY)) % (image_png->getSize().y - 1);

					//_STD cout << floorX << _STD endl;

					floorX += floorStepX;
					floorY += floorStepY;

					image_pixel_dat->setPixel(xx, y, \
						image_png->getPixel(tx, ty));

					// choose texture and draw the pixel
					/*int checkerBoardPattern = (int(cellX + cellY)) & 1;
					int floorTexture;
					if (checkerBoardPattern == 0) floorTexture = 3;
					else floorTexture = 4;
					int ceilingTexture = 6;
					sf::Uint32 color;

					// floor
					color = texture[floorTexture][texWidth * ty + tx];
					color = (color >> 1) & 8355711; // make a bit darker
					buffer[y][x] = color;

					//ceiling (symmetrical, at screenHeight - y - 1 instead of y)
					color = texture[ceilingTexture][texWidth * ty + tx];
					color = (color >> 1) & 8355711; // make a bit darker
					buffer[screenHeight - y - 1][x] = color;*/
				}

			}

			sf::Texture texture;
			texture.create(camera_->window_size_.x, \
				camera_->window_size_.y);
			sf::Sprite spf;
			texture.update(*image_pixel_dat);
			spf.setTexture(texture);
			target.draw(spf);





















			
			
			
			for (auto i = 0; i < camera_->window_size_.x; i++)
			{

				/*
				//std::cout << "FLOOR: " << std::endl;
				//if (i == 400)
				//{
				const float y = math::ctg(i) * sin(buffer_ray_data_impl2_.get(i, 0).rotation) * 800;
				//std::cout << y << std::endl;	
				const float cos_alpha = cos(buffer_ray_data_impl2_.get(i, 0).rotation);
				const float sin_beta = sin(buffer_ray_data_impl2_.get(i, 0).rotation);
				//const float tan_theta = sin_beta / cos_alpha;
				float a_it = math::PI2 / 2 / (camera_->window_size_2_.y);
				for (int j = camera_->window_size_.x; j > camera_->window_size_2_.y; j--)
				{
					const float ctg_gamma = 1 * tan((a_it * j) + (math::PI2 - a_it * j));
					const float texture_position_y = fmodf(camera_->position_.y + ctg_gamma * sin_beta * this->wall_size_.y, wall_size_.y)
					* (texture.getSize().y / this->wall_size_.y);
					const float texture_position_x = fmodf(camera_->position_.x + ctg_gamma * cos_alpha * this->wall_size_.x, wall_size_.x)
					* (texture.getSize().x / this->wall_size_.x);
					if (texture_position_x > -1 && texture_position_x < texture.getSize().x &&
						texture_position_y > -1 && texture_position_y < texture.getSize().y &&
						j > -1 && j < camera_->window_size_.y)
					{
						spf.setTextureRect(sf::IntRect(texture_position_x,texture_position_y, 1, 1));

						if (i == 400)
						{
							//std::cout << texture_position_y << std::endl;
						}

						spf.setPosition(i, j);
						target.draw(spf);
					}
				}
				//}
				*/
				
				for (int l = 0; l < buffer_ray_data_impl2_.size_second; l++)
				{
					if (buffer_ray_data_impl2_.get(i, l).length == 0)
					{
						break;
					}
					
					for (int j = 0; j < buffer_ray_data_impl2_.size_second; j++)
					{
						if (buffer_ray_data_impl2_.get(i, j).length == 0)
						{
							break;
						}

						if (buffer_ray_data_impl2_.get(i, j).length <
							buffer_ray_data_impl2_.get(i, 1 + j).length)
						{
						
							_STD swap(*buffer_ray_data_impl2_[i][j], \
								*buffer_ray_data_impl2_[i][1 + j]);
						}
						
					}
				}

				bool other_transparent_wall = false;
				for (tu::rank_t l = buffer_ray_data_impl2_.size_second - 1; l >= 0; --l)
				{
					if (buffer_ray_data_impl2_.get(i, l).length != 0)
					{
						if (other_transparent_wall)
						{
							buffer_ray_data_impl2_.get(i, l).length = 0;
						}
						else if (!walls_[buffer_ray_data_impl2_.get(i, l).wall_number]->is_transparent_)
						{

							other_transparent_wall = true;
						}
					}

					

				}
				
				const unsigned pixel_depth_max = 
					buffer_ray_data_impl2_.size_second > pixel_depth_max_ ? \
					pixel_depth_max_ : \
					static_cast<unsigned>(buffer_ray_data_impl2_.size_second);
				for (unsigned d_i = 0; d_i <= pixel_depth_max; d_i++)
				{
					//std::cout << _d_i << std::endl;
					//const unsigned d_i = pixel_depth_max - _d_i;
					if (buffer_ray_data_impl2_.get(i, d_i).length != 0)
					{
						/*
						 * В зависимости от того, на какую сторону падает луч,
						 * находим столб текстуры
						 */

						switch (buffer_ray_data_impl2_.get(i, d_i).direction) {
						case dir::left:
							d = fmodf(buffer_ray_data_impl2_.get(i, d_i).position_y, \
								wall_size_.y);
							break;

						case dir::right:
							d = (wall_size_.y - fmodf(buffer_ray_data_impl2_.get(i, d_i).position_y, \
								wall_size_.y));
							break;

						case dir::down:
							d = (wall_size_.x - fmodf(buffer_ray_data_impl2_.get(i, d_i).position_x, \
								wall_size_.x));
							break;

						case dir::up:
							d = fmodf(buffer_ray_data_impl2_.get(i, d_i).position_x, \
								wall_size_.x);
							break;

						case dir::none:
							break;

						default:
							break;
						}

						/*
						 * Берём текстуру у стены.
						 */
						sprite.setTexture(walls_[buffer_ray_data_impl2_ \
							.get(i, d_i).wall_number]->texture_);

						/*
						 * Высота стены.
						 */
						const float wall_height = camera_->window_size_.y \
						/ ((buffer_ray_data_impl2_.get(i, d_i).length / 64) * cosf(buffer_ray_data_impl2_.get(i, d_i).rotation - camera_->rotation_));

						/*
						 * Позиция столба стены на экране по координате Y.
						 */
						const float sprite_position_y = camera_->window_delta_y_ \
							- wall_height / 2.f;

						/*
						 * Ставим столб стены на данную позицию.
						 */
						sprite.setPosition(i, sprite_position_y);

						const float tex_wall_scale = (static_cast<float> \
							(walls_[buffer_ray_data_impl2_.get(i, d_i).wall_number]->texture_.getSize().x) \
							/ wall_size_.x);

						const int texture_position_x = d * tex_wall_scale;

						sprite.setTextureRect(sf::IntRect( \
							texture_position_x, \
							0, 1, \
							walls_[buffer_ray_data_impl2_.get(i, d_i).wall_number] \
							->texture_.getSize().y)); \

						const float scale = 1.f / tex_wall_scale;

						sprite.setScale(1, wall_height / \
							static_cast<float>(walls_[buffer_ray_data_impl2_.get(i, d_i).wall_number] \
								->texture_.getSize().y)); \

						/*
						 * Определяем цвет стены,
						 * в зависимости от её расстояния до камеры.
						 */
						const sf::Uint8 wall_clr = 255.f / (1.f +
							powf(buffer_ray_data_impl2_.get(i, d_i).length, 2.f) *
							camera_->shading_coefficient_);

						sprite.setColor(sf::Color(wall_clr,
							wall_clr, wall_clr));
						/*
						 * Отрисовка данного столба
						 */
						target.draw(sprite);
					}	
				}
				

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

				/*for (auto i = 0; i < sprites_.size(); i++)
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

					//std::cout << a << std::endl;
				}
				*/
				(*render_step_)++;
				
			}
		}

		/*
		 * Передвинуть камеру,
		 * в соответствии с матрицей мира
		 */
	};

	/*
	 * Класс камеры.
	 */

	class Camera final : protected Saveable
	{
		/*
		 * Дружелюбные классы
		 */

		friend class v3d::RayCaster;
		friend class v3d::CPURayCaster;
		//friend void v3d::GPU_RayCaster::start_gpgpu_parallel_raycasting();
		friend void PARALLELGPGPUPROCRAYCASTER::update(const Camera* camera, const World* world) const;
		friend class v3d::World;
		friend class v3d::CPUWorld;

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
			__fastcall move(const float x, const float y)
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
