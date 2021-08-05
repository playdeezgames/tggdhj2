#pragma once
#include <optional>
#include <string>
namespace game
{
	struct StatisticDescriptor
	{
		std::string name;
		double initial;
		std::optional<double> minimum;
		std::optional<double> maximum;
		std::string color;
		std::string format;
	};
}
