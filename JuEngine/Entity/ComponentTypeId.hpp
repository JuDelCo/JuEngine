// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "IComponent.hpp"
#include <vector>

namespace JuEngine
{
typedef unsigned int ComponentId;
typedef std::vector<ComponentId> ComponentIdList;

struct JUENGINEAPI ComponentTypeId
{
	public:
		template<typename T>
		static const ComponentId Get()
		{
			static_assert((std::is_base_of<IComponent, T>::value && ! std::is_same<IComponent, T>::value),
				"Class type must be derived from IComponent");

			static ComponentId id = mCounter++;
			return id;
		}

		static unsigned int Count()
		{
			return mCounter;
		}

	private:
		static unsigned int mCounter;
};
}
