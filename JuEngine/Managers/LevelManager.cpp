// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "LevelManager.hpp"
#include "../Resources/Level.hpp"
#include "EntityManager.hpp"
#include "TimerManager.hpp"
#include "TimerCallbackManager.hpp"
#include "../Resources/DebugLog.hpp"

namespace JuEngine
{
LevelManager* LevelManager::mInstance = nullptr;

LevelManager::LevelManager() : IObject("levelManager")
{
	if(LevelManager::mInstance != nullptr)
	{
		ThrowRuntimeError("Error, there are a LevelManager instance created already");
	}

	LevelManager::mInstance = this;
}

LevelManager::~LevelManager()
{
	LevelManager::mInstance = nullptr;
}

void LevelManager::Update()
{
	if(! mRequestedLoadLevel)
	{
		return;
	}

	shared_ptr<Level> level;

	if(mRequestedLoadLevelName.empty())
	{
		level = mLevels.at(mRequestedLoadLevelType);
	}
	else
	{
		for(auto& iLevel : mLevels)
		{
			if(iLevel.second->GetName() == mRequestedLoadLevelName)
			{
				level = iLevel.second;
				break;
			}
		}
	}

	if(mLoadAdditive)
	{
		level->LoadAdditive();
	}
	else
	{
		level->Load();
	}

	mRequestedLoadLevel = false;
	mLoadAdditive = false;
}

void LevelManager::Unload()
{
	mLevels.clear();
}

void LevelManager::LoadLevel(const string& name)
{
	LevelManager::mInstance->mRequestedLoadLevelType = typeid(void);
	LevelManager::mInstance->mRequestedLoadLevel = false;

	for(auto& iLevel : LevelManager::mInstance->mLevels)
	{
		if(iLevel.second->GetName() == name)
		{
			LevelManager::mInstance->mRequestedLoadLevelName = name;
			LevelManager::mInstance->mLoadAdditive = false;
			LevelManager::mInstance->mRequestedLoadLevel = true;

			return;
		}
	}
}

void LevelManager::LoadLevelAdditive(const string& name)
{
	LevelManager::mInstance->mRequestedLoadLevelType = typeid(void);
	LevelManager::mInstance->mRequestedLoadLevel = false;

	for(auto& iLevel : LevelManager::mInstance->mLevels)
	{
		if(iLevel.second->GetName() == name)
		{
			LevelManager::mInstance->mRequestedLoadLevelName = name;
			LevelManager::mInstance->mLoadAdditive = true;
			LevelManager::mInstance->mRequestedLoadLevel = true;

			return;
		}
	}
}

void LevelManager::UnloadLevel()
{
	EntityManager::Unload();
	TimerManager::Unload();
	TimerCallbackManager::Unload();
}

void LevelManager::Add(shared_ptr<Level> level, type_index type)
{
	if(LevelManager::mInstance->mLevels.count(type) != 0)
	{
		DebugLog::Write("Warning: LevelManager.Add: Level of type %s exists already", type.name());

		return;
	}

	LevelManager::mInstance->mLevels[type] = std::move(level);
}

void LevelManager::LoadLevel(Level* level)
{
	if(level)
	{
		LevelManager::LoadLevel(level->GetName());
	}
}

void LevelManager::LoadLevelAdditive(Level* level)
{
	if(level)
	{
		LevelManager::LoadLevelAdditive(level->GetName());
	}
}

auto LevelManager::Get(type_index type) -> Level*
{
	if(LevelManager::mInstance->mLevels.count(type) != 0)
	{
		return &*LevelManager::mInstance->mLevels.at(type);
	}

	DebugLog::Write("Warning: LevelManager.Get: No level found of type %s", type.name());

	return nullptr;
}
}
