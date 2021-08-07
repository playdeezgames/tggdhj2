#pragma once
#include <functional>
namespace game::audio::Mux
{
	enum class Theme
	{
		MAIN,
		WIN,
		LOSE
	};
	void Play(const Theme&);
	std::function<void()> GoToTheme(const Theme&);
}

