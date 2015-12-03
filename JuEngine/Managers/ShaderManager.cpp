// Copyright (c) 2015 Juan Delgado (JuDelCo)
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

auto ShaderManager::Add(const string& name, const string& vertexPath, const string& fragmentPath) -> Shader*
{
	if(ShaderManager::mInstance->mShaders.count(name) != 0)
	{
		DebugLog::Write("Warning, attempted to load a shader with name '%s'. The shader name is already being used", name.c_str());

		return Get(name);
	}

	auto shadersPath = "Assets/Shaders/";

	ShaderManager::mInstance->mShaders[name] = std::make_shared<Shader>();
	ShaderManager::mInstance->mShaders.at(name)->SetName(name);

	ShaderManager::mInstance->mShaders.at(name)->AddShader(GL_VERTEX_SHADER, shadersPath + vertexPath);
	ShaderManager::mInstance->mShaders.at(name)->AddShader(GL_FRAGMENT_SHADER, shadersPath + fragmentPath);
	ShaderManager::mInstance->mShaders.at(name)->Reload(true);

	return Get(name);
}

auto ShaderManager::Get(const string& name) -> Shader*
{
	if(ShaderManager::mInstance->mShaders.count(name) != 0)
	{
		return ShaderManager::mInstance->mShaders.at(name).get();
	}

	DebugLog::Write("Warning: ShaderManager.Get: No shader found with name %s", name.c_str());

	return nullptr;
}

void ShaderManager::Use(const string& name)
{
	auto shader = Get(name);

	if(shader)
	{
		shader->Use();
	}
}

void ShaderManager::ReloadAll()
{
	for(auto shader : ShaderManager::mInstance->mShaders)
	{
		shader.second->Reload();
	}
}

void ShaderManager::DisableShaders()
{
	Shader::DisableShaders();
}
}
