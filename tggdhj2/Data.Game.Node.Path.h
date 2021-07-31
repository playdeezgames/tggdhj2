#pragma once
#include <optional>
#include <map>
namespace data::game::node::Path
{
	std::map<int, int> Read(int);
	std::optional<int> Read(int, int);
	void Write(int, int, int);
	void Clear();
}
