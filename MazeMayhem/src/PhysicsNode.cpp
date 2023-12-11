#include "../include/PhysicsNode.h"

#include "../../core-engine/core/glm-master/glm/gtc/quaternion.hpp"

PhysicsNode::~PhysicsNode()
{
	if (m_pPhysics && m_pRigidBody)
	{
		m_pPhysics->GetDynamicsWorld()->removeRigidBody(m_pRigidBody.get());

	}
}

void PhysicsNode::SetPhysics(std::shared_ptr<Physics>& physics, float mass, CollisionShape shape)
{
	if (physics)
	{
		m_pPhysics = physics;

		switch (shape)
		{
		case CollisionShape::None:
			m_pShape = nullptr;
			break;

		case CollisionShape::Box:
			// Make node box shaped
			m_pShape = std::make_unique<btBoxShape>(btVector3(GetRadius(), GetRadius(), GetRadius()));
			break;

		case CollisionShape::Sphere:
			// Make node sphere shaped
			m_pShape = std::make_unique<btSphereShape>(GetRadius());
			break;
		}

		if (m_pShape)
		{
			btVector3 fallInertia(0.0f, 0.0f, 0.0f);
			m_pShape->calculateLocalInertia(mass, fallInertia);
			m_fMass = mass;

			// Build initial motion state (transformation where object begins at)
			const auto worldMatrix = GetWorldMatrix();
			const glm::quat q = glm::quat_cast(worldMatrix);
			btQuaternion qq(q.x, q.y, q.z, q.w);
			const btVector3 pos(worldMatrix[3][0], worldMatrix[3][1], worldMatrix[3][2]);
			m_pMotionState = std::make_unique<btDefaultMotionState>(btTransform(qq, pos));

			// Rigid body construction
			btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, m_pMotionState.get(), m_pShape.get(), fallInertia);
			rigidBodyCI.m_restitution = 0.5f; // When object rests and no longer uses physics
			rigidBodyCI.m_friction = 10.0f;

			m_pRigidBody = std::make_unique<btRigidBody>(rigidBodyCI);

			m_pPhysics->GetDynamicsWorld()->addRigidBody(m_pRigidBody.get());
		}
	}
	else
	{
		if (m_pPhysics) m_pPhysics->GetDynamicsWorld()->removeRigidBody(m_pRigidBody.get());
		m_pRigidBody = nullptr;
		m_pMotionState = nullptr;
		m_pShape = nullptr;
		m_pPhysics = nullptr;
	}
}

void PhysicsNode::Update(float frametime)
{
	if (m_pPhysics)
	{
		// Update the center of mass from node position
		/*btTransform transform = m_pRigidBody->getCenterOfMassTransform();
		const auto pos = GetPos();
		transform.setOrigin(btVector3(pos.x, pos.y, pos.z));
		m_pRigidBody->setCenterOfMassTransform(transform);*/

		// Get the local matrix from physics engine
		btTransform transform;
		m_pRigidBody->getMotionState()->getWorldTransform(transform);
		transform.getOpenGLMatrix(&m_mModel[0][0]);

		// Update all the children
		for (auto& node : m_arrNodes)
		{
			node->Update(frametime);
		}
	}
	else
	{
		GeometryNode::Update(frametime);
	}
}

