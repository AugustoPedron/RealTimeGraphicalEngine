#pragma once

#include <fstream>
#include <memory>
#include <vector>
#include <directxmath.h>

#include "Texture/Texture.hpp"

class Sprite
{
private:
  struct VertexType
  {
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT2 texture;
  };

public:
  Sprite();
  Sprite(const Sprite&);
  ~Sprite();

  bool Initialize(ID3D11Device*, ID3D11DeviceContext*, long, long, const char*, int, int);
  void Shutdown();
  bool Render(ID3D11DeviceContext*);

  void Update(float);

  int GetIndexCount();
  ID3D11ShaderResourceView* GetTexture();

  void SetRenderLocation(int, int);

private:
  bool InitializeBuffers(ID3D11Device*);
  void ShutdownBuffers();
  bool UpdateBuffers(ID3D11DeviceContext*);
  void RenderBuffers(ID3D11DeviceContext*);

  bool LoadTextures(ID3D11Device*, ID3D11DeviceContext*, const char*);
  void ReleaseTextures();

private:
  ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
  std::vector<std::unique_ptr<Texture>> m_Textures;

  long m_vertexCount, m_indexCount, m_screenWidth, m_screenHeight, m_bitmapWidth, m_bitmapHeight, m_currentTexture, m_textureCount;
  int m_renderX, m_renderY, m_prevPosX, m_prevPosY;
  float m_frameTime, m_cycleTime;
};