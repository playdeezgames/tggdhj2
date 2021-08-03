#include <format>
#include "Game.Avatar.h"
#include "Game.Avatar.Facing.h"
#include "Game.Avatar.Items.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Directions.h"
#include "Visuals.Data.Colors.h"
#include "Visuals.HorizontalAlignment.h"
namespace state::in_play::Node
{
	const std::string FORMAT_HEALTH = "\x80{:3.0f}";
	const std::string FORMAT_HYDRATION = "\x81{:3.0f}";
	const std::string FORMAT_SCORE = "\x82{:2.0f}%";
	const std::string FORMAT_COMPASS = "\x83 {}";
	const int STATISTICS_ROW_OFFSET = 1;
	const size_t GRID_COLUMNS = 39;

	void WriteGridText(int, int, const std::string&, const std::string&, const visuals::HorizontalAlignment&);

	static void RefreshScore()
	{
		WriteGridText(
			GRID_COLUMNS,
			STATISTICS_ROW_OFFSET - 1,//TODO: magic number
			std::format(FORMAT_SCORE, game::Avatar::GetScore()),
			visuals::data::Colors::HIGHLIGHT,
			visuals::HorizontalAlignment::RIGHT);
	}

	static void RefreshHealth()
	{
		WriteGridText(
			GRID_COLUMNS, STATISTICS_ROW_OFFSET+0,//TODO: magic number
			std::format(FORMAT_HEALTH, game::avatar::Statistics::GetHealth()),
			visuals::data::Colors::HEALTH,
			visuals::HorizontalAlignment::RIGHT);
	}

	static void RefreshHydration()
	{
		WriteGridText(
			GRID_COLUMNS, STATISTICS_ROW_OFFSET + 1,//TODO: magic number
			std::format(FORMAT_HYDRATION, game::avatar::Statistics::GetHydration()),
			visuals::data::Colors::HYDRATION,
			visuals::HorizontalAlignment::RIGHT);
	}

	static void RefreshCompass()
	{
		if (game::avatar::Items::Read(game::Item::COMPASS)>0)
		{
			WriteGridText(
				GRID_COLUMNS, STATISTICS_ROW_OFFSET + 2,//TODO: magic number
				std::format(FORMAT_COMPASS, game::Directions::Read(game::avatar::Facing::Read()).abbreviation),
				visuals::data::Colors::DISABLED,
				visuals::HorizontalAlignment::RIGHT);
		}
	}

	void RefreshStatistics()
	{
		RefreshHealth();
		RefreshHydration();
		RefreshScore();
		RefreshCompass();
	}
}