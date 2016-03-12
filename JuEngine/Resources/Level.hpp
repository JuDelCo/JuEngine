// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/IObject.hpp"

namespace JuEngine
{
class JUENGINEAPI Level : public IObject
{
	public:
		Level();

		virtual void Load();
		virtual void LoadAdditive() = 0;
};
}
