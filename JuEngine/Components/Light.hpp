// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../ECS/Component.hpp"
#include "../Math.hpp"

namespace JuEngine
{
enum class LightType
{
	LIGHT_DIRECTIONAL,
	LIGHT_POINT
};

class JUENGINEAPI Light : public Component
{
public:
	Light() = default;
	Light(const vec3 color, const float intensity = 1.0f, const LightType type = LightType::LIGHT_POINT);
	virtual ~Light() = default;

	auto GetType() -> const LightType&;
	void SetType(const LightType type);
	auto GetColor() -> const vec3&;
	void SetColor(const vec3 color);
	auto GetIntensity() -> const float&;
	void SetIntensity(const float intensity);

private:
	LightType mType{LightType::LIGHT_POINT};
	vec3 mColor{1.0f, 1.0f, 1.0f}; // Sunlight: 255,244,214
	float mIntensity{1.0f};
};
}
