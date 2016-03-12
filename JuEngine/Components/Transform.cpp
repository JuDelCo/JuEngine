// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "Transform.hpp"
#include "../Entity/Entity.hpp"

namespace JuEngine
{
void Transform::Reset()
{
	SetLocalPosition(vec3(0.f, 0.f, 0.f));
	SetLocalScale(vec3(1.f, 1.f, 1.f));
	SetLocalRotation(quat(1.f, 0.f, 0.f, 0.f));
}

void Transform::Reset(const vec3 position)
{
	SetLocalPosition(position);
	SetLocalScale(vec3(1.f, 1.f, 1.f));
	SetLocalRotation(quat(1.f, 0.f, 0.f, 0.f));
}

void Transform::Reset(const vec3 position, const quat orientation)
{
	SetLocalPosition(position);
	SetLocalScale(vec3(1.f, 1.f, 1.f));
	SetLocalRotation(orientation);
}

void Transform::SetParent(const EntityPtr& parent)
{
	SetParent(parent->Get<Transform>());
}

void Transform::SetParent(Transform* parent)
{
	mParent = parent;
}

vec3 Transform::GetPosition() const
{
	if(mParent)
	{
		return mPosition + mParent->GetPosition();
	}

	return mPosition;
}

vec3 Transform::GetScale() const
{
	if(mParent)
	{
		return mScale + mParent->GetScale();
	}

	return mScale;
}

vec3 Transform::GetEulerAngles()
{
	if(mParent)
	{
		return Math::EulerAngles(GetRotation());
	}

	return GetLocalEulerAngles();
}

quat Transform::GetRotation() const
{
	if(mParent)
	{
		return GetLocalRotation() * mParent->GetRotation();
	}

	return GetLocalRotation();
}

auto Transform::Right() -> const vec3&
{
	if(mRightRefreshNeeded)
	{
		mRightCache = mOrientation * vec3(1.f, 0.f, 0.f);

		mRightRefreshNeeded = false;
	}

	return mRightCache;
}

auto Transform::Up() -> const vec3&
{
	if(mUpRefreshNeeded)
	{
		mUpCache = mOrientation * vec3(0.f, 1.f, 0.f);

		mUpRefreshNeeded = false;
	}

	return mUpCache;
}

auto Transform::Forward() -> const vec3&
{
	if(mForwardRefreshNeeded)
	{
		mForwardCache = mOrientation * vec3(0.f, 0.f, 1.f);

		mForwardRefreshNeeded = false;
	}

	return mForwardCache;
}

auto Transform::GetLocalPosition() const -> const vec3&
{
	return mPosition;
}

auto Transform::GetLocalScale() const -> const vec3&
{
	return mScale;
}

auto Transform::GetLocalEulerAngles() -> const vec3&
{
	if(mEulerRefreshNeeded)
	{
		mEulerAnglesCache = Math::EulerAngles(mOrientation);

		mEulerRefreshNeeded = false;
	}

	return mEulerAnglesCache;
}

auto Transform::GetLocalRotation() const -> const quat&
{
	return mOrientation;
}

void Transform::SetLocalPosition(const vec3 position)
{
	mPosition = position;
	mMatrixRefreshNeeded = true;
	mInverseMatrixRefreshNeeded = true;
}

void Transform::SetLocalScale(const vec3 scale)
{
	mScale = scale;
	mMatrixRefreshNeeded = true;
	mInverseMatrixRefreshNeeded = true;
}

void Transform::SetLocalEulerAngles(const vec3 eulerAngles)
{
	mOrientation = Math::Normalize(quat(eulerAngles));

	mEulerRefreshNeeded = true;
	mRightRefreshNeeded = true;
	mUpRefreshNeeded = true;
	mForwardRefreshNeeded = true;
	mMatrixRefreshNeeded = true;
	mInverseMatrixRefreshNeeded = true;
}

void Transform::SetLocalRotation(const quat orientation)
{
	mOrientation = Math::Normalize(orientation);

	mEulerRefreshNeeded = true;
	mRightRefreshNeeded = true;
	mUpRefreshNeeded = true;
	mForwardRefreshNeeded = true;
	mMatrixRefreshNeeded = true;
	mInverseMatrixRefreshNeeded = true;
}

void Transform::Translate(const vec3 translation, const bool relativeToWorld)
{
	if(relativeToWorld)
	{
		mPosition += translation;
	}
	else
	{
		mPosition += (mOrientation * translation);
	}

	mMatrixRefreshNeeded = true;
	mInverseMatrixRefreshNeeded = true;
}

void Transform::Translate(const vec3 translation, Transform* relativeTo)
{
	Translate(relativeTo->TransformVector(translation), true);
}

void Transform::Scale(const vec3 scale)
{
	mScale *= scale;
	mMatrixRefreshNeeded = true;
	mInverseMatrixRefreshNeeded = true;
}

void Transform::Rotate(const vec3 eulerAngles, const bool relativeToWorld)
{
	Rotate(quat(eulerAngles), relativeToWorld);
}

void Transform::Rotate(const vec3 eulerAngles, Transform* relativeTo)
{
	Rotate(relativeTo->TransformDirection(eulerAngles), true);
}

void Transform::Rotate(const quat orientation, const bool relativeToWorld)
{
	if(relativeToWorld)
	{
		mOrientation = orientation * mOrientation;
	}
	else
	{
		mOrientation *= orientation;
	}

	mOrientation = Math::Normalize(mOrientation);

	mEulerRefreshNeeded = true;
	mRightRefreshNeeded = true;
	mUpRefreshNeeded = true;
	mForwardRefreshNeeded = true;
	mMatrixRefreshNeeded = true;
	mInverseMatrixRefreshNeeded = true;
}

void Transform::Rotate(const quat orientation, const Transform* relativeTo)
{
	Rotate(Math::EulerAngles(orientation), relativeTo);
}

void Transform::RotateAround(const vec3 point, const vec3 axis, const float angle)
{
	mPosition += (mOrientation * point);
	mOrientation *= quat(axis * angle);
	mOrientation = Math::Normalize(mOrientation);
	mPosition += (mOrientation * -point);

	mEulerRefreshNeeded = true;
	mRightRefreshNeeded = true;
	mUpRefreshNeeded = true;
	mForwardRefreshNeeded = true;
	mMatrixRefreshNeeded = true;
	mInverseMatrixRefreshNeeded = true;
}

void Transform::LookAt(const vec3 worldPosition, const vec3 worldUp)
{
	quat angles = Math::LookAt(GetPosition(), worldPosition, worldUp);

	if(angles == angles) // NaN prevention
	{
		SetLocalRotation(angles);
	}
}

auto Transform::GetMatrix() -> const mat4&
{
	bool matrixRecalculated = false;

	if(mMatrixRefreshNeeded)
	{
		mMatrixCache = CalculateMatrix();
		mMatrixRefreshNeeded = false;
		matrixRecalculated = true;
	}

	if(mParent)
	{
		auto parentMatrix = mParent->GetMatrix();

		if(mParentWorldMatrixCache != parentMatrix)
		{
			mParentWorldMatrixCache = parentMatrix;
			mWorldMatrixCache = mParentWorldMatrixCache * mMatrixCache;
		}

		if(matrixRecalculated)
		{
			mWorldMatrixCache = mParentWorldMatrixCache * mMatrixCache;
		}

		return mWorldMatrixCache;
	}

	return mMatrixCache;
}

vec3 Transform::TransformPoint(const vec3 position)
{
	return vec3(GetMatrix() * vec4(position, 1.f));
}

vec3 Transform::TransformVector(const vec3 vector)
{
	return mat3(GetMatrix()) * vector;
}

vec3 Transform::TransformDirection(const vec3 direction)
{
	return Math::QuatToMat(mOrientation) * direction;
}

auto Transform::GetInverseMatrix() -> const mat4&
{
	bool matrixRecalculated = false;

	if(mInverseMatrixRefreshNeeded)
	{
		mInverseMatrixCache = CalculateInverseMatrix();
		mInverseMatrixRefreshNeeded = false;
		matrixRecalculated = true;
	}

	if(mParent)
	{
		auto parentInverseMatrix = mParent->GetMatrix();

		if(mParentWorldInverseMatrixCache != parentInverseMatrix)
		{
			mParentWorldInverseMatrixCache = parentInverseMatrix;
			mWorldInverseMatrixCache = mInverseMatrixCache * mParentWorldInverseMatrixCache;
		}

		if(matrixRecalculated)
		{
			mWorldInverseMatrixCache = mInverseMatrixCache * mParentWorldInverseMatrixCache;
		}

		return mWorldInverseMatrixCache;
	}

	return mInverseMatrixCache;
}

vec3 Transform::InverseTransformPoint(const vec3 position)
{
	return vec3(GetInverseMatrix() * vec4(position, 1.f));
}

vec3 Transform::InverseTransformVector(const vec3 vector)
{
	return mat3(GetInverseMatrix()) * vector;
}

vec3 Transform::InverseTransformDirection(const vec3 direction)
{
	return direction * Math::QuatToMat(mOrientation);
}

mat4 Transform::CalculateMatrix()
{
	mat4 translationMatrix = mat4(1.f);
	translationMatrix[3] = vec4(mPosition, 1.f);

	mat4 scalingMatrix = mat4(1.f);
	scalingMatrix[0][0] = mScale.x;
	scalingMatrix[1][1] = mScale.y;
	scalingMatrix[2][2] = mScale.z;

	mat4 rotationMatrix = mat4(Math::QuatToMat(mOrientation));

	return (translationMatrix * rotationMatrix * scalingMatrix);
}

mat4 Transform::CalculateInverseMatrix()
{
	mat4 translationMatrix = mat4(1.f);
	translationMatrix[3] = vec4(-mPosition, 1.f);

	mat4 scalingMatrix = mat4(1.f);
	scalingMatrix[0][0] = 1.f / mScale.x;
	scalingMatrix[1][1] = 1.f / mScale.y;
	scalingMatrix[2][2] = 1.f / mScale.z;

	mat4 rotationMatrix = mat4(Math::QuatToMat(Math::Conjugate(mOrientation)));

	return (scalingMatrix * rotationMatrix * translationMatrix);
}
}
