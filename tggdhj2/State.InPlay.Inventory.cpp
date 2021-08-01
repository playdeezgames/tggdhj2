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

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::BACK, ::application::UIState::GoTo(::UIState::LEAVE_PLAY) },
		{::Command::RED, ::application::UIState::GoTo(::UIState::LEAVE_PLAY) }
	};

	//static std::map<game::Item, size_t> floorItems;
	//static std::optional<int> hoverFloorItem = std::nullopt;

	//static void UpdateFloorContents()
	//{
	//	floorItems = game::nodes::Items::Read(game::avatar::Position::Read().value());
	//}

	//static void RefreshFloorContents()
	//{
	//	visuals::SpriteGrid::WriteText(
	//		LAYOUT_NAME,
	//		SPRITE_GRID,
	//		{ 0, FLOOR_ROW_OFFSET - 1 },
	//		FONT_DEFAULT,
	//		"Floor:",
	//		visuals::data::Colors::SUBHEADING,
	//		visuals::HorizontalAlignment::LEFT);
	//	int row = 0;
	//	for (auto& floorItem : floorItems)
	//	{
	//		auto& descriptor = game::Items::Read(floorItem.first);
	//		std::string color = (hoverFloorItem.has_value() && hoverFloorItem.value() == row) ? (visuals::data::Colors::HOVER) : (visuals::data::Colors::NORMAL);
	//		visuals::SpriteGrid::WriteText(
	//			LAYOUT_NAME,
	//			SPRITE_GRID,
	//			{ 0, row + FLOOR_ROW_OFFSET },
	//			FONT_DEFAULT,
	//			std::format(FORMAT_FLOOR_ITEM_COUNT, descriptor.name, floorItem.second),
	//			color,
	//			visuals::HorizontalAlignment::LEFT);
	//		++row;
	//	}
	//	if (row == 0)
	//	{
	//		visuals::SpriteGrid::WriteText(
	//			LAYOUT_NAME,
	//			SPRITE_GRID,
	//			{ 0, row + FLOOR_ROW_OFFSET },
	//			FONT_DEFAULT,
	//			"(nothing)",
	//			visuals::data::Colors::DISABLED,
	//			visuals::HorizontalAlignment::LEFT);
	//	}
	//}

	static void Refresh()
	{
		visuals::SpriteGrid::Clear(LAYOUT_NAME, SPRITE_GRID);//TODO: put in a clear row?
		//RefreshFloorContents();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
		//UpdateFloorContents();
		Refresh();
	}

	static void OnMouseMotionInArea(const std::string& areaName, const common::XY<int>& xy)
	{
		//if (areaName == AREA_FLOOR)
		//{
		//	HandleFloorMouseMotion(xy);
		//}
	}

	static void OnMouseMotionOutsideArea(const common::XY<int>& xy)
	{
		Refresh();
	}

	static bool OnMouseButtonUpInArea(const std::string& areaName)
	{
		//if (areaName == AREA_FLOOR)
		//{
		//	return HandleFloorMouseButtonUp();
		//}
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