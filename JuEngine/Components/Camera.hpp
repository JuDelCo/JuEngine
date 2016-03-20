// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Entity/IComponent.hpp"
#include "../Resources/Math.hpp"

namespace JuEngine
{
class Transform;

class JUENGINEAPI Camera : public IComponent
{
	public:
		void Reset(Transform* transform);

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
		auto GetViewMatrix() -> const mat4;

	private:
		Transform* mTransform;
		float mFarDistance{100.f};
		float mNearDistance{0.01f};
		float mFovDeg{45.f};
		float mFrustumScale;
		vec2 mScreenSize{0.f, 0.f};
		vec4 mViewport{0.f, 0.f, 1.f, 1.f};
		bool mPerspectiveMatrixRefreshNeeded{false};
		mat4 mPerspectiveMatrixCache;
		bool mIsOrthographic{false};
		float mZoom{100.f};
};
}
