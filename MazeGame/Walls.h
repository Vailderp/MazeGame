#pragma once

#include "Engine3d.h"

/*
 * С помощью Engine3d.h
 * можно создавать стены с неограниченными возможностями
 */

class CircleWall : public v3d::Wall_api
{

public:
	CircleWall()
	{
		render_texture_.create(1000, 1000);
		circle_shape_.setRadius(50);
		circle_shape_.setOrigin(50, 50);
		circle_shape_.setFillColor(sf::Color::Red);
	}
	
protected:
	void wall_states(v3d::RayCaster_api::RayData& data) override
	{
		if (timer_.microseconds() > 100)
		{

			if (circle_shape_.getPosition().x + 151 > 1000)
			{
				circle_shape_.setPosition(849, circle_shape_.getPosition().y);
				speedX = -speedX;
			}
			if (circle_shape_.getPosition().x - 50 < 0)
			{
				circle_shape_.setPosition(51, circle_shape_.getPosition().y);
				speedX = -speedX;
			}
			if (circle_shape_.getPosition().y + 50 > 1000)
			{
				circle_shape_.setPosition(circle_shape_.getPosition().x, 949);
				speedY = -speedY;
			}
			if (circle_shape_.getPosition().y - 50 < 0)
			{
				circle_shape_.setPosition(circle_shape_.getPosition().x, 51);
				speedY = -speedY;
			}

			circle_shape_.move(speedX, speedY);
			
			render_texture_.clear(sf::Color::Green);
			render_texture_.draw(circle_shape_);
			
			this->texture = render_texture_.getTexture();

			render_texture_.clear();
			
			timer_.clear();
		}
	}

private:
	float speedX = 0.8f;
	float speedY = 1.5f;
	v3d::Timer<float> timer_;
	sf::CircleShape circle_shape_;
	sf::RenderTexture render_texture_;
};

