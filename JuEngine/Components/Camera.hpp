// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "Component.hpp"
#include "../Math.hpp"

namespace JuEngine
{
class JUENGINEAPI Camera : public Component
{
public:
	Camera();
	virtual ~Camera() = default;

	auto GetFov() -> const float&;
	void SetFov(const float fovDeg);
	auto GetViewport() -> const vec4&;
	void SetViewport(const vec4 viewport);
	auto IsOrthographic() -> const bool&;
	void SetOrthographic(const bool enabled);
	vec2 GetDistance();
	void SetDistance(const float nearDistance, const float farDistance);
	auto GetRotation() -> const vec3&;
	void SetRotation(const float pitch, const float yaw, const float roll = 0.f);
	void Rotate(const float pitch, const float yaw, const float roll = 0.f);
	auto GetZoom() -> const float&;
	void SetZoom(const float zoom);
	void SetScreenSize(const vec2 size);
	auto GetPerspectiveMatrix() -> const mat4&;
	mat4 GetViewMatrix();

private:
	float mFarDistance{100.0f};
	float mNearDistance{0.01f};
	float mFovDeg{45.0f};
	float mFrustumScale;
	vec2 mScreenSize{0.0f, 0.0f};
	vec4 mViewport{0.0f, 0.0f, 1.0f, 1.0f};
	bool mPerspectiveMatrixRefreshNeeded{false};
	mat4 mPerspectiveMatrixCache;
	bool mIsOrthographic{false};
	float mZoom{100.0f};

	//shared_ptr<Camera> mSelf;
};
}
