// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "Camera.hpp"
#include "Transform.hpp"
#include "../Resources/Entity.hpp"

namespace JuEngine
{
Camera::Camera()
{
	//mSelf = shared_ptr<Camera>(this, [](void*){});

	SetFov(mFovDeg);
}

auto Camera::GetFov() -> const float&
{
	return mFovDeg;
}

void Camera::SetFov(const float fovDeg)
{
	mFovDeg = fovDeg;

	if(mFovDeg < 0.1f) mFovDeg = 0.1f;
	if(mFovDeg > 180.f) mFovDeg = 180.f;

	mFrustumScale = (1.0f / tan(Math::DegToRad(mFovDeg) / 2.0f));

	mPerspectiveMatrixRefreshNeeded = true;
}

auto Camera::GetViewport() -> const vec4&
{
	return mViewport;
}

void Camera::SetViewport(const vec4 viewport)
{
	mViewport = viewport;
}

auto Camera::IsOrthographic() -> const bool&
{
	return mIsOrthographic;
}

void Camera::SetOrthographic(const bool enabled)
{
	mIsOrthographic = enabled;
	mPerspectiveMatrixRefreshNeeded = true;
}

vec2 Camera::GetDistance()
{
	return vec2(mNearDistance, mFarDistance);
}

void Camera::SetDistance(const float nearDistance, const float farDistance)
{
	mNearDistance = nearDistance;
	mFarDistance = farDistance;
	mPerspectiveMatrixRefreshNeeded = true;
}

auto Camera::GetRotation() -> const vec3&
{
	return mEntity->GetComponent<Transform>()->GetLocalEulerAngles();
}

void Camera::SetRotation(const float pitch, const float yaw, const float roll)
{
	mEntity->GetComponent<Transform>()->SetLocalEulerAngles(vec3(pitch, yaw, roll));
}

void Camera::Rotate(const float pitch, const float yaw, const float roll)
{
	auto transform = mEntity->GetComponent<Transform>();

	transform->Rotate(vec3(pitch, 0.f, 0.f));
	transform->Rotate(vec3(0.f, yaw, 0.f), true);
	transform->Rotate(vec3(0.f, 0.f, roll));
}

auto Camera::GetZoom() -> const float&
{
	return mZoom;
}

void Camera::SetZoom(const float zoom)
{
	mZoom = zoom;

	if(mZoom < 1.0f) mZoom = 1.0f;
	if(mZoom > 10000.f) mZoom = 10000.f;

	mPerspectiveMatrixRefreshNeeded = true;
}

void Camera::SetScreenSize(const vec2 size)
{
	if(mScreenSize != size)
	{
		mScreenSize = size;
		mPerspectiveMatrixRefreshNeeded = true;
	}
}

auto Camera::GetPerspectiveMatrix() -> const mat4&
{
	if(! mPerspectiveMatrixRefreshNeeded)
	{
		return mPerspectiveMatrixCache;
	}

	if(! mIsOrthographic)
	{
		mPerspectiveMatrixCache = mat4(1.0f);
		mPerspectiveMatrixCache[0].x = mFrustumScale / (mScreenSize.x / mScreenSize.y);
		mPerspectiveMatrixCache[1].y = mFrustumScale;
		mPerspectiveMatrixCache[2].z = -(mFarDistance + mNearDistance) / (mNearDistance - mFarDistance);
		mPerspectiveMatrixCache[2].w = 1.0f;
		mPerspectiveMatrixCache[3].z = -(2.0f * mFarDistance * mNearDistance) / (mNearDistance - mFarDistance);
	}
	else
	{
		mPerspectiveMatrixCache = mat4(1.0f);
		mPerspectiveMatrixCache[0].x = mZoom / mScreenSize.x;
		mPerspectiveMatrixCache[1].y = mZoom / mScreenSize.y;
		mPerspectiveMatrixCache[2].z = 2.0f / (mFarDistance - mNearDistance);
		mPerspectiveMatrixCache[3].z = -(mFarDistance + mNearDistance) / (mFarDistance - mNearDistance);
		mPerspectiveMatrixCache[3].w = 1.0f;
	}

	mPerspectiveMatrixRefreshNeeded = false;

	return mPerspectiveMatrixCache;
}

mat4 Camera::GetViewMatrix()
{
	auto transform = mEntity->GetComponent<Transform>();
	mat4 viewMatrix = transform->GetInverseMatrix();
	vec3 eye = transform->GetPosition() + transform->Forward();

	viewMatrix[3][0] = -Math::Dot(vec3(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0]), eye);
	viewMatrix[3][1] = -Math::Dot(vec3(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1]), eye);
	viewMatrix[3][2] = -Math::Dot(vec3(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2]), eye);

	return viewMatrix;
}
}
