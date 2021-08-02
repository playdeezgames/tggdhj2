#include "Common.Utility.h"
#include "Game.Directions.h"
namespace game::Directions
{
	const std::map<Direction, DirectionDescriptor> descriptors = 
	{
		{
			Direction::NORTH,
			{
				"North",
				Direction::EAST,
				Direction::SOUTH,
				Direction::WEST
			}
		},
		{
			Direction::EAST,
			{
				"East",
				Direction::SOUTH,
				Direction::WEST,
				Direction::NORTH
			}
		},
		{
			Direction::SOUTH,
			{
				"South",
				Direction::WEST,
				Direction::NORTH,
				Direction::EAST
			}
		},
		{
			Direction::WEST,
			{
				"West",
				Direction::NORTH,
				Direction::EAST,
				Direction::SOUTH
			}
		}
	};

	const std::list<Direction> allDirections = common::Utility::ExtractListFromMapKeys(descriptors);

	const DirectionDescriptor& Read(const Direction& direction)
	{
		return descriptors.find(direction)->second;
	}

	const std::list<Direction>& All()
	{
		return allDirections;
	}
}