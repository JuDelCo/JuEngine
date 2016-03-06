// Copyright (c) 2016 Juan Delgado (JuDelCo)
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

auto MaterialManager::Add(const Identifier& id, const std::string& textureName, const std::string& shaderName) -> Material*
{
	if(MaterialManager::mInstance->mMaterials.count(id) != 0)
	{
		DebugLog::Write("Warning, attempted to load a material with id '%s'. The material id is already being used", id.GetStringRef().c_str());

		return Get(id);
	}

	MaterialManager::mInstance->mMaterials[id] = std::make_shared<Material>();
	MaterialManager::mInstance->mMaterials.at(id)->SetId(id);
	MaterialManager::mInstance->mMaterials.at(id)->SetShader(shaderName);
	// TODO
	//MaterialManager::mInstance->mMaterials.at(id)->SetTexture(textureName);

	return Get(id);
}

auto MaterialManager::Get(const Identifier& id) -> Material*
{
	if(MaterialManager::mInstance->mMaterials.count(id) != 0)
	{
		return MaterialManager::mInstance->mMaterials.at(id).get();
	}

	DebugLog::Write("Warning: MaterialManager.Get: No material found with id %s", id.GetStringRef().c_str());

	return nullptr;
}

void MaterialManager::Use(const Identifier& id)
{
	Get(id)->Use();
}

void MaterialManager::DisableMaterials()
{
	Material::DisableMaterials();
}
}
