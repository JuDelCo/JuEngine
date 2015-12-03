// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../IObject.hpp"

namespace JuEngine
{
class JUENGINEAPI Renderer : public IObject
{
public:
	Renderer() : IObject("renderer") {}
	virtual ~Renderer() = default;

	virtual void Render() = 0;
};
}
