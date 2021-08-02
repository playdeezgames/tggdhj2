#include "Common.Utility.h"
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
		},
		{
			Item::COMPASS,
			{
				"Compass",
				{
					{game::Difficulty::EASY, 1},
					{game::Difficulty::NORMAL, 1},
					{game::Difficulty::HARD, 1},
					{game::Difficulty::HARDCORE, 1}
				}
			}
		}
	};

	const std::list<Item> allItems = common::Utility::ExtractListFromMapKeys(descriptors);

	const ItemDescriptor& Read(const Item& item)
	{
		return descriptors.find(item)->second;
	}

	const std::list<Item>& All()
	{
		return allItems;
	}

}
