#pragma once

#include <d3d11.h>
#include <directxmath.h>
#include <memory>

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

public:
	Model();
	Model(const Model&);
	~Model();

	bool Initialize(ID3D11Device*);
	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device*);
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();
	void RenderBuffers(ID3D11DeviceContext*);
	void ShutdownBuffers();

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	std::unique_ptr<Texture> m_texture;
	int m_vertexCount, m_indexCount;
};