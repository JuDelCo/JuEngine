// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "Component.hpp"
#include "../Math.hpp"

class btRigidBody;
class btCompoundShape;

namespace JuEngine
{
class JUENGINEAPI RigidBody : public Component
{
friend class Entity;

public:
	RigidBody() = default;
	~RigidBody();

	vec3 GetLinearVelocity();
	void SetLinearVelocity(vec3 linearVelocity);
	vec3 GetAngularVelocity();
	void SetAngularVelocity(vec3 angularVelocity);
	auto GetRestitution() -> float;
	void SetRestitution(float restitution);
	auto GetLinearDamping() -> float;
	auto GetAngularDamping() -> float;
	void SetDamping(float linearDamping, float angularDamping);
	vec3 GetLinearFactor();
	void SetLinearFactor(vec3 linearFactor);
	vec3 GetAngularFactor();
	void SetAngularFactor(vec3 angularFactor);
	auto GetMass() -> float;
	void SetMass(float mass);
	void Rebuild();

protected:
	void Init();

private:
	btRigidBody* mRigidBody;
	btCompoundShape* mCompoundShape;
	float mMass{0.f};
};
}
