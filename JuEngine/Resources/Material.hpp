// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../IObject.hpp"
#include "../Math.hpp"

namespace JuEngine
{
class Shader;

class JUENGINEAPI Material : public IObject
{
public:
	Material() : IObject("material") {}
	virtual ~Material() = default;

	void Use();
	static void DisableMaterials();

	auto GetDiffuseColor() -> const vec3&;
	void SetDiffuseColor(const vec3 diffuseColor);

	auto GetSpecularColor() -> const vec3&;
	void SetSpecularColor(const vec3 specularColor);

	auto GetShininessFactor() -> const float&;
	void SetShininessFactor(const float shininessFactor);

	auto GetShader() -> Shader*;
	void SetShader(const string& name);

	// auto GetTexture() -> Texture*;
	// void SetTexture(const string& name);

private:
	vec3 mDiffuseColor{1.0f, 0.f, 1.0f};
	vec3 mSpecularColor{1.0f, 1.0f, 1.0f};
	float mShininessFactor{0.1f};
	Shader* mShader;
	//Texture* mTexture;
};
}
