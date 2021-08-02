#include "Common.RNG.h"
#include "Data.Game.Avatar.Position.h"
#include "Data.Game.Node.h"
#include "Data.Game.Node.Path.h"
#include "Game.Avatar.Facing.h"
#include "Game.Avatar.Position.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Avatar.Visits.h"
namespace game::avatar::Position
{
	std::optional<int> Read()
	{
		return data::game::avatar::Position::Read();
	}

	void Write(int positionId)
	{
		game::avatar::Visits::Write(positionId);
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

	const double HYDRATION_DELTA = -1.0;

	void Move()
	{
		auto direction = game::avatar::Facing::Read();
		auto avatarPosition = Read();
		if (avatarPosition)
		{
			auto overage = game::avatar::Statistics::ChangeHydration(HYDRATION_DELTA);
			game::avatar::Statistics::ChangeHealth(overage);
			auto nodePathTo = data::game::node::Path::Read(avatarPosition.value(), (int)direction);
			if (nodePathTo)
			{
				Write(nodePathTo.value());
			}
		}
	}

}