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
	const std::string FORMAT_HEALTH = "Health {:.0f}";
	const std::string FORMAT_HYDRATION = "H2O {:.0f}";
	const std::string SPRITE_GRID_FLOOR = "Floor";
	const std::string FORMAT_FLOOR_ITEM_COUNT = "{} (x{})";
	const std::string FONT_DEFAULT = "default";
	const std::string AREA_FLOOR = "Floor";
	const std::string AREA_EXITS = "Exits";
	const int FLOOR_ROW_OFFSET = 7;
	const int EXITS_ROW_OFFSET = 1;
	const int STATISTICS_ROW_OFFSET = 1;
	const size_t GRID_COLUMNS = 39;
	const size_t GRID_ROWS = 21;

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

	static void RefreshExits()
	{
		int row = 0;
		for (auto directionName : directionNames)
		{
			std::string color = (hoverDirection.has_value() && hoverDirection.value() == directionName.first) ? (visuals::data::Colors::HOVER) : (visuals::data::Colors::NORMAL);

			visuals::SpriteGrid::WriteText(
				LAYOUT_NAME,
				SPRITE_GRID_FLOOR,
				{ 0, row + EXITS_ROW_OFFSET},
				FONT_DEFAULT,
				directionName.second,
				color,
				visuals::HorizontalAlignment::LEFT);
			++row;
		}
	}

	static void RefreshScore()
	{
		visuals::SpriteGrid::WriteText(
			LAYOUT_NAME, 
			SPRITE_GRID_FLOOR, 
			{ GRID_COLUMNS, 0 }, 
			FONT_DEFAULT, 
			std::format("Discovered {:.0f}%",game::Avatar::GetScore()), 
			visuals::data::Colors::HIGHLIGHT, 
			visuals::HorizontalAlignment::RIGHT);
	}

	static void RefreshAvatarPosition()
	{
		visuals::SpriteGrid::WriteText(
			LAYOUT_NAME,
			SPRITE_GRID_FLOOR,
			{ 0, 0 },
			FONT_DEFAULT,
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
		visuals::SpriteGrid::WriteText(
			LAYOUT_NAME, 
			SPRITE_GRID_FLOOR, 
			{ 0, FLOOR_ROW_OFFSET-1 }, 
			FONT_DEFAULT, 
			"Floor:", 
			visuals::data::Colors::SUBHEADING,
			visuals::HorizontalAlignment::LEFT);
		int row = 0;
		for (auto& floorItem : floorItems)
		{
			auto& descriptor = game::Items::Read(floorItem.first);
			std::string color = (hoverFloorItem.has_value() && hoverFloorItem.value() == row) ? (visuals::data::Colors::HOVER) : (visuals::data::Colors::NORMAL);
			visuals::SpriteGrid::WriteText(
				LAYOUT_NAME, 
				SPRITE_GRID_FLOOR, 
				{ 0, row+ FLOOR_ROW_OFFSET }, 
				FONT_DEFAULT, 
				std::format(FORMAT_FLOOR_ITEM_COUNT, descriptor.name, floorItem.second), 
				color,
				visuals::HorizontalAlignment::LEFT);
			++row;
		}
		if (row == 0)
		{
			visuals::SpriteGrid::WriteText(
				LAYOUT_NAME, 
				SPRITE_GRID_FLOOR, 
				{ 0, row+ FLOOR_ROW_OFFSET }, 
				FONT_DEFAULT, 
				"(nothing)", 
				visuals::data::Colors::DISABLED,
				visuals::HorizontalAlignment::LEFT);
		}
	}

	static void RefreshStatistics()
	{
		visuals::SpriteGrid::WriteText(
			LAYOUT_NAME,
			SPRITE_GRID_FLOOR,
			{ GRID_COLUMNS, STATISTICS_ROW_OFFSET },
			FONT_DEFAULT,
			std::format(FORMAT_HEALTH,game::avatar::Statistics::GetHealth()),
			visuals::data::Colors::HEALTH,
			visuals::HorizontalAlignment::RIGHT);
		visuals::SpriteGrid::WriteText(
			LAYOUT_NAME,
			SPRITE_GRID_FLOOR,
			{ GRID_COLUMNS, STATISTICS_ROW_OFFSET+1 },
			FONT_DEFAULT,
			std::format(FORMAT_HYDRATION, game::avatar::Statistics::GetHydration()),
			visuals::data::Colors::HYDRATION,
			visuals::HorizontalAlignment::RIGHT);
	}

	static void Refresh()
	{
		visuals::SpriteGrid::Clear(LAYOUT_NAME, SPRITE_GRID_FLOOR);//TODO: put in a clear row?
		RefreshAvatarPosition();
		RefreshFloorContents();
		RefreshScore();
		RefreshExits();
		RefreshStatistics();
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
		int row = xy.GetY() / visuals::SpriteGrid::GetCellHeight(LAYOUT_NAME, SPRITE_GRID_FLOOR);
		if (row < floorItems.size())
		{
			hoverFloorItem = row;
		}
		Refresh();
	}

	static void HandleExitsMouseMotion(const common::XY<int>& xy)
	{
		hoverDirection = std::nullopt;
		int row = xy.GetY() / visuals::SpriteGrid::GetCellHeight(LAYOUT_NAME, SPRITE_GRID_FLOOR);
		hoverDirection = (game::Direction)row;
		Refresh();
	}

	static void OnMouseMotionInArea(const std::string& areaName, const common::XY<int>& xy)
	{
		if (areaName == AREA_FLOOR)
		{
			HandleFloorMouseMotion(xy);
		}
		else if (areaName == AREA_EXITS)
		{
			HandleExitsMouseMotion(xy);
		}
	}

	static void OnMouseMotionOutsideArea(const common::XY<int>& xy)
	{
		hoverFloorItem = std::nullopt;
		hoverDirection = std::nullopt;
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

	static bool OnMouseButtonUpInArea(const std::string& areaName)
	{
		if (areaName == AREA_FLOOR)
		{
			return HandleFloorMouseButtonUp();
		}
		else if (areaName == AREA_EXITS)
		{
			return HandleExitsMouseButtonUp();
		}
		return false;
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