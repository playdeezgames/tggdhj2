#include "Common.RNG.h"
#include "Common.Utility.h"
#include "Game.Achievements.h"
#include <SDL.h>
namespace common::Application
{
	int Run(const std::string&, const std::vector<std::string>&);
}
int main(int argc, char** argv)
{
	if (game::Achievements::ShouldRestartApp())
	{
		return 0;
	}
	const std::string APPLICATION = "config/ui/application.json";
	common::RNG::Seed();
	auto arguments = common::Utility::ParseCommandLine(argc, argv);
	return common::Application::Run(APPLICATION, arguments);
}

