// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "DataManager.hpp"
#include "../App.hpp"

namespace JuEngine
{
DataManager::DataManager()
{
	SetId("dataManager");
}

void DataManager::DeleteAll()
{
	mAssets.clear();
}

auto DataManager::Add(const std::type_index type, const Identifier& id, void* asset) -> void*
{
	if(mAssets[type].count(id) != 0)
	{
		App::Log()->Warning("Warning, attempted to load an asset of type '%s' with id '%s'. The asset id for that type is already being used", type.name(), id.GetStringRef().c_str());

		return Get(type, id);
	}

	mAssets[type][id] = std::shared_ptr<void>(asset);

	return Get(type, id);
}

auto DataManager::Get(const std::type_index type) -> void*
{
	if(mAssets[type].size() > 1)
	{
		App::Log()->Warning("Warning: DataManager.Get: Two or more assets of type '%s' were found. Returning the first...", type.name());
	}

	if(mAssets[type].size() >= 1)
	{
		return mAssets[type].begin()->second.get();
	}

	App::Log()->Warning("Warning: DataManager.Get: No asset of type '%s' found", type.name());

	return nullptr;
}

auto DataManager::Get(const std::type_index type, const Identifier& id) -> void*
{
	if(mAssets[type].count(id) != 0)
	{
		return mAssets[type].at(id).get();
	}

	App::Log()->Warning("Warning: DataManager.Get: No asset of type '%s' found with id '%s'", type.name(), id.GetStringRef().c_str());

	return nullptr;
}

auto DataManager::GetAll(const std::type_index type) -> std::vector<void*>
{
	std::vector<void*> vector;

	for(auto &pair : mAssets[type])
	{
		vector.push_back(pair.second.get());
	}

	return vector;
}

void DataManager::Delete(const std::type_index type, const Identifier& id)
{
	if(mAssets[type].count(id) == 0)
	{
		App::Log()->Warning("Warning: DataManager.Delete: No asset of type '%s' found with id '%s'", type.name(), id.GetStringRef().c_str());

		return;
	}

	mAssets[type].erase(id);
}

void DataManager::DeleteAll(const std::type_index type)
{
	mAssets[type].clear();
}
}
