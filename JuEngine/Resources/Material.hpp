// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/IObject.hpp"
#include "../Resources/Math.hpp"

namespace JuEngine
{
class Shader;

class JUENGINEAPI Material : public IObject
{
	public:
		Material() : IObject("material") {}

		void Use();
		static void DisableMaterials();

		auto GetDiffuseColor() -> const vec3&;
		void SetDiffuseColor(const vec3 diffuseColor);

		auto GetSpecularColor() -> const vec3&;
		void SetSpecularColor(const vec3 specularColor);

		auto GetShininessFactor() -> const float&;
		void SetShininessFactor(const float shininessFactor);

		auto GetShader() -> Shader*;
		void SetShader(const Identifier& id);

		// TODO
		// auto GetTexture() -> Texture*;
		// void SetTexture(const Identifier& id);

	private:
		vec3 mDiffuseColor{1.f, 0.f, 1.f};
		vec3 mSpecularColor{1.f, 1.f, 1.f};
		float mShininessFactor{0.1f};
		Shader* mShader{nullptr};
		// TODO
		//Texture* mTexture;
};
}
