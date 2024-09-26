#include "Application/Application.hpp"

#define RENDER_TYPE 2 //0: COLOR, 1: TEXTURE, 2: LIGHT

using namespace DirectX;
using namespace std;

const char* cube_file_path = "D:\\ProgettiPersonali\\RealTimeGraphicalEngine\\data\\models\\Cube.txt";
const char* sphere_file_path = "D:\\ProgettiPersonali\\RealTimeGraphicalEngine\\data\\models\\Sphere.txt";
const char* texture_file_path = "D:\\ProgettiPersonali\\RealTimeGraphicalEngine\\data\\texture\\stone01.tga";

Application::Application()
{
	m_D3DHandler = nullptr;
	m_Camera = nullptr;
	m_Model = nullptr;
	m_ColorShader = nullptr;
	m_TextureShader = nullptr;
	m_LightShader = nullptr;
	m_Light = nullptr;
}

Application::Application(const Application& other)
{
}

Application::~Application()
{
}

bool Application::Frame()
{
	static float rotation = 0.0f;
	bool result;

	// Update the rotation variable each frame.
	rotation -= 0.0174532925f * 0.1f;
	if (rotation < 0.0f)
	{
		rotation += 360.0f;
	}

	// Render the graphics scene.
	result = Render(rotation);
	if (!result)
	{
		return false;
	}

	return true;
}

bool Application::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	char modelFilename[128];
	char textureFilename[128];
	bool result;


	// Create and initialize the Direct3D object.
	m_D3DHandler = make_unique<D3DHandler>();

	result = m_D3DHandler->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = make_unique<Camera>();

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -7.0f);

	// Create and initialize the model object.
	m_Model = make_unique<Model>();	

#if RENDER_TYPE == 0
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
#else
	// Set the file name of the model.
	strcpy_s(modelFilename, sphere_file_path);

	// Set the name of the texture file that we will be loading.
	strcpy_s(textureFilename, texture_file_path);

	result = m_Model->Initialize(m_D3DHandler->GetDevice(), m_D3DHandler->GetDeviceContext(), modelFilename, textureFilename);

	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}
#if RENDER_TYPE == 1
	// Create and initialize the texture shader object.
	m_TextureShader = make_unique<TextureShader>();

	result = m_TextureShader->Initialize(m_D3DHandler->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}
#else
	// Create and initialize the light shader object.
	m_LightShader = make_unique<LightShader>();

	result = m_LightShader->Initialize(m_D3DHandler->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create and initialize the light object.
	m_Light = make_unique<Light>();

	m_Light->SetAmbientColor(0.15f, 0.15f, 0.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f, 0.0f, 1.0f);
	m_Light->SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetSpecularPower(32.0f);
#endif
#endif

	return true;
}

bool Application::Render(float rotation)
{
	XMMATRIX worldMatrix, viewMatrix, projectionMatrix, rotateMatrix, translateMatrix, scaleMatrix, srMatrix;
	bool result;


	// Clear the buffers to begin the scene.
	m_D3DHandler->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_D3DHandler->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3DHandler->GetProjectionMatrix(projectionMatrix);

#if RENDER_TYPE == 0
	// Rotate the world matrix by the rotation value so that the triangle will spin.
	worldMatrix = XMMatrixRotationY(rotation);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_D3DHandler->GetDeviceContext());

	// Render the model using the color shader.
	result = m_ColorShader->Render(m_D3DHandler->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix);
	if (!result)
	{
		return false;
	}
#elif RENDER_TYPE == 1
	// Rotate the world matrix by the rotation value so that the triangle will spin.
	worldMatrix = XMMatrixRotationY(rotation);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_D3DHandler->GetDeviceContext());

	// Render the model using the texture shader.
	result = m_TextureShader->Render(m_D3DHandler->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture());
	if (!result)
	{
		return false;
	}
#elif RENDER_TYPE == 2
	rotateMatrix = XMMatrixRotationY(rotation);  // Build the rotation matrix.
	translateMatrix = XMMatrixTranslation(-2.0f, 0.0f, 0.0f);  // Build the translation matrix.

	// Multiply them together to create the final world transformation matrix.
	worldMatrix = XMMatrixMultiply(rotateMatrix, translateMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_D3DHandler->GetDeviceContext());

	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3DHandler->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}

	scaleMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f);  // Build the scaling matrix.
	rotateMatrix = XMMatrixRotationY(rotation);  // Build the rotation matrix.
	translateMatrix = XMMatrixTranslation(2.0f, 0.0f, 0.0f);  // Build the translation matrix.

	// Multiply the scale, rotation, and translation matrices together to create the final world transformation matrix.
	srMatrix = XMMatrixMultiply(scaleMatrix, rotateMatrix);
	worldMatrix = XMMatrixMultiply(srMatrix, translateMatrix);

	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	m_Model->Render(m_D3DHandler->GetDeviceContext());

	// Render the model using the light shader.
	result = m_LightShader->Render(m_D3DHandler->GetDeviceContext(), m_Model->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, m_Model->GetTexture(),
		m_Light->GetDirection(), m_Light->GetAmbientColor(), m_Light->GetDiffuseColor(), m_Camera->GetPosition(), m_Light->GetSpecularColor(), m_Light->GetSpecularPower());
	if (!result)
	{
		return false;
	}
#endif

	// Present the rendered scene to the screen.
	m_D3DHandler->EndScene();

	return true;
}

void Application::Shutdown()
{
	// Release the light object.
	if (m_Light)
	{
		m_Light.reset();
		m_Light = nullptr;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		m_LightShader.reset();
		m_LightShader = nullptr;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		m_TextureShader.reset();
		m_TextureShader = nullptr;
	}

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
