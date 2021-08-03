#include "Common.Utility.h"
#include "Data.Game.Avatar.Counter.h"
#include "Game.Avatar.Counters.h"
#include <map>
namespace game::avatar::Counters
{
	const std::map<Counter, CounterDescriptor> descriptors =
	{
		{
			Counter::MOVES_MADE,
			{
				"Moves Made"
			}
		}
	};

	const std::list<Counter> allCounters = common::Utility::ExtractListFromMapKeys(descriptors);

	const CounterDescriptor& GetDescriptor(const Counter& counter)
	{
		return descriptors.find(counter)->second;
	}

	const std::list<Counter>& All()
	{
		return allCounters;
	}

	size_t Read(const Counter& counter)
	{
		return data::game::avatar::Counter::Read((int)counter).value_or(0);
	}

	void Reset(const Difficulty&)
	{
		data::game::avatar::Counter::Clear();
	}

	void Increment(const Counter& counter)
	{
		data::game::avatar::Counter::Write((int)counter, Read(counter) + 1);
	}
}
