#include "Common.Utility.h"
#include "Game.Statistics.h"
#include <map>
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
				100.0
			}
		},
		{
			game::Statistic::HYDRATION, 
			{
				"Hydration",
				100.0,
				0.0,
				100.0
			}
		},
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