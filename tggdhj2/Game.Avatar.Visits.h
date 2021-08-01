#pragma once
#include "Game.Difficulty.h"
namespace game::avatar::Visits
{
	bool Read(int);
	void Write(int);
	size_t Count();
	void Reset(const game::Difficulty&);
}