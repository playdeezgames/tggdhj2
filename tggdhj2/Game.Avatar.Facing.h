#pragma once
#include "Game.Difficulty.h"
#include "Game.Direction.h"
namespace game::avatar::Facing
{
	Direction Read();
	void TurnLeft();
	void TurnRight();
	void TurnAround();
	void Reset(const Difficulty&);
}
