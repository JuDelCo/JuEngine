// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/IObject.hpp"
#include "../Resources/Math.hpp"
#include <vector>

namespace JuEngine
{
class Shader;
class Texture;

class JUENGINEAPI Material : public IObject
{
	public:
		Material(const Identifier& shaderId);
		Material(const Identifier& shaderId, const Identifier& textureId);

		void Use();

		auto GetDiffuseColor() -> const vec3&;
		auto SetDiffuseColor(const vec3 diffuseColor) -> Material*;

		auto GetSpecularColor() -> const vec3&;
		auto SetSpecularColor(const vec3 specularColor) -> Material*;

		auto GetShininessFactor() -> const float&;
		auto SetShininessFactor(const float shininessFactor) -> Material*;

		auto GetShader() -> Shader*;
		auto SetShader(const Identifier& id) -> Material*;

		auto GetTexture(const unsigned int unit = 0) -> Texture*;
		auto AddTexture(const Identifier& id, const unsigned int unit = 0) -> Material*;

	private:
		vec3 mDiffuseColor{1.f, 0.f, 1.f};
		vec3 mSpecularColor{1.f, 1.f, 1.f};
		float mShininessFactor{0.1f};
		Shader* mShader{nullptr};
		std::vector<Texture*> mTextures{16, nullptr};
};
}
