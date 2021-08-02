#pragma once
#include "Game.Difficulty.h"
#include <map>
#include <string>
namespace game
{
	struct ItemDescriptor
	{
		std::string name;
		std::string pickUpText;
		std::map<game::Difficulty, size_t> itemCount;
	};
}
