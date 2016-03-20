// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Entity/IComponent.hpp"
#include "../Resources/Math.hpp"

namespace JuEngine
{
enum class LightType
{
	LIGHT_DIRECTIONAL,
	LIGHT_POINT
};

class JUENGINEAPI Light : public IComponent
{
	public:
		void Reset(const vec3 color, const float intensity = 1.f, const LightType type = LightType::LIGHT_POINT);

		auto GetType() -> const LightType&;
		auto SetType(const LightType type) -> Light*;
		auto GetColor() -> const vec3&;
		auto SetColor(const vec3 color) -> Light*;
		auto GetIntensity() -> const float&;
		auto SetIntensity(const float intensity) -> Light*;

	private:
		LightType mType{LightType::LIGHT_POINT};
		vec3 mColor{1.f, 1.f, 1.f}; // 255,244,214 (sunlight)
		float mIntensity{1.f};
};
}
