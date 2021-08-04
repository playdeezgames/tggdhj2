#pragma once
#include <optional>
namespace data::game::avatar::Equipment
{
	std::optional<int> Read(int);
	void Write(int, int);
	void Clear(int);
	void ClearAll();
}
