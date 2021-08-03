#pragma once
#include <optional>
namespace data::game::avatar::Counter
{
	std::optional<size_t> Read(int);
	void Write(int, size_t);
	void Clear();
}