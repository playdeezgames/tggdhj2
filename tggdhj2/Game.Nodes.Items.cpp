#include "Common.RNG.h"
#include "Data.Game.Node.h"
#include "Data.Game.Node.Item.h"
#include "Game.Items.h"
#include "Game.Nodes.Items.h"
#include <map>
namespace game::nodes::Items
{
	size_t Read(int positionId, const Item& item)
	{
		return data::game::node::Item::Read(positionId, (int)item);
	}

	std::map<Item, size_t> Read(int positionId)
	{
		std::map<Item, size_t> results;
		for (auto item : game::Items::All())
		{
			auto itemCount = Read(positionId, item);
			if (itemCount > 0)
			{
				results[item] = itemCount;
			}
		}
		return results;
	}


	void Add(int positionId, const Item& item, size_t count)
	{
		data::game::node::Item::Write(positionId, (int)item, count + Read(positionId, item));
	}

	void Remove(int positionId, const Item& item, size_t count)
	{
		auto current = Read(positionId, item);
		data::game::node::Item::Write(positionId, (int)item, (current>count) ? (current-count) : (0));
	}

	static void GenerateItems(const Difficulty& difficulty)
	{
		auto allNodes = data::game::Node::All();
		std::map<int, size_t> generator;
		for (auto node : allNodes)
		{
			generator[node] = 1;
		}
		auto items = game::Items::All();
		for (auto item : items)
		{
			auto& descriptor = game::Items::Read(item);
			for (size_t index = 0; index < descriptor.itemCount.find(difficulty)->second; ++index)
			{
				auto positionId = common::RNG::FromGenerator(generator, 0);
				Add(positionId, item, 1);
			}
		}
	}

	void Reset(const Difficulty& difficulty)
	{
		data::game::node::Item::Clear();
		GenerateItems(difficulty);
	}

}
