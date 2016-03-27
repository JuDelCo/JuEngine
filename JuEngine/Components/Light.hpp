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
	LIGHT_POINT,
	LIGHT_SPOT
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
		auto GetLinearAttenuation() -> const float&;
		auto SetLinearAttenuation(const float linearAttenuation) -> Light*;
		auto GetQuadraticAttenuation() -> const float&;
		auto SetQuadraticAttenuation(const float quadraticAttenuation) -> Light*;
		auto GetSpotCutOff() -> const float&;
		auto SetSpotCutOff(const float degrees) -> Light*;
		auto GetSpotOuterCutOff() -> const float&;
		auto SetSpotOuterCutOff(const float degrees) -> Light*;

	private:
		LightType mType{LightType::LIGHT_POINT};
		vec3 mColor{1.f, 1.f, 1.f}; // 255,244,214 (sunlight)
		float mIntensity{1.f};
		float mLinearAttenuation{0.09f};
		float mQuadraticAttenuation{0.032f};
		float mSpotCutOff{0.9f}; // 25 deg
		float mSpotOuterCutOff{0.82f}; // 35 deg
};
}

/*
|       Attenuation table
|   Distance  Linear   Quadratic
|       7      0.7       1.8
|      13      0.35      0.44
|      20      0.22      0.20
|   *  32      0.14      0.07
|   *  50      0.09      0.032
|   *  65      0.07      0.017
|   * 100      0.045     0.0075
|     160      0.027     0.0028
|     200      0.022     0.0019
|     325      0.014     0.0007
|     600      0.007     0.0002
|    3250      0.0014    0.000007
*/
