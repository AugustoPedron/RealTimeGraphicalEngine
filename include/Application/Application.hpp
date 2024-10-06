#pragma once

#include <memory>

#include "Application/Camera.hpp"
#include "D3DRendering/ColorShader.hpp"
#include "D3DRendering/D3DHandler.hpp"
#include "D3DRendering/LightShader.hpp"
#include "D3DRendering/TextureShader.hpp"
#include "Geometry/Model.hpp"
#include "Lighting/Light.hpp"

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
	bool Render(float);

	std::unique_ptr<D3DHandler> m_D3DHandler;
	std::unique_ptr<Camera> m_Camera;
	std::unique_ptr<Model> m_Model;
	std::unique_ptr<ColorShader> m_ColorShader;
	std::unique_ptr<TextureShader> m_TextureShader;
	std::unique_ptr<LightShader> m_LightShader;
	std::unique_ptr<Light> m_Light;
	std::vector<std::shared_ptr<Light>> m_Lights;
	int m_numLights;
};
