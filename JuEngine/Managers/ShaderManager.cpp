// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "ShaderManager.hpp"
#include "../Resources/DebugLog.hpp"

namespace JuEngine
{
ShaderManager* ShaderManager::mInstance = nullptr;

ShaderManager::ShaderManager() : IObject("shaderManager")
{
	if(ShaderManager::mInstance != nullptr)
	{
		ThrowRuntimeError("Error, there are a ShaderManager instance created already");
	}

	ShaderManager::mInstance = this;
}

ShaderManager::~ShaderManager()
{
	ShaderManager::mInstance = nullptr;
}

void ShaderManager::Unload()
{
	mShaders.clear();
}

auto ShaderManager::Add(const Identifier& id, const std::string& vertexPath, const std::string& fragmentPath) -> Shader*
{
	if(ShaderManager::mInstance->mShaders.count(id) != 0)
	{
		DebugLog::Write("Warning, attempted to load a shader with id '%s'. The shader id is already being used", id.GetStringRef().c_str());

		return Get(id);
	}

	auto shadersPath = "Assets/Shaders/";

	ShaderManager::mInstance->mShaders[id] = std::make_shared<Shader>();
	ShaderManager::mInstance->mShaders.at(id)->SetId(id);

	ShaderManager::mInstance->mShaders.at(id)->AddShader(GL_VERTEX_SHADER, shadersPath + vertexPath);
	ShaderManager::mInstance->mShaders.at(id)->AddShader(GL_FRAGMENT_SHADER, shadersPath + fragmentPath);
	ShaderManager::mInstance->mShaders.at(id)->Reload(true);

	return Get(id);
}

auto ShaderManager::Get(const Identifier& id) -> Shader*
{
	if(ShaderManager::mInstance->mShaders.count(id) != 0)
	{
		return ShaderManager::mInstance->mShaders.at(id).get();
	}

	DebugLog::Write("Warning: ShaderManager.Get: No shader found with id %s", id.GetStringRef().c_str());

	return nullptr;
}

void ShaderManager::Use(const Identifier& id)
{
	auto shader = Get(id);

	if(shader)
	{
		shader->Use();
	}
}

void ShaderManager::ReloadAll()
{
	for(const auto &shader : ShaderManager::mInstance->mShaders)
	{
		shader.second->Reload();
	}
}

void ShaderManager::DisableShaders()
{
	Shader::DisableShaders();
}
}
