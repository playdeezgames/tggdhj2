#pragma once
#include <optional>
namespace game::avatar::Position
{
	std::optional<int> Read();
	void Write(int);
}
