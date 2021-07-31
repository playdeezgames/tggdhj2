#pragma once
#include "Game.Difficulty.h"
#include "Game.Item.h"
#include <map>
namespace game::nodes::Items
{
	size_t Read(int, const Item&);
	std::map<Item, size_t> Read(int);
	void Add(int, const Item&, size_t);
	void Remove(int, const Item&, size_t);
	void Reset(const Difficulty&);
}
