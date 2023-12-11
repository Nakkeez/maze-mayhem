#pragma once

#include "../../core-engine/core/include/GeometryNode.h"
#include "Physics.h"

class PhysicsNode :
	public GeometryNode
{
public:
	enum class CollisionShape
	{
		None = 0,
		Box = 1,
		Sphere = 2
	};

	PhysicsNode(
		const std::shared_ptr<Geometry>& geometry,
		const std::shared_ptr<Material>& material) :
		GeometryNode(geometry, material),
		m_fMass(1.0f)
	{
	}

	~PhysicsNode();

	/**
	 * Set physics for the node
	 * @param physics physics to be set for the node
	 * @param mass mass of the node
	 */
	void SetPhysics(std::shared_ptr<Physics>& physics, float mass, CollisionShape shape);

	/**
	 * Update physics of the node
	 * @param frametime frame delta time
	 */
	void Update(float frametime) override;

private:
	// Global physics for all the nodes
	std::shared_ptr<Physics>				m_pPhysics;

	// Unique physics properties for individual nodes
	std::unique_ptr<btCollisionShape>		m_pShape;
	std::unique_ptr<btDefaultMotionState>	m_pMotionState;
	std::unique_ptr<btRigidBody>			m_pRigidBody;


	// Node's mass
	float									m_fMass;
};
