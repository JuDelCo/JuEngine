// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "Component.hpp"

namespace JuEngine
{
class JUENGINEAPI ScriptBehaviour : public Component
{
public:
	ScriptBehaviour() = default;
	virtual ~ScriptBehaviour() = default;

	virtual void Init() {}
	virtual void Update() {}
	virtual void FixedUpdate() {}
};
}
