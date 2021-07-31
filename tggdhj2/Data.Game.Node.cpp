#include "Common.Data.h"
#include "Data.Game.Common.h"
#include "Data.Game.Node.h"
#include <format>
namespace data::game::Node
{
	const std::string FIELD_POSITION_ID = "PositionId";
	const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [Nodes]([PositionId] INT NOT NULL UNIQUE);";
	const std::string QUERY_ITEM = "SELECT [PositionId] FROM [Nodes] WHERE [PositionId]={};";
	const std::string QUERY_ALL = "SELECT [PositionId] FROM [Nodes];";
	const std::string REPLACE_ITEM = "REPLACE INTO [Nodes]([PositionId]) VALUES({});";
	const std::string DELETE_ALL = "DELETE FROM [Nodes];";

	static void AutoCreateNodesTable()
	{
		data::game::Common::Execute(CREATE_TABLE);
	}

	bool Read(int positionId)
	{
		AutoCreateNodesTable();
		return !data::game::Common::Execute(std::format(QUERY_ITEM, positionId)).empty();
	}

	void Write(int positionId)
	{
		AutoCreateNodesTable();
		data::game::Common::Execute(std::format(REPLACE_ITEM, positionId));
	}

	std::list<int> All()
	{
		std::list<int> results;
		AutoCreateNodesTable();
		auto records = data::game::Common::Execute(QUERY_ALL);
		for (auto& record : records)
		{
			results.push_back(common::Data::StringToInt(record[FIELD_POSITION_ID]));
		}
		return results;
	}

	void Clear()
	{
		AutoCreateNodesTable();
		data::game::Common::Execute(DELETE_ALL);
	}

}