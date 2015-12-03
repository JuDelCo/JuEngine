// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../IObject.hpp"
#include "../Resources/Shader.hpp"
#include <unordered_map>

namespace JuEngine
{
using JuEngine::Shader;
using std::unordered_map;

class JUENGINEAPI ShaderManager : public IObject
{
public:
	ShaderManager();
	virtual ~ShaderManager();

	void Unload();
	static auto Add(const string& name, const string& vertexPath, const string& fragmentPath) -> Shader*;
	static auto Get(const string& name) -> Shader*;
	static void Use(const string& name);
	static void ReloadAll();
	static void DisableShaders();

private:
	unordered_map<string, shared_ptr<Shader>> mShaders;

	// Singleton
	static ShaderManager* mInstance;
};
}
