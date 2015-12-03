// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../IObject.hpp"
#include "../Math.hpp"

class btBroadphaseInterface;
class btDefaultCollisionConfiguration;
class btCollisionDispatcher;
class btSequentialImpulseConstraintSolver;
class btDiscreteDynamicsWorld;
class JuPhysicsDebugDrawer;
class btRigidBody;

namespace JuEngine
{
class JUENGINEAPI PhysicsManager : public IObject
{
friend class Application;
friend class RigidBody;

public:
	PhysicsManager();
	virtual ~PhysicsManager();

	void FixedUpdate();
	vec3 GetGravity();
	void SetGravity(vec3 gravity);
	static void DebugDraw();

protected:
	void Unload();
	static void AddRigidBody(btRigidBody* rigidBody);
	static void DeleteRigidBody(btRigidBody* rigidBody);

private:
	btBroadphaseInterface* mBroadphase;
	btDefaultCollisionConfiguration* mCollisionConf;
	btCollisionDispatcher* mDispatcher;
	btSequentialImpulseConstraintSolver* mSolver;
	btDiscreteDynamicsWorld* mDynamicsWorld;
	JuPhysicsDebugDrawer* mDebugDrawer;

	// Singleton
	static PhysicsManager* mInstance;
};
}
