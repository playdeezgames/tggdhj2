#pragma once
#include "Game.Difficulty.h"
#include "Game.Direction.h"
#include <optional>
namespace game::avatar::Position
{
	std::optional<int> Read();
	void Write(int);
	void Reset(const game::Difficulty&);
	void Move();
}
