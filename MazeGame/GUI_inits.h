#pragma once
#include "Gui.h"
#include "Engine3d.h"

class MenuSettings
{
private:

	bool active_ = false;

	gui::Drawable drawable_{ 100, 100 };

	gui::Range range_fov_{ 30,3,60, 20 };
	gui::Range range_shading_coefficient_{ 20,26,60, 20 };
	gui::Range range_zoom_{ 20,49,60, 20 };
	gui::Range range_radius_{ 20,72,60, 20 };

	v3d::Camera* camera_;

public:
	explicit MenuSettings(sf::RenderWindow* window, v3d::Camera* camera) :
		camera_(camera)
	{
		drawable_.setWindow(window);
		drawable_ << &range_fov_ << &range_shading_coefficient_ << &range_zoom_ << &range_radius_;

		range_fov_.setMinValue(0.01_deg).setMaxValue(359.9_deg).setValue(camera_->getFov()).setOnChangeFunction(
			[&](const float value) -> void
			{
				camera_->setFov(value);
			}
		);

		range_shading_coefficient_
			.setMinValue(0.01F).
			setMaxValue(128.0F).
			setValue(camera_->getShadingCoefficient()).
			setOnChangeFunction(
				[&](const float value) -> void
				{
					camera_->setShadingCoefficient(value);
				}
		);

		range_zoom_.
			setMinValue(0.0000075F)
			.setMaxValue(7.5F)
			.setValue(camera_->getZoom())
			.setOnChangeFunction(
				[&](const float value) -> void
				{
					camera_->setZoom(value);
				}
		);
	}

	void setActive(const bool active)
	{
		active_ = active;
	}

	bool active() const
	{
		return active_;
	}

	void draw() const
	{
		if (active_)
		{
			drawable_.draw();
		}
	}
};

class Menu
{
private:

	bool active_ = true;
	bool settings_menu_active_ = false;

	gui::Drawable drawable_{ 100, 100,
		"data/tex/background.bmp" };

	gui::Button button_online_{ 25, 3, 50, 20,
		"data/tex/buttons/menu/online_click.png",
		"data/tex/buttons/menu/online_move.png",
		"data/tex/buttons/menu/online.png" };

	gui::Button button_single_{ 25, 26, 50, 20,
	"data/tex/buttons/menu/single_click.png",
	"data/tex/buttons/menu/single_move.png",
	"data/tex/buttons/menu/single.png" };

	gui::Button button_settings_{ 25, 49, 50, 20,
"data/tex/buttons/menu/settings_click.png",
"data/tex/buttons/menu/settings_move.png",
"data/tex/buttons/menu/settings.png" };

	gui::Button button_exit_{ 25, 72, 50, 20,
"data/tex/buttons/menu/exit_click.png",
"data/tex/buttons/menu/exit_move.png",
"data/tex/buttons/menu/exit.png" };

public:

	MenuSettings settings;

	explicit Menu(sf::RenderWindow* window, v3d::Camera* camera) :
		settings(window, camera)
	{
		button_single_.setOnDownFunction([&]() -> void
			{
				active_ = false;
				ShowCursor(true);
			});

		button_settings_.setOnDownFunction([&]() -> void
			{
				settings_menu_active_ = true;
				settings.setActive(true);
			});

		button_exit_.setOnDownFunction([]() -> void
			{
				exit(1);
			});

		drawable_ << &button_online_ << &button_single_ << &button_settings_ << &button_exit_;

		drawable_.setWindow(window);
	}

	void setActive(const bool active)
	{
		active_ = active;
	}

	bool active() const
	{
		return active_;
	}

	bool activeSettings() const
	{
		return settings_menu_active_;
	}

	void setActiveSettings(const bool active_settings)
	{
		settings_menu_active_ = active_settings;
		settings.setActive(active_settings);
	}

	void draw() const
	{
		if (active_ && !settings.active())
		{
			drawable_.draw();
		}
		else
		{
			settings.draw();
		}
	}
};