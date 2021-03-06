#pragma once
#include <list>
#include <map>
#include <optional>
namespace common::RNG
{
	void Seed();
	int FromRange(int, int);
	size_t FromRange(size_t, size_t);
	double FromRange(double, double);
	template <typename TResult>
	std::optional<TResult> FromList(const std::list<TResult> items)
	{
		if (!items.empty())
		{
			size_t index = FromRange(0u, items.size());
			auto iter = items.begin();
			while (index > 0)
			{
				--index;
				++iter;
			}
			return *iter;
		}
		return std::nullopt;
	}
	template <typename TResult>
	TResult FromGenerator(const std::map<TResult, size_t>& table, TResult defaultValue)
	{
		size_t total = 0u;
		for (auto& entry : table)
		{
			total += entry.second;
		}
		if (total > 0)
		{
			size_t generated = FromRange(0u, total);
			for (auto& entry : table)
			{
				if (generated < entry.second)
				{
					return entry.first;
				}
				else
				{
					generated -= entry.second;
				}
			}
		}
		return defaultValue;
	}
}