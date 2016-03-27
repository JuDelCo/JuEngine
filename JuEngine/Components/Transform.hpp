// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Entity/IComponent.hpp"
#include "../Resources/Math.hpp"
#include <memory>

namespace JuEngine
{
class Entity;
typedef std::shared_ptr<Entity> EntityPtr;

class JUENGINEAPI Transform : public IComponent
{
	public:
		void Reset();
		void Reset(const vec3 position);
		void Reset(const vec3 position, const quat orientation);

		void SetParent(const EntityPtr& parent);
		void SetParent(Transform* parent);
		vec3 GetPosition() const;
		vec3 GetScale() const;
		vec3 GetEulerAngles();
		quat GetRotation() const;
		auto Right() -> const vec3;
		auto Up() -> const vec3;
		auto Forward() -> const vec3;
		auto GetLocalPosition() const -> const vec3&;
		auto GetLocalScale() const -> const vec3&;
		auto GetLocalEulerAngles() -> const vec3&;
		auto GetLocalRotation() const -> const quat&;
		void SetLocalPosition(const vec3 position);
		void SetLocalScale(const vec3 scale);
		void SetLocalEulerAngles(const vec3 eulerAngles);
		void SetLocalRotation(const quat orientation);
		auto LocalRight() -> const vec3&;
		auto LocalUp() -> const vec3&;
		auto LocalForward() -> const vec3&;
		void Translate(const vec3 translation, const bool relativeToWorld = false);
		void Translate(const vec3 translation, Transform* relativeTo);
		void Scale(const vec3 scale);
		void Rotate(const vec3 eulerAngles, const bool relativeToWorld = false);
		void Rotate(const vec3 eulerAngles, Transform* relativeTo);
		void Rotate(const quat orientation, const bool relativeToWorld = false);
		void Rotate(const quat orientation, const Transform* relativeTo);
		void RotateAround(const vec3 point, const vec3 axis, const float angle);
		void LookAt(const vec3 worldPosition, const vec3 worldUp = vec3(0.f, 1.f, 0.f));
		auto GetMatrix() -> const mat4&;					// Local to World
		vec3 TransformPoint(const vec3 position);
		vec3 TransformVector(const vec3 vector);
		vec3 TransformDirection(const vec3 direction);
		auto GetInverseMatrix() -> const mat4&;				// World to Local
		vec3 InverseTransformPoint(const vec3 position);
		vec3 InverseTransformVector(const vec3 vector);
		vec3 InverseTransformDirection(const vec3 direction);

	private:
		mat4 CalculateMatrix();
		mat4 CalculateInverseMatrix();

		// TODO: Transform: If a parent entity is deleted, it must have a list of child entities to remove them too.
		Transform* mParent;
		vec3 mPosition{0.f, 0.f, 0.f};
		vec3 mScale{1.f, 1.f, 1.f};
		quat mOrientation{1.f, 0.f, 0.f, 0.f};
		bool mEulerRefreshNeeded{false};
		vec3 mEulerAnglesCache{0.f, 0.f, 0.f};
		bool mRightRefreshNeeded{false};
		vec3 mRightCache{1.f, 0.f, 0.f};
		bool mUpRefreshNeeded{false};
		vec3 mUpCache{0.f, 1.f, 0.f};
		bool mForwardRefreshNeeded{false};
		vec3 mForwardCache{0.f, 0.f, 1.f};
		bool mMatrixRefreshNeeded{false};
		mat4 mMatrixCache{mat4(1.f)};
		mat4 mWorldMatrixCache{mat4(1.f)};
		mat4 mParentWorldMatrixCache{mat4(1.f)};
		bool mInverseMatrixRefreshNeeded{false};
		mat4 mInverseMatrixCache{mat4(1.f)};
		mat4 mWorldInverseMatrixCache{mat4(1.f)};
		mat4 mParentWorldInverseMatrixCache{mat4(1.f)};
};
}
