// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/IObject.hpp"
#include "../Resources/Math.hpp"
#include <map>

namespace JuEngine
{
class Shader;
class Texture;

class JUENGINEAPI Material : public IObject
{
	public:
		Material();

		void Use(Shader* shader);

		auto GetDiffuseColor() -> const vec3&;
		auto SetDiffuseColor(const vec3 diffuseColor) -> Material*;

		auto GetSpecularColor() -> const vec3&;
		auto SetSpecularColor(const vec3 specularColor) -> Material*;

		auto GetShininessFactor() -> const float&;
		auto SetShininessFactor(const float shininessFactor) -> Material*;

		auto GetTexture(const std::string& name) -> Texture*;
		auto SetTexture(const std::string& name, Texture* texture) -> Material*;

	private:
		vec3 mDiffuseColor{1.f, 0.f, 1.f}; // Magenta
		vec3 mSpecularColor{1.f, 1.f, 1.f};
		float mShininessFactor{32.0f}; // 2~256
		std::map<std::string, Texture*> mTextures;
		// TODO: Material: Add "ForceDraw" property support
		//bool mForceDraw{false};
};
}
