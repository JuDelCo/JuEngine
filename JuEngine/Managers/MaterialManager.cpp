// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "MaterialManager.hpp"
#include "../Resources/DebugLog.hpp"

namespace JuEngine
{
MaterialManager* MaterialManager::mInstance = nullptr;

MaterialManager::MaterialManager() : IObject("materialManager")
{
	if(MaterialManager::mInstance != nullptr)
	{
		ThrowRuntimeError("Error, there are a MaterialManager instance created already");
	}

	MaterialManager::mInstance = this;
}

MaterialManager::~MaterialManager()
{
	MaterialManager::mInstance = nullptr;
}

void MaterialManager::Unload()
{
	mMaterials.clear();
}

auto MaterialManager::Add(const string& name, const string& textureName, const string& shaderName) -> Material*
{
	if(MaterialManager::mInstance->mMaterials.count(name) != 0)
	{
		DebugLog::Write("Warning, attempted to load a material with name '%s'. The material name is already being used", name.c_str());

		return Get(name);
	}

	MaterialManager::mInstance->mMaterials[name] = std::make_shared<Material>();
	MaterialManager::mInstance->mMaterials.at(name)->SetName(name);
	MaterialManager::mInstance->mMaterials.at(name)->SetShader(shaderName);
	//MaterialManager::mInstance->mMaterials.at(name)->SetTexture(textureName);

	return Get(name);
}

auto MaterialManager::Get(const string& name) -> Material*
{
	if(MaterialManager::mInstance->mMaterials.count(name) != 0)
	{
		return MaterialManager::mInstance->mMaterials.at(name).get();
	}

	DebugLog::Write("Warning: MaterialManager.Get: No material found with name %s", name.c_str());

	return nullptr;
}

void MaterialManager::Use(const string& name)
{
	Get(name)->Use();
}

void MaterialManager::DisableMaterials()
{
	Material::DisableMaterials();
}
}
