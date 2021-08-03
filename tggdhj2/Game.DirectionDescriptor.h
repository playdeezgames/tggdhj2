#pragma once
#include "Game.Direction.h"
#include <string>
namespace game
{
	struct DirectionDescriptor
	{
		std::string name;
		std::string abbreviation;
		Direction next;
		Direction opposite;
		Direction previous;
	};
}