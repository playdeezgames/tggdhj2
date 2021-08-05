#pragma once
#include "Game.Difficulty.h"
#include "Game.Statistic.h"
namespace game::avatar::Statistics
{
	void Reset(const game::Difficulty&);

	double GetCurrent(const Statistic&);
	double ChangeCurrent(const Statistic&, double);

	double GetHealth();
	double ChangeHealth(double);
	bool IsDead();

	double GetHydration();
	double ChangeHydration(double);
	bool IsDehydrated();
	void FullyHydrate();
}