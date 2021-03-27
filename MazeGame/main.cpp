#include "Engine3d.h"



int main()
{

	sf::RenderWindow window(sf::VideoMode(800, 800), "3D", sf::Style::Default);
	sf::RenderWindow DW(sf::VideoMode(800, 800), "3D", sf::Style::Default);

	const int sizeX = 32;
	const int sizeY = 32;

	v3d::Maze<sizeX, sizeY, 132434> maze{};
	v3d::FileManager::save(&maze, "maze");
	v3d::Matrix lab = maze.generate();

	v3d::World world(DW, sf::Vector2<float>(800, 800), lab, sizeX * sizeY);
	v3d::Camera camera(DW, sf::Vector2f(100, 100), 60, 100, 0.3f, 40);
	camera.setPosition(100, 100);
	
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
			camera.rotate(1.53f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			camera.rotate(-1.53f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			camera.move(math::cos180(camera.getRotation()), math::sin180(camera.getRotation()));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			camera.move(-math::cos180(camera.getRotation()), -math::sin180(camera.getRotation()));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			camera.move(math::cos180(camera.getRotation() + 90), math::sin180(camera.getRotation() + 90));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			camera.move(math::cos180(camera.getRotation() - 90), math::sin180(camera.getRotation() - 90));
		}

		window.clear();
		DW.clear();

		for (int i = 0; i < sizeX; i++)
		{
			for (int l = 0; l < sizeY; l++)
			{
				if (lab[i][l] == 1)
				{
					r.setPosition(window.getSize().x / sizeX * i, window.getSize().y / sizeY * l);
					window.draw(r);
				}
			}
		}

		circle.setPosition(camera.getPosition());
		ray.setRotation(camera.getRotation());
		ray.setPosition(camera.getPosition());
		window.draw(circle);
		//window.draw(ray);
		world.render(camera, window);
		Time = clock.getElapsedTime();
		float lastTime = Time.asSeconds();
		float fps = 1 / (lastTime - currentTime);
		FPStext.setString(std::to_string(fps));
		 
		//world.render(camera);
		DW.draw(world);
		window.draw(FPStext);
		DW.display();
		window.display();
		
	}

	return EXIT_SUCCESS;
}