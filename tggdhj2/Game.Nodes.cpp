#include "Game.Nodes.h"
#include "Data.Game.Node.h"
#include <map>
namespace game::Nodes
{
	const std::map<game::Difficulty, size_t> worldSizes =
	{
		{game::Difficulty::EASY, 20},
		{game::Difficulty::NORMAL, 40},
		{game::Difficulty::HARD, 60},
		{game::Difficulty::HARDCORE, 100}
	};

	void Reset(const game::Difficulty& difficulty)
	{
		data::game::Node::Clear();
		auto worldSize = worldSizes.find(difficulty)->second;
		for (int positionId = 1; positionId <= worldSize; ++positionId)
		{
			data::game::Node::Write(positionId);
		}
	}
}