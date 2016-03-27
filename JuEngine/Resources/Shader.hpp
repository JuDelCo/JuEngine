// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/IObject.hpp"
#include "../Resources/Math.hpp"
#include <vector>
#include <map>
#include <unordered_map>

typedef unsigned int GLenum;
typedef int GLint;
typedef unsigned int GLuint;

namespace JuEngine
{
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
		void BindUniformBlock(const std::string& name, const GLuint mUniformBufferBindingIndex);

		void AddShader(const GLenum shaderType, const std::string& shaderPath);
		void Reload(const bool forceLoad = false);

		auto PrintAttributeNames() -> std::string;
		auto PrintUniformNames() -> std::string;
		auto PrintUniformBlockNames() -> std::string;

	private:
		auto GetUniformLocation(const std::string& name) -> GLint;
		static auto ReadFile(const std::string& shaderPath) -> const std::string;
		static auto CreateShader(const GLenum shaderType, const std::string& shaderPath) -> GLuint;
		static auto CreateProgram(const std::vector<GLuint>& shaders) -> GLuint;
		static auto GetUniformLocation(const GLuint shaderProgram, const std::string& name) -> GLint;
		static auto GetUniformBlockLocation(const GLuint shaderProgram, const std::string& name) -> GLint;

		std::map<std::string, GLint> mUniformCache;
		std::unordered_map<GLenum, std::string> mShaderFiles;
		GLuint mShaderProgram{0};
};
}
