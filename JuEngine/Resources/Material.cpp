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
Material::Material() : IObject("material")
{
}

void Material::Use(Shader* shader)
{
	if(shader == nullptr)
	{
		return;
	}

	shader->SetUniform("material.diffuseColor", mDiffuseColor);
	shader->SetUniform("material.specularColor", mSpecularColor);
	shader->SetUniform("material.shininess", mShininessFactor);

	unsigned int counter = 0;

	if(! mTextures.empty())
	{
		for(auto& pair : mTextures)
		{
			shader->SetUniformTexture("material." + pair.first, counter);
			pair.second->Use(counter++);
		}
	}
	else
	{
		Texture::DisableTextures();
	}
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

auto Material::GetTexture(const std::string& name) -> Texture*
{
	if(mTextures.find(name) == mTextures.end())
	{
		return nullptr;
	}

	return mTextures.at(name);
}

auto Material::SetTexture(const std::string& name, Texture* texture) -> Material*
{
	if(texture != nullptr)
	{
		mTextures[name] = texture;
	}

	return this;
}
}
