// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "PhysicsManager.hpp"
#include "../Resources/DebugLog.hpp"
#include "AppTimeManager.hpp"
#include "../Resources/Mesh.hpp"
#include "../Resources/Shader.hpp"
#include "MaterialManager.hpp"
#include <btBulletDynamicsCommon.h>

class JuPhysicsDebugDrawer : public btIDebugDraw
{
public:
	JuPhysicsDebugDrawer()
	{
		mDebugMode = DBG_DrawWireframe | DBG_DrawAabb | DBG_NoHelpText;
	}

	~JuPhysicsDebugDrawer() {}

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
	{
		drawLine(from, to, color, color);
	}

	void drawLine(const btVector3& from, const btVector3& to, const btVector3& fromColor, const btVector3& toColor)
	{
		mVertexArray.push_back(from.x());
		mVertexArray.push_back(from.y());
		mVertexArray.push_back(from.z());
		mVertexArray.push_back(0.f);
		mVertexArray.push_back(1.0f);
		mVertexArray.push_back(0.f);
		mVertexArray.push_back(0.f);
		mVertexArray.push_back(0.f);
		mVertexArray.push_back(fromColor.x());
		mVertexArray.push_back(fromColor.y());
		mVertexArray.push_back(fromColor.z());

		mVertexArray.push_back(to.x());
		mVertexArray.push_back(to.y());
		mVertexArray.push_back(to.z());
		mVertexArray.push_back(0.f);
		mVertexArray.push_back(1.0f);
		mVertexArray.push_back(0.f);
		mVertexArray.push_back(0.f);
		mVertexArray.push_back(0.f);
		mVertexArray.push_back(toColor.x());
		mVertexArray.push_back(toColor.y());
		mVertexArray.push_back(toColor.z());

		unsigned int iOffset = mIndexArray.size();
		mIndexArray.push_back(iOffset+0);
		mIndexArray.push_back(iOffset+1);
	}

	void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) {}
	void draw3dText(const btVector3& location, const char* textString) {}

	void reportErrorWarning(const char* warningString)
	{
		JuEngine::DebugLog::Write("%s", warningString);
	}

	void setDebugMode(int debugMode)
	{
		mDebugMode = debugMode;
	}

	int getDebugMode() const
	{
		return mDebugMode;
	}

	std::vector<float> mVertexArray;
	std::vector<unsigned int> mIndexArray;

private:
	int mDebugMode;
};

// =======================================================================================
btCollisionShape* groundShape;
btRigidBody* groundRigidBody;
// =======================================================================================

namespace JuEngine
{
PhysicsManager* PhysicsManager::mInstance = nullptr;

PhysicsManager::PhysicsManager() : IObject("physicsManager")
{
	if(PhysicsManager::mInstance != nullptr)
	{
		ThrowRuntimeError("Error, there are an PhysicsManager instance created already");
	}

	PhysicsManager::mInstance = this;

	mBroadphase = new btDbvtBroadphase();
	mCollisionConf = new btDefaultCollisionConfiguration();
	mDispatcher = new btCollisionDispatcher(mCollisionConf);
	mSolver = new btSequentialImpulseConstraintSolver;
	mDebugDrawer = new JuPhysicsDebugDrawer();

	mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher, mBroadphase, mSolver, mCollisionConf);
	mDynamicsWorld->setDebugDrawer(mDebugDrawer);
	mDynamicsWorld->setGravity(btVector3(0.f, -9.81f, 0.f));

	// =======================================================================================
	groundShape = new btStaticPlaneShape(btVector3(0.f, 1.f, 0.f), 1.f);

	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
	btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0.f, groundMotionState, groundShape, btVector3(0, 0, 0)); // 0 mass == static
	groundRigidBody = new btRigidBody(groundRigidBodyCI);
	AddRigidBody(groundRigidBody);
	// =======================================================================================
}

PhysicsManager::~PhysicsManager()
{
	PhysicsManager::mInstance = nullptr;
}

void PhysicsManager::FixedUpdate()
{
	mDynamicsWorld->stepSimulation(AppTimeManager::GetDeltaTime().AsSeconds(), 10);
}

vec3 PhysicsManager::GetGravity()
{
	auto gravity = mDynamicsWorld->getGravity();

	return vec3(gravity.x(), gravity.y(), gravity.z());
}

void PhysicsManager::SetGravity(vec3 gravity)
{
	mDynamicsWorld->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}

void PhysicsManager::DebugDraw()
{
	PhysicsManager::mInstance->mDebugDrawer->mVertexArray.clear();
	PhysicsManager::mInstance->mDebugDrawer->mVertexArray.shrink_to_fit();

	PhysicsManager::mInstance->mDebugDrawer->mIndexArray.clear();
	PhysicsManager::mInstance->mDebugDrawer->mIndexArray.shrink_to_fit();

	PhysicsManager::mInstance->mDynamicsWorld->debugDrawWorld();

	Mesh lines(PhysicsManager::mInstance->mDebugDrawer->mVertexArray, PhysicsManager::mInstance->mDebugDrawer->mIndexArray, GL_LINES);
	lines.Use();

	Material* material = MaterialManager::Get("mat_vertexColor");
	material->Use();
	material->GetShader()->SetUniform("modelToWorldMatrix", mat4(1.f));

	glDrawElements(lines.GetDrawMode(), lines.GetIndexCount(), GL_UNSIGNED_INT, 0);
}

void PhysicsManager::Unload()
{
	delete mDynamicsWorld;
	delete mSolver;
	delete mDispatcher;
	delete mCollisionConf;
	delete mBroadphase;
}

void PhysicsManager::AddRigidBody(btRigidBody* rigidBody)
{
	PhysicsManager::mInstance->mDynamicsWorld->addRigidBody(rigidBody);
}

void PhysicsManager::DeleteRigidBody(btRigidBody* rigidBody)
{
	PhysicsManager::mInstance->mDynamicsWorld->removeRigidBody(rigidBody);
}
}
