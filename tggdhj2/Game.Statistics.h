#pragma once
#include "Game.Statistic.h"
#include "Game.StatisticDescriptor.h"
#include <list>
namespace game::Statistics
{
	const game::StatisticDescriptor& Read(const game::Statistic&);
	const std::list<game::Statistic>& All();
}
