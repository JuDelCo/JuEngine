// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/IObject.hpp"

namespace JuEngine
{
class JUENGINEAPI IAppController : public IObject
{
	public:
		virtual void Run() = 0;
		virtual void Stop() = 0;

	protected:
		virtual void BootstrapServices() = 0;
};
}
