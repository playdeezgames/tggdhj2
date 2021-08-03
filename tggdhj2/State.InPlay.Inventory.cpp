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
#include "Game.Avatar.Items.h"
#include "Game.Avatar.Position.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Avatar.Visits.h"
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
	const std::string FORMAT_INVENTORY_ITEM_COUNT = "{} (x{})";
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

	static void UpdateInventoryContents()
	{
		inventoryItems = game::avatar::Items::ReadAll();
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
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
		UpdateInventoryContents();
		Refresh();
	}

	static void OnMouseMotionInArea(const std::string& areaName, const common::XY<int>& xy)
	{
		hoverGoBack = areaName == AREA_GO_BACK;
		Refresh();
		//if (areaName == AREA_FLOOR)
		//{
		//	HandleFloorMouseMotion(xy);
		//}
	}

	static void OnMouseMotionOutsideArea(const common::XY<int>& xy)
	{
		hoverGoBack = false;
		Refresh();
	}

	static bool OnMouseButtonUpInArea(const std::string& areaName)
	{
		if (areaName == AREA_GO_BACK)
		{
			application::UIState::Write(::UIState::IN_PLAY_NEXT);
			return true;
		}
		return false;
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