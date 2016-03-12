// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Services/ISystemService.hpp"

namespace JuEngine
{
class SystemContainer;

class JUENGINEAPI SystemManager : public ISystemService
{
	public:
		SystemManager();

		void Add(std::shared_ptr<ISystem> system);

	protected:
		void Initialize();
		void Execute();
		void FixedExecute();
		void Reset();

	private:
		std::shared_ptr<SystemContainer> mSystemContainer;
};
}
