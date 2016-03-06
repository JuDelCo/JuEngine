// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Entity/IComponent.hpp"
#include "../Resources/Math.hpp"

namespace JuEngine
{
class JUENGINEAPI World : public IComponent
{
	public:
		void Reset(const vec3 ambientColor, const vec3 skyColor, const float ambientIntensity = 1.f, const float lightAttenuation = 1.f, const float gammaCorrection = 2.2f);

		auto GetAmbientColor() -> const vec3&;
		void SetAmbientColor(const vec3 ambientColor);
		auto GetSkyColor() -> const vec3&;
		void SetSkyColor(const vec3 skyColor);
		auto GetAmbientIntensity() -> const float&;
		void SetAmbientIntensity(const float ambientIntensity);
		auto GetLightAttenuation() -> const float&;
		void SetLightAttenuation(const float lightAttenuation);
		auto GetGammaCorrection() -> const float&;
		void SetGammaCorrection(const float gammaCorrection);

	private:
		vec3 mAmbientColor{0.f, 0.f, 0.f}; // 0.211f, 0.227f, 0.258f -> 54,58,66
		vec3 mSkyColor{0.f, 0.f, 0.f}; // 0.529, 0.807, 1.0 -> 135,206,250 (light sky blue)
		float mAmbientIntensity{1.f};
		float mLightAttenuation{1.f};
		float mGammaCorrection{2.2f};

	public:
		static const vec3 Right;
		static const vec3 Up;
		static const vec3 Forward;
};
}
