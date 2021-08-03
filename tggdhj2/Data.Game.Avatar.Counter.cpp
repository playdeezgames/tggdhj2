#include "Common.Data.h"
#include "Data.Game.Common.h"
#include "Data.Game.Avatar.Counter.h"
#include <format>
namespace data::game::avatar::Counter
{
	const std::string FIELD_COUNTER_VALUE = "CounterValue";
	const std::string CREATE_TABLE = "CREATE TABLE IF NOT EXISTS [AvatarCounters]([AvatarId] INT NOT NULL, [CounterId] INT NOT NULL, [CounterValue] INT NOT NULL, UNIQUE([AvatarId],[CounterId]));";
	const std::string DELETE_ALL = "DELETE FROM [AvatarCounters] WHERE [AvatarId]={};";
	const std::string QUERY_ITEM = "SELECT [CounterValue] FROM [AvatarCounters] WHERE [AvatarId]={} AND [CounterId]={};";
	const std::string REPLACE_ITEM = "REPLACE INTO [AvatarCounters]([AvatarId],[CounterId],[CounterValue]) VALUES({},{},{});";

	const auto AutoCreateAvatarCountersTable = Common::Run(CREATE_TABLE);

	std::optional<size_t> Read(int counterId)
	{
		AutoCreateAvatarCountersTable();
		auto records = Common::Execute(std::format(QUERY_ITEM, Common::AVATAR_ID, counterId));
		if (!records.empty())
		{
			return (size_t)common::Data::StringToInt(records.front()[FIELD_COUNTER_VALUE]);
		}
		return std::nullopt;
	}

	void Write(int counterId, size_t counterValue)
	{
		AutoCreateAvatarCountersTable();
		Common::Execute(std::format(REPLACE_ITEM, Common::AVATAR_ID, counterId, counterValue));
	}

	void Clear()
	{
		AutoCreateAvatarCountersTable();
		Common::Execute(std::format(DELETE_ALL, Common::AVATAR_ID));
	}
}