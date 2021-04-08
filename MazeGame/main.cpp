#include "Walls.h"
#include <iostream>
#include <iomanip>
#include "Gui.h"
#include "WallGameGenerator.h"
#include <windows.h>
int main()
{
	ShowCursor(false);
	//sf::RenderWindow window0(sf::VideoMode(800, 800), "Rays", sf::Style::Default);
	sf::RenderWindow window(sf::VideoMode(800, 800), "3D", sf::Style::Fullscreen);
	window.setPosition({ 10, 10 });
	//window0.close();
	sf::View view0;
	view0.setSize(200, 200);
	view0.setCenter(100, 100);
	//window0.setView(view0);
	//window0.setPosition({ 810, 10 });
	window.setVerticalSyncEnabled(true);
	//sf::RenderWindow DW(sf::VideoMode(800, 800), "3D", sf::Style::Default);

	const int sizeX = 10;
	const int sizeY = 10;

	v3d::Maze<sizeX, sizeY, 2> maze{};
	v3d::Matrix lab = maze.generate();
	/*v3d::Matrix lab = 
	{   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 1, 1, 1, 1, 2, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 1, 2, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 2, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
	};*/
	
	v3d::World world(window, sf::Vector2f(250, 250), lab);
	v3d::Camera camera(window);
	world.setCamera(&camera);
	camera.setRadius(1);
	camera.setShadingCoefficient(10);
	camera.setBackgroundRepeatingFov(360);

	sf::Vector2f spawn_position;
	
	for (int i = 0; i < world.getMatrixSize().x; i++)
	{
		for (int l = 0; l < world.getMatrixSize().y; l++)
		{
			if(world[i][l] == 0)
			{
				spawn_position = { i * world.getWallSize().x + world.getWallSize().x / 2, l * world.getWallSize().y + world.getWallSize().y / 2};
				break;
			}
		}
	}
	
	for (int i = 0; i < world.getMatrixSize().x; i++)
	{
		for (int l = 0; l < world.getMatrixSize().y; l++)
		{
			if (world[i][l] == 1)
			{
				world[i][l] = math::rand(1, 4, i * l);
			}
		}
	}
	
	camera.setPosition(spawn_position.x, spawn_position.y);

	sf::Vector2f r_size(world.getSize().x / sizeX, world.getSize().y / sizeY);
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

	world << new CircleWall;
	world << new v3d::MainWall("data/tex/wall5.png");
	world << new v3d::MainWall("data/tex/wall2.png");

	const int fps_i = 3;
	int fps_ii = 0;
	float fps_data = 0;

	const float speed = 0.15f;


	gui::Drawable drawable_esc;
	
	gui::Button button(100, 100, 100, 50, "data/tex/4.png");

	//button.setBorderThickness(3);
	
	std::string message = "S";
	
	button.onClick([&button]()
	{
			//std::cout << "g";
		button.setTexture("data/tex/8.png");
	});

	button.onOut([&button]()
	{
		button.setTexture("data/tex/4.png");
	});
	
	drawable_esc.addGuiElement(&button);

	v3d::RayCaster_api ray_caster;
	
	while (window.isOpen())
	{
		//FPS
		Time = clock.getElapsedTime();
		float currentTime = Time.asSeconds();

		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
			{
				window.close();
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			world.camera_move_with_cls(camera, cosf(camera.getRotation()) * speed, sinf(camera.getRotation()) * speed);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			world.camera_move_with_cls(camera, -cosf(camera.getRotation()) * speed, -sinf(camera.getRotation()) * speed);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			world.camera_move_with_cls(camera, cosf(camera.getRotation() + 90) * speed, sinf(camera.getRotation() + 90) * speed);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			world.camera_move_with_cls(camera, cosf(camera.getRotation() - 90) * speed, sinf(camera.getRotation() - 90) * speed);
		}
		//if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
//{
	
		//}

		window.clear();
		//window0.clear();

		circle.setPosition(camera.getPosition());
		//drawable.draw(window);
		
		/*for (int i = 0; i < world.getMatrixSize().x; i++)
		{
			for (int l = 0; l < world.getMatrixSize().y; l++)
			{
				if (world[i][l] != 0)
				{
					r.setPosition(i * r_size.x, l * r_size.y);
					//window0.draw(r);
				}
			}
		}

		auto ray_data = ray_caster.RayCast(&camera, &world);
		circle.setPosition(camera.getPosition());
		ray.setPosition(camera.getPosition());
		for (int i = 0; i < ray_data.size(); i += ray_data.size() / 30)
		{
			ray.setRotation(math::toDeg(ray_data[i].rotation));
			ray.setSize(sf::Vector2f(ray_data[i].length, 0.3f));
			//window0.draw(ray);
		}*/
		
		sf::Vector2i mouse_pos0 = sf::Mouse::getPosition(window);
		window.draw(world);
		sf::Vector2i mouse_pos1 = sf::Mouse::getPosition(window);
		camera.rotate(math::toRad(static_cast<float>(mouse_pos1.x - mouse_pos0.x) * speed));
		camera.windowDeltaY_unary((static_cast<float>(mouse_pos0.y) - static_cast<float>(mouse_pos1.y)) * speed * 10);
		sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2));
		
		//FPS
		Time = clock.getElapsedTime();
		float lastTime = Time.asSeconds();
		float fps = 1 / (lastTime - currentTime);
		fps_data += fps;
		if (fps_ii == fps_i)
		{
			FPStext.setString(std::to_string(static_cast<int>(fps_data / static_cast<float>(fps_ii))));
			fps_ii = 0;
			fps_data = 0;
		}
		fps_ii++;
		window.draw(FPStext);
		//window0.display();
		window.display();
		
	}

	return EXIT_SUCCESS;
}
