#include "Game.Items.h"
#include <map>
namespace game::Items
{
	const std::map<Item, ItemDescriptor> descriptors =
	{
		{
			Item::TROUSERS, 
			{
				"Trousers",	
				{
					{game::Difficulty::EASY, 1},
					{game::Difficulty::NORMAL, 1},
					{game::Difficulty::HARD, 1},
					{game::Difficulty::HARDCORE, 1}
				}
			}
		},
		{
			Item::CODPIECE, 
			{
				"Codpiece",
				{
					{game::Difficulty::EASY, 1},
					{game::Difficulty::NORMAL, 1},
					{game::Difficulty::HARD, 1},
					{game::Difficulty::HARDCORE, 1}
				}
			}
		},
		{
			Item::WATERSKIN, 
			{
				"Waterskin",
				{
					{game::Difficulty::EASY, 1},
					{game::Difficulty::NORMAL, 1},
					{game::Difficulty::HARD, 1},
					{game::Difficulty::HARDCORE, 1}
				}
			}
		}
	};
	static std::list<Item> allItems;

	const ItemDescriptor& Read(const Item& item)
	{
		return descriptors.find(item)->second;
	}

	const std::list<Item>& All()
	{
		if (allItems.empty())
		{
			for (auto descriptor : descriptors)
			{
				allItems.push_back(descriptor.first);
			}
		}
		return allItems;
	}

}
