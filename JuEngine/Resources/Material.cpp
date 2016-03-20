// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "Material.hpp"
#include "../Resources/Shader.hpp"
#include "../Resources/Texture.hpp"
#include "../App.hpp"
#include "../Services/IDataService.hpp"

namespace JuEngine
{
Material::Material(const Identifier& shaderId) : IObject("material")
{
	SetShader(shaderId);
}

Material::Material(const Identifier& shaderId, const Identifier& textureId) : IObject("material")
{
	SetShader(shaderId);
	AddTexture(textureId, 0);
}

void Material::Use()
{
	if(mShader)
	{
		mShader->Use();

		for(unsigned int i = 0; i < 16; ++i)
		{
			if(mTextures[i] != nullptr)
			{
				mTextures[i]->Use(i);
			}
		}

		return;
	}

	Shader::DisableShaders();
}

auto Material::GetDiffuseColor() -> const vec3&
{
	return mDiffuseColor;
}

auto Material::SetDiffuseColor(const vec3 diffuseColor) -> Material*
{
	mDiffuseColor = diffuseColor;

	return this;
}

auto Material::GetSpecularColor() -> const vec3&
{
	return mSpecularColor;
}

auto Material::SetSpecularColor(const vec3 specularColor) -> Material*
{
	mSpecularColor = specularColor;

	return this;
}

auto Material::GetShininessFactor() -> const float&
{
	return mShininessFactor;
}

auto Material::SetShininessFactor(const float shininessFactor) -> Material*
{
	mShininessFactor = shininessFactor;

	return this;
}

auto Material::GetShader() -> Shader*
{
	return mShader;
}

auto Material::SetShader(const Identifier& id) -> Material*
{
	mShader = App::Data()->Get<Shader>(id);

	return this;
}

auto Material::GetTexture(const unsigned int unit) -> Texture*
{
	return mTextures[unit];
}

auto Material::AddTexture(const Identifier& id, const unsigned int unit) -> Material*
{
	mTextures[unit] = App::Data()->Get<Texture>(id);

	return this;
}
}
