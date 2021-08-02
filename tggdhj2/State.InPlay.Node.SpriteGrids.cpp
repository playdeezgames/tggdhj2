#include "Visuals.SpriteGrid.h"
namespace state::in_play::Node
{
	const std::string LAYOUT_NAME = "State.InPlay.Node";
	const std::string SPRITE_GRID = "Grid";
	const std::string SPRITE_GRID_LOG = "Log";
	const std::string FONT_DEFAULT = "default";
	const std::string FONT_SMALL = "font4x6";

	void ClearGrids()
	{
		visuals::SpriteGrid::Clear(LAYOUT_NAME, SPRITE_GRID);
		visuals::SpriteGrid::Clear(LAYOUT_NAME, SPRITE_GRID_LOG);
	}

	int GetGridCellHeight()
	{
		return visuals::SpriteGrid::GetCellHeight(LAYOUT_NAME, SPRITE_GRID);
	}

	void WriteLogText(int column, int row, const std::string& text, const std::string& color, const visuals::HorizontalAlignment& alignment)
	{
		visuals::SpriteGrid::WriteText(
			LAYOUT_NAME,
			SPRITE_GRID_LOG,
			{ column, row },
			FONT_SMALL,
			text,
			color,
			alignment);
	}

	void WriteGridText(int column, int row, const std::string& text, const std::string& color, const visuals::HorizontalAlignment& alignment)
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
}