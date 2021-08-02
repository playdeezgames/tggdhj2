#include "Common.RNG.h"
#include "Data.Game.Avatar.Facing.h"
#include "Game.Avatar.Facing.h"
#include "Game.Directions.h"
namespace game::avatar::Facing
{
	Direction Read()
	{
		return (Direction)data::game::avatar::Facing::Read();
	}

	void TurnLeft()
	{
		auto descriptor = game::Directions::Read(Read());
		data::game::avatar::Facing::Write((int)descriptor.previous);
	}

	void TurnRight()
	{
		auto descriptor = game::Directions::Read(Read());
		data::game::avatar::Facing::Write((int)descriptor.next);
	}

	void TurnAround()
	{
		auto descriptor = game::Directions::Read(Read());
		data::game::avatar::Facing::Write((int)descriptor.opposite);
	}

	void Reset(const Difficulty&)
	{
		data::game::avatar::Facing::Write(
			(int)common::RNG::FromList(
				game::Directions::All())
			.value());
	}
}
