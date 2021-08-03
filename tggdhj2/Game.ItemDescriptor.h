#pragma once
#include "Game.Difficulty.h"
#include "Game.EquipSlot.h"
#include <map>
#include <set>
#include <string>
namespace game
{
	struct ItemDescriptor
	{
		std::string name;
		std::string pickUpText;
		std::map<game::Difficulty, size_t> itemCount;
		std::set<game::EquipSlot> equipSlots;
	};
}
