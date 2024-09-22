#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include <fstream>
#include <memory>
#include <vector>

#include "Texture/Texture.hpp"

class Model
{
private:

	struct VertexTypeColor
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

	struct VertexTypeTexture
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
		DirectX::XMFLOAT3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	Model();
	Model(const Model&);
	~Model();

	bool Initialize(ID3D11Device*); //simple initialize
	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*); //initialize with texture
	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, char*); //initialize with texture and model
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	long GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device*);
	bool LoadModel(char*);
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseModel();
	void ReleaseTexture();
	void RenderBuffers(ID3D11DeviceContext*);
	void ShutdownBuffers();

	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	std::unique_ptr<Texture> m_texture;
	std::vector<ModelType> m_model;
	long m_vertexCount, m_indexCount;
};