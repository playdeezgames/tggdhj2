#pragma once
#include "Game.Difficulty.h"
namespace game::avatar::Statistics
{
	void Reset(const game::Difficulty&);

	double GetHealth();
	double ChangeHealth(double);
	bool IsDead();

	double GetHydration();
	double ChangeHydration(double);
	bool IsDehydrated();
}