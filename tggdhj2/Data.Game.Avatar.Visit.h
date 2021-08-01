#pragma once
#include <list>
namespace data::game::avatar::Visit
{
	bool Read(int);
	void Write(int);
	std::list<int> All();
	void Clear();
}
