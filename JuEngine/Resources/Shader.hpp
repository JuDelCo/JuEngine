// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../IObject.hpp"
#include "../OpenGL.hpp"
#include "../Math.hpp"
#include <vector>
#include <unordered_map>

namespace JuEngine
{
using std::vector;
using std::unordered_map;

class JUENGINEAPI Shader : public IObject
{
public:
	Shader();
	virtual ~Shader();

	void Use();
	static void DisableShaders();
	void SetUniform(const string& name, const float x);
	void SetUniform(const string& name, const float x, const float y);
	void SetUniform(const string& name, const float x, const float y, const float z);
	void SetUniform(const string& name, const float x, const float y, const float z, const float w);
	void SetUniform(const string& name, const vec2 vector);
	void SetUniform(const string& name, const vec3 vector);
	void SetUniform(const string& name, const vec4 vector);
	void SetUniform(const string& name, const mat3 matrix);
	void SetUniform(const string& name, const mat4 matrix);
	void BindUniformBlock(const string& name, const GLuint mUniformBufferBindingIndex);
	void AddShader(const GLenum shaderType, const string& shaderPath);
	void Reload(const bool forceLoad = false);
	void PrintAttributeNames();
	void PrintUniformNames();
	void PrintUniformBlockNames();

private:
	static auto ReadFile(const string& shaderPath) -> const string&;
	static GLuint CreateShader(const GLenum shaderType, const string& shaderPath);
	static GLuint CreateProgram(const vector<GLuint>& shaders);
	static GLint GetUniformLocation(const GLuint shaderProgram, const string& name);
	static GLint GetUniformBlockLocation(const GLuint shaderProgram, const string& name);

	unordered_map<GLenum, string> mShaderFiles;
	GLuint mShaderProgram;
};
}
