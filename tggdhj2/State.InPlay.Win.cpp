#include "Application.Renderer.h"
#include "Application.Command.h"
#include "Application.MouseButtonUp.h"
#include "Application.OnEnter.h"
#include "Application.UIState.h"
#include <format>
#include "Game.Audio.Mux.h"
#include "Game.Avatar.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Statistics.h"
#include "Visuals.Texts.h"
namespace state::in_play::Win
{
	const std::string LAYOUT_NAME = "State.InPlay.Win";
	const std::string TEXT_EFFECTIVENESS = "Effectiveness";
	const std::string TEXT_CONFIDENCE = "Confidence";
	const std::string TEXT_DIGNITY = "Dignity";
	const std::string FORMAT_EFFECTIVENESS = "Effectiveness: {:.2f}%";

	static bool OnMouseButtonUp(const common::XY<int>& xy, MouseButton)
	{
		::application::UIState::Write(::UIState::MAIN_MENU);
		return true;
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_EFFECTIVENESS, std::format(FORMAT_EFFECTIVENESS, game::Avatar::GetEffectiveness()));
		auto descriptor = game::Statistics::Read(game::Statistic::CONFIDENCE);
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_CONFIDENCE, std::format(descriptor.format, game::avatar::Statistics::GetCurrent(game::Statistic::CONFIDENCE)));
		descriptor = game::Statistics::Read(game::Statistic::DIGNITY);
		visuals::Texts::SetText(LAYOUT_NAME, TEXT_DIGNITY, std::format(descriptor.format, game::avatar::Statistics::GetCurrent(game::Statistic::DIGNITY)));
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_WIN, OnEnter);
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_WIN, OnMouseButtonUp);
		::application::Command::SetHandler(::UIState::IN_PLAY_WIN, ::application::UIState::GoTo(::UIState::MAIN_MENU));
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_WIN, LAYOUT_NAME);
	}
}
