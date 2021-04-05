#pragma once

#include "Engine3d.h"


/*
 * С помощью Engine3d.h
 * можно создавать стены с неограниченными возможностями
 */

class CircleWall final : public v3d::Wall_api
{

public:
	CircleWall()
	{
		render_texture_.create(1000, 1000);
		circle_shape_.setRadius(radius_);
		circle_shape_.setOrigin(radius_, radius_);
		circle_shape_.setFillColor(sf::Color::Red);
		
		player_x.setPosition(30, 10);
		player_x.setSize({ 50, 200 });
		player_x.setFillColor(sf::Color::Green);
		
		player_y.setPosition(1000 - 30 - 50, 10);
		player_y.setSize({ 50, 200 });
		player_y.setFillColor(sf::Color::Green);
	}
	
protected:
	void wall_states(const v3d::RayCaster_api::RayData& data) override
	{
			render_texture_.clear(sf::Color::Blue);
			
			this->texture = render_texture_.getTexture();
	}

	void wall_states_center_ray(const v3d::RayCaster_api::RayData& data, const sf::Vector2<float> center_ray) override
	{
		if (timer_.milliseconds() > 10)
		{
			circle_shape_.move(speed_.x, speed_.y);

			if (circle_shape_.getPosition().x > 1000 || circle_shape_.getPosition().x < 0)
			{
				circle_shape_.setPosition(500, 500);
			}
			if (circle_shape_.getPosition().y > 1000 || circle_shape_.getPosition().y < 0)
			{
				speed_.y = -speed_.y;
			}
			
			
			if (player_y.getPosition().y < circle_shape_.getPosition().y)
			{
				player_y.move(0, sqrt(abs(player_y.getPosition().y - center_ray.y) / 10));
			}
			else if (player_y.getPosition().y > circle_shape_.getPosition().y)
			{
				player_y.move(0, -sqrt(abs(player_y.getPosition().y - center_ray.y) / 10));
			}

			if (player_x.getPosition().y < center_ray.y)
			{
				player_x.move(0, sqrt(abs(player_x.getPosition().y - center_ray.y) / 10));
			}
			else if (player_y.getPosition().y > center_ray.y)
			{
				player_x.move(0, -sqrt(abs(player_x.getPosition().y - center_ray.y) / 10));
			}
			
			render_texture_.draw(circle_shape_);
			render_texture_.draw(player_x);
			render_texture_.draw(player_y);
			this->texture = render_texture_.getTexture();
			timer_.clear();
		}
	}

private:
	sf::Vector2f speed_ = { 8.5f, 3.5 };
	v3d::Timer<float> timer_;
	sf::CircleShape circle_shape_;
	sf::RenderTexture render_texture_;
	float radius_ = 10;

	sf::RectangleShape player_x;
	sf::RectangleShape player_y;
	
};

