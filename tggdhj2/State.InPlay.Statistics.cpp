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
#include "Game.Avatar.Equipment.h"
#include "Game.Avatar.Items.h"
#include "Game.Avatar.Position.h"
#include "Game.Avatar.Statistics.h"
#include "Game.Avatar.Visits.h"
#include "Game.EquipSlots.h"
#include "Game.Items.h"
#include "Game.Nodes.h"
#include "Game.Nodes.Items.h"
#include "Game.Statistic.h"
#include "Visuals.Areas.h"
#include "Visuals.Data.Colors.h"
#include "Visuals.Menus.h"
#include "Visuals.SpriteGrid.h"
#include "Visuals.Texts.h"
namespace state::in_play::Statistics
{
	const std::string LAYOUT_NAME = "State.InPlay.Statistics";
	const std::string SPRITE_GRID = "Grid";
	const std::string FONT_DEFAULT = "default";
	const size_t GRID_COLUMNS = 39;
	const size_t GRID_ROWS = 21;
	const int STATISTICS_ROW_OFFSET = 1;
	const std::string AREA_GO_BACK = "GoBack";

	const std::map<::Command, std::function<void()>> commandHandlers =
	{
		{::Command::BACK, ::application::UIState::GoTo(::UIState::IN_PLAY_NODE) },
		{::Command::RED, ::application::UIState::GoTo(::UIState::IN_PLAY_NODE) }
	};

	static bool hoverGoBack = false;

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
	static int GetGridCellHeight()
	{
		return visuals::SpriteGrid::GetCellHeight(LAYOUT_NAME, SPRITE_GRID);
	}

	static void RefreshGoBack()
	{
		WriteGridText(
			GRID_COLUMNS,
			GRID_ROWS - 1,
			"Go Back",
			(hoverGoBack) ? (visuals::data::Colors::HOVER) : (visuals::data::Colors::NORMAL),
			visuals::HorizontalAlignment::RIGHT);
	}

	struct StatisticsFormatter
	{
		game::Statistic statistic;
		std::string color;
		std::string format;
	};

	const std::list<StatisticsFormatter> statisticsFormatters =
	{
		{
			game::Statistic::HEALTH,
			visuals::data::Colors::HEALTH,
			"\x80{:3.0f}"
		},
		{
			game::Statistic::HYDRATION,
			visuals::data::Colors::HYDRATION,
			"\x81{:3.0f}"
		}
	};

	static void RefreshStatistics()
	{
		WriteGridText(
			0,
			STATISTICS_ROW_OFFSET - 1,
			"Statistics:",
			visuals::data::Colors::SUBHEADING,
			visuals::HorizontalAlignment::LEFT);
		int row = 0;
		for (auto formatter : statisticsFormatters)
		{
			WriteGridText(0, row + STATISTICS_ROW_OFFSET, std::format(formatter.format, game::avatar::Statistics::GetCurrent(formatter.statistic)), formatter.color, visuals::HorizontalAlignment::LEFT);
			++row;
		}
	}

	static void Refresh()
	{
		visuals::SpriteGrid::Clear(LAYOUT_NAME, SPRITE_GRID);
		RefreshStatistics();
		RefreshGoBack();
	}

	static void UpdateContents()
	{
	}

	static void OnEnter()
	{
		game::audio::Mux::Play(game::audio::Mux::Theme::MAIN);
		UpdateContents();
		Refresh();
	}

	static void HandleGoBackMouseMotion(const common::XY<int>&)
	{
		hoverGoBack = true;
	}

	const std::map<std::string, std::function<void(const common::XY<int>&)>> mouseMotionHandlers =
	{
		{AREA_GO_BACK, HandleGoBackMouseMotion}
	};

	static void OnMouseMotionInArea(const std::string& areaName, const common::XY<int>& xy)
	{
		hoverGoBack = false;
		mouseMotionHandlers.find(areaName)->second(xy);
		Refresh();
	}

	static void OnMouseMotionOutsideArea(const common::XY<int>& xy)
	{
		hoverGoBack = false;
		Refresh();
	}

	static bool HandleGoBackMouseButtonUp()
	{
		application::UIState::Write(::UIState::IN_PLAY_NEXT);
		return true;
	}

	const std::map<std::string, std::function<bool()>> mouseButtonUpHandlers =
	{
		{AREA_GO_BACK, HandleGoBackMouseButtonUp},
	};

	static bool OnMouseButtonUpInArea(const std::string& areaName)
	{
		return mouseButtonUpHandlers.find(areaName)->second();
	}

	void Start()
	{
		::application::OnEnter::AddHandler(::UIState::IN_PLAY_STATISTICS, OnEnter);
		::application::MouseMotion::AddHandler(::UIState::IN_PLAY_STATISTICS, visuals::Areas::HandleMouseMotion(LAYOUT_NAME, OnMouseMotionInArea, OnMouseMotionOutsideArea));
		::application::MouseButtonUp::AddHandler(::UIState::IN_PLAY_STATISTICS, visuals::Areas::HandleMouseButtonUp(LAYOUT_NAME, OnMouseButtonUpInArea));
		::application::Command::SetHandlers(::UIState::IN_PLAY_STATISTICS, commandHandlers);
		::application::Renderer::SetRenderLayout(::UIState::IN_PLAY_STATISTICS, LAYOUT_NAME);
	}
}