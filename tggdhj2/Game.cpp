#include "Data.SQLite.Stores.h"
#include <functional>
#include "Game.h"
#include "Game.Avatar.Counters.h"
#include "Game.Avatar.Equipment.h"
#include "Game.Avatar.Facing.h"
#include "Game.Avatar.Items.h"
#include "Game.Avatar.Log.h"
#include "Game.Avatar.Position.h"
#include "Game.Avatar.Visits.h"
#include "Game.Nodes.h"
#include "Game.Nodes.Items.h"
#include <list>
#include <map>
namespace game
{
	const std::list<std::function<void(const Difficulty&)>> resetters =
	{
		game::Nodes::Reset,
		game::avatar::Log::Reset,
		game::nodes::Items::Reset,
		game::avatar::Visits::Reset,
		game::avatar::Position::Reset,
		game::avatar::Items::Reset,
		game::avatar::Facing::Reset,
		game::avatar::Counters::Reset,
		game::avatar::Equipment::Reset
	};

	void Reset(const Difficulty& difficulty)
	{
		data::sqlite::Stores::Bounce(data::sqlite::Store::IN_MEMORY);
		for (auto resetter : resetters)
		{
			resetter(difficulty);
		}
	}

	void Start()
	{
		Reset(Difficulty::NORMAL);
	}

	void AutoSave()
	{
		data::sqlite::Stores::Copy(data::sqlite::Store::IN_MEMORY, data::sqlite::Store::AUTOSAVE);
	}

	void LoadFromAutosave()
	{
		data::sqlite::Stores::Copy(data::sqlite::Store::AUTOSAVE, data::sqlite::Store::IN_MEMORY);
	}

	static bool DoesWorldExistInStore(const data::sqlite::Store& store)
	{
		return !data::sqlite::Stores::Execute(store, "SELECT name FROM sqlite_master WHERE type='table' AND name='Nodes';").empty();
	}

	bool DoesAutosaveExist()
	{
		return DoesWorldExistInStore(data::sqlite::Store::AUTOSAVE);
	}

	const std::map<int, data::sqlite::Store> slotTable =
	{
		{1, data::sqlite::Store::SLOT_1},
		{2, data::sqlite::Store::SLOT_2},
		{3, data::sqlite::Store::SLOT_3},
		{4, data::sqlite::Store::SLOT_4},
		{5, data::sqlite::Store::SLOT_5}
	};

	bool DoesSlotExist(int slot)
	{
		auto iter = slotTable.find(slot);
		if (iter != slotTable.end())
		{
			return DoesWorldExistInStore(iter->second);
		}
		return false;
	}

	void LoadFromSlot(int slot)
	{
		auto iter = slotTable.find(slot);
		if (iter != slotTable.end())
		{
			data::sqlite::Stores::Copy(iter->second, data::sqlite::Store::IN_MEMORY);
		}
	}

	void SaveToSlot(int slot)
	{
		auto iter = slotTable.find(slot);
		if (iter != slotTable.end())
		{
			data::sqlite::Stores::Copy(data::sqlite::Store::IN_MEMORY, iter->second);
		}
	}

}