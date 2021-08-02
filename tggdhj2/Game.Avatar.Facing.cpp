#include "Common.RNG.h"
#include "Data.Game.Avatar.Facing.h"
#include "Game.Avatar.Facing.h"
#include "Game.Avatar.Log.h"
#include "Game.Directions.h"
#include "Visuals.Data.Colors.h"
namespace game::avatar::Facing
{
	const std::string TURN_LEFT_TEXT = "You turn left.";
	const std::string TURN_RIGHT_TEXT = "You turn right.";
	const std::string TURN_AROUND_TEXT = "You turn around.";

	Direction Read()
	{
		return (Direction)data::game::avatar::Facing::Read();
	}

	void TurnLeft()
	{
		game::avatar::Log::Write({ visuals::data::Colors::NORMAL, TURN_LEFT_TEXT });
		auto descriptor = game::Directions::Read(Read());
		data::game::avatar::Facing::Write((int)descriptor.previous);
	}

	void TurnRight()
	{
		game::avatar::Log::Write({ visuals::data::Colors::NORMAL, TURN_RIGHT_TEXT});
		auto descriptor = game::Directions::Read(Read());
		data::game::avatar::Facing::Write((int)descriptor.next);
	}

	void TurnAround()
	{
		game::avatar::Log::Write({ visuals::data::Colors::NORMAL, TURN_AROUND_TEXT });
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
