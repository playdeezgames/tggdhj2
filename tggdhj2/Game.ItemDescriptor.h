#pragma once
#include "Game.Difficulty.h"
#include "Game.EquipSlot.h"
#include <map>
#include <optional>
#include <string>
namespace game
{
	struct ItemDescriptor
	{
		std::string name;
		std::string pickUpText;
		std::map<game::Difficulty, size_t> itemCount;
		std::optional<game::EquipSlot> equipSlot;
	};
}
