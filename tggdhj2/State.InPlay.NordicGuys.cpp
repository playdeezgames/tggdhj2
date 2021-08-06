#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.MouseMotion.h"
#include "Application.OnEnter.h"
#include "Application.Renderer.h"
#include "Application.UIState.h"
#include "Common.Utility.h"
#include "Game.Audio.Mux.h"
#include "Game.Avatar.Items.h"
#include "Game.Avatar.Log.h"
#include "Game.Avatar.Position.h"
#include "Game.Nodes.Items.h"
#include "Visuals.Areas.h"
#include "Visuals.Data.Colors.h"
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

	static void OnYes()
	{
		auto position = game::avatar::Position::Read().value();
		game::avatar::Items::Remove(game::Item::TRANSFUNCTIONER, 1);
		game::nodes::Items::Remove(position, game::Item::NORDIC_GUYS, 1);
		game::avatar::Log::Write({visuals::data::Colors::HOVER, "You give the Transfunctioner to the Nordic Guys."});
		game::avatar::Log::Write({ visuals::data::Colors::SUBHEADING, "They use it to return to their home planet. Of Norway." });
		::application::UIState::Write(::UIState::IN_PLAY_NEXT);
	}

	const std::map<ConfirmNordicGuysItem, std::function<void()>> activators =
	{
		{ ConfirmNordicGuysItem::NO, ::application::UIState::GoTo(::UIState::IN_PLAY_NEXT) },
		{ ConfirmNordicGuysItem::YES, OnYes}
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