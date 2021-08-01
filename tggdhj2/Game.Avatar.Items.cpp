#include "Data.Game.Avatar.Item.h"
#include "Game.Avatar.Items.h"
#include "Game.Items.h"
namespace game::avatar::Items
{
	std::map<game::Item, size_t> ReadAll()
	{
		std::map<game::Item, size_t> results;
		for (auto item : game::Items::All())
		{
			auto itemCount = Read(item);
			if (itemCount > 0)
			{
				results[item] = itemCount;
			}
		}
		return results;
	}

	size_t Read(const game::Item& item)
	{
		return data::game::avatar::Item::Read((int)item);
	}

	void Add(const game::Item& item, size_t delta)
	{
		data::game::avatar::Item::Write((int)item, Read(item) + delta);
	}

	void Remove(const game::Item& item, size_t delta)
	{
		auto itemCount = Read(item);
		data::game::avatar::Item::Write((int)item, (itemCount>delta) ? (itemCount - delta) : (0));
	}

	void Reset(const game::Difficulty&)
	{
		data::game::avatar::Item::Clear();
	}

}