#include "Common.Utility.h"
#include "Game.Statistics.h"
#include <map>
#include "Visuals.Data.Colors.h"
namespace game::Statistics
{
	const std::map<game::Statistic, game::StatisticDescriptor> descriptors =
	{
		{
			game::Statistic::HEALTH, 
			{
				"Health",
				100.0,
				0.0,
				100.0,
				visuals::data::Colors::HEALTH,
				"\x80{:3.0f}"
			}
		},
		{
			game::Statistic::HYDRATION, 
			{
				"Hydration",
				100.0,
				0.0,
				100.0,
				visuals::data::Colors::HYDRATION,
				"\x81{:3.0f}"
			}
		},
		{
			game::Statistic::CONFIDENCE,
			{
				"Confidence",
				0.0,
				0.0,
				std::nullopt,
				visuals::data::Colors::NORMAL,
				"Confidence: {:.0f}"
			}
		},
		{
			game::Statistic::DIGNITY,
			{
				"Dignity",
				0.0,
				0.0,
				std::nullopt,
				visuals::data::Colors::NORMAL,
				"Dignity: {:.0f}"
			}
		}
	};

	const std::list<game::Statistic> allStatistics = common::Utility::ExtractListFromMapKeys(descriptors);

	const game::StatisticDescriptor& Read(const game::Statistic& statistic)
	{
		return descriptors.find(statistic)->second;
	}

	const std::list<game::Statistic>& All()
	{
		return allStatistics;
	}
}