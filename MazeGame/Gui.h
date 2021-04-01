#pragma once
#include <functional>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <utility>

namespace gui
{

	class Events
	{
	public:
		bool is_mouse_move;
		bool is_mouse_clicked;

		explicit Events(const bool is_mouse_move = false,
		                const bool is_mouse_clicked = false) :
			is_mouse_move(is_mouse_move),
			is_mouse_clicked(is_mouse_clicked)
		{

		}
	};

	class Drawable;


	class GuiElement : private sf::Drawable
	{

		friend class gui::Drawable;

	protected:
		sf::Vector2f size_;
		sf::Vector2f position_;
		sf::Vector2<sf::Vector2f> q_position_;

	public:

		GuiElement() = default;

		virtual ~GuiElement() = default;

		GuiElement(const float x, const float y, const float width, const float height) :
			size_(width, height),
			position_(x, y),
			q_position_(sf::Vector2f(x, y), sf::Vector2f(x + width, y + height))
		{

		}

		GuiElement(const sf::Vector2f position, const sf::Vector2f size) :
			size_(size),
			position_(position),
			q_position_(position, sf::Vector2f(position.x + size.x, position.y + size.y))
		{

		}

		void setPosition(const float x, const float y)
		{
			position_.x = x;
			position_.y = y;
		}

		void setPosition(const sf::Vector2f position)
		{
			position_ = position;
		}

		sf::Vector2f getPosition() const
		{
			return position_;
		}

		void setSize(const float x, const float y)
		{
			size_.x = x;
			size_.y = y;
		}

		void setSize(const sf::Vector2f size)
		{
			size_ = size;
		}

		sf::Vector2f getSize() const
		{
			return size_;
		}

		virtual void setFillColor(const sf::Color& color)
		{
			
		}
		
		virtual void setThickness(const float thickness)
		{

		}

		virtual void events(Events events)
		{

		}

	protected:

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override
		{

		}

	};

	class Button;

	class Border : public GuiElement
	{

		friend class Button;

	private:
		sf::RectangleShape u_;
		sf::RectangleShape d_;
		sf::RectangleShape l_;
		sf::RectangleShape r_;
		float thickness_{};
	public:

		Border() = default;

		Border(const sf::Vector2f position, const sf::Vector2f size, const float thickness, const sf::Color color) :
			GuiElement(position, size),
			thickness_(thickness)
		{
			setFillColor(color);
		}

		Border(const float x, const float y, const float width, const float height, const float thickness, const sf::Color color) :
			GuiElement(x, y, width, height),
			thickness_(thickness)
		{
			setFillColor(color);
		}

		void setFillColor(const sf::Color& color) override
		{
			u_.setFillColor(color);
			d_.setFillColor(color);
			l_.setFillColor(color);
			r_.setFillColor(color);
		}

		sf::Color getFillColor() const
		{
			return u_.getFillColor();
		}

		void setThickness(const float thickness) override
		{
			thickness_ = thickness;
		}

		float getThickness() const
		{
			return thickness_;
		}

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override
		{
			target.draw(u_);
			target.draw(d_);
			target.draw(l_);
			target.draw(r_);
		}

		void events(Events events) override
		{
			u_.setPosition(this->position_.x, this->position_.y);
			u_.setSize(sf::Vector2f(this->size_.x, thickness_));

			d_.setPosition(this->position_.x, this->position_.y + this->size_.y - thickness_);
			d_.setSize(sf::Vector2f(this->size_.x, thickness_));

			l_.setPosition(this->position_.x, this->position_.y);
			l_.setSize(sf::Vector2f(thickness_, this->size_.y));

			r_.setPosition(this->position_.x + this->size_.x - thickness_, this->position_.y);
			r_.setSize(sf::Vector2f(thickness_, this->size_.y));
		}
	};

	class Button : public GuiElement
	{
	private:

		GuiElement* border_{};

		std::function<void()> onClick_function_;
		std::function<void()> onMove_function_;
		std::function<void()> onOut_function_;
		std::function<void()> onUp_function_;

		sf::Sprite button_sprite_;
		sf::Texture button_texture_;

	public:

		Button(const float x = 0,
			const float y = 0,
			const float width = 0,
			const float height = 0,
			const std::string& texture_path = "", 
			std::function<void()> on_click_function = []() {},
			std::function<void()> on_move_function = []() {},
			std::function<void()> on_out_function = []() {},
			std::function<void()> on_up_function = []() {}) :
		
			GuiElement(x, y, width, height),
			onClick_function_(std::move(on_click_function)),
			onMove_function_(std::move(on_move_function)),
			onOut_function_(std::move(on_out_function)),
			onUp_function_(std::move(on_up_function))
		{
			if (!texture_path.empty())
			{
				button_texture_.loadFromFile(texture_path);
				button_sprite_.setTexture(button_texture_);
				
				border_ = new Border(x, y, width, height, 4, sf::Color::Black);
			
			}
		}

		void setTexture(const std::string& texture_path)
		{
			button_texture_.loadFromFile(texture_path);
			button_sprite_.setTexture(button_texture_);
		}

		void setSprite(const sf::Sprite& sprite)
		{
			button_sprite_ = sprite;
		}

		sf::Sprite getSprite() const
		{
			return button_sprite_;
		}

		void setBorderThickness(const float thickness) const
		{
			border_->setThickness(thickness);
		}

		void setBorderFillColor(const sf::Color color) const
		{
			border_->setFillColor(color);
		}

		void onClick(const std::function<void()>& function)
		{
			onClick_function_ = function;
		}

		void onUp(const std::function<void()>& function)
		{
			onUp_function_ = function;
		}

		void onMove(const std::function<void()>& function)
		{
			onMove_function_ = function;
		}

		void onOut(const std::function<void()>& function)
		{
			onOut_function_ = function;
		}

		void events(Events events) override
		{
			button_sprite_.setPosition(this->position_);
			button_sprite_.setScale(this->size_.x / button_texture_.getSize().x, this->size_.y / button_texture_.getSize().y);

			if (events.is_mouse_clicked)
			{
				onClick_function_();
			}
			else
			{
				onUp_function_();
			}

			if (events.is_mouse_move)
			{
				onMove_function_();
			}
			else
			{
				onOut_function_();
			}
		}

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override
		{
			target.draw(button_sprite_);
		}

	};

	class Drawable
	{
	private:
		std::map<std::string, GuiElement* > gui_elements_;

	public:

		Drawable() = default;

		Drawable(const sf::Window& window)
		{

		}


		void addGuiElement(GuiElement* element, const std::string& name)
		{
			gui_elements_[name] = (element);
		}

		void draw(sf::RenderWindow& window)
		{

			for (const auto& it : gui_elements_)
			{
				window.draw(*it.second);
				Events events{};
				if (sf::Mouse::getPosition(window).x > it.second->q_position_.x.x)
				{
					if (sf::Mouse::getPosition(window).x < it.second->q_position_.y.y)
					{
						if (sf::Mouse::getPosition(window).y > it.second->q_position_.x.y)
						{
							if (sf::Mouse::getPosition(window).y < it.second->q_position_.y.x)
							{
								events.is_mouse_move = true;
								if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
								{
									events.is_mouse_clicked = true;
								}
							}
						}
					}
				}
				it.second->events(events);
			}
		}
	};
};