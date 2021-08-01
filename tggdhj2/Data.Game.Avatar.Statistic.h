#pragma once
#include <optional>
namespace data::game::avatar::Statistic
{
	void Clear();
	std::optional<double> Read(int);
	void Write(int, double);
}
