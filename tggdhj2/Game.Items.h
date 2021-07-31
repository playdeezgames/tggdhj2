#pragma once
#include "Game.Item.h"
#include "Game.ItemDescriptor.h"
#include <list>
namespace game::Items
{
	const ItemDescriptor& Read(const Item&);
	const std::list<Item>& All();
}
