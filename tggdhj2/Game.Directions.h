#pragma once
#include "Game.Direction.h"
#include "Game.DirectionDescriptor.h"
#include <list>
namespace game::Directions
{
	const DirectionDescriptor& Read(const Direction&);
	const std::list<Direction>& All();
}
