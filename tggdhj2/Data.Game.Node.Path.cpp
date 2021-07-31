#include "Common.Data.h"
#include "Data.Game.Common.h"
#include "Data.Game.Node.Path.h"
#include <format>
namespace data::game::node::Path
{
	const std::string FIELD_DIRECTION_ID = "DirectionId";
	const std::string FIELD_TO_POSITION_ID = "ToPositionId";
	const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [NodePaths]([FromPositionId] INT NOT NULL,[DirectionId] INT NOT NULL,[ToPositionId] INT NOT NULL, UNIQUE([FromPositionId],[DirectionId]));";
	const std::string QUERY_NODE = "SELECT [DirectionId], [ToPositionId] FROM [NodePaths] WHERE [FromPositionId]={};";
	const std::string QUERY_ITEM = "SELECT [ToPositionId] FROM [NodePaths] WHERE [FromPositionId]={} AND [DirectionId]={};";
	const std::string REPLACE_ITEM = "REPLACE INTO [NodePaths]([FromPositionId],[DirectionId],[ToPositionId]) VALUES ({},{},{});";
	const std::string DELETE_ALL = "DELETE FROM [NodePaths];";

	static void AutoCreateNodePathsTable()
	{
		data::game::Common::Execute(CREATE_TABLE);
	}

	std::map<int, int> Read(int positionId)
	{
		std::map<int, int> results;
		AutoCreateNodePathsTable();
		auto records = data::game::Common::Execute(std::format(QUERY_NODE,positionId));
		for (auto& record : records)
		{
			results[common::Data::StringToInt(record[FIELD_DIRECTION_ID])] = common::Data::StringToInt(record[FIELD_TO_POSITION_ID]);
		}
		return results;
	}

	std::optional<int> Read(int positionId, int directionId)
	{
		AutoCreateNodePathsTable();
		auto records = data::game::Common::Execute(std::format(QUERY_ITEM, positionId, directionId));
		if (!records.empty())
		{
			return common::Data::StringToInt(records.front()[FIELD_TO_POSITION_ID]);
		}
		return std::nullopt;
	}

	void Write(int fromPositionId, int directionId, int toPositionId)
	{
		AutoCreateNodePathsTable();
		data::game::Common::Execute(std::format(REPLACE_ITEM, fromPositionId, directionId, toPositionId));
	}

	void Clear()
	{
		AutoCreateNodePathsTable();
		data::game::Common::Execute(DELETE_ALL);
	}
}