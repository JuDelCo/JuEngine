// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "Shader.hpp"
#include "DebugLog.hpp"
#include <fstream>
#include <streambuf>

namespace JuEngine
{
Shader::Shader() : IObject("shader")
{
}

Shader::~Shader()
{
	glDeleteProgram(mShaderProgram);
}

void Shader::Use()
{
	glUseProgram(mShaderProgram);
}

void Shader::DisableShaders()
{
	glUseProgram(0);
}

void Shader::SetUniform(const string& name, const float x)
{
	auto location = GetUniformLocation(mShaderProgram, name);
	glUniform1f(location, x);
}

void Shader::SetUniform(const string& name, const float x, const float y)
{
	auto location = GetUniformLocation(mShaderProgram, name);
	glUniform2f(location, x, y);
}

void Shader::SetUniform(const string& name, const float x, const float y, const float z)
{
	auto location = GetUniformLocation(mShaderProgram, name);
	glUniform3f(location, x, y, z);
}

void Shader::SetUniform(const string& name, const float x, const float y, const float z, const float w)
{
	auto location = GetUniformLocation(mShaderProgram, name);
	glUniform4f(location, x, y, z, w);
}

void Shader::SetUniform(const string& name, const vec2 vector)
{
	auto location = GetUniformLocation(mShaderProgram, name);
	glUniform2fv(location, 1, Math::GetDataPtr(vector));
}

void Shader::SetUniform(const string& name, const vec3 vector)
{
	auto location = GetUniformLocation(mShaderProgram, name);
	glUniform3fv(location, 1, Math::GetDataPtr(vector));
}

void Shader::SetUniform(const string& name, const vec4 vector)
{
	auto location = GetUniformLocation(mShaderProgram, name);
	glUniform4fv(location, 1, Math::GetDataPtr(vector));
}

void Shader::SetUniform(const string& name, const mat3 matrix)
{
	auto location = GetUniformLocation(mShaderProgram, name);
	glUniformMatrix3fv(location, 1, GL_FALSE, Math::GetDataPtr(matrix));
}

void Shader::SetUniform(const string& name, const mat4 matrix)
{
	auto location = GetUniformLocation(mShaderProgram, name);
	glUniformMatrix4fv(location, 1, GL_FALSE, Math::GetDataPtr(matrix));
}

void Shader::BindUniformBlock(const string& name, const GLuint mUniformBufferBindingIndex)
{
	auto location = GetUniformBlockLocation(mShaderProgram, name);
	glUniformBlockBinding(mShaderProgram, location, mUniformBufferBindingIndex);
}

void Shader::AddShader(const GLenum shaderType, const string& shaderPath)
{
	mShaderFiles[shaderType] = shaderPath;
}

void Shader::Reload(const bool forceLoad)
{
	vector<GLuint> shaders;
	bool errors = false;

	for(auto shaderFile : mShaderFiles)
	{
		auto shader = Shader::CreateShader(shaderFile.first, shaderFile.second);

		if(! shader)
		{
			if(forceLoad) { ThrowRuntimeError(""); }
			else errors = true;

			break;
		}

		shaders.emplace_back(shader);
	}

	if(errors)
	{
		for(auto shader : shaders)
		{
			glDeleteShader(shader);
		}

		return;
	}

	auto shaderProgram = Shader::CreateProgram(shaders);

	if(! shaderProgram)
	{
		if(forceLoad) { ThrowRuntimeError(""); }
	}
	else
	{
		if(mShaderProgram)
		{
			glDeleteProgram(mShaderProgram);
		}

		mShaderProgram = shaderProgram;

		// TODO: Shader: UBO indexes -> Renderer (Forward Renderer)
		BindUniformBlock("GlobalMatrices", 0);
		BindUniformBlock("World", 1);
		BindUniformBlock("Material", 2);
		BindUniformBlock("Light", 3);
	}
}

void Shader::PrintAttributeNames()
{
	GLint numActiveAttribs = 0;
	glGetProgramInterfaceiv(mShaderProgram, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numActiveAttribs);

    const GLuint numProperties = 4, numValues = 4;
	const GLenum properties[numProperties] = {GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_ARRAY_SIZE};
	GLint values[numValues];

	for(int attrIndex = 0; attrIndex < numActiveAttribs; ++attrIndex)
	{
		glGetProgramResourceiv(mShaderProgram, GL_PROGRAM_INPUT, attrIndex, numProperties, properties, numValues, NULL, values);

		char buffer[values[0]];
		glGetProgramResourceName(mShaderProgram, GL_PROGRAM_INPUT, attrIndex, values[0], NULL, buffer);
		DebugLog::Write("> %s", buffer);

		string uTypeText;
		GLuint uTypeSize;

		switch(values[1])
		{
			case GL_FLOAT: 				uTypeText = "GL_FLOAT"; 			uTypeSize = 1 * sizeof(GLfloat);	break;
			case GL_FLOAT_VEC2: 		uTypeText = "GL_FLOAT_VEC2"; 		uTypeSize = 2 * sizeof(GLfloat);	break;
			case GL_FLOAT_VEC3: 		uTypeText = "GL_FLOAT_VEC3"; 		uTypeSize = 3 * sizeof(GLfloat);	break;
			case GL_FLOAT_VEC4: 		uTypeText = "GL_FLOAT_VEC4"; 		uTypeSize = 4 * sizeof(GLfloat);	break;
			case GL_INT: 				uTypeText = "GL_INT"; 				uTypeSize = 1 * sizeof(GLint);		break;
			case GL_INT_VEC2: 			uTypeText = "GL_INT_VEC2"; 			uTypeSize = 2 * sizeof(GLint);		break;
			case GL_INT_VEC3: 			uTypeText = "GL_INT_VEC3"; 			uTypeSize = 3 * sizeof(GLint);		break;
			case GL_INT_VEC4: 			uTypeText = "GL_INT_VEC4"; 			uTypeSize = 4 * sizeof(GLint);		break;
			case GL_UNSIGNED_INT: 		uTypeText = "GL_UNSIGNED_INT"; 		uTypeSize = 1 * sizeof(GLuint);		break;
			case GL_UNSIGNED_INT_VEC2: 	uTypeText = "GL_UNSIGNED_INT_VEC2"; uTypeSize = 2 * sizeof(GLuint);		break;
			case GL_UNSIGNED_INT_VEC3: 	uTypeText = "GL_UNSIGNED_INT_VEC3"; uTypeSize = 3 * sizeof(GLuint);		break;
			case GL_UNSIGNED_INT_VEC4: 	uTypeText = "GL_UNSIGNED_INT_VEC4"; uTypeSize = 4 * sizeof(GLuint);		break;
			case GL_BOOL: 				uTypeText = "GL_BOOL"; 				uTypeSize = 1 * sizeof(GLboolean);	break;
			case GL_BOOL_VEC2: 			uTypeText = "GL_BOOL_VEC2"; 		uTypeSize = 2 * sizeof(GLboolean);	break;
			case GL_BOOL_VEC3: 			uTypeText = "GL_BOOL_VEC3"; 		uTypeSize = 3 * sizeof(GLboolean);	break;
			case GL_BOOL_VEC4: 			uTypeText = "GL_BOOL_VEC4"; 		uTypeSize = 4 * sizeof(GLboolean);	break;
			case GL_FLOAT_MAT2: 		uTypeText = "GL_FLOAT_MAT2"; 		uTypeSize = 4 * sizeof(GLfloat);	break;
			case GL_FLOAT_MAT3: 		uTypeText = "GL_FLOAT_MAT3"; 		uTypeSize = 9 * sizeof(GLfloat);	break;
			case GL_FLOAT_MAT4: 		uTypeText = "GL_FLOAT_MAT4"; 		uTypeSize = 16 * sizeof(GLfloat);	break;
			case GL_FLOAT_MAT2x3: 		uTypeText = "GL_FLOAT_MAT2x3"; 		uTypeSize = 6 * sizeof(GLfloat);	break;
			case GL_FLOAT_MAT2x4: 		uTypeText = "GL_FLOAT_MAT2x4"; 		uTypeSize = 8 * sizeof(GLfloat);	break;
			case GL_FLOAT_MAT3x2: 		uTypeText = "GL_FLOAT_MAT3x2"; 		uTypeSize = 6 * sizeof(GLfloat);	break;
			case GL_FLOAT_MAT3x4: 		uTypeText = "GL_FLOAT_MAT3x4"; 		uTypeSize = 12 * sizeof(GLfloat);	break;
			case GL_FLOAT_MAT4x2: 		uTypeText = "GL_FLOAT_MAT4x2"; 		uTypeSize = 8 * sizeof(GLfloat);	break;
			case GL_FLOAT_MAT4x3: 		uTypeText = "GL_FLOAT_MAT4x3"; 		uTypeSize = 12 * sizeof(GLfloat);	break;
			default: 					uTypeText = "Unknown"; 				uTypeSize = 0; 						break;
		}

		DebugLog::Write("     location: %i", values[2]);
		DebugLog::Write("     type: %s", uTypeText.c_str());
		DebugLog::Write("     size: %i bytes", uTypeSize);
		DebugLog::Write("     elements: %i", values[3]);
	}
}

void Shader::PrintUniformNames()
{
	GLint numActiveUniforms = 0;
	glGetProgramInterfaceiv(mShaderProgram, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numActiveUniforms);

    const GLuint numProperties = 7, numValues = 7;
	const GLenum properties[numProperties] = {GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_ARRAY_SIZE, GL_BLOCK_INDEX, GL_ARRAY_STRIDE, GL_MATRIX_STRIDE};
	GLint values[numValues];

	for(int uniformIndex = 0; uniformIndex < numActiveUniforms; ++uniformIndex)
	{
		glGetProgramResourceiv(mShaderProgram, GL_UNIFORM, uniformIndex, numProperties, properties, numValues, NULL, values);

		// If it's a Uniform Block member...
		if(values[4] != -1) continue;

		char buffer[values[0]];
		glGetProgramResourceName(mShaderProgram, GL_UNIFORM, uniformIndex, values[0], NULL, buffer);
		DebugLog::Write("> %s", buffer);

		string uTypeText;
		GLuint uTypeSize;

		switch(values[1])
		{
			case GL_FLOAT: 				uTypeText = "GL_FLOAT"; 			uTypeSize = 1 * sizeof(GLfloat);	break;
			case GL_FLOAT_VEC2: 		uTypeText = "GL_FLOAT_VEC2"; 		uTypeSize = 2 * sizeof(GLfloat);	break;
			case GL_FLOAT_VEC3: 		uTypeText = "GL_FLOAT_VEC3"; 		uTypeSize = 3 * sizeof(GLfloat);	break;
			case GL_FLOAT_VEC4: 		uTypeText = "GL_FLOAT_VEC4"; 		uTypeSize = 4 * sizeof(GLfloat);	break;
			case GL_INT: 				uTypeText = "GL_INT"; 				uTypeSize = 1 * sizeof(GLint);		break;
			case GL_INT_VEC2: 			uTypeText = "GL_INT_VEC2"; 			uTypeSize = 2 * sizeof(GLint);		break;
			case GL_INT_VEC3: 			uTypeText = "GL_INT_VEC3"; 			uTypeSize = 3 * sizeof(GLint);		break;
			case GL_INT_VEC4: 			uTypeText = "GL_INT_VEC4"; 			uTypeSize = 4 * sizeof(GLint);		break;
			case GL_UNSIGNED_INT: 		uTypeText = "GL_UNSIGNED_INT"; 		uTypeSize = 1 * sizeof(GLuint);		break;
			case GL_UNSIGNED_INT_VEC2: 	uTypeText = "GL_UNSIGNED_INT_VEC2"; uTypeSize = 2 * sizeof(GLuint);		break;
			case GL_UNSIGNED_INT_VEC3: 	uTypeText = "GL_UNSIGNED_INT_VEC3"; uTypeSize = 3 * sizeof(GLuint);		break;
			case GL_UNSIGNED_INT_VEC4: 	uTypeText = "GL_UNSIGNED_INT_VEC4"; uTypeSize = 4 * sizeof(GLuint);		break;
			case GL_BOOL: 				uTypeText = "GL_BOOL"; 				uTypeSize = 1 * sizeof(GLboolean);	break;
			case GL_BOOL_VEC2: 			uTypeText = "GL_BOOL_VEC2"; 		uTypeSize = 2 * sizeof(GLboolean);	break;
			case GL_BOOL_VEC3: 			uTypeText = "GL_BOOL_VEC3"; 		uTypeSize = 3 * sizeof(GLboolean);	break;
			case GL_BOOL_VEC4: 			uTypeText = "GL_BOOL_VEC4"; 		uTypeSize = 4 * sizeof(GLboolean);	break;
			case GL_FLOAT_MAT2: 		uTypeText = "GL_FLOAT_MAT2"; 		uTypeSize = 4 * sizeof(GLfloat);	break;
			case GL_FLOAT_MAT3: 		uTypeText = "GL_FLOAT_MAT3"; 		uTypeSize = 9 * sizeof(GLfloat);	break;
			case GL_FLOAT_MAT4: 		uTypeText = "GL_FLOAT_MAT4"; 		uTypeSize = 16 * sizeof(GLfloat);	break;
			case GL_FLOAT_MAT2x3: 		uTypeText = "GL_FLOAT_MAT2x3"; 		uTypeSize = 6 * sizeof(GLfloat);	break;
			case GL_FLOAT_MAT2x4: 		uTypeText = "GL_FLOAT_MAT2x4"; 		uTypeSize = 8 * sizeof(GLfloat);	break;
			case GL_FLOAT_MAT3x2: 		uTypeText = "GL_FLOAT_MAT3x2"; 		uTypeSize = 6 * sizeof(GLfloat);	break;
			case GL_FLOAT_MAT3x4: 		uTypeText = "GL_FLOAT_MAT3x4"; 		uTypeSize = 12 * sizeof(GLfloat);	break;
			case GL_FLOAT_MAT4x2: 		uTypeText = "GL_FLOAT_MAT4x2"; 		uTypeSize = 8 * sizeof(GLfloat);	break;
			case GL_FLOAT_MAT4x3: 		uTypeText = "GL_FLOAT_MAT4x3"; 		uTypeSize = 12 * sizeof(GLfloat);	break;
			default: 					uTypeText = "Unknown"; 				uTypeSize = 0; 						break;
		}

		DebugLog::Write("     location: %i", values[2]);
		DebugLog::Write("     type: %s", uTypeText.c_str());
		DebugLog::Write("     size: %i bytes", uTypeSize);
		DebugLog::Write("     elements: %i", values[3]);
		DebugLog::Write("     array stride: %i bytes", values[5]);
		DebugLog::Write("     matrix stride: %i bytes", values[6]);
	}
}

void Shader::PrintUniformBlockNames()
{
	GLint numActiveUniformBlocks = 0;
	glGetProgramInterfaceiv(mShaderProgram, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &numActiveUniformBlocks);

    const GLuint numProperties = 4, numValues = 4;
	const GLenum properties[numProperties] = {GL_NAME_LENGTH, GL_NUM_ACTIVE_VARIABLES, GL_BUFFER_BINDING, GL_BUFFER_DATA_SIZE};
	GLint values[numValues];

	GLint numActiveUniforms = 0;
	glGetProgramInterfaceiv(mShaderProgram, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numActiveUniforms);

    const GLuint numPropertiesUnif = 7, numValuesUnif = 7;
	const GLenum propertiesUnif[numPropertiesUnif] = {GL_NAME_LENGTH, GL_TYPE, GL_OFFSET, GL_ARRAY_SIZE, GL_BLOCK_INDEX, GL_ARRAY_STRIDE, GL_MATRIX_STRIDE};
	GLint valuesUnif[numValuesUnif];

	for(int uniformBlockIndex = 0; uniformBlockIndex < numActiveUniformBlocks; ++uniformBlockIndex)
	{
		glGetProgramResourceiv(mShaderProgram, GL_UNIFORM_BLOCK, uniformBlockIndex, numProperties, properties, numValues, NULL, values);

		char buffer[values[0]];
		glGetProgramResourceName(mShaderProgram, GL_UNIFORM_BLOCK, uniformBlockIndex, values[0], NULL, buffer);
		DebugLog::Write("> %s", buffer);
		DebugLog::Write("     binding point: %i", values[2]);
		DebugLog::Write("     total size: %i bytes", values[3]);
		DebugLog::Write("     members: %i", values[1]);

		for(int uniformIndex = 0; uniformIndex < numActiveUniforms; ++uniformIndex)
		{
			glGetProgramResourceiv(mShaderProgram, GL_UNIFORM, uniformIndex, numPropertiesUnif, propertiesUnif, numValuesUnif, NULL, valuesUnif);

			if(valuesUnif[4] != uniformBlockIndex) continue;

			char bufferUnif[valuesUnif[0]];
			glGetProgramResourceName(mShaderProgram, GL_UNIFORM, uniformIndex, valuesUnif[0], NULL, bufferUnif);
			DebugLog::Write("     - %s", bufferUnif);

			string uTypeText;
			GLuint uTypeSize;

			switch(valuesUnif[1])
			{
				case GL_FLOAT: 				uTypeText = "GL_FLOAT"; 			uTypeSize = 1 * sizeof(GLfloat);	break;
				case GL_FLOAT_VEC2: 		uTypeText = "GL_FLOAT_VEC2"; 		uTypeSize = 2 * sizeof(GLfloat);	break;
				case GL_FLOAT_VEC3: 		uTypeText = "GL_FLOAT_VEC3"; 		uTypeSize = 3 * sizeof(GLfloat);	break;
				case GL_FLOAT_VEC4: 		uTypeText = "GL_FLOAT_VEC4"; 		uTypeSize = 4 * sizeof(GLfloat);	break;
				case GL_INT: 				uTypeText = "GL_INT"; 				uTypeSize = 1 * sizeof(GLint);		break;
				case GL_INT_VEC2: 			uTypeText = "GL_INT_VEC2"; 			uTypeSize = 2 * sizeof(GLint);		break;
				case GL_INT_VEC3: 			uTypeText = "GL_INT_VEC3"; 			uTypeSize = 3 * sizeof(GLint);		break;
				case GL_INT_VEC4: 			uTypeText = "GL_INT_VEC4"; 			uTypeSize = 4 * sizeof(GLint);		break;
				case GL_UNSIGNED_INT: 		uTypeText = "GL_UNSIGNED_INT"; 		uTypeSize = 1 * sizeof(GLuint);		break;
				case GL_UNSIGNED_INT_VEC2: 	uTypeText = "GL_UNSIGNED_INT_VEC2"; uTypeSize = 2 * sizeof(GLuint);		break;
				case GL_UNSIGNED_INT_VEC3: 	uTypeText = "GL_UNSIGNED_INT_VEC3"; uTypeSize = 3 * sizeof(GLuint);		break;
				case GL_UNSIGNED_INT_VEC4: 	uTypeText = "GL_UNSIGNED_INT_VEC4"; uTypeSize = 4 * sizeof(GLuint);		break;
				case GL_BOOL: 				uTypeText = "GL_BOOL"; 				uTypeSize = 1 * sizeof(GLboolean);	break;
				case GL_BOOL_VEC2: 			uTypeText = "GL_BOOL_VEC2"; 		uTypeSize = 2 * sizeof(GLboolean);	break;
				case GL_BOOL_VEC3: 			uTypeText = "GL_BOOL_VEC3"; 		uTypeSize = 3 * sizeof(GLboolean);	break;
				case GL_BOOL_VEC4: 			uTypeText = "GL_BOOL_VEC4"; 		uTypeSize = 4 * sizeof(GLboolean);	break;
				case GL_FLOAT_MAT2: 		uTypeText = "GL_FLOAT_MAT2"; 		uTypeSize = 4 * sizeof(GLfloat);	break;
				case GL_FLOAT_MAT3: 		uTypeText = "GL_FLOAT_MAT3"; 		uTypeSize = 9 * sizeof(GLfloat);	break;
				case GL_FLOAT_MAT4: 		uTypeText = "GL_FLOAT_MAT4"; 		uTypeSize = 16 * sizeof(GLfloat);	break;
				case GL_FLOAT_MAT2x3: 		uTypeText = "GL_FLOAT_MAT2x3"; 		uTypeSize = 6 * sizeof(GLfloat);	break;
				case GL_FLOAT_MAT2x4: 		uTypeText = "GL_FLOAT_MAT2x4"; 		uTypeSize = 8 * sizeof(GLfloat);	break;
				case GL_FLOAT_MAT3x2: 		uTypeText = "GL_FLOAT_MAT3x2"; 		uTypeSize = 6 * sizeof(GLfloat);	break;
				case GL_FLOAT_MAT3x4: 		uTypeText = "GL_FLOAT_MAT3x4"; 		uTypeSize = 12 * sizeof(GLfloat);	break;
				case GL_FLOAT_MAT4x2: 		uTypeText = "GL_FLOAT_MAT4x2"; 		uTypeSize = 8 * sizeof(GLfloat);	break;
				case GL_FLOAT_MAT4x3: 		uTypeText = "GL_FLOAT_MAT4x3"; 		uTypeSize = 12 * sizeof(GLfloat);	break;
				default: 					uTypeText = "Unknown"; 				uTypeSize = 0; 						break;
			}

			DebugLog::Write("          offset: %i bytes", valuesUnif[2]);
			DebugLog::Write("          type: %s", uTypeText.c_str());
			DebugLog::Write("          size: %i bytes", uTypeSize);
			DebugLog::Write("          elements: %i", valuesUnif[3]);
			DebugLog::Write("          array stride: %i bytes", valuesUnif[5]);
			DebugLog::Write("          matrix stride: %i bytes", valuesUnif[6]);
		}
	}

	/*GLint activeUniformBlocks = 0;
	glGetProgramiv(mShaderProgram, GL_ACTIVE_UNIFORM_BLOCKS, &activeUniformBlocks);

	for(int uniformBlockIndex = 0; uniformBlockIndex < activeUniformBlocks; ++uniformBlockIndex)
	{
		GLint uniformsIndices[numberOfUniformsInBlock];
		glGetActiveUniformBlockiv(mShaderProgram, uniformBlockIndex, GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES, uniformsIndices);

		for(int uniformMember = 0; uniformMember < numberOfUniformsInBlock; ++uniformMember)
		{
			if(uniformsIndices[uniformMember] > 0)
			{
				GLuint tUniformIndex = uniformsIndices[uniformMember];

				GLint uniformNameLength;
				glGetActiveUniformsiv(mShaderProgram, 1, &tUniformIndex, GL_UNIFORM_NAME_LENGTH, &uniformNameLength);
				char uniformName[uniformNameLength];
				GLsizei length;
				GLint size;
				GLenum type;
				glGetActiveUniform(mShaderProgram, tUniformIndex, uniformNameLength, &length, &size, &type, uniformName);
				DebugLog::Write("     - %s", uniformName);

				GLint uniformOffset;
				glGetActiveUniformsiv(mShaderProgram, 1, &tUniformIndex, GL_UNIFORM_OFFSET, &uniformOffset);
				DebugLog::Write("          offset: %i", uniformOffset);

				GLint uType;
				string uTypeText;
				GLuint uTypeSize;
				glGetActiveUniformsiv(mShaderProgram, 1, &tUniformIndex, GL_UNIFORM_TYPE, &uType);

				switch(uType)
				{
					case GL_FLOAT: 				uTypeText = "GL_FLOAT"; 			uTypeSize = 1 * sizeof(GLfloat);	break;
					case GL_FLOAT_VEC2: 		uTypeText = "GL_FLOAT_VEC2"; 		uTypeSize = 2 * sizeof(GLfloat);	break;
					case GL_FLOAT_VEC3: 		uTypeText = "GL_FLOAT_VEC3"; 		uTypeSize = 3 * sizeof(GLfloat);	break;
					case GL_FLOAT_VEC4: 		uTypeText = "GL_FLOAT_VEC4"; 		uTypeSize = 4 * sizeof(GLfloat);	break;
					case GL_INT: 				uTypeText = "GL_INT"; 				uTypeSize = 1 * sizeof(GLint);		break;
					case GL_INT_VEC2: 			uTypeText = "GL_INT_VEC2"; 			uTypeSize = 2 * sizeof(GLint);		break;
					case GL_INT_VEC3: 			uTypeText = "GL_INT_VEC3"; 			uTypeSize = 3 * sizeof(GLint);		break;
					case GL_INT_VEC4: 			uTypeText = "GL_INT_VEC4"; 			uTypeSize = 4 * sizeof(GLint);		break;
					case GL_UNSIGNED_INT: 		uTypeText = "GL_UNSIGNED_INT"; 		uTypeSize = 1 * sizeof(GLuint);		break;
					case GL_UNSIGNED_INT_VEC2: 	uTypeText = "GL_UNSIGNED_INT_VEC2"; uTypeSize = 2 * sizeof(GLuint);		break;
					case GL_UNSIGNED_INT_VEC3: 	uTypeText = "GL_UNSIGNED_INT_VEC3"; uTypeSize = 3 * sizeof(GLuint);		break;
					case GL_UNSIGNED_INT_VEC4: 	uTypeText = "GL_UNSIGNED_INT_VEC4"; uTypeSize = 4 * sizeof(GLuint);		break;
					case GL_BOOL: 				uTypeText = "GL_BOOL"; 				uTypeSize = 1 * sizeof(GLboolean);	break;
					case GL_BOOL_VEC2: 			uTypeText = "GL_BOOL_VEC2"; 		uTypeSize = 2 * sizeof(GLboolean);	break;
					case GL_BOOL_VEC3: 			uTypeText = "GL_BOOL_VEC3"; 		uTypeSize = 3 * sizeof(GLboolean);	break;
					case GL_BOOL_VEC4: 			uTypeText = "GL_BOOL_VEC4"; 		uTypeSize = 4 * sizeof(GLboolean);	break;
					case GL_FLOAT_MAT2: 		uTypeText = "GL_FLOAT_MAT2"; 		uTypeSize = 4 * sizeof(GLfloat);	break;
					case GL_FLOAT_MAT3: 		uTypeText = "GL_FLOAT_MAT3"; 		uTypeSize = 9 * sizeof(GLfloat);	break;
					case GL_FLOAT_MAT4: 		uTypeText = "GL_FLOAT_MAT4"; 		uTypeSize = 16 * sizeof(GLfloat);	break;
					case GL_FLOAT_MAT2x3: 		uTypeText = "GL_FLOAT_MAT2x3"; 		uTypeSize = 6 * sizeof(GLfloat);	break;
					case GL_FLOAT_MAT2x4: 		uTypeText = "GL_FLOAT_MAT2x4"; 		uTypeSize = 8 * sizeof(GLfloat);	break;
					case GL_FLOAT_MAT3x2: 		uTypeText = "GL_FLOAT_MAT3x2"; 		uTypeSize = 6 * sizeof(GLfloat);	break;
					case GL_FLOAT_MAT3x4: 		uTypeText = "GL_FLOAT_MAT3x4"; 		uTypeSize = 12 * sizeof(GLfloat);	break;
					case GL_FLOAT_MAT4x2: 		uTypeText = "GL_FLOAT_MAT4x2"; 		uTypeSize = 8 * sizeof(GLfloat);	break;
					case GL_FLOAT_MAT4x3: 		uTypeText = "GL_FLOAT_MAT4x3"; 		uTypeSize = 12 * sizeof(GLfloat);	break;
					default: 					uTypeText = "Unknown"; 				uTypeSize = 0; 						break;
				}

				DebugLog::Write("          type: %s", uTypeText.c_str());

				GLint uniformNumElements;
				glGetActiveUniformsiv(mShaderProgram, 1, &tUniformIndex, GL_UNIFORM_SIZE, &uniformNumElements);
				GLuint sizeInBytes = uniformNumElements * uTypeSize;
				DebugLog::Write("          size: %i bytes", sizeInBytes);
				DebugLog::Write("          elements: %i", uniformNumElements);

				GLint arrayStride;
				glGetActiveUniformsiv(mShaderProgram, 1, &tUniformIndex, GL_UNIFORM_ARRAY_STRIDE, &arrayStride);
				DebugLog::Write("          array stride: %i", arrayStride);

				GLint matrixStride;
				glGetActiveUniformsiv(mShaderProgram, 1, &tUniformIndex, GL_UNIFORM_MATRIX_STRIDE, &matrixStride);
				DebugLog::Write("          matrix stride: %i", matrixStride);
			}
			else
			{
				DebugLog::Write("     - Bad uniform");
			}
		}
	}*/
}

auto Shader::ReadFile(const string& shaderPath) -> const string&
{
	// TODO: Shader: Revisar: https://stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
	string* shaderBuffer = new string();
	std::ifstream stream(shaderPath);

	if(stream.is_open())
	{
		stream.seekg(0, std::ios::end);
		shaderBuffer->reserve(stream.tellg());
		stream.seekg(0, std::ios::beg);

		shaderBuffer->assign((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
		stream.close();
	}

	if(shaderBuffer->empty())
	{
		DebugLog::Write("Shader file not found or empty: %s", shaderPath.c_str());
	}

	return std::move(*shaderBuffer);
}

GLuint Shader::CreateShader(const GLenum shaderType, const string& shaderPath)
{
	GLuint shaderID = glCreateShader(shaderType);

	auto shaderSource = Shader::ReadFile(shaderPath);

	if(shaderSource.empty())
	{
		glDeleteShader(shaderID);
		return 0;
	}

	const char *shaderData = shaderSource.c_str();
	glShaderSource(shaderID, 1, &shaderData, NULL);

	glCompileShader(shaderID);

	GLint compileStatus;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compileStatus);

	if(compileStatus == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shaderID, infoLogLength, NULL, strInfoLog);

		DebugLog::Write("Shader compile failure: %s\n%s", shaderPath.c_str(), strInfoLog);
		delete[] strInfoLog;

		glDeleteShader(shaderID);
		shaderID = 0;
	}

	return shaderID;
}

GLuint Shader::CreateProgram(const vector<GLuint>& shaders)
{
	GLuint programID = glCreateProgram();

	for(auto shader : shaders)
	{
		glAttachShader(programID, shader);
	}

	glLinkProgram(programID);

	GLint linkStatus;
	glGetProgramiv(programID, GL_LINK_STATUS, &linkStatus);

	if(linkStatus == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(programID, infoLogLength, NULL, strInfoLog);

		DebugLog::Write("Shader program link failure: %s", strInfoLog);
		delete[] strInfoLog;

		glDeleteProgram(programID);
		programID = 0;
	}

	for(auto shader : shaders)
	{
		glDetachShader(programID, shader);
		glDeleteShader(shader);
	}

	return programID;
}

GLint Shader::GetUniformLocation(const GLuint shaderProgram, const string& name)
{
	return glGetUniformLocation(shaderProgram, name.c_str());
}

GLint Shader::GetUniformBlockLocation(const GLuint shaderProgram, const string& name)
{
	return glGetUniformBlockIndex(shaderProgram, name.c_str());
}
}
