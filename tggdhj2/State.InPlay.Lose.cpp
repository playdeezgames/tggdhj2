#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.OnEnter.h"
#include "Application.UIState.h"
#include "Common.Audio.h"
#include <format>
#include "Game.Audio.Mux.h"
#include "Game.Avatar.Counters.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Statistics.h"
#include "Visuals.Texts.h"
namespace state::in_play::Lose
{
	const std::string LAYOUT_NAME = "State.InPlay.Lose";
	const std::string TEXT_CONFIDENCE = "Confidence";
	const std::string TEXT_DIGNITY = "Dignity";
	const std::string TEXT_FOUND_CAR = "FoundCar";
	const std::string SFX_LOSE = "deadhunter";

	static bool OnMouseButtonUp(const common::XY<int>& xy, MouseButton)
	{
		::application::UIState::Write(::UIState::MAIN_MENU);
		return true;
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::LOSE);
		auto descriptor = game::Statistics::Read(game::Statistic::CONFIDENCE);
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_CONFIDENCE, std::format(descriptor.format, game::avatar::Statistics::GetCurrent(game::Statistic::CONFIDENCE)));
		descriptor = game::Statistics::Read(game::Statistic::DIGNITY);
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_DIGNITY, std::format(descriptor.format, game::avatar::Statistics::GetCurrent(game::Statistic::DIGNITY)));
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_FOUND_CAR, (game::avatar::Counters::IsSet(game::avatar::Counter::FOUND_CAR)) ? ("...but at least you found yer car, dude.") : ("Dude! Where's yer car?"));
		common::audio::Sfx::Play(SFX_LOSE);
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_LOSE, OnEnter);
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_LOSE, OnMouseButtonUp);
		::application::Command::SetHandler(::UIState::IN_PLAY_LOSE, ::application::UIState::GoTo(::UIState::MAIN_MENU));
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_LOSE, LAYOUT_NAME);
	}
}
