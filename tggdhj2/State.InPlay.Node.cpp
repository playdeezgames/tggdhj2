#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Application.OnEnter.h"
#include "Application.Renderer.h"
#include "Application.UIState.h"
#include "Common.Audio.h"
#include "Common.Utility.h"
#include "Data.Stores.h"
#include "Game.Audio.Mux.h"
#include "Visuals.Areas.h"
#include "Visuals.Menus.h"
#include "Visuals.Texts.h"
namespace state::in_play::Node
{
	const std::string LAYOUT_NAME = "State.InPlay.Node";
	const std::string MENU_ID = "Node";

	enum class NodeMenuItem
	{
		NORTH,
		EAST,
		SOUTH,
		WEST
	};

	static void OnNorth()
	{
		return;
	}

	static void OnEast()
	{
		return;
	}

	static void OnSouth()
	{
		return;
	}

	static void OnWest()
	{
		return;
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

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
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