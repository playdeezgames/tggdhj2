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
#include "Game.Avatar.Items.h"
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
	const std::string FORMAT_HEALTH = "\x80{:2.0f}";
	const std::string FORMAT_HYDRATION = "\x81{:2.0f}";
	const std::string FORMAT_SCORE = "\x82{:2.0f}%";
	const std::string FORMAT_FLOOR_ITEM_COUNT = "{} (x{})";

	const std::string SPRITE_GRID = "Grid";

	const std::string FONT_DEFAULT = "default";

	const std::string AREA_FLOOR = "Floor";
	const std::string AREA_EXITS = "Exits";
	const std::string AREA_ACTIONS = "Actions";

	const size_t GRID_COLUMNS = 39;
	const size_t GRID_ROWS = 21;

	const std::string FLOOR_LABEL = "Floor:";
	const std::string ACTIONS_LABEL = "Actions:";
	const std::string NOTHING_LABEL = "(nothing)";

	const int FLOOR_ROW_OFFSET = 7;
	const int EXITS_ROW_OFFSET = 1;
	const int STATISTICS_ROW_OFFSET = 1;
	const int ACTION_ROW_OFFSET = 7;

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::BACK, ::application::UIState::GoTo(::UIState::LEAVE_PLAY) },
		{::Command::RED, ::application::UIState::GoTo(::UIState::LEAVE_PLAY) }
	};

	const std::map<game::Direction, std::string> directionNames =
	{
		{game::Direction::NORTH, "Go North"},
		{game::Direction::EAST, "Go East"},
		{game::Direction::SOUTH, "Go South"},
		{game::Direction::WEST, "Go West"}
	};

	std::optional<game::Direction> hoverDirection = std::nullopt;

	static void WriteGridText(int column, int row, const std::string& text, const std::string& color, const visuals::HorizontalAlignment& alignment)
	{
		visuals::SpriteGrid::WriteText(
			LAYOUT_NAME,
			SPRITE_GRID,
			{ column, row },
			FONT_DEFAULT,
			text,
			color,
			alignment);
	}

	static void RefreshExits()
	{
		int row = 0;
		for (auto directionName : directionNames)
		{
			std::string color = (hoverDirection.has_value() && hoverDirection.value() == directionName.first) ? (visuals::data::Colors::HOVER) : (visuals::data::Colors::NORMAL);
			WriteGridText(0, row + EXITS_ROW_OFFSET, directionName.second, color, visuals::HorizontalAlignment::LEFT);
			++row;
		}
	}

	static void RefreshScore()
	{
		WriteGridText(
			GRID_COLUMNS, 
			0, 
			std::format(FORMAT_SCORE,game::Avatar::GetScore()),
			visuals::data::Colors::HIGHLIGHT, 
			visuals::HorizontalAlignment::RIGHT);
	}

	static void RefreshAvatarPosition()
	{
		WriteGridText(
			0, 0,
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
			0, FLOOR_ROW_OFFSET-1, 
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

	static void RefreshStatistics()
	{
		WriteGridText(
			GRID_COLUMNS, STATISTICS_ROW_OFFSET,
			std::format(FORMAT_HEALTH,game::avatar::Statistics::GetHealth()),
			visuals::data::Colors::HEALTH,
			visuals::HorizontalAlignment::RIGHT);
		WriteGridText(
			GRID_COLUMNS, STATISTICS_ROW_OFFSET+1,
			std::format(FORMAT_HYDRATION, game::avatar::Statistics::GetHydration()),
			visuals::data::Colors::HYDRATION,
			visuals::HorizontalAlignment::RIGHT);
	}

	enum class ActionType
	{
		INVENTORY
	};

	const std::map<ActionType, std::string> actionLabels =
	{
		{ActionType::INVENTORY, "Inventory"}
	};
	std::optional<int> hoverAction = std::nullopt;

	static void RefreshActions()
	{
		WriteGridText(
			GRID_COLUMNS, ACTION_ROW_OFFSET - 1,
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

	static void Refresh()
	{
		visuals::SpriteGrid::Clear(LAYOUT_NAME, SPRITE_GRID);//TODO: put in a clear row?
		RefreshAvatarPosition();
		RefreshFloorContents();
		RefreshScore();
		RefreshExits();
		RefreshStatistics();
		RefreshActions();
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
		int row = xy.GetY() / visuals::SpriteGrid::GetCellHeight(LAYOUT_NAME, SPRITE_GRID);
		if (row < floorItems.size())
		{
			hoverFloorItem = row;
		}
		Refresh();
	}

	static void HandleExitsMouseMotion(const common::XY<int>& xy)
	{
		hoverDirection = std::nullopt;
		int row = xy.GetY() / visuals::SpriteGrid::GetCellHeight(LAYOUT_NAME, SPRITE_GRID);
		hoverDirection = (game::Direction)row;
		Refresh();
	}

	static void HandleActionsMouseMotion(const common::XY<int>& xy)
	{
		hoverAction = std::nullopt;
		int row = xy.GetY() / visuals::SpriteGrid::GetCellHeight(LAYOUT_NAME, SPRITE_GRID);
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
		hoverDirection = std::nullopt;
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
				game::nodes::Items::Remove(positionId, iter->first, 1);
				game::avatar::Items::Add(iter->first, 1);
				UpdateFloorContents();
				Refresh();
				return true;
			}
		}
		return false;
	}

	static bool HandleExitsMouseButtonUp()
	{
		if (hoverDirection)
		{
			game::avatar::Position::Move(hoverDirection.value());
			application::UIState::Write(::UIState::IN_PLAY_NEXT);
			return true;
		}
		return false;
	}

	static bool HandleActionsMouseButtonUp()
	{
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