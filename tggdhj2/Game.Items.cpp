#include "Application.UIState.h"
#include "Common.Audio.h"
#include "Common.Utility.h"
#include "Game.Avatar.Equipment.h"
#include "Game.Avatar.Items.h"
#include "Game.Avatar.Log.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Items.h"
#include "Game.Nodes.Items.h"
#include <map>
#include "Visuals.Data.Colors.h"
namespace game::Items
{
	const std::string SFX_AND_THEN = "andthen";

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

	static void AndThen(int)
	{
		common::audio::Sfx::Play(SFX_AND_THEN);
		game::avatar::Log::Write({ visuals::data::Colors::SUBHEADING, "AND THENNNNNN?" });
	}

	static void Zoltan(int)
	{
		//TODO: recording of me saying "ZOLTAN!"
		game::avatar::Log::Write({ visuals::data::Colors::SUBHEADING, "ZOLTAN!" });
	}

	static void NordicGuys(int)
	{
		//TODO: something
		application::UIState::Write(::UIState::IN_PLAY_NORDIC_GUYS);
	}

	static void HotChicks(int)
	{
		//TODO: something
		application::UIState::Write(::UIState::IN_PLAY_HOT_CHICKS);
	}

	static void InteractFountain(int)
	{
		game::avatar::Statistics::FullyHydrate();
		game::avatar::Log::Write({ visuals::data::Colors::NORMAL, "You drink from the fountain." });
	}

	static std::function<void()> DoStandardEquip(const game::Item& item)
	{
		return [item]() 
		{
			game::avatar::Equipment::Equip(item);
		};
	}

	static std::function<void()> DoLogMessage(const std::string& color, const std::string& message)
	{
		return [color, message]()
		{
			game::avatar::Log::Write({ color, message });
		};
	}

	const std::map<Item, ItemDescriptor> descriptors =
	{
		{
			Item::TROUSERS, 
			{
				"Trousers",	
				DoStandardPickUp("You pick up trousers.", Item::TROUSERS),
				DoStandardEquip(Item::TROUSERS),
				[]()
				{
					game::avatar::Log::Write({visuals::data::Colors::NORMAL, "You put them on one leg at a time."});
					game::avatar::Statistics::ChangeCurrent(game::Statistic::DIGNITY, 100.0);
				},
				[]()
				{
					game::avatar::Log::Write({visuals::data::Colors::NORMAL, "You remove yer trousers. Feel the breeze!"});
					game::avatar::Statistics::ChangeCurrent(game::Statistic::DIGNITY, -100.0);
				},
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
				DoStandardEquip(Item::CODPIECE),
				[]() 
				{
					game::avatar::Log::Write({visuals::data::Colors::NORMAL, "Yer confidence... enlarges."});
					game::avatar::Statistics::ChangeCurrent(game::Statistic::CONFIDENCE, 100.0);
				},
				[]() 
				{
					game::avatar::Log::Write({visuals::data::Colors::NORMAL, "You remove yer codpiece and feel... smaller!"});
					game::avatar::Statistics::ChangeCurrent(game::Statistic::CONFIDENCE, -100.0);
				},
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
			Item::COMPASS,
			{
				"Compass",
				DoStandardPickUp("You pick up a compass.", Item::COMPASS),
				std::nullopt,
				std::nullopt,
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
			Item::FOUNTAIN,
			{
				"Fountain",
				InteractFountain,
				std::nullopt,
				std::nullopt,
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
				std::nullopt,
				std::nullopt,
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
			Item::TRANSFUNCTIONER,
			{
				"Transfunctioner",
				DoStandardPickUp("Its mystery is only exceeded by its power!", Item::TRANSFUNCTIONER),
				std::nullopt,
				std::nullopt,
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
			Item::HOT_CHICKS,
			{
				"Hot chicks",
				HotChicks,
				std::nullopt,
				std::nullopt,
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
			Item::CHINESE_FOOOOOD_LADY,
			{
				"Chinese Foooood Lady",
				AndThen,
				std::nullopt,
				std::nullopt,
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
			Item::MACGUFFIN,
			{
				"Macguffin",
				DoStandardPickUp("You pick up the macguffin!", Item::MACGUFFIN),
				std::nullopt,
				std::nullopt,
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
			Item::NORDIC_GUYS,
			{
				"Nordic Guys",
				NordicGuys,
				std::nullopt,
				std::nullopt,
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
			Item::ZOLTAN,
			{
				"Zoltan",
				Zoltan,
				std::nullopt,
				std::nullopt,
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
