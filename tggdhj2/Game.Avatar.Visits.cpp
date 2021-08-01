#include "Data.Game.Avatar.Visit.h"
#include "Game.Avatar.Visits.h"
namespace game::avatar::Visits
{
	bool Read(int positionId)
	{
		return data::game::avatar::Visit::Read(positionId);
	}

	void Write(int positionId)
	{
		data::game::avatar::Visit::Write(positionId);
	}

	size_t Count()
	{
		return data::game::avatar::Visit::All().size();
	}

	void Reset(const game::Difficulty&)
	{
		data::game::avatar::Visit::Clear();
	}

}