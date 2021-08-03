#pragma once
#include "Game.EquipSlot.h"
#include "Game.EquipSlotDescriptor.h"
#include <list>
namespace game::EquipSlots
{
	const EquipSlotDescriptor& Read(const EquipSlot&);
	const std::list<EquipSlot>& All();
}
