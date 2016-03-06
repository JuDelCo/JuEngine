// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/IObject.hpp"
#include "../Managers/LevelManager.hpp"

namespace JuEngine
{
class JUENGINEAPI Level : public IObject
{
	public:
		Level() : IObject("level") {}

		virtual void Load()
		{
			LevelManager::UnloadLevel();
			LoadAdditive();
		}

		virtual void LoadAdditive() = 0;
};
}
