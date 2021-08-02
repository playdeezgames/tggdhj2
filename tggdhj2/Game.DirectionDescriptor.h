#pragma once
#include "Game.Direction.h"
#include <string>
namespace game
{
	struct DirectionDescriptor
	{
		std::string name;
		Direction next;
		Direction opposite;
		Direction previous;
	};
}