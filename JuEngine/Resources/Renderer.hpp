// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/IObject.hpp"
#include <vector>

namespace JuEngine
{
class Pool;

class JUENGINEAPI Renderer : public IObject
{
	public:
		Renderer() : IObject("renderer") {}

		virtual void Render() = 0;

		void Register(Pool* pool)
		{
			mPools.push_back(pool);
		}

		void Reset()
		{
			mPools.clear();
		}

	protected:
		std::vector<Pool*> mPools;
};
}
