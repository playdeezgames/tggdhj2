#pragma once
#include "Game.Avatar.Counter.h"
#include "Game.Avatar.CounterDescriptor.h"
#include "Game.Difficulty.h"
#include <list>
namespace game::avatar::Counters
{
	const CounterDescriptor& GetDescriptor(const Counter&);
	const std::list<Counter>& All();
	size_t Read(const Counter&);
	void Reset(const Difficulty&);
	void Increment(const Counter&);
}
