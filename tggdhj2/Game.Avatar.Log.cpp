#include "Data.Game.Avatar.Log.h"
#include "Game.Avatar.Log.h"
#include "Visuals.Data.Colors.h"
namespace game::avatar::Log
{
	const std::string WELCOME_TEXT = "Now is the time to DISCOVER!";
	const size_t ENTRY_COUNT = 12;//TODO: maybe this could be configurable somewhere?

	void Reset(const Difficulty&)
	{
		data::game::avatar::Log::Clear();
		Write({ visuals::data::Colors::HIGHLIGHT,  WELCOME_TEXT });
	}

	std::list<LogEntry> Read()
	{
		std::list<LogEntry> results;
		auto entries = data::game::avatar::Log::Read(ENTRY_COUNT);
		for (auto entry : entries)
		{
			results.push_back(
				{
					std::get<0>(entry),
					std::get<1>(entry)
				});
		}
		return results;
	}

	void Write(const LogEntry& entry)
	{
		data::game::avatar::Log::Write(entry.color, entry.text);
	}
}
