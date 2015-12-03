// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "RigidBody.hpp"
#include "Collider.hpp"
#include "Transform.hpp"
#include "../Resources/Entity.hpp"
#include "../Managers/PhysicsManager.hpp"
#include <btBulletDynamicsCommon.h>

class JuMotionState : public btMotionState
{
public:
	JuMotionState(JuEngine::Entity* entity)
	{
		mTransform = entity->GetComponent<JuEngine::Transform>();
	}

	~JuMotionState() {}

	void getWorldTransform(btTransform &worldTrans) const
	{
		JuEngine::vec3 position = mTransform->GetPosition();
		JuEngine::quat orientation = mTransform->GetRotation();

		btVector3 btPosition(position.x, position.y, position.z);
		btQuaternion btOrientation(orientation.x, orientation.y, orientation.z, orientation.w);

		btTransform transform;
		transform.setIdentity();
		transform.setRotation(btOrientation);
		transform.setOrigin(btPosition);

		worldTrans = transform;
	}

	void setWorldTransform(const btTransform &worldTrans)
	{
		btVector3 btPosition = worldTrans.getOrigin();
		btQuaternion btOrientation = worldTrans.getRotation();

		mTransform->SetLocalPosition(JuEngine::vec3(btPosition.x(), btPosition.y(), btPosition.z()));
		mTransform->SetLocalRotation(JuEngine::quat(btOrientation.w(), btOrientation.x(), btOrientation.y(), btOrientation.z()));
	}

protected:
	JuEngine::Transform* mTransform;
};

namespace JuEngine
{
RigidBody::~RigidBody()
{
	if(mRigidBody)
	{
		PhysicsManager::DeleteRigidBody(mRigidBody);
		delete mRigidBody;
		delete mCompoundShape;
	}
}

vec3 RigidBody::GetLinearVelocity()
{
	if(! mRigidBody)
	{
		return vec3(0.f, 0.f, 0.f);
	}

	auto btLinearVelocity = mRigidBody->getLinearVelocity();

	return vec3(btLinearVelocity.x(), btLinearVelocity.y(), btLinearVelocity.z());
}

void RigidBody::SetLinearVelocity(vec3 linearVelocity)
{
	if(! mRigidBody)
	{
		return;
	}

	btVector3 btLinearVelocity{linearVelocity.x, linearVelocity.y, linearVelocity.z};

	mRigidBody->setLinearVelocity(btLinearVelocity);
}

vec3 RigidBody::GetAngularVelocity()
{
	if(! mRigidBody)
	{
		return vec3(0.f, 0.f, 0.f);
	}

	auto btAngularVelocity = mRigidBody->getAngularVelocity();

	return vec3(btAngularVelocity.x(), btAngularVelocity.y(), btAngularVelocity.z());
}

void RigidBody::SetAngularVelocity(vec3 angularVelocity)
{
	if(! mRigidBody)
	{
		return;
	}

	btVector3 btAngularVelocity{angularVelocity.x, angularVelocity.y, angularVelocity.z};

	mRigidBody->setAngularVelocity(btAngularVelocity);
}

auto RigidBody::GetRestitution() -> float
{
	if(! mRigidBody)
	{
		return 0.f;
	}

	return mRigidBody->getRestitution();
}

void RigidBody::SetRestitution(float restitution)
{
	if(! mRigidBody)
	{
		return;
	}

	// Spring / Bounciness
	mRigidBody->setRestitution(restitution);
}

auto RigidBody::GetLinearDamping() -> float
{
	if(! mRigidBody)
	{
		return 0.f;
	}

	return mRigidBody->getLinearDamping();
}

auto RigidBody::GetAngularDamping() -> float
{
	if(! mRigidBody)
	{
		return 0.f;
	}

	return mRigidBody->getAngularDamping();
}

void RigidBody::SetDamping(float linearDamping, float angularDamping)
{
	if(! mRigidBody)
	{
		return;
	}

	// Friction
	mRigidBody->setDamping(linearDamping, angularDamping);
}

vec3 RigidBody::GetLinearFactor()
{
	if(! mRigidBody)
	{
		return vec3(0.f, 0.f, 0.f);
	}

	auto btLinearFactor = mRigidBody->getLinearFactor();

	return vec3(btLinearFactor.x(), btLinearFactor.y(), btLinearFactor.z());
}

void RigidBody::SetLinearFactor(vec3 linearFactor)
{
	if(! mRigidBody)
	{
		return;
	}

	btVector3 btLinearFactor{linearFactor.x, linearFactor.y, linearFactor.z};

	mRigidBody->setLinearFactor(btLinearFactor);
}

vec3 RigidBody::GetAngularFactor()
{
	if(! mRigidBody)
	{
		return vec3(0.f, 0.f, 0.f);
	}

	auto btAngularFactor = mRigidBody->getAngularFactor();

	return vec3(btAngularFactor.x(), btAngularFactor.y(), btAngularFactor.z());
}

void RigidBody::SetAngularFactor(vec3 angularFactor)
{
	if(! mRigidBody)
	{
		return;
	}

	btVector3 btAngularFactor{angularFactor.x, angularFactor.y, angularFactor.z};

	mRigidBody->setAngularFactor(btAngularFactor);
}

auto RigidBody::GetMass() -> float
{
	if(! mRigidBody)
	{
		return 0.f;
	}

	return mMass;
}

void RigidBody::SetMass(float mass)
{
	mMass = mass;

	if(mRigidBody)
	{
		Rebuild();
	}
}

void RigidBody::Rebuild()
{
	if(mRigidBody)
	{
		PhysicsManager::DeleteRigidBody(mRigidBody);
		delete mRigidBody;
		delete mCompoundShape;
	}

	if(! mEntity->HasComponent<Collider>())
	{
		return;
	}

	mCompoundShape = new btCompoundShape();

	auto colliders = mEntity->GetComponents<Collider>();

	for(auto& collider : colliders)
	{
		btTransform localTrans;
		localTrans.setIdentity();
		// localTrans.setOrigin(btVector3(0,0,1)); // shifts the center of mass with respect to the chassis
		mCompoundShape->addChildShape(localTrans, ((Collider*)collider)->GetCollisionShape());
	}

	btVector3 inertia;
	mCompoundShape->calculateLocalInertia(mMass, inertia);

	auto scale = mEntity->GetComponent<Transform>()->GetLocalScale();
	btVector3 btScale(scale.x, scale.y, scale.z);
	mCompoundShape->setLocalScaling(btScale);

	JuMotionState* motionState = new JuMotionState(mEntity);
	btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mMass, motionState, mCompoundShape, btVector3(0, 0, 0));
	mRigidBody = new btRigidBody(rigidBodyCI);
	mRigidBody->setUserPointer((void*)mEntity);

	if (mMass == 0.f)
	{
		mRigidBody->setCollisionFlags(mRigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
		mRigidBody->setActivationState(DISABLE_DEACTIVATION);
	}
	else
	{
		mRigidBody->setActivationState(ACTIVE_TAG);
		mRigidBody->activate();
	}

	PhysicsManager::AddRigidBody(mRigidBody);
}

void RigidBody::Init()
{
	Rebuild();
}
}
