#include "Common.Data.h"
#include "Data.Game.Avatar.Facing.h"
#include "Data.Game.Common.h"
#include <format>
namespace data::game::avatar::Facing
{
	const std::string FIELD_DIRECTION_ID = "DirectionId";
	const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [AvatarFacings]([AvatarId] INT NOT NULL UNIQUE,[DirectionId] INT NOT NULL);";
	const std::string REPLACE_ITEM = "REPLACE INTO [AvatarFacings]([AvatarId],[DirectionId]) VALUES({},{});";
	const std::string QUERY_ITEM = "SELECT [DirectionId] FROM [AvatarFacings] WHERE [AvatarId]={};";

	static auto AutoCreateAvatarFacingsTable = data::game::Common::Run(CREATE_TABLE);

	void Write(int directionId)
	{
		AutoCreateAvatarFacingsTable();
		Common::Execute(std::format(REPLACE_ITEM, Common::AVATAR_ID, directionId));
	}

	int Read()
	{
		AutoCreateAvatarFacingsTable();
		return common::Data::StringToInt(Common::Execute(std::format(QUERY_ITEM, Common::AVATAR_ID)).front()[FIELD_DIRECTION_ID]);
		
	}
}
