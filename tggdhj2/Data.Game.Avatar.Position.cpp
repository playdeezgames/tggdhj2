#include "Common.Data.h"
#include "Data.Game.Avatar.Position.h"
#include "Data.Game.Common.h"
#include <format>
namespace data::game::avatar::Position
{
	const std::string FIELD_POSITION_ID = "PositionId";
	const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXIST [AvatarPositions]([AvatarId] INT NOT NULL UNIQUE,[PositionId] INT NOT NULL);";
	const std::string QUERY_ITEM = "SELECT [PositionId] FROM [AvatarPositions] WHERE [AvatarId]={};";
	const std::string REPLACE_ITEM = "REPLACE INTO [AvatarPositions]([AvatarId],[PositionId]) VALUES ({},{});";

	static void AutoCreateAvatarPositionsTable()
	{
		data::game::Common::Execute(CREATE_TABLE);
	}

	std::optional<int> Read()
	{
		AutoCreateAvatarPositionsTable();
		auto records = data::game::Common::Execute(std::format(QUERY_ITEM, data::game::Common::AVATAR_ID));
		if (!records.empty())
		{
			return common::Data::StringToInt(records.front()[FIELD_POSITION_ID]);
		}
		return std::nullopt;
	}

	void Write(int positionId)
	{
		AutoCreateAvatarPositionsTable();
		data::game::Common::Execute(std::format(REPLACE_ITEM, data::game::Common::AVATAR_ID, positionId));
	}
}