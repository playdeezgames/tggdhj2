#include "Data.Game.Avatar.Position.h"
#include "Game.Avatar.Position.h"
namespace game::avatar::Position
{
	std::optional<int> Read()
	{
		return data::game::avatar::Position::Read();
	}

	void Write(int positionId)
	{
		data::game::avatar::Position::Write(positionId);
	}
}