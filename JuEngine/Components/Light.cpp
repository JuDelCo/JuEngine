// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "Light.hpp"

namespace JuEngine
{
void Light::Reset(const vec3 color, const float intensity, const LightType type)
{
	SetColor(color);
	SetIntensity(intensity);
	SetType(type);
}

auto Light::GetType() -> const LightType&
{
	return mType;
}

auto Light::SetType(const LightType type) -> Light*
{
	mType = type;

	return this;
}

auto Light::GetColor() -> const vec3&
{
	return mColor;
}

auto Light::SetColor(const vec3 color) -> Light*
{
	mColor = Math::Clamp(color);

	return this;
}

auto Light::GetIntensity() -> const float&
{
	return mIntensity;
}

auto Light::SetIntensity(const float intensity) -> Light*
{
	mIntensity = intensity;

	return this;
}
}
