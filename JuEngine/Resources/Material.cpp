// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "Material.hpp"
#include "../Resources/Shader.hpp"
#include "../App.hpp"
#include "../Services/IDataService.hpp"

namespace JuEngine
{
Material::Material(const std::string& shaderName, const std::string& textureName) : IObject("material")
{
	SetShader(shaderName);
	//SetTexture(textureName);
}

void Material::Use()
{
	if(mShader)
	{
		mShader->Use();

		return;
	}

	Shader::DisableShaders();
}

auto Material::GetDiffuseColor() -> const vec3&
{
	return mDiffuseColor;
}

void Material::SetDiffuseColor(const vec3 diffuseColor)
{
	mDiffuseColor = diffuseColor;
}

auto Material::GetSpecularColor() -> const vec3&
{
	return mSpecularColor;
}

void Material::SetSpecularColor(const vec3 specularColor)
{
	mSpecularColor = specularColor;
}

auto Material::GetShininessFactor() -> const float&
{
	return mShininessFactor;
}

void Material::SetShininessFactor(const float shininessFactor)
{
	mShininessFactor = shininessFactor;
}

auto Material::GetShader() -> Shader*
{
	return mShader;
}

void Material::SetShader(const Identifier& id)
{
	mShader = App::Data()->Get<Shader>(id);
}
}
