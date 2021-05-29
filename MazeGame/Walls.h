#pragma once

#include "Engine3d.h"
#include "Gui.h"

/*
 * С помощью Engine3d.h
 * можно создавать стены с неограниченными возможностями
 */

class CircleWall final : public v3d::Wall
{
public:
	explicit CircleWall(const bool is_transparent = false) :
		Wall(is_transparent)
	{
		render_texture_.create(1000, 1000);
		circle_shape_.setRadius(radius_);
		mouse_cursor_shape_.setRadius(10);
		circle_shape_.setOrigin(radius_, radius_);
		mouse_cursor_shape_.setOrigin(10, 10);
		circle_shape_.setFillColor(sf::Color::Red);

		player_x_.setPosition(30, 400);
		player_x_.setSize({ 50, 200 });
		player_x_.setFillColor(sf::Color::Green);

		player_y_.setPosition(1000 - 30 - 50, 400);
		player_y_.setSize({ 50, 200 });
		player_y_.setFillColor(sf::Color::Green);

		score_font_.loadFromFile("data/fonts/pix.otf");
		score_text_.setFont(score_font_);
		score_text_.setCharacterSize(30);
		score_text_.setPosition(800, 800);
		score_text_.setScale(1, -1);

		/*drawable_.setPositionStates(&mouse_position_);
		drawable_.addGuiElement(&button_play_);
		button_play_.setPosition(100, 100);
		button_play_.setSize(400, 350);
		button_play_.onMove([&]()
			{
				button_play_.setTexture("data/tex/4.png");
			});
		button_play_.onOut([&]()
			{
				button_play_.setTexture("data/tex/8.png");
			});*/
	}

protected:
	void wall_states(const v3d::ray_data& data) override
	{
		circle_shape_.move(speed_.x, speed_.y);

		if (circle_shape_.getPosition().x > 1000)
		{
			speed_.x = static_cast<float>(math::rand(100, 1000)) / 1000.f * (speed_.x < 0 ? -1 : 1);
			speed_.y = static_cast<float>(math::rand(100, 1000)) / 1000.f * (speed_.y < 0 ? -1 : 1);
			score_.x++;
			score_text_.setString(std::to_string(score_.x) + " : " + std::to_string(score_.y));
			circle_shape_.setPosition(500, 500);
		}
		if (circle_shape_.getPosition().x < 0)
		{
			speed_.x = static_cast<float>(math::rand(100, 1000)) / 1000.f * (speed_.x < 0 ? -1 : 1);
			speed_.y = static_cast<float>(math::rand(100, 1000)) / 1000.f * (speed_.y < 0 ? -1 : 1);
			score_.y++;
			score_text_.setString(std::to_string(score_.x) + " : " + std::to_string(score_.y));
			circle_shape_.setPosition(500, 500);
		}

		if (circle_shape_.getPosition().y + radius_ > 1000)
		{
			speed_.y = -speed_.y;
			circle_shape_.move(0, -10);
		}
		if (circle_shape_.getPosition().y - radius_ < 0)
		{
			speed_.y = -speed_.y;
			circle_shape_.move(0, 10);
		}

		if (circle_shape_.getPosition().x - radius_ < player_x_.getPosition().x + 50 && circle_shape_.getPosition().y > player_x_.getPosition().y && circle_shape_.getPosition().y < player_x_.getPosition().y + 200)
		{
			speed_.x += 0.5;
			speed_.x = -speed_.x;
			speed_.y = static_cast<float>(math::rand(500, 2000)) / 1000.f * (speed_.y < 0 ? -1 : 1);
			circle_shape_.move(10, 0);
		}
		if (circle_shape_.getPosition().x + radius_ > player_y_.getPosition().x && circle_shape_.getPosition().y > player_y_.getPosition().y && circle_shape_.getPosition().y < player_y_.getPosition().y + 200)
		{
			speed_.x += 0.5;
			speed_.x = -speed_.x;
			speed_.y = static_cast<float>(math::rand(500, 2000)) / 1000.f * (speed_.y < 0 ? -1 : 1);
			circle_shape_.move(-10, 0);
		}

		if (speed_.x > 0 && circle_shape_.getPosition().x > 300)
		{
			if (player_y_.getPosition().y + 100 < circle_shape_.getPosition().y && player_y_.getPosition().y + powf(abs(player_y_.getPosition().y + 100 - circle_shape_.getPosition().y) / 2, 0.6f) > -10 && player_y_.getPosition().y + 200 + powf(abs(player_y_.getPosition().y + 100 - circle_shape_.getPosition().y) / 2, 0.6f) < 1010)
			{
				player_y_.move(0, powf(abs(player_y_.getPosition().y + 100 - circle_shape_.getPosition().y) / 2, 0.5f));
			}
			if (player_y_.getPosition().y + 100 > circle_shape_.getPosition().y && player_y_.getPosition().y - powf(abs(player_y_.getPosition().y + 100 - circle_shape_.getPosition().y) / 2, 0.6f) > -10 && player_y_.getPosition().y + 200 - powf(abs(player_y_.getPosition().y + 100 - circle_shape_.getPosition().y) / 2, 0.6f) < 1010)
			{
				player_y_.move(0, -powf(abs(player_y_.getPosition().y + 100 - circle_shape_.getPosition().y) / 2, 0.5f));
			}
		}

		render_texture_.clear(sf::Color::Blue);
		render_texture_.draw(mouse_cursor_shape_);
		render_texture_.draw(circle_shape_);
		render_texture_.draw(player_x_);
		render_texture_.draw(player_y_);
		render_texture_.draw(score_text_);
		this->texture_ = render_texture_.getTexture();
	}

	void wall_states_center_ray(const v3d::ray_data& data, const sf::Vector2<float> center_ray) override
	{
		//mouse_position_.x = center_ray.x;
		//mouse_position_.y = center_ray.y;
		//std::cout << mouse_position_.x << std::endl;
		if (timer_.milliseconds() > 10)
		{
			mouse_cursor_shape_.setPosition(center_ray);
			if (player_x_.getPosition().y + 100 < center_ray.y && player_x_.getPosition().y + powf(abs(player_x_.getPosition().y + 100 - center_ray.y) / 2, 0.6f) > -10 && player_x_.getPosition().y + 200 + powf(abs(player_x_.getPosition().y + 100 - center_ray.y) / 2, 0.6f) < 1010)
			{
				player_x_.move(0, powf(abs(player_x_.getPosition().y + 100 - center_ray.y) / 2, 0.5f));
			}
			if (player_x_.getPosition().y + 100 > center_ray.y && player_x_.getPosition().y - powf(abs(player_x_.getPosition().y + 100 - center_ray.y) / 2, 0.6f) > -10 && player_x_.getPosition().y + 200 - powf(abs(player_x_.getPosition().y + 100 - center_ray.y) / 2, 0.6f) < 1010)
			{
				player_x_.move(0, -powf(abs(player_x_.getPosition().y + 100 - center_ray.y) / 2, 0.5f));
			}

			//render_texture_.draw(drawable_);
			timer_.clear();
		}
	}

private:
	sf::Vector2f speed_ = { 3.5f, 0 };
	v3d::Timer<float> timer_;
	sf::CircleShape circle_shape_;
	sf::RenderTexture render_texture_;
	float radius_ = 15;
	sf::RectangleShape player_x_;
	sf::RectangleShape player_y_;
	sf::CircleShape mouse_cursor_shape_;

	sf::Vector2i score_;
	sf::Font score_font_;
	sf::Text score_text_;

	/*sf::Vector2i mouse_position_;
	gui::Drawable drawable_;
	gui::Button button_play_;*/
};
