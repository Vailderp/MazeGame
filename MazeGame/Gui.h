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


	class GuiElement : public sf::Drawable
	{

		friend class gui::Drawable;

	protected:
		sf::Vector2f size_;
		sf::Vector2f position_;
		sf::Vector2<sf::Vector2i> q_position_;

		std::function<void()> onClick_function_ = [](){};
		std::function<void()> onMove_function_ = []() {};
		std::function<void()> onOut_function_ = []() {};
		std::function<void()> onUp_function_ = []() {};

	public:

		GuiElement() = default;

		virtual ~GuiElement() = default;

		GuiElement(const float x, const float y, const float width, const float height) :
			size_(width, height),
			position_(x, y),
			q_position_(sf::Vector2i(x, y), sf::Vector2i(x + width, y + height))
		{

		}

		GuiElement(const sf::Vector2f position, const sf::Vector2f size) :
			size_(size),
			position_(position),
			q_position_(sf::Vector2i( position.x, position.y ), sf::Vector2i(position.x + size.x, position.y + size.y))
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

		virtual void events(const Events events) = 0;

	protected:

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override = 0;

	};

	class Button;

	class Button final : public GuiElement
	{
	private:

		sf::Sprite button_sprite_;
		sf::Texture button_texture_;
		sf::Text text_;

	public:

		Button(const float x = 0,
			const float y = 0,
			const float width = 0,
			const float height = 0,
			const std::string& texture_path = "") :
	
			GuiElement(x, y, width, height)
		
		{
			if (!texture_path.empty())
			{
				button_texture_.loadFromFile(texture_path);
				button_sprite_.setTexture(button_texture_);
			}
		}

		void setTexture(const std::string& texture_path)
		{
			button_texture_.loadFromFile(texture_path);
			button_sprite_.setTexture(button_texture_);
		}

		void onClick(const std::function<void()>& function)
		{
			this->onClick_function_ = function;
		}

		void onUp(const std::function<void()>& function)
		{
			this->onUp_function_ = function;
		}

		void onMove(const std::function<void()>& function)
		{
			this->onMove_function_ = function;
		}

		void onOut(const std::function<void()>& function)
		{
			this->onOut_function_ = function;
		}

		void events(const Events events) override
		{
			button_sprite_.setPosition(this->position_);
			button_sprite_.setScale(this->size_.x / button_texture_.getSize().x, this->size_.y / button_texture_.getSize().y);

			if (events.is_mouse_clicked)
			{
				this->onClick_function_();
			}
			else
			{
				this->onUp_function_();
			}

			if (events.is_mouse_move)
			{
				this->onMove_function_();
			}
			else
			{
				this->onOut_function_();
			}
		}

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override
		{
			target.draw(button_sprite_);
			target.draw(text_);
		}

	};

	class Drawable : public sf::Drawable
	{
	private:
		std::vector<GuiElement *> gui_elements_;
		sf::Vector2i *position_;

	public:

		Drawable() = default;



		void addGuiElement(GuiElement* element)
		{
			gui_elements_.emplace_back(element);
		}

		void setPositionStates(sf::Vector2i *position)
		{
			position_ = position;
		}

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override
		{
			for (GuiElement *const &it : gui_elements_)
			{
				target.draw( *it);
				Events events{};
				if (position_->x > it->q_position_.x.x)
				{
					if (position_->y < it->q_position_.y.y)
					{
						if (position_->y > it->q_position_.x.y)
						{
							if (position_->x < it->q_position_.y.x)
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
				it->events(events);
			}
		}
	};
};