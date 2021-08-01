#include "Application.OnEnter.h"
#include "Application.UIState.h"
#include "Game.h"
#include "Game.Avatar.h"
namespace state::in_play::Next
{
	static void OnEnter()
	{
		if (game::Avatar::HasWon())
		{
			application::UIState::Write(::UIState::IN_PLAY_WIN);
			return;
		}
		game::AutoSave();
		application::UIState::Write(::UIState::IN_PLAY_NODE);
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_NEXT, OnEnter);
	}
}
