#include "Application/Application.hpp"

using namespace DirectX;
using namespace std;

const char* cube_file_path = "D:\\ProgettiPersonali\\RealTimeGraphicalEngine\\data\\models\\Cube.txt";
const char* plane_file_path = "D:\\ProgettiPersonali\\RealTimeGraphicalEngine\\data\\models\\Plane.txt";
const char* sphere_file_path = "D:\\ProgettiPersonali\\RealTimeGraphicalEngine\\data\\models\\Sphere.txt";
const char* sprite_file_path = "D:\\ProgettiPersonali\\RealTimeGraphicalEngine\\data\\sprite\\sprite_data_01.txt";
const char* texture_file_path = "D:\\ProgettiPersonali\\RealTimeGraphicalEngine\\data\\texture\\stone01.tga";

Application::Application()
{
	m_D3DHandler = nullptr;
	m_Timer = nullptr;
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
	float frameTime;
	bool result;

	// Update the system stats.
	m_Timer->Frame();

	// Get the current frame time.
	frameTime = m_Timer->GetTime();

	// Update the rotation variable each frame.
	rotation -= 0.0174532925f * 0.1f;
	if (rotation < 0.0f)
	{
		rotation += 360.0f;
	}

	// Render the graphics scene.
	result = Render(frameTime, rotation);
	if (!result)
	{
		return false;
	}

	return true;
}

bool Application::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	// Create and initialize the Direct3D object.
	m_D3DHandler = make_shared<D3DHandler>();

	result = m_D3DHandler->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D", L"Error", MB_OK);
		return false;
	}

	m_scene = make_unique<Scene>(m_D3DHandler);
	m_scene->Initialize(hwnd);

	// Create the camera object.
	m_scene->AddCamera(XMFLOAT3(0.0f, 7.0f, -14.0f), XMFLOAT3(18.0f, 0.0f, 0.0f));

	// Create and initialize the model object.
	/*result = m_scene->AddModel(plane_file_path, texture_file_path, ShaderType::LightShaderType);

	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the model object.", L"Error", MB_OK);
		return false;
	}

	result = m_scene->AddBitmap(texture_file_path);

	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}*/

	result = m_scene->AddSprite(sprite_file_path);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the sprite object.", L"Error", MB_OK);
		return false;
	}

	m_Timer = make_unique<Timer>();

	result = m_Timer->Initialize();
	if (!result)
	{
		return false;
	}

	m_scene->AddLightSource(XMFLOAT3(-3.0f, 1.0f, 3.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f));  // Red
	m_scene->AddLightSource(XMFLOAT3(3.0f, 1.0f, 3.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f));  // Green
	m_scene->AddLightSource(XMFLOAT3(-3.0f, 1.0f, -3.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f));  // Blue
	m_scene->AddLightSource(XMFLOAT3(3.0f, 1.0f, -3.0f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f));  // White

	return result;
}

bool Application::Render(float frameTime, float rotation)
{
	m_scene->Render(frameTime);

	return true;
}

void Application::Shutdown()
{
	// Release the timer object.
	if (m_Timer)
	{
		m_Timer.reset();
		m_Timer = nullptr;
	}

	if (m_scene)
	{
		m_scene->Shutdown();
		m_scene.reset();
		m_scene = nullptr;
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
