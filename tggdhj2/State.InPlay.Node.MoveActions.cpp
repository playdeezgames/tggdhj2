#include "Application.UIState.h"
#include "Common.XY.h"
#include <functional>
#include "Game.Avatar.Facing.h"
#include "Game.Avatar.Position.h"
#include <optional>
#include <map>
#include <string>
#include "Visuals.Data.Colors.h"
#include "Visuals.HorizontalAlignment.h"
namespace state::in_play::Node
{
	const int EXITS_ROW_OFFSET = 1;

	void Refresh();
	void WriteGridText(int, int, const std::string&, const std::string&, const visuals::HorizontalAlignment&);
	int GetGridCellHeight();

	enum class MoveAction
	{
		MOVE_AHEAD,
		TURN_RIGHT,
		TURN_AROUND,
		TURN_LEFT
	};

	const std::map<MoveAction, std::string> moveActionNames =
	{
		{MoveAction::MOVE_AHEAD, "Move Ahead"},
		{MoveAction::TURN_RIGHT, "Turn Right"},
		{MoveAction::TURN_AROUND, "Turn Around"},
		{MoveAction::TURN_LEFT, "Turn Left"}
	};
	std::optional<MoveAction> hoverMoveAction = std::nullopt;

	void ClearHoverMoveAction()
	{
		hoverMoveAction = std::nullopt;
	}

	void RefreshMoveActions()
	{
		int row = 0;
		for (auto moveActionName : moveActionNames)
		{
			std::string color = (hoverMoveAction.has_value() && hoverMoveAction.value() == moveActionName.first) ? (visuals::data::Colors::HOVER) : (visuals::data::Colors::NORMAL);
			WriteGridText(0, row + EXITS_ROW_OFFSET, moveActionName.second, color, visuals::HorizontalAlignment::LEFT);
			++row;
		}
	}

	void HandleExitsMouseMotion(const common::XY<int>& xy)
	{
		hoverMoveAction = std::nullopt;
		int row = xy.GetY() / GetGridCellHeight();
		hoverMoveAction = (MoveAction)row;
		Refresh();
	}

	const std::map<MoveAction, std::function<void()>> moveActionTable =
	{
		{ MoveAction::MOVE_AHEAD, game::avatar::Position::Move},
		{ MoveAction::TURN_AROUND, game::avatar::Facing::TurnAround},
		{ MoveAction::TURN_LEFT, game::avatar::Facing::TurnLeft},
		{ MoveAction::TURN_RIGHT, game::avatar::Facing::TurnRight}
	};

	bool HandleExitsMouseButtonUp()
	{
		if (hoverMoveAction)
		{
			moveActionTable.find(hoverMoveAction.value())->second();
			application::UIState::Write(::UIState::IN_PLAY_NEXT);
			return true;
		}
		return false;
	}
}