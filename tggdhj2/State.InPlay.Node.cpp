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
#include "Game.Avatar.Position.h"
#include "Game.Items.h"
#include "Game.Nodes.Items.h"
#include "Visuals.Areas.h"
#include "Visuals.Data.Colors.h"
#include "Visuals.Menus.h"
#include "Visuals.SpriteGrid.h"
#include "Visuals.Texts.h"
namespace state::in_play::Node
{
	const std::string LAYOUT_NAME = "State.InPlay.Node";
	const std::string MENU_ID = "Node";
	const std::string TEXT_CAPTION = "Caption";
	const std::string FORMAT_CAPTION = "==NODE #{}==";
	const std::string SPRITE_GRID_FLOOR = "Floor";
	const std::string FORMAT_FLOOR_ITEM_COUNT = "{} (x{})";
	const std::string FONT_DEFAULT = "default";
	const std::string AREA_FLOOR = "Floor";
	const int FLOOR_ROW_OFFSET = 6;

	enum class NodeMenuItem
	{
		NORTH,
		EAST,
		SOUTH,
		WEST
	};

	static void OnNorth()
	{
		game::avatar::Position::Move(game::Direction::NORTH);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnEast()
	{
		game::avatar::Position::Move(game::Direction::EAST);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnSouth()
	{
		game::avatar::Position::Move(game::Direction::SOUTH);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	static void OnWest()
	{
		game::avatar::Position::Move(game::Direction::WEST);
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	const std::map<NodeMenuItem, std::function<void()>> activators =
	{
		{ NodeMenuItem::NORTH, OnNorth },
		{ NodeMenuItem::EAST, OnEast  },
		{ NodeMenuItem::SOUTH, OnSouth },
		{ NodeMenuItem::WEST, OnWest  },
	};

	static void ActivateItem()
	{
		common::Utility::Dispatch(activators, (NodeMenuItem)visuals::Menus::ReadIndex(LAYOUT_NAME, MENU_ID).value());
	}

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{::Command::GREEN, ActivateItem },
		{::Command::BACK, ::application::UIState::GoTo(::UIState::LEAVE_PLAY) },
		{::Command::RED, ::application::UIState::GoTo(::UIState::LEAVE_PLAY) }
	};

	static void RefreshAvatarPosition()
	{
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_CAPTION, std::format(FORMAT_CAPTION, game::avatar::Position::Read().value()));
	}

	static std::map<game::Item, size_t> floorItems;
	static std::optional<int> hoverFloorItem = std::nullopt;

	static void UpdateFloorContents()
	{
		floorItems = game::nodes::Items::Read(game::avatar::Position::Read().value());
	}

	static void RefreshFloorContents()
	{
		visuals::SpriteGrid::Clear(LAYOUT_NAME, SPRITE_GRID_FLOOR);
		visuals::SpriteGrid::WriteText(LAYOUT_NAME, SPRITE_GRID_FLOOR, { 0, FLOOR_ROW_OFFSET-1 }, FONT_DEFAULT, "Floor:", visuals::data::Colors::HIGHLIGHT);
		int row = 0;
		for (auto& floorItem : floorItems)
		{
			auto& descriptor = game::Items::Read(floorItem.first);
			std::string color = (hoverFloorItem.has_value() && hoverFloorItem.value() == row) ? (visuals::data::Colors::HOVER) : (visuals::data::Colors::NORMAL);
			visuals::SpriteGrid::WriteText(LAYOUT_NAME, SPRITE_GRID_FLOOR, { 0, row+ FLOOR_ROW_OFFSET }, FONT_DEFAULT, std::format(FORMAT_FLOOR_ITEM_COUNT, descriptor.name, floorItem.second), color);
			++row;
		}
		if (row == 0)
		{
			visuals::SpriteGrid::WriteText(LAYOUT_NAME, SPRITE_GRID_FLOOR, { 0, row+ FLOOR_ROW_OFFSET }, FONT_DEFAULT, "(nothing)", visuals::data::Colors::NORMAL);
		}
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
		UpdateFloorContents();
		RefreshAvatarPosition();
		RefreshFloorContents();
	}

	static void HandleFloorMouseMotion(const common::XY<int>& xy)
	{
		hoverFloorItem = std::nullopt;
		int row = xy.GetY() / visuals::SpriteGrid::GetCellHeight(LAYOUT_NAME, SPRITE_GRID_FLOOR);
		if (row < floorItems.size())
		{
			hoverFloorItem = row;
		}
		RefreshFloorContents();
	}

	static void OnMouseMotionInArea(const std::string& areaName, const common::XY<int>& xy)
	{
		if (areaName == AREA_FLOOR)
		{
			HandleFloorMouseMotion(xy);
		}
	}

	static void OnMouseMotionOutsideArea(const common::XY<int>& xy)
	{
		hoverFloorItem = std::nullopt;
		RefreshFloorContents();
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_NODE, OnEnter);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_NODE, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_NODE, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, OnMouseMotionInArea, OnMouseMotionOutsideArea));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_NODE, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::Command::SetHandlers(::UIState::IN_PLAY_NODE, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_NODE, LAYOUT_NAME);
	}
}