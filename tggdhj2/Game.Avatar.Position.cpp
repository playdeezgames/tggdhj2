#include "Common.RNG.h"
#include "Data.Game.Avatar.Position.h"
#include "Data.Game.Node.h"
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


	void Reset(const game::Difficulty&)
	{
		auto nodes = data::game::Node::All();
		auto index = common::RNG::FromRange(0u, nodes.size());
		auto iter = nodes.begin();
		while (index > 0)
		{
			++iter;
			--index;
		}
		Write(*iter);
	}
}