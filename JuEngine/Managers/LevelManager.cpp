// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "LevelManager.hpp"
#include "../Entity/Pool.hpp"
#include "../Resources/Level.hpp"
#include "../Resources/Renderer.hpp"
#include "../Resources/Timer.hpp"
#include "../App.hpp"
#include "../Services/IDataService.hpp"
#include "../Services/ISystemService.hpp"
#include "../Services/IWindowService.hpp"

namespace JuEngine
{
LevelManager::LevelManager()
{
	SetId("levelManager");
}

LevelManager::~LevelManager()
{
	if(mRequestedLoadLevelName != nullptr)
	{
		delete mRequestedLoadLevelName;
	}
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

	App::System()->Initialize();

	mRequestedLoadLevel = false;
	mLoadAdditive = false;
}

void LevelManager::LoadLevel(const Identifier& id)
{
	mRequestedLoadLevelType = typeid(void);
	mRequestedLoadLevel = false;

	for(const auto &iLevel : mLevels)
	{
		if(iLevel.second->GetId() == id)
		{
			if(mRequestedLoadLevelName != nullptr)
			{
				delete mRequestedLoadLevelName;
			}

			mRequestedLoadLevelName = new Identifier(id);
			mLoadAdditive = false;
			mRequestedLoadLevel = true;

			return;
		}
	}
}

void LevelManager::LoadLevelAdditive(const Identifier& id)
{
	mRequestedLoadLevelType = typeid(void);
	mRequestedLoadLevel = false;

	for(const auto &iLevel : mLevels)
	{
		if(iLevel.second->GetId() == id)
		{
			if(mRequestedLoadLevelName != nullptr)
			{
				delete mRequestedLoadLevelName;
			}

			mRequestedLoadLevelName = new Identifier(id);
			mLoadAdditive = true;
			mRequestedLoadLevel = true;

			return;
		}
	}
}

void LevelManager::UnloadLevel()
{
	App::System()->Reset();
	App::Data()->DeleteAll<Pool>();
	App::Window()->GetRenderer()->Reset();
	App::Data()->DeleteAll<Timer>();
}

void LevelManager::DeleteAll()
{
	mLevels.clear();
}

void LevelManager::Add(std::shared_ptr<Level> level, std::type_index type)
{
	if(mLevels.count(type) != 0)
	{
		App::Log()->Warning("Warning: LevelManager.Add: Level of type %s exists already", type.name());

		return;
	}

	mLevels[type] = level;
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
	if(mLevels.count(type) != 0)
	{
		return &*mLevels.at(type);
	}

	App::Log()->Warning("Warning: LevelManager.Get: No level found of type %s", type.name());

	return nullptr;
}
}
