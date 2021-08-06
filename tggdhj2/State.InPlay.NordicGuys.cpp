#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Application.OnEnter.h"
#include "Application.Renderer.h"
#include "Application.UIState.h"
#include "Common.Utility.h"
#include "Game.Audio.Mux.h"
#include "Visuals.Areas.h"
#include "Visuals.Menus.h"
namespace state::in_play::NordicGuys
{
	const std::string LAYOUT_NAME = "State.InPlay.NordicGuys";
	const std::string MENU_ID = "ConfirmNordicGuys";

	enum class ConfirmNordicGuysItem
	{
		NO,
		YES
	};

	const std::map<ConfirmNordicGuysItem, std::function<void()>> activators =
	{
		{ ConfirmNordicGuysItem::NO, ::application::UIState::GoTo(::UIState::IN_PLAY_NEXT) },
		{ ConfirmNordicGuysItem::YES, ::application::UIState::GoTo(::UIState::IN_PLAY_NEXT) }
	};

	static void ActivateItem()
	{
		common::Utility::Dispatch(activators, (ConfirmNordicGuysItem)visuals::Menus::ReadIndex(LAYOUT_NAME, MENU_ID).value());
	}

	const std::map<Command, std::function<void()>> commandHandlers =
	{
		{ ::Command::UP, visuals::Menus::NavigatePrevious(LAYOUT_NAME, MENU_ID) },
		{ ::Command::DOWN, visuals::Menus::NavigateNext(LAYOUT_NAME, MENU_ID) },
		{ ::Command::GREEN, ActivateItem },
		{ ::Command::BACK, ::application::UIState::GoTo(::UIState::IN_PLAY_NEXT) },
		{ ::Command::RED, ::application::UIState::GoTo(::UIState::IN_PLAY_NEXT) }
	};

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_NORDIC_GUYS, game::audio::Mux::GoToTheme(game::audio::Mux::Theme::MAIN));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_NORDIC_GUYS, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_NORDIC_GUYS, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::Command::SetHandlers(::UIState::IN_PLAY_NORDIC_GUYS, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_NORDIC_GUYS, LAYOUT_NAME);
	}
}