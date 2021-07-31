#pragma once
#include "Game.Difficulty.h"
#include <optional>
namespace game::avatar::Position
{
	std::optional<int> Read();
	void Write(int);
	void Reset(const game::Difficulty&);
}
