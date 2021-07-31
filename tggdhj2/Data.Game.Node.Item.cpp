#include "Common.Data.h"
#include "Data.Game.Common.h"
#include "Data.Game.Node.Item.h"
#include <format>
namespace data::game::node::Item
{
	const std::string FIELD_ITEM_COUNT = "ItemCount";
	const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [NodeItems]([PositionId] INT NOT NULL,[ItemId] INT NOT NULL,[ItemCount] INT NOT NULL, UNIQUE([PositionId],[ItemId]));";
	const std::string QUERY_ITEM = "SELECT [ItemCount] FROM [NodeItems] WHERE [PositionId]={} AND [ItemId]={};";
	const std::string REPLACE_ITEM = "REPLACE INTO [NodeItems]([PositionId],[ItemId],[ItemCount]) VALUES({},{},{});";
	const std::string DELETE_ITEM = "DELETE FROM [NodeItems] WHERE [PositionId]={} AND [ItemId]={};";
	const std::string DELETE_ALL = "DELETE FROM [NodeItems];";

	static void AutoCreateNodeItemsTable()
	{
		data::game::Common::Execute(CREATE_TABLE);
	}

	size_t Read(int positionId, int itemId)
	{
		AutoCreateNodeItemsTable();
		auto records = data::game::Common::Execute(std::format(QUERY_ITEM,positionId, itemId));
		if (!records.empty())
		{
			return (size_t)common::Data::StringToInt(records.front()[FIELD_ITEM_COUNT]);
		}
		return 0;
	}

	void Write(int positionId, int itemId, size_t count)
	{
		AutoCreateNodeItemsTable();
		data::game::Common::Execute(std::format(DELETE_ITEM, positionId, itemId));
		if (count > 0)
		{
			data::game::Common::Execute(std::format(REPLACE_ITEM, positionId, itemId, count));
		}
	}

	void Clear()
	{
		AutoCreateNodeItemsTable();
		data::game::Common::Execute(DELETE_ALL);
	}
}