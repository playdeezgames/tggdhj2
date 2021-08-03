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
				"N",
				Direction::EAST,
				Direction::SOUTH,
				Direction::WEST
			}
		},
		{
			Direction::EAST,
			{
				"East",
				"E",
				Direction::SOUTH,
				Direction::WEST,
				Direction::NORTH
			}
		},
		{
			Direction::SOUTH,
			{
				"South",
				"S",
				Direction::WEST,
				Direction::NORTH,
				Direction::EAST
			}
		},
		{
			Direction::WEST,
			{
				"West",
				"W",
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