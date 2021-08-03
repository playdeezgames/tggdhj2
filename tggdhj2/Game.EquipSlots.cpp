#include "Common.Utility.h"
#include "Game.EquipSlots.h"
#include <map>
namespace game::EquipSlots
{
	const std::map<EquipSlot, EquipSlotDescriptor> descriptors =
	{
		{
			EquipSlot::LEGS,
			{
				"Legs"
			}
		},
		{
			EquipSlot::GROIN,
			{
				"Groin"
			}
		}
	};
	const std::list<EquipSlot> allEquipSlots = common::Utility::ExtractListFromMapKeys(descriptors);

	const EquipSlotDescriptor& Read(const EquipSlot& equipSlot)
	{
		return descriptors.find(equipSlot)->second;
	}

	const std::list<EquipSlot>& All()
	{
		return allEquipSlots;
	}
}
