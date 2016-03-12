// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "Level.hpp"
#include "../App.hpp"
#include "../Services/ILevelService.hpp"

namespace JuEngine
{
Level::Level() : IObject("level")
{
}

void Level::Load()
{
	App::Level()->UnloadLevel();
	LoadAdditive();
}
}
