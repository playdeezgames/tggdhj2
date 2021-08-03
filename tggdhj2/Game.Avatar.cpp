#include "Game.Avatar.h"
#include "Game.Avatar.Counters.h"
#include "Game.Avatar.Visits.h"
#include "Game.Nodes.h"
namespace game::Avatar
{
	const double MAXIMUM_SCORE = 100.0;

	double GetScore()
	{
		return MAXIMUM_SCORE * game::avatar::Visits::Count() / game::Nodes::Count();
	}

	bool HasWon()
	{
		return game::avatar::Visits::Count() == game::Nodes::Count();
	}

	double GetEffectiveness()
	{
		return MAXIMUM_SCORE * game::Nodes::Count() / (double)game::avatar::Counters::Read(avatar::Counter::MOVES_MADE);
	}
}