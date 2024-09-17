#include "Application/Application.hpp"

using namespace DirectX;

Application::Application()
{
	m_D3DHandler = nullptr;
	m_Camera = nullptr;
	m_Model = nullptr;
	m_ColorShader = nullptr;
}


Application::Application(const Application& other)
{
}


Application::~Application()
{
}


bool Application::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;


	// Create and initialize the Direct3D object.
	m_D3DHandler = std::make_unique<D3DHandler>();

	result = m_D3DHandler->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = std::make_unique<Camera>();

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -5.0f);

	// Create and initialize the model object.
	m_Model = std::make_unique<Model>();

	result = m_Model->Initialize(m_D3DHandler->GetDevice());
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the color shader object.
	m_ColorShader = std::make_unique<ColorShader>();

	result = m_ColorShader->Initialize(m_D3DHandler->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the color shader object.", L"Error", MB_OK);
		return false;
	}

	return true;
}

bool Application::Frame()
{
	bool result;


	// Render the graphics scene.
	result = Render();
	if (!result)
	{
		return false;
	}

	return true;
}


bool Application::Render()
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	m_D3DHandler->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_D3DHandler->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3DHandler->GetProjectionMatrix(projectionMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_D3DHandler->GetDeviceContext());

	// Render the model using the color shader.
	result = m_ColorShader->Render(m_D3DHandler->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}

	// Clear the buffers to begin the scene.
	m_D3DHandler->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);


	// Present the rendered scene to the screen.
	m_D3DHandler->EndScene();

	return true;
}

void Application::Shutdown()
{
	// Release the color shader object.
	if (m_ColorShader)
	{
		m_ColorShader->Shutdown();
		m_ColorShader.reset();
		m_ColorShader = nullptr;
	}

	// Release the model object.
	if (m_Model)
	{
		m_Model->Shutdown();
		m_Model.reset();
		m_Model = nullptr;
	}

	// Release the camera object.
	if (m_Camera)
	{
		m_Camera.reset();
		m_Camera = nullptr;
	}

	// Release the Direct3D object.
	if (m_D3DHandler)
	{
		m_D3DHandler->Shutdown();
		m_D3DHandler.reset();
		m_D3DHandler = nullptr;
	}

	return;
}
