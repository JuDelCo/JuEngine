// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/IObject.hpp"
#include "../Resources/Shader.hpp"
#include <unordered_map>
#include <memory>

namespace JuEngine
{
using JuEngine::Shader;

class JUENGINEAPI ShaderManager : public IObject
{
	public:
		ShaderManager();
		~ShaderManager();

		void Unload();
		static auto Add(const Identifier& id, const std::string& vertexPath, const std::string& fragmentPath) -> Shader*;
		static auto Get(const Identifier& id) -> Shader*;
		static void Use(const Identifier& id);
		static void ReloadAll();
		static void DisableShaders();

	private:
		std::unordered_map<Identifier, std::shared_ptr<Shader>> mShaders;

		// Singleton
		static ShaderManager* mInstance;
};
}
