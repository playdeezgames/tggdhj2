#include <map>
#include "Visuals.Data.Types.h"
namespace visuals::data::Types
{
	const std::string AVATAR_INVENTORY = "AvatarInventory";
	const std::string IMAGE = "Image";
	const std::string LAYOUT = "Layout";
	const std::string MENU = "Menu";
	const std::string TEXT = "Text";
	const std::string AREA = "Area";
	const std::string SPRITE_GRID = "SpriteGrid";
}
namespace visuals::data
{
	const std::map<std::string, Type> table =
	{
		{ visuals::data::Types::AVATAR_INVENTORY, Type::AVATAR_INVENTORY },
		{ visuals::data::Types::IMAGE, Type::IMAGE },
		{ visuals::data::Types::LAYOUT, Type::LAYOUT },
		{ visuals::data::Types::MENU, Type::MENU },
		{ visuals::data::Types::TEXT, Type::TEXT },
		{ visuals::data::Types::AREA, Type::AREA },
		{ visuals::data::Types::SPRITE_GRID, Type::SPRITE_GRID }
	};
}
namespace visuals::data::Types
{
	std::optional<Type> FromString(const std::string& name)
	{
		auto iter = table.find(name);
		if (iter != table.end())
		{
			return iter->second;
		}
		return std::nullopt;
	}
}
