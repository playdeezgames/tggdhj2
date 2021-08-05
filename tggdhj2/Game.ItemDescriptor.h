#pragma once
#include <functional>
#include "Game.Difficulty.h"
#include "Game.Item.h"
#include "Game.EquipSlot.h"
#include <map>
#include <optional>
#include <string>
namespace game
{
	struct ItemDescriptor
	{
		std::string name;
		std::optional<std::function<void(int)>> onPickUp;
		std::optional<std::function<void()>> onInteract;
		std::optional<std::function<void()>> onEquip;
		std::optional<std::function<void()>> onUnequip;
		std::map<game::Difficulty, size_t> itemCount;
		std::optional<game::EquipSlot> equipSlot;
	};
}
