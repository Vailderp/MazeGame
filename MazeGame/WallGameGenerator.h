#pragma once
#include <iostream>
#include <vector>
#include <SFML/System/Vector2.hpp>

class WallGameGenerator
{
public:
	std::vector<std::vector<int>> generate(const float size)
	{
		std::vector<std::vector<int>> matrix(size, std::vector<int>(size, 0));

		sf::Vector2i position(size / 2, size / 2);

		for (auto i = 0; i < size; i++)
		{
			for (auto l = 0; l < size; l++)
			{
				if (position.x + 1 < size)
				{
					std::cout << position.x << std::endl;
					if (matrix[position.x + 1][position.y] == 0)
					{
						position.x += 1;
						matrix[position.x][position.y] = 1;
					}
				}

				if (position.x - 1 > -1)
				{
					if (matrix[position.x - 1][position.y] == 0)
					{
						position.x -= 1;
						matrix[position.x][position.y] = 2;
					}
				}

				if (position.y + 1 < size)
				{
					if (matrix[position.x][position.y + 1] == 0)
					{
						position.y += 1;
						matrix[position.x][position.y] = 3;
					}
				}

				if (position.y - 1 > -1)
				{
					if (matrix[position.x][position.y - 1] == 0)
					{
						position.y -= 1;
						matrix[position.x][position.y] = 4;
					}
				}
			}
		}

		return matrix;
	}
};
