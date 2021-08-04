#include "Data.Game.Avatar.Equipment.h"
#include <format>
#include "Game.Avatar.Equipment.h"
#include "Game.Avatar.Items.h"
#include "Game.Avatar.Log.h"
#include "Game.Items.h"
#include "Visuals.Data.Colors.h"
namespace game::avatar::Equipment
{
	const std::string FORMAT_EQUIP = "You equip {}.";
	const std::string FORMAT_UNEQUIP = "You unequip {}.";

	std::map<EquipSlot, Item> ReadAll()
	{
		std::map<EquipSlot, Item> results;
		return results;
	}

	std::optional<game::Item> Read(const EquipSlot& equipSlot)
	{
		auto itemId = data::game::avatar::Equipment::Read((int)equipSlot);
		if (itemId)
		{
			return (game::Item)itemId.value();
		}
		return std::nullopt;
	}

	void Equip(const Item& item)
	{
		if (game::avatar::Items::Read(item) > 0)
		{
			auto descriptor = game::Items::Read(item);
			if (descriptor.equipSlot)
			{
				Unequip(descriptor.equipSlot.value());
				game::avatar::Items::Remove(item, 1);
				data::game::avatar::Equipment::Write((int)descriptor.equipSlot.value(), (int)item);
				game::avatar::Log::Write({ visuals::data::Colors::NORMAL, std::format(FORMAT_EQUIP, descriptor.name) });
			}
		}
	}

	void Unequip(const EquipSlot& equipSlot)
	{
		auto equipped = game::avatar::Equipment::Read(equipSlot);
		if (equipped)
		{
			data::game::avatar::Equipment::Clear((int)equipSlot);
			game::avatar::Items::Add(equipped.value(), 1, false);
			auto descriptor = game::Items::Read(equipped.value());
			game::avatar::Log::Write({ visuals::data::Colors::NORMAL, std::format(FORMAT_UNEQUIP, descriptor.name) });
		}
	}

	void Reset(const Difficulty&)
	{
		data::game::avatar::Equipment::ClearAll();
	}

}
