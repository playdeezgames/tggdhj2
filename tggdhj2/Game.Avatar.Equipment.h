#pragma once
#include "Game.Difficulty.h"
#include "Game.Item.h"
#include "Game.EquipSlot.h"
#include <map>
#include <optional>
namespace game::avatar::Equipment
{
	std::map<EquipSlot, Item> ReadAll();
	std::optional<game::Item> Read(const EquipSlot&);
	void Equip(const Item&);
	void Unequip(const EquipSlot&);
	void Reset(const Difficulty&);
}
