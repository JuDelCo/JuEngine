// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "World.hpp"

namespace JuEngine
{
const vec3 World::Right = {1.f, 0.f, 0.f};
const vec3 World::Up = {0.f, 1.f, 0.f};
const vec3 World::Forward = {0.f, 0.f, 1.f};

void World::Reset(const vec3 ambientColor, const vec3 skyColor, const float ambientIntensity, const float lightAttenuation, const float gammaCorrection)
{
	SetAmbientColor(ambientColor);
	SetSkyColor(skyColor);
	SetAmbientIntensity(ambientIntensity);
	SetLightAttenuation(lightAttenuation);
	SetGammaCorrection(gammaCorrection);
}

auto World::GetAmbientColor() -> const vec3&
{
	return mAmbientColor;
}

void World::SetAmbientColor(const vec3 ambientColor)
{
	mAmbientColor = Math::Clamp(ambientColor);
}

auto World::GetSkyColor() -> const vec3&
{
	return mSkyColor;
}

void World::SetSkyColor(const vec3 skyColor)
{
	mSkyColor = Math::Clamp(skyColor);
}

auto World::GetAmbientIntensity() -> const float&
{
	return mAmbientIntensity;
}

void World::SetAmbientIntensity(const float ambientIntensity)
{
	mAmbientIntensity = ambientIntensity;
}

auto World::GetLightAttenuation() -> const float&
{
	return mLightAttenuation;
}

void World::SetLightAttenuation(const float lightAttenuation)
{
	mLightAttenuation = lightAttenuation;
}

auto World::GetGammaCorrection() -> const float&
{
	return mGammaCorrection;
}

void World::SetGammaCorrection(const float gammaCorrection)
{
	mGammaCorrection = gammaCorrection;
}
}
