#include "Walls.h"



int main()
{

	sf::RenderWindow window(sf::VideoMode(800, 800), "3D", sf::Style::Default);
	
	//sf::RenderWindow DW(sf::VideoMode(800, 800), "3D", sf::Style::Default);

	const int sizeX = 8;
	const int sizeY = 8;

	v3d::Maze<sizeX, sizeY, 132434> maze{};
	v3d::FileManager::save(&maze, "maze");
	v3d::Matrix lab = maze.generate();


	v3d::Matrix matrix = {
		{2, 2, 2, 2, 2, 2, 2, 2},
		{2, 0, 0, 0, 0, 0, 0, 2},
		{2, 0, 0, 0, 0, 0, 0, 2},
		{2, 0, 0, 0, 2, 1, 0, 2},
		{2, 0, 2, 0, 0, 0, 0, 2},
		{2, 0, 0, 0, 2, 0, 0, 2},
		{2, 0, 0, 0, 0, 0, 0, 2},
		{2, 2, 2, 2, 2, 2, 2, 2}
	};

	
	v3d::World world(window, sf::Vector2<float>(800, 800), matrix, sizeX * sizeY);
	v3d::Camera camera(window, sf::Vector2f(100.f, 100.f), 45.f, 100.f, 18.f, 1.f);
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

	world << new CircleWall;

	const int fps_i = 300;
	int fps_ii = 0;
	float fps_data = 0;

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
			camera.rotate(0.53f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			camera.rotate(-0.53f);
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
		{
			camera.move(math::cos180(camera.getRotation()), math::sin180(camera.getRotation()));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
		{
			camera.move(-math::cos180(camera.getRotation()), -math::sin180(camera.getRotation()));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
		{
			camera.move(math::cos180(camera.getRotation() + 90), math::sin180(camera.getRotation() + 90));
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
		{
			camera.move(math::cos180(camera.getRotation() - 90), math::sin180(camera.getRotation() - 90));
		}

		window.clear();

		circle.setPosition(camera.getPosition());
		//ray.setRotation(camera.getRotation());
		//ray.setPosition(camera.getPosition());
		//window.draw(circle);
		//window.draw(ray);
		world.render(camera);
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
		 
		//world.render(camera);
		window.draw(world);
		window.draw(FPStext);
		window.display();

		
		
	}

	return EXIT_SUCCESS;
}