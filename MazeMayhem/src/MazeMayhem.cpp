#include "../include/MazeMayhem.h"

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

	// Generate geometry
	constexpr float radius = 0.5f;
	m_pGeometry = std::make_shared<Geometry>();
    m_pGeometry->GenQuad(glm::vec2(40.0f, 30.0f));

	// Setup the material
	m_pMaterial = std::make_shared<Material>();

	// Build the SceneGraph
	m_pSceneRoot = std::make_unique<Node>();

	// Create the camera
	auto camera = std::make_shared<CameraNode>();
	camera->SetName("camera");
	camera->SetProjectionParameters(0.61f, GetAspect(), 1.0f, 500.0f);
	camera->LookAt(glm::vec3(0.0f, 0.0f, 30.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	// Add camera node as root to the SceneGraph
	m_pSceneRoot->AddNode(camera);

	// Create terrain geometry object
	auto terrain = std::make_shared<GeometryNode>(m_pGeometry, m_pMaterial);
    terrain->SetPos(glm::vec3(0.0f, 0.0f, -30.0f));
    //terrain->RotateAxisAngle(glm::vec3(-1.0f, 0.0f, 0.0f), glm::half_pi<float>());
    terrain->SetName("terrain");

    m_pSceneRoot->AddNode(terrain);

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
}

void MazeMayhem::OnDraw(IRenderer& renderer)
{
    // Clear background to grey color
    renderer.Clear(0.2f, 0.2f, 0.2f, 1.0f);

    // Render the geometry
    glUseProgram(m_uProgram);

    const glm::vec3 lightDirection(glm::normalize(glm::vec3(-1.0f, 0.0f, -1.0f)));
    const glm::vec3 cameraPos(-renderer.GetViewMatrix()[3]); // Last row of view matrix (negative)
    OpenGLRenderer::SetUniformVec3(m_uProgram, "lightDirection", lightDirection);
    OpenGLRenderer::SetUniformVec3(m_uProgram, "cameraPosition", cameraPos);

    renderer.SetTexture(m_uProgram, m_uTexture, 0, "texture01");

    // Setup the camera matrices before rendering so there is no problems with SceneGraph
    const auto* camera = static_cast<CameraNode*>(m_pSceneRoot->FindNode("camera"));
    renderer.SetViewMatrix(camera->GetViewMatrix());
    renderer.SetProjectionMatrix(camera->GetProjectionMatrix());

    if (m_pSceneRoot)
    {
        m_pSceneRoot->Render(renderer, m_uProgram);
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
