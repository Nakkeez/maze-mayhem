#pragma once

#include "../../core-engine/core/include/IApplication.h"
#include "../../core-engine/core/include/Geometry.h"
#include "../../core-engine/core/include/Material.h"
#include "../../core-engine/core/include/GeometryNode.h"
#include "../../core-engine/core/include/CameraNode.h"

// Bullet Physics Engine
#include "Physics.h"


class MazeMayhem : public IApplication
{
public:
	MazeMayhem();

	/**
	 * App initializer, called by abstraction layer when engine initialization is complete
	 * @return true if successful, false otherwise.
	 */
	bool OnCreate() override;

	/**
	 * App destroyer, called by the abstraction layer when app is about to get destoyed
	 */
	virtual void OnDestroy() override;

	/**
	 * App update loop
	 * @param frametime time since previous update, in seconds
	 */
	virtual void OnUpdate(float frametime) override;

	/**
	 * App drawing method
	 * @param renderer renderer object
	 */
	virtual void OnDraw(IRenderer& renderer) override;

	/**
	 * Key down event
	 * @param keyCode key code
	 * @return true if event was handled by the receiver
	 */
	bool OnKeyDown(uint32_t keyCode) override;

private:
	/**
	 * Helper method to access the OpenGL renderer layer
	 * @return pointer to OpenGL renderer
	 */
	OpenGLRenderer* GetOpenGLRenderer() { return static_cast<OpenGLRenderer*>(GetRenderer()); }

	GLuint						m_uVertexShader;
	GLuint						m_uFragmentShader;
	GLuint						m_uProgram;

	GLuint						m_uTexture;

	std::shared_ptr<Geometry>	m_pGeometry;
	std::shared_ptr<Geometry>	m_pSphere;
	std::shared_ptr<Material>	m_pMaterial;

	std::unique_ptr<Node>		m_pSceneRoot;

	std::shared_ptr<Physics>	m_pPhysics;
};