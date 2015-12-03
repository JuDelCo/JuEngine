// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Math.hpp"

namespace JuEngine
{
namespace World
{
const vec3 Right{1.0f, 0.f, 0.f};
const vec3 Up{0.f, 1.0f, 0.f};
const vec3 Forward{0.f, 0.f, 1.0f};

struct Data
{
	auto GetAmbientColor() -> const vec3& { return mAmbientColor; }
	void SetAmbientColor(const vec3 ambientColor) { mAmbientColor = Math::Clamp(ambientColor); }
	auto GetAmbientIntensity() -> const float& { return mAmbientIntensity; }
	void SetAmbientIntensity(const float ambientIntensity) { mAmbientIntensity = ambientIntensity; }
	auto GetSkyColor() -> const vec3& { return mSkyColor; }
	void SetSkyColor(const vec3 skyColor) { mSkyColor = Math::Clamp(skyColor); }
	auto GetLightAttenuation() -> const float& { return mLightAttenuation; }
	void SetLightAttenuation(const float lightAttenuation) { mLightAttenuation = lightAttenuation; }
	auto GetGammaCorrection() -> const float& { return mGammaCorrection; }
	void SetGammaCorrection(const float gammaCorrection) { mGammaCorrection = gammaCorrection; }

private:
	vec3 mAmbientColor{0.211f, 0.227f, 0.258f}; // 54,58,66
	float mAmbientIntensity{1.0f};
	vec3 mSkyColor{0.529f, 0.807f, 1.0f}; // light sky blue 135,206,250
	float mLightAttenuation{1.0f};
	float mGammaCorrection{2.2f};
};
}
}
