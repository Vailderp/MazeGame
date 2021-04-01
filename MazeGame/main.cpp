#include "Walls.h"
#include <iostream>
#include <iomanip>

int main()
{
	sf::RenderWindow window(sf::VideoMode(1680, 1050), "3D", sf::Style::Fullscreen);
	window.setVerticalSyncEnabled(true);
	//sf::RenderWindow DW(sf::VideoMode(800, 800), "3D", sf::Style::Default);

	const int sizeX = 50;
	const int sizeY = 50;

	v3d::Maze<sizeX, sizeY, 132434> maze{};
	v3d::Matrix lab = maze.generate();
	
	v3d::World world(window, sf::Vector2f(1000, 1000), lab);
	v3d::Camera camera(window, 100.f, 100.f, 45, 100.f, 1.f, 1.f);

	sf::Vector2f spawn_position;
	
	for (float i = 0; i < world.getMatrixSize().x; i++)
	{
		for (float l = 0; l < world.getMatrixSize().y; l++)
		{
			if(world[i][l] == 0)
			{
				spawn_position = { i * world.getWallSize().x + world.getWallSize().x / 2, l * world.getWallSize().y + world.getWallSize().y / 2};
				break;
			}
		}
	}
	
	camera.setPosition(spawn_position.x, spawn_position.y);
	
	sf::RectangleShape r(sf::Vector2f(window.getSize().x / sizeX, window.getSize().y / sizeY));
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
	font.loadFromFile("fonts/ob.otf");
	sf::Text FPStext;
	FPStext.setPosition(10, 10);
	FPStext.setFillColor(sf::Color(255, 0, 0, 150));
	FPStext.setCharacterSize(30);
	FPStext.setStyle(sf::Text::Bold);
	FPStext.setFont(font);

	world << new CircleWall;

	const int fps_i = 3;
	int fps_ii = 0;
	float fps_data = 0;

	const float speed = 0.3f;

	
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
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			camera.rotate(2.20f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			camera.rotate(-2.20f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			world.camera_move_with_cls(camera, math::cos180(camera.getRotation()) * speed, math::sin180(camera.getRotation()) * speed);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			world.camera_move_with_cls(camera, -math::cos180(camera.getRotation()) * speed, -math::sin180(camera.getRotation()) * speed);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			world.camera_move_with_cls(camera, math::cos180(camera.getRotation() + 90) * speed, math::sin180(camera.getRotation() + 90) * speed);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			world.camera_move_with_cls(camera, math::cos180(camera.getRotation() - 90) * speed, math::sin180(camera.getRotation() - 90) * speed);
		}

		window.clear();

		circle.setPosition(camera.getPosition());
		world.render(camera);
		window.draw(world);
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
		window.display();


	}

	return EXIT_SUCCESS;
}
