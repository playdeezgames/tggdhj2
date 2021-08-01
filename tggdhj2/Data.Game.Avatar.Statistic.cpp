#include "Common.Data.h"
#include "Data.Game.Avatar.Statistic.h"
#include "Data.Game.Common.h"
#include <format>
namespace data::game::avatar::Statistic
{
	const std::string FIELD_STATISTIC_VALUE = "StatisticValue";
	const std::string CREATE_TABLE = "CREATE TABLE [AvatarStatistics]([AvatarId] INT NOT NULL,[StatisticId] INT NOT NULL,[StatisticValue] REAL NOT NULL,UNIQUE([AvatarId],[StatisticId]));";
	const std::string DELETE_ALL = "DELETE FROM [AvatarStatistics] WHERE [AvatarId]={};";
	const std::string QUERY_ITEM = "SELECT [StatisticValue] FROM [AvatarStatistics] WHERE [AvatarId]={} AND [StatisticId]={};";
	const std::string REPLACE_ITEM = "REPLACE INTO [AvatarStatistics]([AvatarId],[StatisticId],[StatisticValue]) VALUES({},{},{:.4f});";

	static void AutoCreateAvatarStatisticsTable()
	{
		data::game::Common::Execute(CREATE_TABLE);
	}

	void Clear()
	{
		AutoCreateAvatarStatisticsTable();
		data::game::Common::Execute(std::format(DELETE_ALL, data::game::Common::AVATAR_ID));
	}

	std::optional<double> Read(int statisticId)
	{
		AutoCreateAvatarStatisticsTable();
		auto records = data::game::Common::Execute(std::format(QUERY_ITEM, data::game::Common::AVATAR_ID, statisticId));
		if(!records.empty())
		{
			return common::Data::StringToDouble(records.front()[FIELD_STATISTIC_VALUE]);
		}
		return std::nullopt;
	}

	void Write(int statisticId, double value)
	{
		AutoCreateAvatarStatisticsTable();
		data::game::Common::Execute(std::format(REPLACE_ITEM, data::game::Common::AVATAR_ID, statisticId, value));
	}
}
