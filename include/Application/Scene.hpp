#pragma once

#include <memory>

#include <d3d11.h>
#include <directxmath.h>

#include "Application/Camera.hpp"
#include "D3DRendering/D3DHandler.hpp"
#include "D3DRendering/LightShader.hpp"
#include "D3DRendering/TextureShader.hpp"
#include "Geometry/Model.hpp"
#include "Lighting/Light.hpp"
#include "Texture/BitMap.hpp"
#include "Texture/Sprite.hpp"
#include "Texture/Texture.hpp"

enum ShaderType {
	ColorShaderType,
	TextureShaderType,
	LightShaderType,
	MultipleLightsShaderType,
	Count
};

class Scene {
public:
	Scene(std::shared_ptr<D3DHandler>);
	Scene(const Scene&);
	~Scene();

	bool AddBitmap(const char* bitmapFilePath);

	bool AddCamera(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& rotation);

	bool AddLightSource(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT4& diffuseColor);
	
	bool AddModel(const char* modelFilePath, const char* textureFilePath, const ShaderType);

	bool AddSprite(const char* spriteFilePath);

	bool Initialize(HWND hwnd);

	bool Render(float frameTime);

	void Shutdown();

	void Update(float frameTime);

private:
	DirectX::XMMATRIX m_orthoViewMatrix;
	std::shared_ptr<D3DHandler> m_D3DHandler;
	std::unique_ptr<Camera> m_camera;
	std::unique_ptr<LightShader> m_LightShader;
	std::unique_ptr<TextureShader> m_TextureShader;
	std::vector<std::unique_ptr<Bitmap>> m_bitmaps;
	std::vector<std::vector<std::unique_ptr<Model>>> m_models;
	std::vector<std::unique_ptr<Sprite>> m_sprites;
	std::vector<std::unique_ptr<Light>> m_lights;
};