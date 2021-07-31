#include "Application.OnEnter.h"
#include "Application.UIState.h"
#include "Game.h"
namespace state::in_play::Next
{
	static void OnEnter()
	{
		game::AutoSave();
		application::UIState::Write(::UIState::IN_PLAY_NODE);
		return;
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_NEXT, OnEnter);
	}
}
