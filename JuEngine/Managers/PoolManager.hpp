// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/IObject.hpp"
#include "../Entity/Pool.hpp"
#include <unordered_map>
#include <memory>

namespace JuEngine
{
class JUENGINEAPI PoolManager : public IObject
{
	public:
		PoolManager();
		~PoolManager();

		static auto Create(const Identifier& id) -> Pool*;
		static auto Get(const Identifier& id) -> Pool*;
		static void Unload();

	private:
		std::unordered_map<Identifier, std::shared_ptr<Pool>> mPools;

		// Singleton
		static PoolManager* mInstance;
};
}
