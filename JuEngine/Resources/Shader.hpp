// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/IObject.hpp"
#include "../Resources/Math.hpp"
#include <vector>
#include <map>

namespace JuEngine
{
enum class ShaderType
{
	Vertex,
	Geometry,
	Fragment,
	TessControl,
	TessEvaluation,
	Compute
};

class JUENGINEAPI Shader : public IObject
{
	public:
		Shader(const std::string& vertexPath, const std::string& fragmentPath);
		~Shader();

		void Use();
		static void ReloadAll();
		static void DisableShaders();
		void SetUniform(const std::string& name, const float x);
		void SetUniform(const std::string& name, const float x, const float y);
		void SetUniform(const std::string& name, const float x, const float y, const float z);
		void SetUniform(const std::string& name, const float x, const float y, const float z, const float w);
		void SetUniform(const std::string& name, const vec2 vector);
		void SetUniform(const std::string& name, const vec3 vector);
		void SetUniform(const std::string& name, const vec4 vector);
		void SetUniform(const std::string& name, const mat3 matrix);
		void SetUniform(const std::string& name, const mat4 matrix);
		void SetUniformTexture(const std::string& name, const unsigned int index);
		void BindUniformBlock(const std::string& name, const uint32_t mUniformBufferBindingIndex);

		void AddShader(const ShaderType shaderType, const std::string& shaderPath);
		void Reload(const bool forceLoad = false);

		auto PrintAttributeNames() -> std::string;
		auto PrintUniformNames() -> std::string;
		auto PrintUniformBlockNames() -> std::string;

	private:
		auto GetUniformLocation(const std::string& name) -> int32_t;
		static auto ReadFile(const std::string& shaderPath) -> const std::string;
		static auto CreateShader(const ShaderType shaderType, const std::string& shaderPath) -> uint32_t;
		static auto CreateProgram(const std::vector<uint32_t>& shaders) -> uint32_t;
		static auto GetUniformLocation(const uint32_t shaderProgram, const std::string& name) -> int32_t;
		static auto GetUniformBlockLocation(const uint32_t shaderProgram, const std::string& name) -> int32_t;

		std::map<std::string, int32_t> mUniformCache;
		std::map<ShaderType, std::string> mShaderFiles;
		uint32_t mShaderProgram{0};
};
}
