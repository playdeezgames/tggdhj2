#include "Common.RNG.h"
#include "Data.Game.Node.h"
#include "Data.Game.Node.Path.h"
#include "Game.Direction.h"
#include "Game.Nodes.h"
#include <map>
namespace game::Nodes
{
	const std::map<game::Difficulty, size_t> worldSizes =
	{
		{game::Difficulty::EASY, 25},
		{game::Difficulty::NORMAL, 50},
		{game::Difficulty::HARD, 75},
		{game::Difficulty::HARDCORE, 100}
	};

	static void GenerateNodes(const game::Difficulty& difficulty)
	{
		auto worldSize = worldSizes.find(difficulty)->second;
		for (int positionId = 1; positionId <= worldSize; ++positionId)
		{
			data::game::Node::Write(positionId);
		}
	}

	static void GenerateNorthSouthPaths(const game::Difficulty& difficulty)
	{
		auto nodes = data::game::Node::All();
		std::map<int, size_t> north;
		std::map<int, size_t> south;
		for (auto node : nodes)
		{
			north[node] = 1;
			south[node] = 1;
		}
		while (!north.empty())
		{
			auto northPick = common::RNG::FromGenerator(north, 0);
			auto southPick = common::RNG::FromGenerator(south, 0);
			data::game::node::Path::Write(northPick, (int)game::Direction::SOUTH, southPick);
			data::game::node::Path::Write(southPick, (int)game::Direction::NORTH, northPick);
			north.erase(northPick);
			south.erase(southPick);
		}
	}

	static void GenerateEastWestPaths(const game::Difficulty& difficulty)
	{
		auto nodes = data::game::Node::All();
		std::map<int, size_t> east;
		std::map<int, size_t> west;
		for (auto node : nodes)
		{
			east[node] = 1;
			west[node] = 1;
		}
		while (!east.empty())
		{
			auto eastPick = common::RNG::FromGenerator(east, 0);
			auto westPick = common::RNG::FromGenerator(west, 0);
			data::game::node::Path::Write(eastPick, (int)game::Direction::WEST, westPick);
			data::game::node::Path::Write(westPick, (int)game::Direction::EAST, eastPick);
			east.erase(eastPick);
			west.erase(westPick);
		}
	}

	static void GenerateNodePaths(const game::Difficulty& difficulty)
	{
		GenerateNorthSouthPaths(difficulty);
		GenerateEastWestPaths(difficulty);
	}

	void Reset(const game::Difficulty& difficulty)
	{
		data::game::Node::Clear();
		GenerateNodes(difficulty);
		GenerateNodePaths(difficulty);
	}
}