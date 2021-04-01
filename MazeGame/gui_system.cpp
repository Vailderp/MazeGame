#pragma once
#include <iostream>
#include <SFML/Graphics.hpp>
namespace gui
{

	class Events
	{
	public:

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

		virtual void events()
		{

		}

	protected:

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override
		{

		}

	};

	class Button;

	class Border final : public GuiElement
	{

		friend class Button;
		
	private:
		sf::RectangleShape u_;
		sf::RectangleShape d_;
		sf::RectangleShape l_;
		sf::RectangleShape r_;
		float thickness_;
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

		void setFillColor(const sf::Color color)
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

		void setThickness(const float thickness)
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

		void events() override
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
		
		Border border_{};

	public:
		
		void setBorderThickness(const float thickness)
		{
			border_.setThickness(thickness);
		}

		float getBorderThickness() const
		{
			return border_.thickness_;
		}

		void setBorderFillColor(const sf::Color color)
		{
			border_.setFillColor(color);
		}

		sf::Color getBorderFillColor() const
		{
			return border_.u_.getFillColor();
		}
		
	};


	class Drawable
	{
	private:
		std::vector<GuiElement* > gui_elements_;

	public:

		Drawable() = default;

		Drawable(const sf::Window& window)
		{

		}


		void addGuiElement(GuiElement* element)
		{
			gui_elements_.push_back(element);
		}

		void operator << (GuiElement* element)
		{
			addGuiElement(element);
		}

		void draw(sf::RenderWindow& window)
		{

			for (auto* it : gui_elements_)
			{
				window.draw(*it);
				it->events();

				if (sf::Mouse::getPosition(window).x > it->q_position_.x.x)
				{
					if (sf::Mouse::getPosition(window).x < it->q_position_.y.y)
					{
						if (sf::Mouse::getPosition(window).y > it->q_position_.x.y)
						{
							if (sf::Mouse::getPosition(window).y < it->q_position_.y.x)
							{
								std::cout << "cls" << std::endl;
							}
						}
					}
				}
			}
		}
	};
};