#include "Game.Items.h"
#include <map>
namespace game::Items
{
	const std::map<Item, ItemDescriptor> descriptors =
	{
		{Item::TROUSERS, {"Trousers"}},
		{Item::CODPIECE, {"Codpiece"}},
		{Item::WATERSKIN, {"Waterskin"}}
	};
	static std::list<Item> allItems;

	const ItemDescriptor& Read(const Item& item)
	{
		return descriptors.find(item)->second;
	}

	const std::list<Item>& All()
	{
		if (allItems.empty())
		{
			for (auto descriptor : descriptors)
			{
				allItems.push_back(descriptor.first);
			}
		}
		return allItems;
	}

}
