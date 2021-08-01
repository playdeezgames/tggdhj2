#include "Common.Data.h"
#include "Data.Game.Avatar.Visit.h"
#include "Data.Game.Common.h"
#include <format>
namespace data::game::avatar::Visit
{
	const std::string FIELD_POSITION_ID = "PositionId";
	const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [AvatarVisits]([PositionId] INT NOT NULL UNIQUE);";
	const std::string QUERY_ITEM = "SELECT [PositionId] FROM [AvatarVisits] WHERE [PositionId]={};";
	const std::string REPLACE_ITEM = "REPLACE INTO [AvatarVisits]([PositionId]) VALES({});";
	const std::string QUERY_ALL = "SELECT [PositionId] FROM [AvatarVisits];";
	const std::string DELETE_ALL = "DELETE FROM [AvatarVisits];";

	static void AutoCreatAvatarVisitsTable()
	{
		data::game::Common::Execute(CREATE_TABLE);
	}

	bool Read(int positionId)
	{
		AutoCreatAvatarVisitsTable();
		return !data::game::Common::Execute(std::format(QUERY_ITEM, positionId)).empty();
	}

	void Write(int positionId)
	{
		AutoCreatAvatarVisitsTable();
		data::game::Common::Execute(std::format(REPLACE_ITEM,positionId));
	}

	std::list<int> All()
	{
		std::list<int> result;
		AutoCreatAvatarVisitsTable();
		auto records = data::game::Common::Execute(QUERY_ALL);
		for (auto record : records)
		{
			result.push_back(common::Data::StringToInt(record[FIELD_POSITION_ID]));
		}
		return result;
	}

	void Clear()
	{
		AutoCreatAvatarVisitsTable();
		data::game::Common::Execute(DELETE_ALL);
	}
}