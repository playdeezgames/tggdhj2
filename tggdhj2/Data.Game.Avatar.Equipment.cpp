#include "Common.Data.h"
#include "Data.Game.Common.h"
#include "Data.Game.Avatar.Equipment.h"
#include <format>
namespace data::game::avatar::Equipment
{
	const std::string FIELD_ITEM_ID = "ItemId";
	const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [AvatarEquipment]([AvatarId] INT NOT NULL, [EquipSlotId] INT NOT NULL, [ItemId] INT NOT NULL, UNIQUE([AvatarId],[EquipSlotId]));";
	const std::string QUERY_ITEM = "SELECT [ItemId] FROM [AvatarEquipment] WHERE [AvatarId]={} AND [EquipSlotId]={};";
	const std::string REPLACE_ITEM = "REPLACE INTO [AvatarEquipment]([AvatarId],[EquipSlotId],[ItemId]) VALUES({},{},{});";
	const std::string DELETE_ITEM = "DELETE FROM [AvatarEquipment] WHERE [AvatarId]={} AND [EquipSlotId]={};";
	const std::string DELETE_ALL = "DELETE FROM [AvatarEquipment] WHERE [AvatarId]={};";

	static auto AutoCreateAvatarEquipmentTable = data::game::Common::Run(CREATE_TABLE);

	std::optional<int> Read(int equipSlotId)
	{
		AutoCreateAvatarEquipmentTable();
		auto records = Common::Execute(std::format(QUERY_ITEM, Common::AVATAR_ID, equipSlotId));
		if (!records.empty())
		{
			return common::Data::StringToInt(records.front()[FIELD_ITEM_ID]);
		}
		return std::nullopt;
	}

	void Write(int equipSlotId, int itemId)
	{
		AutoCreateAvatarEquipmentTable();
		Common::Execute(std::format(REPLACE_ITEM, Common::AVATAR_ID, equipSlotId, itemId));
	}

	void Clear(int equipSlotId)
	{
		AutoCreateAvatarEquipmentTable();
		Common::Execute(std::format(DELETE_ITEM, Common::AVATAR_ID, equipSlotId));
	}

	void ClearAll()
	{
		AutoCreateAvatarEquipmentTable();
		Common::Execute(std::format(DELETE_ALL, Common::AVATAR_ID));
	}
}