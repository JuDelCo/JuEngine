// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "LevelManager.hpp"
#include "TimerManager.hpp"
#include "PoolManager.hpp"
#include "SystemManager.hpp"
#include "WindowManager.hpp"
#include "../Resources/Level.hpp"
#include "../Resources/Renderer.hpp"
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
	if(LevelManager::mInstance->mRequestedLoadLevelName != nullptr)
	{
		delete LevelManager::mInstance->mRequestedLoadLevelName;
	}

	LevelManager::mInstance = nullptr;
}

void LevelManager::Update()
{
	if(! mRequestedLoadLevel)
	{
		return;
	}

	std::shared_ptr<Level> level;

	if(mRequestedLoadLevelName == nullptr)
	{
		level = mLevels.at(mRequestedLoadLevelType);
	}
	else
	{
		for(const auto &iLevel : mLevels)
		{
			if(iLevel.second->GetId() == *mRequestedLoadLevelName)
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

	SystemManager::Initialize();

	mRequestedLoadLevel = false;
	mLoadAdditive = false;
}

void LevelManager::Unload()
{
	mLevels.clear();
}

void LevelManager::LoadLevel(const Identifier& id)
{
	LevelManager::mInstance->mRequestedLoadLevelType = typeid(void);
	LevelManager::mInstance->mRequestedLoadLevel = false;

	for(const auto &iLevel : LevelManager::mInstance->mLevels)
	{
		if(iLevel.second->GetId() == id)
		{
			if(LevelManager::mInstance->mRequestedLoadLevelName != nullptr)
			{
				delete LevelManager::mInstance->mRequestedLoadLevelName;
			}

			LevelManager::mInstance->mRequestedLoadLevelName = new Identifier(id);
			LevelManager::mInstance->mLoadAdditive = false;
			LevelManager::mInstance->mRequestedLoadLevel = true;

			return;
		}
	}
}

void LevelManager::LoadLevelAdditive(const Identifier& id)
{
	LevelManager::mInstance->mRequestedLoadLevelType = typeid(void);
	LevelManager::mInstance->mRequestedLoadLevel = false;

	for(const auto &iLevel : LevelManager::mInstance->mLevels)
	{
		if(iLevel.second->GetId() == id)
		{
			if(LevelManager::mInstance->mRequestedLoadLevelName != nullptr)
			{
				delete LevelManager::mInstance->mRequestedLoadLevelName;
			}

			LevelManager::mInstance->mRequestedLoadLevelName = new Identifier(id);
			LevelManager::mInstance->mLoadAdditive = true;
			LevelManager::mInstance->mRequestedLoadLevel = true;

			return;
		}
	}
}

void LevelManager::UnloadLevel()
{
	SystemManager::Reset();
	PoolManager::Unload();
	WindowManager::GetRenderer()->Reset();
	TimerManager::Unload();
}

void LevelManager::Add(std::shared_ptr<Level> level, std::type_index type)
{
	if(LevelManager::mInstance->mLevels.count(type) != 0)
	{
		DebugLog::Write("Warning: LevelManager.Add: Level of type %s exists already", type.name());

		return;
	}

	LevelManager::mInstance->mLevels[type] = level;
}

void LevelManager::LoadLevel(Level* level)
{
	if(level)
	{
		LevelManager::LoadLevel(level->GetId());
	}
}

void LevelManager::LoadLevelAdditive(Level* level)
{
	if(level)
	{
		LevelManager::LoadLevelAdditive(level->GetId());
	}
}

auto LevelManager::Get(std::type_index type) -> Level*
{
	if(LevelManager::mInstance->mLevels.count(type) != 0)
	{
		return &*LevelManager::mInstance->mLevels.at(type);
	}

	DebugLog::Write("Warning: LevelManager.Get: No level found of type %s", type.name());

	return nullptr;
}
}
