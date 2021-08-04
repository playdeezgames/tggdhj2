#include "Common.Utility.h"
#include "Game.Avatar.Items.h"
#include "Game.Avatar.Log.h"
#include "Game.Items.h"
#include "Game.Nodes.Items.h"
#include <map>
#include "Visuals.Data.Colors.h"
namespace game::Items
{
	static std::function<void(int)> DoStandardPickUp(const std::string& pickUpMessage, const game::Item& item)
	{
		return [pickUpMessage, item](int positionId) 
		{
			game::nodes::Items::Remove(positionId, item, 1);
			game::avatar::Log::Write({ visuals::data::Colors::NORMAL, pickUpMessage });
			game::avatar::Items::Add(item, 1);
		};
	}

	static void FoundCar(int)
	{
		game::avatar::Log::Write({ visuals::data::Colors::HIGHLIGHT, "Dude! You found yer car! Sweet!"});
	}

	const std::map<Item, ItemDescriptor> descriptors =
	{
		{
			Item::TROUSERS, 
			{
				"Trousers",	
				DoStandardPickUp("You pick up trousers.", Item::TROUSERS),
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
				DoStandardPickUp("You pick up a codpiece.", Item::CODPIECE),
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
				DoStandardPickUp("You pick up a waterskin.", Item::WATERSKIN),
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
				DoStandardPickUp("You pick up a compass.", Item::COMPASS),
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
			Item::FOUNTAIN,
			{
				"Fountain",
				std::nullopt,
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
			Item::YER_CAR,
			{
				"Yer car",
				FoundCar,
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
			Item::TRANSFUNCTIONER,
			{
				"Transfunctioner",
				DoStandardPickUp("Its mystery is only exceeded by its power!", Item::TRANSFUNCTIONER),
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
