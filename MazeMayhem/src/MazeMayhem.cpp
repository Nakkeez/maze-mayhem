#include "../include/MazeMayhem.h"
#include "../include/PhysicsNode.h"

MazeMayhem::MazeMayhem() :
    m_uVertexShader(0),
    m_uFragmentShader(0),
    m_uProgram(0),
    m_uTexture(0)
{
    RandSeed(); // Initialize fresh set of pseudorandom numbers
}

bool MazeMayhem::OnCreate()
{
	// OnCreate is called by the application when window and graphics initialization is complete
	auto renderer = GetOpenGLRenderer();
	m_uVertexShader = renderer->CreateVertexShaderFromFile("./shaders/phongshader.vert");
	m_uFragmentShader = renderer->CreateFragmentShaderFromFile("./shaders/phongshader.frag");
	m_uProgram = renderer->CreateProgram(m_uVertexShader, m_uFragmentShader);
	m_uTexture = renderer->CreateTexture("./assets/ground.png");

	// Do not start the app if initialization of shader program was unsuccesful
	if (!m_uVertexShader || !m_uFragmentShader || !m_uProgram || !m_uTexture)
	{
		return false;
	}

    // Start the physics
    m_pPhysics = std::make_shared<Physics>();

	// Generate geometry
	constexpr float radius = 0.5f;
	m_pGeometry = std::make_shared<Geometry>();
    m_pGeometry->GenQuad(glm::vec2(40.0f, 30.0f));

    constexpr float size = 2.0f;
    m_pSphere = std::make_shared<Geometry>();
    m_pSphere->GenSphere(
        glm::vec3(size * 0.5f),
        glm::vec3(0.0f),
        24,
        24);

	// Setup the material
	m_pMaterial = std::make_shared<Material>();

	// Build the SceneGraph
	m_pSceneRoot = std::make_unique<Node>();

	// Create the camera
	auto player = std::make_shared<PlayerNode>();
    player->SetPlayerState(PlayerNode::PlayerState::STATIONARY);
    player->SetName("player");
    player->SetProjectionParameters(0.61f, GetAspect(), 1.0f, 500.0f);
    player->LookAt(glm::vec3(0.0f, 2.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	// Add camera node as root to the SceneGraph
	m_pSceneRoot->AddNode(player);

    // Add static collision bodies
    m_pPhysics->AddStaticCollisionPlane(glm::vec4(0.0f, 1.0f, 0.0f, 0.0f));

	// Create terrain geometry object
	auto terrain = std::make_shared<GeometryNode>(m_pGeometry, m_pMaterial);
    terrain->SetPos(glm::vec3(0.0f, 0.0f, 0.0f));
    terrain->RotateAxisAngle(glm::vec3(1.0f, 0.0f, 0.0f), -glm::half_pi<float>());
    terrain->SetName("terrain");

    // Create sphere geometry object
    auto sphere = std::make_shared<PhysicsNode>(m_pSphere, m_pMaterial);
    sphere->SetPos(0.0f, 10.0f, 0.0f);
    sphere->SetRotationAxis(glm::vec3(0.0f, 1.0f, 0.0f));
    sphere->SetRotationSpeed(glm::linearRand(-1.0f, 1.0f));
    sphere->SetName("sphere");
    sphere->SetRadius(size * 0.5f);

    sphere->SetPhysics(m_pPhysics, 1.0f, PhysicsNode::CollisionShape::Sphere);

    m_pSceneRoot->AddNode(terrain);
    m_pSceneRoot->AddNode(sphere);

	return true;
}

void MazeMayhem::OnDestroy()
{
    // Delete root and all the child nodes
    m_pSceneRoot = nullptr;

    // Delete OpenGL resources
    glDeleteTextures(1, &m_uTexture);
    glDeleteProgram(m_uProgram);
    glDeleteShader(m_uFragmentShader);
    glDeleteShader(m_uVertexShader);
}

void MazeMayhem::OnUpdate(float frametime)
{
    if (m_pSceneRoot)
    {
        // Update the entire SceneGraph
        m_pSceneRoot->Update(frametime);
    }

    if (m_pPhysics)
    {
        m_pPhysics->Update(frametime);
    }

    auto player = m_pSceneRoot->FindNode("player");
    glm::vec3 velocity(player->GetVelocity());

    if (IsKeyDown(KEY_LEFT))
    {
        velocity.x -= frametime * 10.0f;
    }
    if (IsKeyDown(KEY_RIGHT))
    {
        velocity.x += frametime * 10.0f;
    }
    if (IsKeyDown(KEY_UP))
    {
        velocity.z -= frametime * 10.0f;
    }
    if (IsKeyDown(KEY_DOWN))
    {
        velocity.z += frametime * 10.0f;
    }
    if (IsKeyDown(KEY_SPACE))
    {
        // TODO: Jump
        velocity.y += frametime * 10.0f;
    }

    float len = glm::length(velocity);
    if (len)
    {
        len -= frametime * 1.5f;
        velocity = glm::normalize(velocity) * len;
    }

    player->SetVelocity(velocity);

}

void MazeMayhem::OnDraw(IRenderer& renderer)
{
    // Clear background to grey color
    renderer.Clear(0.2f, 0.2f, 0.2f, 1.0f);

    // Render the geometry
    glUseProgram(m_uProgram);

    const glm::vec3 lightDirection(glm::normalize(glm::vec3(0.0f, -0.5f, -1.0f)));
    const glm::vec3 cameraPos(-renderer.GetViewMatrix()[3]); // Last row of view matrix (negative)
    OpenGLRenderer::SetUniformVec3(m_uProgram, "lightDirection", lightDirection);
    OpenGLRenderer::SetUniformVec3(m_uProgram, "cameraPosition", cameraPos);

    renderer.SetTexture(m_uProgram, m_uTexture, 0, "texture01");

    // Setup the camera matrices before rendering so there is no problems with SceneGraph
    const auto* player = static_cast<CameraNode*>(m_pSceneRoot->FindNode("player"));
    renderer.SetViewMatrix(player->GetViewMatrix());
    renderer.SetProjectionMatrix(player->GetProjectionMatrix());

    if (m_pSceneRoot)
    {
        m_pSceneRoot->Render(renderer, m_uProgram);
    }
}

void MazeMayhem::OnScreenSizeChanged(uint32_t widthPixels, uint32_t heightPixels)
{
    // Check in case the function gets called before application is initialized
    if (m_pSceneRoot)
    {
        auto* camera = static_cast<CameraNode*>(m_pSceneRoot->FindNode("camera"));
        if (camera)
        {
            // Update aspect ratio
            auto params = camera->GetProjectionParameters();
            params.y = GetAspect();
            camera->SetProjectionParameters(params);
        }
    }

}

bool MazeMayhem::OnKeyDown(uint32_t keyCode)
{
    if (keyCode == KEY_ESC)
    {
        Close();
        return true;
    }

    return false;
}
