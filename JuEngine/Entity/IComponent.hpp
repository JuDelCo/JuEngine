// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../DllExport.hpp"

namespace JuEngine
{
class JUENGINEAPI IComponent
{
	friend class Entity;

	protected:
		IComponent() = default;
};
}
