#pragma once

#include "../../core-engine/core/include/IApplication.h"

#include <bullet/btBulletCollisionCommon.h>
#include <bullet/btBulletDynamicsCommon.h>

class Physics
{
public:
	struct STATIC_COLLISION
	{
		std::unique_ptr<btCollisionShape>	m_pShape;
		std::unique_ptr<btMotionState>		m_pMotion;
		std::unique_ptr<btRigidBody>		m_pRigidBody;
	};

	Physics();
	~Physics();

	/**
	 * Update physics
	 * @param frametime frame delta time
	 */
	void Update(float frametime);

	/**
	 * Add collision plane in 3D space.
	 * x, y and z components are normal of a plane that points straight away
	 * from the plane. w component is distant from the center of world
	 * @param plane plane in 3D space (x,y,z,w)
	 */
	void AddStaticCollisionPlane(const glm::vec4& plane);

	/**
	 * Helper method to access the interface class for physics implementation
	 * @return pointer to the physics interface implementation
	 */
	btDiscreteDynamicsWorld* GetDynamicsWorld() { return m_pDynamicsWorld.get(); }
	const btDiscreteDynamicsWorld* GetDynamicsWorld() const { return m_pDynamicsWorld.get(); }

private:
	std::unique_ptr<btBroadphaseInterface>					m_pBroadphase;
	std::unique_ptr<btDefaultCollisionConfiguration>		m_pCollisionConfiguration;
	std::unique_ptr<btCollisionDispatcher>					m_pDispatcher;
	std::unique_ptr<btSequentialImpulseConstraintSolver>	m_pSolver;
	std::unique_ptr<btDiscreteDynamicsWorld>				m_pDynamicsWorld;

	std::vector<STATIC_COLLISION>							m_arrCollisionObjects;
};
