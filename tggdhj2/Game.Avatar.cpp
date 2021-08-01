#include "Game.Avatar.h"
#include "Game.Avatar.Visits.h"
#include "Game.Nodes.h"
namespace game::Avatar
{
	const double MAXIMUM_SCORE = 100.0;

	double GetScore()
	{
		return MAXIMUM_SCORE * game::avatar::Visits::Count() / game::Nodes::Count();
	}
}