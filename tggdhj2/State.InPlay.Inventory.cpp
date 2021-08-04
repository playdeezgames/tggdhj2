#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Application.OnEnter.h"
#include "Application.Renderer.h"
#include "Application.UIState.h"
#include "Common.Audio.h"
#include "Common.Utility.h"
#include "Data.Stores.h"
#include <format>
#include "Game.Audio.Mux.h"
#include "Game.Avatar.h"
#include "Game.Avatar.Equipment.h"
#include "Game.Avatar.Items.h"
#include "Game.Avatar.Position.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Avatar.Visits.h"
#include "Game.EquipSlots.h"
#include "Game.Items.h"
#include "Game.Nodes.h"
#include "Game.Nodes.Items.h"
#include "Visuals.Areas.h"
#include "Visuals.Data.Colors.h"
#include "Visuals.Menus.h"
#include "Visuals.SpriteGrid.h"
#include "Visuals.Texts.h"
namespace state::in_play::Inventory
{
	const std::string LAYOUT_NAME = "State.InPlay.Inventory";
	const std::string SPRITE_GRID = "Grid";
	const std::string FONT_DEFAULT = "default";
	const size_t GRID_COLUMNS = 39;
	const size_t GRID_ROWS = 21;
	const int INVENTORY_ROW_OFFSET = 1;
	const int EQUIPMENT_ROW_OFFSET = 1;
	const std::string FORMAT_INVENTORY_ITEM_COUNT = "{} (x{})";
	const std::string FORMAT_EQUIPMENT_SLOT = "{}({})";
	const std::string AREA_INVENTORY = "Inventory";
	const std::string AREA_EQUIPMENT = "Equipment";
	const std::string AREA_GO_BACK = "GoBack";

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::BACK, ::application::UIState::GoTo(::UIState::IN_PLAY_NODE) },
		{::Command::RED, ::application::UIState::GoTo(::UIState::IN_PLAY_NODE) }
	};

	static std::map<game::Item, size_t> inventoryItems;
	static std::optional<int> hoverInventoryItem = std::nullopt;
	static std::map<game::EquipSlot, game::Item> equipment;
	static std::optional<int> hoverEquipment = std::nullopt;
	static bool hoverGoBack = false;

	static void WriteGridText(int column, int row, const std::string& text, const std::string& color, const visuals::HorizontalAlignment& alignment)
	{
		visuals::SpriteGrid::WriteText(
			LAYOUT_NAME,
			SPRITE_GRID,
			{ column, row },
			FONT_DEFAULT,
			text,
			color,
			alignment);
	}
	static int GetGridCellHeight()
	{
		return visuals::SpriteGrid::GetCellHeight(LAYOUT_NAME, SPRITE_GRID);
	}

	static void UpdateInventoryContents()
	{
		inventoryItems = game::avatar::Items::ReadAll();
	}

	static void UpdateEquipmentContents()
	{
		equipment = game::avatar::Equipment::ReadAll();
	}

	static void RefreshEquipmentContents()
	{
		WriteGridText(
			GRID_COLUMNS, 
			EQUIPMENT_ROW_OFFSET - 1,
			"Equipment:",
			visuals::data::Colors::SUBHEADING,
			visuals::HorizontalAlignment::RIGHT);
		int row = 0;
		for (auto equipped : equipment)
		{
			auto itemDescriptor = game::Items::Read(equipped.second);
			auto equipSlotDescriptor = game::EquipSlots::Read(equipped.first);
			std::string color = (hoverEquipment.has_value() && hoverEquipment.value() == row) ? (visuals::data::Colors::HOVER) : (visuals::data::Colors::NORMAL);
			WriteGridText(
				GRID_COLUMNS,
				row+EQUIPMENT_ROW_OFFSET,
				std::format(FORMAT_EQUIPMENT_SLOT, itemDescriptor.name, equipSlotDescriptor.name),
				color,
				visuals::HorizontalAlignment::RIGHT);
			++row;
		}
		if (row == 0)
		{
			WriteGridText(
				GRID_COLUMNS, row + EQUIPMENT_ROW_OFFSET,
				"(nothing)",
				visuals::data::Colors::DISABLED,
				visuals::HorizontalAlignment::RIGHT);
		}

	}

	static void RefreshInventoryContents()
	{
		WriteGridText(
			0, INVENTORY_ROW_OFFSET - 1,
			"Inventory:",
			visuals::data::Colors::SUBHEADING,
			visuals::HorizontalAlignment::LEFT);
		int row = 0;
		for (auto& inventoryItem : inventoryItems)
		{
			auto& descriptor = game::Items::Read(inventoryItem.first);
			std::string color = (hoverInventoryItem.has_value() && hoverInventoryItem.value() == row) ? (visuals::data::Colors::HOVER) : (visuals::data::Colors::NORMAL);
			WriteGridText(
				0, row + INVENTORY_ROW_OFFSET,
				std::format(FORMAT_INVENTORY_ITEM_COUNT, descriptor.name, inventoryItem.second),
				color,
				visuals::HorizontalAlignment::LEFT);
			++row;
		}
		if (row == 0)
		{
			WriteGridText(
				0, row + INVENTORY_ROW_OFFSET,
				"(nothing)",
				visuals::data::Colors::DISABLED,
				visuals::HorizontalAlignment::LEFT);
		}
	}

	static void RefreshGoBack()
	{
		WriteGridText(
			GRID_COLUMNS, 
			GRID_ROWS - 1, 
			"Go Back", 
			(hoverGoBack) ? (visuals::data::Colors::HOVER) : (visuals::data::Colors::NORMAL),
			visuals::HorizontalAlignment::RIGHT);
	}

	static void Refresh()
	{
		visuals::SpriteGrid::Clear(LAYOUT_NAME, SPRITE_GRID);//TODO: put in a clear row?
		RefreshGoBack();
		RefreshInventoryContents();
		RefreshEquipmentContents();
	}

	static void UpdateContents()
	{
		UpdateInventoryContents();
		UpdateEquipmentContents();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
		UpdateContents();
		Refresh();
	}

	static void HandleGoBackMouseMotion(const common::XY<int>&)
	{
		hoverGoBack = true;
	}

	static void HandleInventoryMouseMotion(const common::XY<int>& xy)
	{
		int row = xy.GetY() / GetGridCellHeight();
		if (row < inventoryItems.size())
		{
			hoverInventoryItem = row;
		}
	}

	static void HandleEquipmentMouseMotion(const common::XY<int>& xy)
	{
		int row = xy.GetY() / GetGridCellHeight();
		if (row < equipment.size())
		{
			hoverEquipment = row;
		}
	}

	const std::map<std::string, std::function<void(const common::XY<int>&)>> mouseMotionHandlers =
	{
		{AREA_GO_BACK, HandleGoBackMouseMotion},
		{AREA_EQUIPMENT, HandleEquipmentMouseMotion},
		{AREA_INVENTORY, HandleInventoryMouseMotion}
	};

	static void OnMouseMotionInArea(const std::string& areaName, const common::XY<int>& xy)
	{
		hoverGoBack = false;
		hoverInventoryItem = std::nullopt;
		hoverEquipment = std::nullopt;
		mouseMotionHandlers.find(areaName)->second(xy);
		Refresh();
	}

	static void OnMouseMotionOutsideArea(const common::XY<int>& xy)
	{
		hoverGoBack = false;
		hoverInventoryItem = std::nullopt;
		hoverEquipment = std::nullopt;
		Refresh();
	}

	static bool HandleGoBackMouseButtonUp()
	{
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
		return true;
	}

	static bool HandleInventoryMouseButtonUp()
	{
		if (hoverInventoryItem)
		{
			auto index = hoverInventoryItem.value();
			auto iter = inventoryItems.begin();
			while (index > 0 && iter != inventoryItems.end())
			{
				--index;
				++iter;
			}
			if (iter != inventoryItems.end())
			{
				game::avatar::Items::Interact(iter->first);
				UpdateContents();
				Refresh();
				return true;
			}
		}
		return false;
	}

	static bool HandleEquipmentMouseButtonUp()
	{
		if (hoverEquipment)
		{
			auto index = hoverEquipment.value();
			auto iter = equipment.begin();
			while (index > 0 && iter != equipment.end())
			{
				--index;
				++iter;
			}
			if (iter != equipment.end())
			{
				game::avatar::Equipment::Unequip(iter->first);
				UpdateContents();
				Refresh();
				return true;
			}
		}
		return false;
	}

	const std::map<std::string, std::function<bool()>> mouseButtonUpHandlers =
	{
		{AREA_GO_BACK, HandleGoBackMouseButtonUp},
		{AREA_INVENTORY, HandleInventoryMouseButtonUp},
		{AREA_EQUIPMENT, HandleEquipmentMouseButtonUp},
	};

	static bool OnMouseButtonUpInArea(const std::string& areaName)
	{
		return mouseButtonUpHandlers.find(areaName)->second();
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_INVENTORY, OnEnter);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_INVENTORY, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, OnMouseMotionInArea, OnMouseMotionOutsideArea));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_INVENTORY, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUpInArea));
		::application::Command::SetHandlers(::UIState::IN_PLAY_INVENTORY, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_INVENTORY, LAYOUT_NAME);
	}
}