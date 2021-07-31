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

	static void RefreshFloorContents()
	{
		visuals::SpriteGrid::Clear(LAYOUT_NAME, SPRITE_GRID_FLOOR);
		auto floorItems = game::nodes::Items::Read(game::avatar::Position::Read().value());
		int row = 0;
		for (auto& floorItem : floorItems)
		{
			auto& descriptor = game::Items::Read(floorItem.first);
			visuals::SpriteGrid::WriteText(LAYOUT_NAME, SPRITE_GRID_FLOOR, { 0, row }, FONT_DEFAULT, std::format(FORMAT_FLOOR_ITEM_COUNT, descriptor.name, floorItem.second), visuals::data::Colors::NORMAL);
			++row;
		}
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
		RefreshAvatarPosition();
		RefreshFloorContents();
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_NODE, OnEnter);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_NODE, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_NODE, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::Command::SetHandlers(::UIState::IN_PLAY_NODE, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_NODE, LAYOUT_NAME);
	}
}