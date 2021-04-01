#pragma once

/*
 * Пишем, что будем использовать .dll библиотеки
 * (библиотеки динамической компановки)
 */
#ifndef SFML_DYNAMIC
#define SFML_DYNAMIC
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

#include "include/my/binary.h"
#include <SFML/Graphics.hpp>
#include "Math.h"
#include <iostream>
#include <iomanip>

/*
 * Проверяем тип имени на тип и мени с плавающей точкой
 */
template<typename Type>
constexpr bool IsFloatType =
std::is_same<Type, float>::value ||
std::is_same<Type, double>::value ||
std::is_same<Type, long double>::value;

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
	class FileManager
	{
	public:

		/*
		 * Читает файл и переводит его в std::map<std::string, std::string >
		 */
		static std::map<std::string, std::string>
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
	};
	
public:

	/*
	 * Короткое имя для матрицы лабиринта / мира и прочего
	 */
	typedef std::vector<std::vector<int>> Matrix;

	/*
	 * Функция, генерируящая матрицу
	 */
	static Matrix generateMatrix(const unsigned int sizeX,
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
		static_assert(IsFloatType<Time>,
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
	 * Имеет возможность сохраняться с помощью класса FileManager.
	 */
	template<const unsigned int SizeX,
		const unsigned int SizeY,
	const unsigned Seed>
		class Maze
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
	
	private:

		/*
		 * Развёрнутый ключ генерации, имеющий только 4 символа
		 */
		unsigned __int8 seed_[64] = {};
		
		Matrix matrix_;

	private:
		
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
												return ret(Rot::UP, sf::Vector2<int>(sdpX, sdpY));
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
												return ret(Rot::DOWN, sf::Vector2<int>(sdpX, sdpY));
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
												return ret(Rot::LEFT, sf::Vector2<int>(sdpX, sdpY));
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
		void save(const std::string& name) const
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
				ofstream.write((char*)&this->matrix_, sizeof(Matrix));
			}
			ofstream.close();
		}

	   /*
		* Открыть лабиринт из файла
		*/
		void open(const std::string& name)
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
				ifstream.read((char*)&matrix, sizeof(Matrix));
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

private:
	
   /*
	* Объявляем класс RayCaster_api,
	* чтобы обозначить его как friend class в других классах.
	*/
	class RayCaster_api;

   /*
	* Объявляем класс World,
	* чтобы обозначить его как friend class в других классах.
	*/
	class World;

   /*
	* Объявляем класс MainWorldStates,
	* чтобы обозначить его как friend class в других классах,
	* и чтобы наследовать этот класс для быстрейшего доступа к его элементам.
	*/
	class MainWorldStates
	{
	public:
		
		MainWorldStates() :
			size_(),
			matrix_(),
			matrix_size_(),
			wall_size_(),
			render_distance_(),
			ray_num_(),
			ray_num_2_()
		{

		}
	
	protected:
		sf::Vector2<float> size_;
		Matrix matrix_;
		sf::Vector2<int> matrix_size_;
		sf::Vector2<float> wall_size_;
		unsigned int render_distance_;
		int ray_num_;
		int ray_num_2_;

		friend class RayCaster_api;

		friend class World;
		
		MainWorldStates(const sf::Vector2<float> size,
			Matrix& matrix,
			const  sf::Vector2<int> matrix_size,
			const  sf::Vector2<float> wall_size,
			const  unsigned int render_distance,
			sf::RenderTarget& target) :
		
			size_(size),
			matrix_(matrix),
			matrix_size_(matrix_size),
			wall_size_(wall_size),
			render_distance_(render_distance),
			ray_num_(target.getSize().x),
			ray_num_2_(ray_num_ / 2)
		{

		}
		
	};

   /*
	* Объявляем класс MainCameraStates,
	* чтобы обозначить его как friend class в других классах,
	* и чтобы наследовать этот класс для быстрейшего доступа к его элементам.
	*/
private:
	
	class MainCameraStates
	{

		friend class World;
		
	protected:

		/*
		 * Переопределение размеров холста
		 */
		void resizeTarget(const sf::Vector2<unsigned> target_size)
		{
			this->window_size_.x = static_cast<float>(target_size.x);
			this->window_size_.y = static_cast<float>(target_size.y);
			this->window_size_2_.x = static_cast<float>(target_size.x) / 2.f;
			this->window_size_2_.y = static_cast<float>(target_size.y) / 2.f;
		}

		void change_render_constant()
		{
			/*
			 * Важная константа.
			 * Используется для определения размера стены
			 */
			this->render_constant_ = static_cast<float>(this->window_size_.x)
			/ abs(2 * (tanf(this->fov_) * math::PI_180))
			* static_cast<float>(this->window_size_.y) * this->zoom_;
		}

	protected:
		/*
		 * FOV - Field Of View.
		 * FOV - Угол Обзора.
		 */
		sf::Vector2<float> window_size_;
		sf::Vector2<float> window_size_2_;
		sf::Vector2<float> position_;
		float rotation_;
		float fov_;
		float zoom_;
		float render_constant_{};
		float wwf_;
		float shading_coefficient_;

	protected:
		MainCameraStates(const sf::RenderTarget& target,
			const sf::Vector2<float> position = sf::Vector2<float>(),
			const float fov = 60,
			const float rotation = 0, 
			const float zoom = 4.f, 
			const float shading_coefficient = 40.f) :
		
			window_size_(target.getSize()),
			window_size_2_(sf::Vector2<int>(target.getSize().x / 2, target.getSize().y / 2)),
			position_(position),
			rotation_(math::toRad(rotation)),
			fov_(math::toRad(fov)),
			zoom_(zoom),
			wwf_(window_size_.x / fov),
			shading_coefficient_(shading_coefficient * 0.00001f)
		{
			change_render_constant();
		}
	};

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
	class RayCaster_api : public MainWorldStates
	{
	public:

	   /*
		* Объявляем класс RayData,
		* чтобы обозначить его как friend class в других классах.
		*/
		class RayData;

		/*
		 * Динамический массив для
		 * бытрого доступа к состоянию определённого луча.
		 */
		RayData* t_ray_data_vec{};

		RayCaster_api(const MainWorldStates& states) :
			MainWorldStates(states),
		/*
		 * Выделяем место в оперативной памяти
		 */
			t_ray_data_vec(static_cast<RayData*>(malloc(
				(states.ray_num_) * sizeof(RayData))))
		{

		}

		/*
		 * При удалении класса удаляем массив лучей
		 */
		~RayCaster_api()
		{
			free(t_ray_data_vec);
		}

		/*
		 * В случае перехода на другой холст, обновляем состояния
		 */
		void ResetStates(const MainWorldStates& states)
		{
			*this = states;
			
			/*
			 * Переопределения размера динамического массива состояний лучей
			 */
			if (realloc(t_ray_data_vec, (states.ray_num_) *
				sizeof(RayData)) == NULL)
			{
				/*
				 * В случае неудачи выходим с кодом ошибки -4
				 */
				exit(-4);
			}
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
				const RayCaster_api::dir dir) :

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

		/*
		 * Возврацает состояния все лучей в виде массива
		 */
		RayData* RayCast(Camera& camera)
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
			float frn = camera.fov_ / static_cast<float>(ray_num_);

			/*
			 * Счётчик лучей
			 */
			int f = static_cast<float>(-ray_num_2_);

			/*
			 * Процедура бросания лучей
			 */
			for (auto r = 0; r < ray_num_; r++)
			{

				/*
				 * Вектор состояний лучей.
				 * Для дальнейшей его сортировки
				 */
				std::vector<RayData> ray_data_vec;

				/*
				 * Угол данного луча
				 */
				const float angle = camera.rotation_ + (f * (frn));

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

					const float vA = wall_size_.x - (camera.position_.x - 
						math::getMatrixPos(camera.position_.x, 
							wall_size_.x) * wall_size_.x);
					const float vB = vA * tan_angle;
					float vcx = camera.position_.x + vA;
					float vcy = camera.position_.y + vB;
					int vjx = math::getMatrixPos(vcx, wall_size_.x);
					int vjy = math::getMatrixPos(vcy, wall_size_.y);
					if (vjx < matrix_size_.x && vjx >= 0 &&
						vjy < matrix_size_.y && vjy >= 0)
					{
						if (matrix_[vjx][vjy] != 0)
						{
							ray_data_vec.emplace_back(vcx, vcy, 
								math::fast_hypot(vcx - camera.position_.x, 
									vcy - camera.position_.y), 
								matrix_[vjx][vjy], angle, dir::left);
						}
					}
					const float vAA = wall_size_.x;
					const float vBB = vAA * tan_angle;

					//Hor
					const float hB = wall_size_.y - (camera.position_.y -
						math::getMatrixPos(camera.position_.y,
							wall_size_.y) * wall_size_.y);
					const float hA = hB * ctg_angle;
					float hcx = camera.position_.x + hA;
					float hcy = camera.position_.y + hB;
					int hjx = math::getMatrixPos(hcx, wall_size_.x);
					int hjy = math::getMatrixPos(hcy, wall_size_.y);
					if (hjx < matrix_size_.x && hjx >= 0 &&
						hjy < matrix_size_.y && hjy >= 0)
					{
						if (matrix_[hjx][hjy] != 0)
						{
							ray_data_vec.emplace_back(hcx, hcy,
								math::fast_hypot(hcx - camera.position_.x,
									hcy - camera.position_.y),
								matrix_[hjx][hjy], angle, dir::up);
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
						if (vjx < matrix_size_.x && vjx >= 0 &&
							vjy < matrix_size_.y && vjy >= 0)
						{
							if (matrix_[vjx][vjy] != 0)
							{
								ray_data_vec.emplace_back(vcx, vcy,
									math::fast_hypot(vcx - camera.position_.x,
										vcy - camera.position_.y),
									matrix_[vjx][vjy], angle, dir::left);
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
						if (hjx < matrix_size_.x && hjx >= 0 &&
							hjy < matrix_size_.y && hjy >= 0)
						{
							if (matrix_[hjx][hjy] != 0)
							{
								ray_data_vec.emplace_back(hcx, hcy,
									math::fast_hypot(hcx - camera.position_.x,
										hcy - camera.position_.y),
									matrix_[hjx][hjy], angle, dir::up);
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
							return a.length < b.length;
						}
					);
					t_ray_data_vec[r] = ray_data_vec[0];
				}

				else if (cos_angle < 0 && sin_angle > 0)
				{
					//Ver
					const float vA = camera.position_.x - 
						math::getMatrixPos(camera.position_.x,
							wall_size_.x) * wall_size_.x;
					const float vB = vA * tan_angle;
					float vcx = camera.position_.x - vA;
					float vcy = camera.position_.y - vB;
					int vjx = math::getMatrixPos(vcx - 0.1f,
						wall_size_.x);
					int vjy = math::getMatrixPos(vcy, wall_size_.y);
					if (vjx < matrix_size_.x && vjx >= 0 &&
						vjy < matrix_size_.y && vjy >= 0)
					{
						if (matrix_[vjx][vjy] != 0)
						{
							ray_data_vec.emplace_back(vcx, vcy,
								math::fast_hypot(vcx - camera.position_.x,
									vcy - camera.position_.y),
								matrix_[vjx][vjy], angle, dir::right);
						}
					}
					const float vAA = wall_size_.x;
					const float vBB = vAA * tan_angle;
					//Hor
					const float hB = wall_size_.y - (camera.position_.y -
						math::getMatrixPos(camera.position_.y,
							wall_size_.y) * wall_size_.y);
					const float hA = hB * ctg_angle;
					float hcx = camera.position_.x + hA;
					float hcy = camera.position_.y + hB;
					int hjx = math::getMatrixPos(hcx, wall_size_.x);
					int hjy = math::getMatrixPos(hcy, wall_size_.y);
					if (hjx < matrix_size_.x && hjx >= 0 &&
						hjy < matrix_size_.y && hjy >= 0)
					{
						if (matrix_[hjx][hjy] != 0)
						{
							ray_data_vec.emplace_back(hcx, hcy,
								math::fast_hypot(hcx - camera.position_.x,
									hcy - camera.position_.y), 
								matrix_[hjx][hjy], angle, dir::up);
						}
					}
					const float hBB = wall_size_.y;
					const float hAA = hBB * ctg_angle;
					//RAY LEN SEARCHER
					for (unsigned int i = 0; i < render_distance_; i++)
					{
						vcx -= vAA;
						vcy -= vBB;
						vjx = math::getMatrixPos(vcx - 0.1f,
							wall_size_.x);
						vjy = math::getMatrixPos(vcy, wall_size_.y);
						if (vjx < matrix_size_.x && vjx >= 0 && 
							vjy < matrix_size_.y && vjy >= 0)
						{
							if (matrix_[vjx][vjy] != 0)
							{
								ray_data_vec.emplace_back(vcx, vcy,
									math::fast_hypot(vcx - camera.position_.x,
										vcy - camera.position_.y),
									matrix_[vjx][vjy], angle, dir::right);
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
						if (hjx < matrix_size_.x && hjx >= 0 && 
							hjy < matrix_size_.y && hjy >= 0)
						{
							if (matrix_[hjx][hjy] != 0)
							{
								ray_data_vec.emplace_back(hcx, hcy,
									math::fast_hypot(hcx - camera.position_.x,
										hcy - camera.position_.y), 
									matrix_[hjx][hjy], angle, dir::up);
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
							return a.length < b.length;
						}
					);

					t_ray_data_vec[r] = ray_data_vec[0];

				}

				else if (cos_angle > 0 && sin_angle < 0)
				{
					//Ver
					const float vA = wall_size_.x - (camera.position_.x -
						math::getMatrixPos(camera.position_.x, 
							wall_size_.x) * wall_size_.x);
					const float vB = vA * tan_angle;
					float vcx = camera.position_.x + vA;
					float vcy = camera.position_.y + vB;
					int vjx = math::getMatrixPos(vcx, wall_size_.x);
					int vjy = math::getMatrixPos(vcy, wall_size_.y);
					if (vjx < matrix_size_.x && vjx >= 0 &&
						vjy < matrix_size_.y && vjy >= 0)
					{
						if (matrix_[vjx][vjy] != 0)
						{
							ray_data_vec.emplace_back(vcx, vcy,
								math::fast_hypot(vcx - camera.position_.x,
									vcy - camera.position_.y),
								matrix_[vjx][vjy], angle, dir::left);
						}
					}
					const float vAA = wall_size_.x;
					const float vBB = vAA * tan_angle;

					//Hor
					const float hB = camera.position_.y -
						math::getMatrixPos(camera.position_.y,
							wall_size_.y) * wall_size_.y;
					const float hA = hB * ctg_angle;
					float hcx = camera.position_.x - hA;
					float hcy = camera.position_.y - hB;
					int hjx = math::getMatrixPos(hcx, wall_size_.x);
					int hjy = math::getMatrixPos(hcy - 0.1f,
						wall_size_.y);
					if (hjx < matrix_size_.x && hjx >= 0 &&
						hjy < matrix_size_.y && hjy >= 0)
					{
						if (matrix_[hjx][hjy] != 0)
						{
							ray_data_vec.emplace_back(hcx, hcy, 
								math::fast_hypot(hcx - camera.position_.x,
									hcy - camera.position_.y),
								matrix_[hjx][hjy], angle, dir::down);
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
						if (vjx < matrix_size_.x && vjx >= 0 &&
							vjy < matrix_size_.y && vjy >= 0)
						{
							if (matrix_[vjx][vjy] != 0)
							{
								ray_data_vec.emplace_back(vcx, vcy,
									math::fast_hypot(vcx - camera.position_.x,
										vcy - camera.position_.y),
									matrix_[vjx][vjy], angle, dir::left);
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
						if (hjx < matrix_size_.x && hjx >= 0 &&
							hjy < matrix_size_.y && hjy >= 0)
						{
							if (matrix_[hjx][hjy] != 0)
							{
								ray_data_vec.emplace_back(hcx, hcy,
									math::fast_hypot(hcx - camera.position_.x,
										hcy - camera.position_.y),
									matrix_[hjx][hjy], angle, dir::down);
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
							return a.length < b.length;
						}
					);

					t_ray_data_vec[r] = ray_data_vec[0];

				}

				else if (cos_angle < 0 && sin_angle < 0)
				{
					//Ver
					const float vA = camera.position_.x -
						math::getMatrixPos(camera.position_.x,
							wall_size_.x) * wall_size_.x;
					const float vB = vA * tan_angle;
					float vcx = camera.position_.x - vA;
					float vcy = camera.position_.y - vB;
					int vjx = math::getMatrixPos(vcx - 0.1f,
						wall_size_.x);
					int vjy = math::getMatrixPos(vcy, wall_size_.y);
					if (vjx < matrix_size_.x && vjx >= 0 &&
						vjy < matrix_size_.y && vjy >= 0)
					{
						if (matrix_[vjx][vjy] != 0)
						{
							ray_data_vec.emplace_back(vcx, vcy,
								math::fast_hypot(vcx - camera.position_.x, 
									vcy - camera.position_.y), 
								matrix_[vjx][vjy], angle, dir::right);
						}
					}
					const float vAA = wall_size_.x;
					const float vBB = vAA * tan_angle;

					//Hor
					const float hB = camera.position_.y -
						math::getMatrixPos(camera.position_.y,
							wall_size_.y) * wall_size_.y;
					const float hA = hB * ctg_angle;
					float hcx = camera.position_.x - hA;
					float hcy = camera.position_.y - hB;
					int hjx = math::getMatrixPos(hcx, wall_size_.x);
					int hjy = math::getMatrixPos(hcy - 0.1f,
						wall_size_.y);
					if (hjx < matrix_size_.x && hjx >= 0 && 
						hjy < matrix_size_.y && hjy >= 0)
					{
						if (matrix_[hjx][hjy] != 0)
						{
							ray_data_vec.emplace_back(hcx, hcy, 
								math::fast_hypot(hcx - camera.position_.x, 
									hcy - camera.position_.y), 
								matrix_[hjx][hjy], angle, dir::down);
						}
					}
					const float hBB = wall_size_.y;
					const float hAA = hBB * ctg_angle;
					//RAY LEN SEARCHER
					for (unsigned int i = 0; i < render_distance_; i++)
					{
						vcx -= vAA;
						vcy -= vBB;
						vjx = math::getMatrixPos(vcx - 0.1f, 
							wall_size_.x);
						vjy = math::getMatrixPos(vcy, wall_size_.y);
						if (vjx < matrix_size_.x && vjx >= 0 &&
							vjy < matrix_size_.y && vjy >= 0)
						{
							if (matrix_[vjx][vjy] != 0)
							{
								ray_data_vec.emplace_back(vcx, vcy,
									math::fast_hypot(vcx - camera.position_.x, 
										vcy - camera.position_.y), 
									matrix_[vjx][vjy], angle, dir::right);
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
						hjy = math::getMatrixPos(hcy - 0.1f,
							wall_size_.y);
						if (hjx < matrix_size_.x && hjx >= 0 &&
							hjy < matrix_size_.y && hjy >= 0)
						{
							if (matrix_[hjx][hjy] != 0)
							{
								ray_data_vec.emplace_back(hcx, hcy,
									math::fast_hypot(hcx - camera.position_.x,
										hcy - camera.position_.y),
									matrix_[hjx][hjy], angle, dir::down);
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
							return a.length < b.length;
						}
					);
					t_ray_data_vec[r] = ray_data_vec[0];
				}

			}

			return t_ray_data_vec;

		}

	};

public:

	/*
	 * Абстрактный класс - держит связь между движком и вашим классом,
	 * который унаследовал Wall_api.
	 */
	class Wall_api
	{
	public:

		sf::Texture texture;

		virtual ~Wall_api()
		{
			std::cout << "wall api deleted" << std::endl;
		}

		/*
		 * Передаём приватные состояния луча.
		 */
		virtual void wall_states(RayCaster_api::RayData& data) = 0;

		Wall_api() = default;
	};

	/*
	 * Стена, самая обычная, без свойств, имеет номер 1
	 */
	class MainWall : public Wall_api
	{
	public:

		MainWall() :
			Wall_api()
		{
			texture.loadFromFile("data/tex/4.png");
		}

		sf::RenderTexture render_texture;

		void wall_states(RayCaster_api::RayData& data) override
		{

		}

	};

	/*
	 * Самый главный класс.
	 * Хранит все свойства псевдо-3D мира.
	 * Можно отрисовать средствами самого SFMl.
	 * Подчиняется класслу FileManager
	 */
	class World :
		private MainWorldStates,
		public sf::Drawable
	{
	public:
		
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

		std::vector<int> operator [] (const int x)
		{
			return this->matrix_[x];
		}

		int operator [] (const sf::Vector2<int>& position_matrix)
		{
			return this->matrix_[position_matrix.x][position_matrix.y];
		}
	
	private:
		
		/*
		 * Ксласс бросателя лучей
		 */
		RayCaster_api ray_caster_api_;

		RayCaster_api::RayData* ray_data_{};

	   /*
		* Вектор,
		* хранящий все классы,
		* которые унаследовали абстрактный класс Wall_api.
		*/
		std::vector<Wall_api* > wall_{};

		sf::Texture* texture_{};

		MainCameraStates camera_states_;

		/*
		 * Вектор использованых номеров стен
		 */
		std::vector<unsigned int> used_walls_{};

		/*
		 * Вектор уникальных номеров использованых номером стен
		 */
		std::vector<unsigned int> unique_walls_{};

		/*
		 * Цвет пола
		 */
		sf::Color floor_color_;

		/*
		 * Текстура заднего плана
		 */
		sf::Texture background_texture_;

	public:
		
		World(sf::RenderTarget& target,
			const sf::Vector2<float> size =
			sf::Vector2<float>(1024, 1024),
			Matrix matrix = generateMatrix(32, 32, 1),
			const unsigned int render_distance = 32 * 32) :

			MainWorldStates(size,
				matrix,
				sf::Vector2<int>(static_cast<int>(matrix.size()),
					static_cast<int>(matrix[0].size())),
				sf::Vector2<float>(size.x /
					static_cast<float>(matrix.size()),
					size.y / static_cast<float>(matrix[0].size())),
				render_distance,
				target),
			ray_caster_api_(*this),
			texture_(static_cast<sf::Texture*>(malloc(
				(this->ray_num_) * sizeof(sf::Texture)))),
			camera_states_(target),
			used_walls_(this->ray_num_),
		floor_color_(0, 150,0)
		{

			wall_.push_back(new MainWall);
			wall_.push_back(new MainWall);

			background_texture_.loadFromFile("data/tex/bg.jpg");
			
		}

		/*
		 * Смена холста
		 */
		void setRenderTarget(sf::RenderTarget& target)
		{
			this->ray_num_ = target.getSize().x;
			this->ray_num_2_ = static_cast<int>(static_cast<float>
				(this->ray_num_) / 2.f);

			if (realloc(texture_, (this->ray_num_) *
				sizeof(sf::Texture)) == NULL)
			{
				/*
				 * В случае неудачи переопределения
				 * размеров динамического массива,
				 * выходим из программы с кодом ошибки -5
				 */
				exit(-5);
			}

			ray_caster_api_.ResetStates(*this);
		}

		/*
		 * Рендер мира, путём передачи в него камеры
		 */
		void render(Camera& camera)
		{
			
			camera_states_ = camera.getStates();
			
			/*
			 * Получаем данные от рейкастера
			 */
			ray_data_ = (ray_caster_api_.RayCast(camera));

			/*
			 * Записьномеров стен,
			 * для уменьшения количества вызовав виртуальных функций стен
			 * и последующей их сортировки
			 */
			for (int i = 1; i < ray_num_; i++)
			{
				used_walls_[i] = ray_data_[i].wall_number;
			}

			/*
			 * Процедура отбора уникальных номеров стен,
			 * для уменьшения количества вызовав виртуальных функций стен
			 * и их сортировка средствами STL
			 */
			std::unique_copy(used_walls_.begin(),
				used_walls_.end(),
				std::back_inserter(unique_walls_));

			/*
			 * Вызов виртуальных функций стен
			 */
			for (int i = 0; i < unique_walls_.size(); i++)
			{
				wall_[unique_walls_[i]]->wall_states(ray_data_[i]);
			}

			unique_walls_.clear();

		}

		/*
		 * Добавление новой стены в мир
		 */

		void addWallType(Wall_api* wall)
		{
			wall_.push_back(wall);
		}

		/*
		 * Перегрузка / переопределение оператора для вышеописанной функции
		 */
		void operator << (Wall_api* wall)
		{
			addWallType(wall);
		}

		/*
		 * Переопределение виртуальной функции.
		 * Для отрисовки средстсвами SFML.
		 */
	protected:
		void draw(sf::RenderTarget& target,
			sf::RenderStates states) const override
		{

			/*
			 * Отрисовка пола
			 */
			sf::RectangleShape rectangle_shape_floor_;

			rectangle_shape_floor_.setSize(sf::Vector2<float>
				(camera_states_.window_size_.x, 1));

			const float delta_color_r((floor_color_.r) /
				camera_states_.window_size_2_.y);
			
			const float delta_color_g((floor_color_.g) /
				camera_states_.window_size_2_.y);
			
			const float delta_color_b((floor_color_.b) /
				camera_states_.window_size_2_.y);

			for (auto i = camera_states_.window_size_2_.y;
				i < camera_states_.window_size_.y; i++)
			{
				rectangle_shape_floor_.setPosition(0, i);
				
				const auto it = (i - camera_states_.window_size_2_.y);
				
				rectangle_shape_floor_.setFillColor(
					sf::Color(it * delta_color_r,
						it * delta_color_g, it * delta_color_b));
				
				target.draw(rectangle_shape_floor_);
			}

			/*
			 * Отрисовка заднего плана
			 */
			sf::Sprite background_sprite_;
			
			background_sprite_.setTexture(background_texture_);
			
			float a;
			
			const float ww_scale = (static_cast<float>(
				background_texture_.getSize().x) /
				camera_states_.window_size_.x);
			
			const float scale_tex_r3d = (camera_states_.window_size_.x /
				static_cast<float>(this->ray_num_)) / ww_scale;
			
			background_sprite_.setScale(scale_tex_r3d, 1);
			
			for (int i = 0; i < this->ray_num_; i++)
			{
				if (ray_data_[i].rotation < 0)
				{
					a = 4 - abs(fmodf(ray_data_[i].rotation, 4));
				}
				else
				{
					a = abs(fmodf(ray_data_[i].rotation, 4));
				}
				
				background_sprite_.setTextureRect(
					sf::IntRect(static_cast<int>(a *
						(camera_states_.window_size_.x / 4.f) * ww_scale),
						0, static_cast<int>(ww_scale),
				static_cast<int>(camera_states_.window_size_2_.y)));
				
				background_sprite_.setPosition(i, 0);
				
				target.draw(background_sprite_);
			}

			/*
			 * Отрисовка стен
			 */
			sf::Sprite sprite;
			
			for (auto i = 0; i < ray_num_; i++)
			{

				float d{};

				/*
				 * В зависимости от того, на какую сторону падает луч,
				 * находим столб текстуры
				 */
				
				switch (ray_data_[i].direction) {

				case RayCaster_api::dir::left:
					d = fmodf(ray_data_[i].position_y,
						wall_size_.y);
					break;

				case RayCaster_api::dir::right:
					d = (wall_size_.y - fmodf(ray_data_[i].position_y,
						wall_size_.y));
					break;

				case RayCaster_api::dir::down:
					d = (wall_size_.x - fmodf(ray_data_[i].position_x,
						wall_size_.x));
					break;

				case RayCaster_api::dir::up:
					d = fmodf(ray_data_[i].position_x,
						wall_size_.x);
					break;

				case RayCaster_api::dir::none:
					break;

				default:
					break;

				}

				/*
				 * Берём текстуру у стены.
				 */
				sprite.setTexture(wall_[ray_data_[i].wall_number]->texture);

				/*
				 * Высота стены.
				 */
				const float wall_height = camera_states_.render_constant_ /
					(ray_data_[i].length * cosf(
						ray_data_[i].rotation -
						camera_states_.rotation_ /
						1.01f));

				/*
				 * Позиция столба стены на экране по координате Y.
				 */
				const float sprite_position_y = camera_states_.window_size_2_.y
					- wall_height / 2.f;

				/*
				 * Ставим столб стены на данную позицию.
				 */
				sprite.setPosition(i, sprite_position_y);

				const float tex_wall_scale = (static_cast<float>
					(wall_[ray_data_[i].wall_number]->texture.getSize().x)
					/ wall_size_.x);

				sprite.setTextureRect(sf::IntRect(
					d * tex_wall_scale,
					0, 1,
					wall_[ray_data_[i].wall_number]
					->texture.getSize().y));

				const float scale = 1.f / tex_wall_scale;

				sprite.setScale(1, wall_height /
					static_cast<float>(wall_[ray_data_[i].wall_number]
						->texture.getSize().y));

				/*
				 * Определяем цвет стены,
				 * в зависимости от её расстояния до камеры.
				 */
				const sf::Uint8 wall_clr = 255.f / (1.f +
					powf(ray_data_[i].length, 2.f) *
					camera_states_.shading_coefficient_);

				sprite.setColor(sf::Color(wall_clr,
					wall_clr, wall_clr));

				/*
				 * Отрисовка данного столба
				 */
				target.draw(sprite);
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
				(camera.position_.x + camera.radius_) :
				(camera.position_.x - camera.radius_)) + step_x,
				wall_size_.x)]
				[math::getMatrixPos(((step_y > 0) ?
					(camera.position_.y + camera.radius_) :
					(camera.position_.y - camera.radius_)),
					wall_size_.y)] == 0)
			{
				camera.move(step_x, 0);
			}

			if (matrix_[math::getMatrixPos(((step_x > 0) ?
				(camera.position_.x + camera.radius_) :
				(camera.position_.x - camera.radius_)),
				wall_size_.x)]
				[math::getMatrixPos(((step_y > 0) ?
					(camera.position_.y + camera.radius_) :
					(camera.position_.y - camera.radius_)) + step_y,
					wall_size_.y)] == 0)
			{
				camera.move(0, step_y);
			}
		}

	};

	/*
	 * Класс камеры.
	 */
	class Camera : public MainCameraStates
	{
		/*
		 * Дружелюбные классы
		 */
		
		friend class RayCaster_api;

		friend class World;

	private:

		/*
		 * Радиус камеры
		 */
		float radius_;

	public:

		Camera(const sf::RenderTarget& target) :
			MainCameraStates(target),
			radius_(5.f)
		{
			change_render_constant();
		}

		Camera(const sf::RenderTarget& target,
			const float position_x,
			const float position_y,
			const float fov,
			const float rotation,
			const float zoom = 0.01f,
			const float shading_coefficient = 40.f,
			const float radius = 5) :

			MainCameraStates(target, { position_x, position_y }, fov,
				rotation, zoom, shading_coefficient),
			radius_(radius)
		{
			change_render_constant();
		}

		Camera(const sf::RenderTarget& target,
			const sf::Vector2<float> position,
			const float fov,
			const float rotation,
			const float zoom = 0.01f,
			const float shading_coefficient = 40.f,
			const float radius = 5) :

			MainCameraStates(target, position, fov,
				rotation, zoom, shading_coefficient),
			radius_(radius)
		{
			change_render_constant();
		}

		/*
		 * Переопределение холста
		 */
		void setRenderTarget(const sf::RenderTarget& target)
		{
			resizeTarget(target.getSize());
			this->wwf_ = this->window_size_.x / this->fov_;
			change_render_constant();
		}

		/*
		 * Установить коэффициент затенения
		 */
		void setShadingCoefficient(const float shading_coefficient)
		{
			this->shading_coefficient_ = shading_coefficient * 0.00001f;
		}

		/*
		 * Получить коэффициент затенения
		 */
		float getShadingCoefficient() const
		{
			return this->shading_coefficient_;
		}

		/*
		 * Установить поворот камеры
		 */
		void setRotation(const float rotation_degrees)
		{
			this->rotation_ = math::toRad(rotation_degrees);
		}

		/*
		 * Получить поворот камеры
		 */
		float getRotation() const
		{
			return math::toDeg(this->rotation_);
		}

		/*
		 * Повернуть камеру
		 */
		void rotate(const float rotation_degrees)
		{
			this->rotation_ += math::toRad(rotation_degrees);
		}

		/*
		 * Установить позицию камеры
		 */
		void setPosition(const sf::Vector2<float> position)
		{
			this->position_ = position;
		}

		/*
		 * Установить позицию камеры
		 */
		void setPosition(const float x, const float y)
		{
			this->position_.x = x;
			this->position_.y = y;
		}

		/*
		 * Получить позицию камеры
		 */
		sf::Vector2<float> getPosition() const
		{
			return this->position_;
		}

		/*
		 * Передвинуть камеру
		 */
		void move(const float x, const float y)
		{
			this->position_.x += x;
			this->position_.y += y;
		}

		/*
		 * Передвинуть камеру
		 */
		void move(const sf::Vector2<float> position)
		{
			move(position.x, position.y);
		}

		/*
		 * Установить угол обзора
		 */
		void setFov(const float fov)
		{
			this->fov_ = fov;
			this->wwf_ = this->window_size_.x / fov;
			change_render_constant();
		}

		/*
		 * Получить угол обзора
		 */
		float getFov() const
		{
			return math::toDeg(this->fov_);
		}

		/*
		 * Увеличить угол обзора
		 */
		void fov_unary(const float fov)
		{
			setFov(this->fov_ + math::toRad(fov));
		}

		/*
		 * Установить зум камеры
		 */
		void setZoom(const float zoom)
		{
			this->zoom_ = zoom;
			change_render_constant();
		}

		/*
		 * Получить зум камеры
		 */
		float getZoom() const
		{
			return this->zoom_;
		}

		/*
		 * Увеличить зум камеры
		 */
		void zoom_unary(const float zoom)
		{
			setZoom(this->zoom_ + zoom);
		}

		/*
		 * Установить радиус камеры
		 */
		void setRadius(const float radius)
		{
			radius_ = radius;
		}

		/*
		 * Получить радиус камеры
		 */
		float getRadius() const
		{
			return radius_;
		}

		/*
		 * Сохранить состояние камеры
		 */

	public:
		/*
		 * Запись сохранения
		 */
		void save(const std::string& name) const
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
				CameraSave camera_save(this->fov_, this->zoom_,
					this->shading_coefficient_, this->radius_);
				ofstream.write((char*)&camera_save,  sizeof(CameraSave));
			}
			
			ofstream.close();
		}

		/*
		 * Распаковка сохраения
		 */
		void open(const std::string& name)
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
				
				ifstream.read((char*)&camera_save, sizeof(CameraSave));

				this->fov_ = camera_save.fov;
				this->shading_coefficient_ = camera_save.shading_coefficient;
				this->zoom_ = camera_save.zoom;
				this->radius_ = camera_save.radius;

				change_render_constant();
				this->wwf_ = this->window_size_.x / this->fov_;
			}
			ifstream.close();
		}

	private:
		
		/*
		 * Служебная функция.
		 * Получение состояния камеры.
		 */
		MainCameraStates getStates() const
		{
			return static_cast<MainCameraStates>(*this);
		}
	};
};
