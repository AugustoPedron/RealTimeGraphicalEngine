#pragma once

#include <memory>

#include "Application/Camera.hpp"
#include "Application/Scene.hpp"
#include "D3DRendering/ColorShader.hpp"
#include "D3DRendering/D3DHandler.hpp"
#include "D3DRendering/LightShader.hpp"
#include "D3DRendering/TextureShader.hpp"
#include "Geometry/Model.hpp"
#include "Lighting/Light.hpp"
#include "System/Timer.hpp"

const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.3f;

class Application
{
public:
	Application();
	Application(const Application&);
	~Application();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render(float, float);

	std::shared_ptr<D3DHandler> m_D3DHandler;
	std::unique_ptr<Scene> m_scene;
	std::unique_ptr<Timer> m_Timer;
};
