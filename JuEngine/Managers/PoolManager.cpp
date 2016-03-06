// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "PoolManager.hpp"
#include "../Resources/DebugLog.hpp"

namespace JuEngine
{
PoolManager* PoolManager::mInstance = nullptr;

PoolManager::PoolManager() : IObject("poolManager")
{
	if(PoolManager::mInstance != nullptr)
	{
		ThrowRuntimeError("Error, there are a PoolManager instance created already");
	}

	PoolManager::mInstance = this;
}

PoolManager::~PoolManager()
{
	PoolManager::mInstance = nullptr;
}

auto PoolManager::Create(const Identifier& id) -> Pool*
{
	if(PoolManager::mInstance->mPools.count(id) != 0)
	{
		DebugLog::Write("Warning, attempted to create a pool with id '%s'. The pool id is already being used", id.GetStringRef().c_str());

		return Get(id);
	}

	PoolManager::mInstance->mPools[id] = std::make_shared<Pool>();

	return Get(id);
}

auto PoolManager::Get(const Identifier& id) -> Pool*
{
	if(PoolManager::mInstance->mPools.count(id) != 0)
	{
		return PoolManager::mInstance->mPools.at(id).get();
	}

	DebugLog::Write("Warning: PoolManager.Get: No pool found with id %s", id.GetStringRef().c_str());

	return nullptr;
}

void PoolManager::Unload()
{
	PoolManager::mInstance->mPools.clear();
}
}
