//#define V3D_FUNCTIONAL
//#define V3D_GPU_RAYCASTING_MAX_PIXEL_DEPTH 64
//#include "Walls.h"
//#include <Windows.h>
//#include "GUI_inits.h"
//#include <algorithm>
//#include "GPGPU.h"
#include <iostream>

#include "DataImpl.h"


void test1(tu::Data<int, 2>::DataImpl<3, 3>& data_impl)
{
	for (int i = 0; i < 100000000; ++i)
	{
		data_impl.at<2, 2>()++;
	}
}

void test2(const tu::DataImpl2<int>& data_impl)
{
	for (int i = 0; i < 100000000; ++i)
	{
		data_impl.get(2, 2)++;
	}
}


int main()
{
	tu::ArrayFill<int, 9>::type data = new tu::ArrayFill<int, 9>::type_init;
	tu::Data<int, 2>::DataImpl<3, 3> data_impl(data);

	int* data2 = new int[9];
	const tu::DataImpl2<int> data_impl2(&data2, 3, 3);
	
	while (true)
	{
		test1(data_impl);
		test2(data_impl2);
	}
	return 0;
}
	/*
	std::ios_base::sync_with_stdio(false);
	//floorg(90._deg, 10, 10);
	sf::RenderWindow window(sf::VideoMode(1680, 1050), "Maze Game!", sf::Style::Default);
	window.setPosition({ 10, 10 });
	//window.setFramerateLimit(60);
	//window.setVerticalSyncEnabled(true);

	const int sizeX = 10;
	const int sizeY = 10;

	v3d::Maze<sizeX, sizeY, 2> maze{};
	//v3d::Matrix lab = maze.generate();
	v3d::Matrix lab =
	{   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 1, 1, 1, 1, 2, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 2, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 2, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	};

	v3d::ray_data* ray_data = new v3d::ray_data[window.getSize().x \
		* V3D_GPU_RAYCASTING_MAX_PIXEL_DEPTH];
	v3d::CPURayCaster ray_caster(window.getSize().x, &ray_data);
	v3d::Camera camera(window);
	v3d::CPUWorld world(v3d::World(&camera, &ray_data, \
		sf::Vector2f(600, 600), lab));
	
	world.setCamera(&camera.setRadius(0.10F).setShadingCoefficient(0) \
		.setBackgroundRepeatingFov(360.0_deg).setFov(60.0_deg) \
		.setZoom(0.03F).setWindowDeltaY(300)).setPixelDepthMax(10);


	float* matrix = new float[3 * 3]
	{
		1, 2, 3,
		4, 5, 6,
		7, 8, 9
	};
	
	GPGPUHOST<float, 3, 3> h_a(matrix);

	/*concurrency::parallel_for_each(
		h_a.host_array.extent,
		[h_a](concurrency::index<2> idx) restrict(amp)
		{
			for (int i = 0; i < 100000; i++)
			{
				h_a[0][0] += 0.01;
			}
		}
	);*/

	//std::cout << h_a[0][0] << std::endl;
	/*
	sf::Vector2f spawn_position;

	for (int i = 0; i < world.getMatrixSize().x; i++)
	{
		for (int l = 0; l < world.getMatrixSize().y; l++)
		{
			if (world[i][l] == 0)
			{
				spawn_position = { i * world.getWallSize().x
					+ world.getWallSize().x / 2,
					l * world.getWallSize().y +
					world.getWallSize().y / 2 };
				break;
			}
		}
	}
	/*
	for (int i = 0; i < world.getMatrixSize().x; i++)
	{
		for (int l = 0; l < world.getMatrixSize().y; l++)
		{
			if (world[i][l] == 1)
			{
				world[i][l] = math::rand(1, 4, i * l);
			}
		}
	}*/
	/*
	camera.setPosition(spawn_position.x, spawn_position.y);

	sf::Vector2f r_size(world.getSize().x / sizeX,
		world.getSize().y / sizeY);
	sf::RectangleShape r(r_size);
	r.setFillColor(sf::Color::Blue);
	sf::RectangleShape poly(sf::Vector2f(50, 5));
	poly.setRotation(45);
	poly.setFillColor(sf::Color::Green);
	sf::CircleShape circle(3);
	circle.setFillColor(sf::Color::Green);
	circle.setOrigin(3, 3);
	sf::RectangleShape ray(sf::Vector2f(150, 2));
	ray.setFillColor(sf::Color::Red);

	//FPS CONTROLLER
	sf::Clock clock;
	sf::Time Time;
	sf::Clock clock_menu;
	float LastTimeMenu = 0;
	float NewTimeMenu = 0;
	sf::Font font;
	font.loadFromFile("data/fonts/ob.otf");
	sf::Text FPStext;
	FPStext.setPosition(10, 10);
	FPStext.setFillColor(sf::Color(255, 0, 0, 150));
	FPStext.setCharacterSize(30);
	FPStext.setStyle(sf::Text::Bold);
	FPStext.setFont(font);

	const int fps_i = 3;
	int fps_ii = 0;
	float fps_data = 0;
	const float speed = 1.2f;
	const float rot_speed = 0.2f;

	//world.addWalls(new CircleWall);
	
	world
		<< new v3d::MainWall(true, false,"data/tex/window.png")
		<< new v3d::MainSprite(30, 30, 0, 10, 10, "data/tex/flowers/1.png");


	v3d::Timer<float> timer_esc;
	Menu menu(&window, &camera);

	/*v3d::GPU_RayCaster gpu_ray_caster(&camera, &world, window.getSize().x);
	gpu_ray_caster.start_gpgpu_parallel_raycasting();
	v3d::RayData* data = gpu_ray_caster.getDataPtr();

	while (true)
	{
	
		std::cout << data[45].rotation << std::endl;
		Sleep(100);
	}*/

	/*
	while (window.isOpen())
	{
		
		//FPS
		Time = clock.getElapsedTime();
		float currentTime = Time.asSeconds();
		sf::Event event{};
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
			{
				window.close();
			}

			if (event.type == sf::Event::Resized)
			{
				// update the view to the new size of the window
				sf::FloatRect visible_area(0, 0, event.size.width, event.size.height);
				window.setView(sf::View(visible_area));
			}

			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape) && timer_esc.milliseconds() > 500)
			{
				timer_esc.clear();
				menu.setActive(true);
				menu.setActiveSettings(false);
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			world.camera_move_with_cls(camera,
				cosf(camera.getRotation()) * speed,
				sinf(camera.getRotation()) * speed);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			world.camera_move_with_cls(camera,
				-cosf(camera.getRotation()) * speed,
				-sinf(camera.getRotation()) * speed);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			world.camera_move_with_cls(camera,
				cosf(camera.getRotation() + 90.0_deg) * speed,
				sinf(camera.getRotation() + 90.0_deg) * speed);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			world.camera_move_with_cls(camera,
				cosf(camera.getRotation() - 90.0_deg) * speed,
				sinf(camera.getRotation() - 90.0_deg) * speed);
		}
		//window_position = window.getPosition();

		window.clear();
		//window0.clear();

		circle.setPosition(camera.getPosition());

		if (!menu.active() || menu.activeSettings())
		{
			sf::Vector2i mouse_pos0 = sf::Mouse::getPosition(window);
			ray_caster.startRayCasting(&camera, &world);
			window.draw(world);
			if (!menu.active())
			{
				sf::Vector2i mouse_pos1 = sf::Mouse::getPosition(window);
				camera.rotate(math::toRad(static_cast<float> \
					(mouse_pos1.x - mouse_pos0.x) * rot_speed)) \
				.windowDeltaY_unary((static_cast<float>(mouse_pos0.y) \
					- static_cast<float>(mouse_pos1.y)) * rot_speed * 10);

				
				sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2, \
					window.getSize().y / 2));
			}
		}

		menu.draw();

		//FPS

		
		Time = clock.getElapsedTime();
		float lastTime = Time.asSeconds();
		float fps = 1 / (lastTime - currentTime);
		fps_data += fps;
		if (fps_ii == fps_i)
		{
			FPStext.setString(std::to_string(static_cast<int>(fps_data \
				/ static_cast<float>(fps_ii))));
			fps_ii = 0;
			fps_data = 0;
		}
		fps_ii++;
		window.draw(FPStext);
		//window0.display();
		window.display();
	}


	
	
	//tu::ptr<int>::create<1>::value p;

	return EXIT_SUCCESS;
}*/