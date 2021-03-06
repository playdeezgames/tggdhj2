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
#include "Game.Avatar.h"
#include "Game.Avatar.Facing.h"
#include "Game.Avatar.Items.h"
#include "Game.Avatar.Log.h"
#include "Game.Avatar.Position.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Avatar.Visits.h"
#include "Game.Items.h"
#include "Game.Nodes.h"
#include "Game.Nodes.Items.h"
#include "Visuals.Areas.h"
#include "Visuals.Data.Colors.h"
#include "Visuals.Menus.h"
#include "Visuals.SpriteGrid.h"
#include "Visuals.Texts.h"
namespace state::in_play::Node
{
	const std::string LAYOUT_NAME = "State.InPlay.Node";

	const std::string FORMAT_CAPTION = "NODE #{}:";
	const std::string FORMAT_FLOOR_ITEM_COUNT = "{} (x{})";

	const std::string AREA_FLOOR = "Floor";
	const std::string AREA_EXITS = "Exits";
	const std::string AREA_ACTIONS = "Actions";

	const size_t GRID_COLUMNS = 39;
	const size_t GRID_ROWS = 21;

	const std::string FLOOR_LABEL = "Proximity:";
	const std::string ACTIONS_LABEL = "Actions:";
	const std::string NOTHING_LABEL = "(nothing)";

	const int FLOOR_ROW_OFFSET = 7;
	const int ACTION_ROW_OFFSET = 7;

	const std::string INVENTORY_TEXT = "You faff about in yer inventory.";
	const std::string STATISTICS_TEXT = "You take stock of yerself.";

	void ClearGrids();
	int GetGridCellHeight();
	void WriteLogText(int, int, const std::string&, const std::string&, const visuals::HorizontalAlignment&);
	void WriteGridText(int, int, const std::string&, const std::string&, const visuals::HorizontalAlignment&);

	void RefreshMoveActions();
	void ClearHoverMoveAction();
	bool HandleExitsMouseButtonUp();
	void HandleExitsMouseMotion(const common::XY<int>&);

	void RefreshStatistics();

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::BACK, ::application::UIState::GoTo(::UIState::LEAVE_PLAY) },
		{::Command::RED, ::application::UIState::GoTo(::UIState::LEAVE_PLAY) }
	};

	static void RefreshAvatarPosition()
	{
		WriteGridText(
			0, 
			0,
			std::format(FORMAT_CAPTION, game::avatar::Position::Read().value()),
			visuals::data::Colors::CAPTION,
			visuals::HorizontalAlignment::LEFT);
	}

	static std::map<game::Item, size_t> floorItems;
	static std::optional<int> hoverFloorItem = std::nullopt;

	static void UpdateFloorContents()
	{
		floorItems = game::nodes::Items::Read(game::avatar::Position::Read().value());
	}

	static void RefreshFloorContents()
	{
		WriteGridText(
			0, 
			FLOOR_ROW_OFFSET - 1, 
			FLOOR_LABEL, 
			visuals::data::Colors::SUBHEADING,
			visuals::HorizontalAlignment::LEFT);
		int row = 0;
		for (auto& floorItem : floorItems)
		{
			auto& descriptor = game::Items::Read(floorItem.first);
			std::string color = (hoverFloorItem.has_value() && hoverFloorItem.value() == row) ? (visuals::data::Colors::HOVER) : (visuals::data::Colors::NORMAL);
			WriteGridText(
				0, row+ FLOOR_ROW_OFFSET, 
				std::format(FORMAT_FLOOR_ITEM_COUNT, descriptor.name, floorItem.second), 
				color,
				visuals::HorizontalAlignment::LEFT);
			++row;
		}
		if (row == 0)
		{
			WriteGridText(
				0, row+ FLOOR_ROW_OFFSET, 
				NOTHING_LABEL, 
				visuals::data::Colors::DISABLED,
				visuals::HorizontalAlignment::LEFT);
		}
	}


	enum class ActionType
	{
		INVENTORY,
		STATISTICS
	};

	const std::map<ActionType, std::string> actionLabels =
	{
		{ActionType::INVENTORY, "Inventory"},
		{ActionType::STATISTICS, "Statistics"}
	};
	std::optional<int> hoverAction = std::nullopt;

	static void RefreshActions()
	{
		WriteGridText(
			GRID_COLUMNS, 
			ACTION_ROW_OFFSET - 1,
			ACTIONS_LABEL,
			visuals::data::Colors::SUBHEADING,
			visuals::HorizontalAlignment::RIGHT);
		int row = 0;
		for (auto actionLabel : actionLabels)
		{
			std::string color = (hoverAction.has_value() && hoverAction.value() == row) ? (visuals::data::Colors::HOVER) : (visuals::data::Colors::NORMAL);

			WriteGridText(GRID_COLUMNS, row + ACTION_ROW_OFFSET, actionLabel.second, color, visuals::HorizontalAlignment::RIGHT);
			++row;
		}
	}

	static void RefreshLog()
	{
		auto entries = game::avatar::Log::Read();
		int row = (int)entries.size();
		for (auto entry : entries)
		{
			--row;
			WriteLogText(0, row, entry.text, entry.color, visuals::HorizontalAlignment::LEFT);
		}
	}

	void Refresh()
	{
		ClearGrids();
		RefreshAvatarPosition();
		RefreshFloorContents();
		RefreshMoveActions();
		RefreshStatistics();
		RefreshActions();
		RefreshLog();
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
		UpdateFloorContents();
		Refresh();
	}

	static void HandleFloorMouseMotion(const common::XY<int>& xy)
	{
		hoverFloorItem = std::nullopt;
		int row = xy.GetY() / GetGridCellHeight();
		if (row < floorItems.size())
		{
			hoverFloorItem = row;
		}
		Refresh();
	}

	static void HandleActionsMouseMotion(const common::XY<int>& xy)
	{
		hoverAction = std::nullopt;
		int row = xy.GetY() / GetGridCellHeight();
		if (row < actionLabels.size())
		{
			hoverAction = row;
		}
		Refresh();
	}

	const std::map<std::string, std::function<void(const common::XY<int>&)>> mouseMotionHandlers =
	{
		{AREA_FLOOR, HandleFloorMouseMotion},
		{AREA_EXITS, HandleExitsMouseMotion},
		{AREA_ACTIONS, HandleActionsMouseMotion}
	};

	static void OnMouseMotionInArea(const std::string& areaName, const common::XY<int>& xy)
	{
		mouseMotionHandlers.find(areaName)->second(xy);
	}

	static void OnMouseMotionOutsideArea(const common::XY<int>& xy)
	{
		hoverFloorItem = std::nullopt;
		ClearHoverMoveAction();
		hoverAction = std::nullopt;
		Refresh();
	}

	static bool HandleFloorMouseButtonUp()
	{
		if (hoverFloorItem)
		{
			auto index = hoverFloorItem.value();
			auto iter = floorItems.begin();
			while (index > 0 && iter!=floorItems.end())
			{
				++iter;
				--index;
			}
			if (iter != floorItems.end())
			{
				auto positionId = game::avatar::Position::Read().value();
				auto onPickUp = game::Items::Read(iter->first).onPickUp;
				if (onPickUp)
				{
					onPickUp.value()(positionId);
				}
				UpdateFloorContents();
				Refresh();
				return true;
			}
		}
		return false;
	}

	static void DispatchAction(const ActionType& action)
	{
		switch (action)
		{
		case ActionType::INVENTORY:
			game::avatar::Log::Write({visuals::data::Colors::HOVER, INVENTORY_TEXT});
			application::UIState::Write(::UIState::IN_PLAY_INVENTORY);
			break;
		case ActionType::STATISTICS:
			game::avatar::Log::Write({ visuals::data::Colors::HOVER, STATISTICS_TEXT });
			application::UIState::Write(::UIState::IN_PLAY_STATISTICS);
			break;
		}
	}

	static bool HandleActionsMouseButtonUp()
	{
		if (hoverAction)
		{
			auto index = hoverAction.value();
			auto iter = actionLabels.begin();
			while (index > 0 && iter != actionLabels.end())
			{
				++iter;
				--index;
			}
			if (iter != actionLabels.end())
			{
				DispatchAction(iter->first);
				return true;
			}
		}
		return false;
	}

	const std::map<std::string, std::function<bool()>> mouseButtonUpHandlers =
	{
		{AREA_FLOOR, HandleFloorMouseButtonUp},
		{AREA_EXITS, HandleExitsMouseButtonUp},
		{AREA_ACTIONS, HandleActionsMouseButtonUp}
	};

	static bool OnMouseButtonUpInArea(const std::string& areaName)
	{
		return mouseButtonUpHandlers.find(areaName)->second();
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_NODE, OnEnter);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_NODE, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, OnMouseMotionInArea, OnMouseMotionOutsideArea));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_NODE, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUpInArea));
		::application::Command::SetHandlers(::UIState::IN_PLAY_NODE, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_NODE, LAYOUT_NAME);
	}
}