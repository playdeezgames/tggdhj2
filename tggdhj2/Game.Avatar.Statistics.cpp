#include "Data.Game.Avatar.Statistic.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Statistics.h"
namespace game::avatar::Statistics
{
	void Reset(const game::Difficulty&)
	{

	}

	double GetCurrent(const game::Statistic& statistic)
	{
		auto value = data::game::avatar::Statistic::Read((int)statistic);
		if (value)
		{
			return value.value();
		}
		return game::Statistics::Read(statistic).initial;
	}

	static void SetStatistic(const game::Statistic& statistic, double value)
	{
		auto descriptor = game::Statistics::Read(statistic);
		if (descriptor.minimum.has_value() && value < descriptor.minimum.value())
		{
			value = descriptor.minimum.value();
		}
		if (descriptor.maximum.has_value() && value > descriptor.maximum.value())
		{
			value = descriptor.maximum.value();
		}
		data::game::avatar::Statistic::Write((int)statistic, value);
	}

	static void SetToMaximum(const game::Statistic& statistic)
	{
		auto descriptor = game::Statistics::Read(statistic);
		if (descriptor.maximum.has_value())
		{
			SetStatistic(statistic, descriptor.maximum.value());
		}
	}

	double ChangeCurrent(const game::Statistic& statistic, double delta)
	{
		double newValue = GetCurrent(statistic) + delta;
		SetStatistic(statistic, newValue);
		return newValue - GetCurrent(statistic);
	}

	static bool IsMinimum(const game::Statistic& statistic)
	{
		auto descriptor = game::Statistics::Read(statistic);
		if (descriptor.minimum)
		{
			return GetCurrent(statistic) <= descriptor.minimum.value();
		}
		return false;
	}

	double GetHealth()
	{
		return GetCurrent(game::Statistic::HEALTH);
	}

	double ChangeHealth(double delta)
	{
		return ChangeCurrent(game::Statistic::HEALTH, delta);
	}

	bool IsDead()
	{
		return IsMinimum(game::Statistic::HEALTH);
	}

	double GetHydration()
	{
		return GetCurrent(game::Statistic::HYDRATION);
	}

	double ChangeHydration(double delta)
	{
		return ChangeCurrent(game::Statistic::HYDRATION, delta);
	}

	bool IsDehydrated()
	{
		return IsMinimum(game::Statistic::HYDRATION);
	}

	void FullyHydrate()
	{
		SetToMaximum(game::Statistic::HYDRATION);
	}
}