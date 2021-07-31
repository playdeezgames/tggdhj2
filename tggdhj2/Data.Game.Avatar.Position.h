#pragma once
#include <optional>
namespace data::game::avatar::Position
{
	std::optional<int> Read();
	void Write(int);
}