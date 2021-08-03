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
				"You pick up trousers.",
				{
					{game::Difficulty::EASY, 1},
					{game::Difficulty::NORMAL, 1},
					{game::Difficulty::HARD, 1},
					{game::Difficulty::HARDCORE, 1}
				},
				{
					EquipSlot::LEGS
				}
			}
		},
		{
			Item::CODPIECE, 
			{
				"Codpiece",
				"You pick up a codpiece.",
				{
					{game::Difficulty::EASY, 1},
					{game::Difficulty::NORMAL, 1},
					{game::Difficulty::HARD, 1},
					{game::Difficulty::HARDCORE, 1}
				},
				{
					EquipSlot::GROIN
				}
			}
		},
		{
			Item::WATERSKIN, 
			{
				"Waterskin",
				"You pick up a waterskin.",
				{
					{game::Difficulty::EASY, 1},
					{game::Difficulty::NORMAL, 1},
					{game::Difficulty::HARD, 1},
					{game::Difficulty::HARDCORE, 1}
				},
				{
				}
			}
		},
		{
			Item::COMPASS,
			{
				"Compass",
				"You pick up a compass.",
				{
					{game::Difficulty::EASY, 1},
					{game::Difficulty::NORMAL, 1},
					{game::Difficulty::HARD, 1},
					{game::Difficulty::HARDCORE, 1}
				},
				{
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
