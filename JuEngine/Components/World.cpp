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

auto World::SetAmbientColor(const vec3 ambientColor) -> World*
{
	mAmbientColor = Math::Clamp(ambientColor);

	return this;
}

auto World::GetSkyColor() -> const vec3&
{
	return mSkyColor;
}

auto World::SetSkyColor(const vec3 skyColor) -> World*
{
	mSkyColor = Math::Clamp(skyColor);

	return this;
}

auto World::GetAmbientIntensity() -> const float&
{
	return mAmbientIntensity;
}

auto World::SetAmbientIntensity(const float ambientIntensity) -> World*
{
	mAmbientIntensity = ambientIntensity;

	return this;
}

auto World::GetLightAttenuation() -> const float&
{
	return mLightAttenuation;
}

auto World::SetLightAttenuation(const float lightAttenuation) -> World*
{
	mLightAttenuation = lightAttenuation;

	return this;
}

auto World::GetGammaCorrection() -> const float&
{
	return mGammaCorrection;
}

auto World::SetGammaCorrection(const float gammaCorrection) -> World*
{
	mGammaCorrection = gammaCorrection;

	return this;
}
}
