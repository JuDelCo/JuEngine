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

void Light::SetType(const LightType type)
{
	mType = type;
}

auto Light::GetColor() -> const vec3&
{
	return mColor;
}

void Light::SetColor(const vec3 color)
{
	mColor = Math::Clamp(color);
}

auto Light::GetIntensity() -> const float&
{
	return mIntensity;
}

void Light::SetIntensity(const float intensity)
{
	mIntensity = intensity;
}
}
