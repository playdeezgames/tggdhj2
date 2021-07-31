#pragma once
#include "Game.Difficulty.h"
#include "Game.Item.h"
namespace game::nodes::Items
{
	size_t Read(int, const Item&);
	void Add(int, const Item&, size_t);
	void Remove(int, const Item&, size_t);
	void Reset(const Difficulty&);
}
