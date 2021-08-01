#include "Common.Data.h"
#include "Data.Game.Avatar.Item.h"
#include "Data.Game.Common.h"
#include <format>
namespace data::game::avatar::Item
{
	const std::string FIEND_ITEM_COUNT = "ItemCount";
	const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [AvatarItems]([AvatarId] INT NOT NULL,[ItemId] INT NOT NULL,[ItemCount] INT NOT NULL,UNIQUE([AvatarId],[ItemId]));";
	const std::string QUERY_ITEM = "SELECT [ItemCount] FROM [AvatarItems] WHERE [AvatarId]={} AND [ItemId]={};";
	const std::string REPLACE_ITEM = "REPLACE INTO [AvatarItems]([AvatarId],[ItemId],[ItemCount]) VALUES({},{},{});";
	const std::string DELETE_ITEM = "DELETE FROM [AvatarItems] WHERE [AvatarId]={} AND [ItemId]={};";
	const std::string DELETE_ALL = "DELETE FROM [AvatarItems];";

	static void AutoCreateAvatarItemsTable()
	{
		Common::Execute(CREATE_TABLE);
	}

	size_t Read(int itemId)
	{
		AutoCreateAvatarItemsTable();
		auto records = Common::Execute(std::format(QUERY_ITEM, Common::AVATAR_ID, itemId));
		return 0;
	}

	void Write(int itemId, size_t itemCount)
	{
		AutoCreateAvatarItemsTable();
		Common::Execute(std::format(DELETE_ITEM, Common::AVATAR_ID, itemId));
		if (itemCount > 0)
		{
			Common::Execute(std::format(REPLACE_ITEM, Common::AVATAR_ID, itemId, itemCount));
		}
	}

	void Clear()
	{
		AutoCreateAvatarItemsTable();
		Common::Execute(DELETE_ALL);
	}
}