// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "Renderer.hpp"

namespace JuEngine
{
Renderer::Renderer() : IObject("renderer")
{
}

void Renderer::Register(Pool* pool)
{
	mPools.push_back(pool);
}

void Renderer::Reset()
{
	mPools.clear();
}
}
