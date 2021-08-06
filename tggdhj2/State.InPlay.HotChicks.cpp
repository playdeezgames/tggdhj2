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
namespace state::in_play::HotChicks
{
	const std::string LAYOUT_NAME = "State.InPlay.HotChicks";
	const std::string MENU_ID = "ConfirmHotChicks";

	enum class ConfirmHotChicksItem
	{
		NO,
		YES
	};

	const std::map<ConfirmHotChicksItem, std::function<void()>> activators =
	{
		{ ConfirmHotChicksItem::NO, ::application::UIState::GoTo(::UIState::IN_PLAY_NEXT) },
		{ ConfirmHotChicksItem::YES, ::application::UIState::GoTo(::UIState::IN_PLAY_LOSE) }
	};

	static void ActivateItem()
	{
		common::Utility::Dispatch(activators, (ConfirmHotChicksItem)visuals::Menus::ReadIndex(LAYOUT_NAME, MENU_ID).value());
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
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_HOT_CHICKS, game::audio::Mux::GoToTheme(game::audio::Mux::Theme::MAIN));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_HOT_CHICKS, visuals::Areas::HandleMenuMouseButtonUp(LAYOUT_NAME, ActivateItem));
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_HOT_CHICKS, visuals::Areas::HandleMenuMouseMotion(LAYOUT_NAME));
		::application::Command::SetHandlers(::UIState::IN_PLAY_HOT_CHICKS, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_HOT_CHICKS, LAYOUT_NAME);
	}
}