#pragma once

#include <memory>

#include <directxmath.h>

#include "Texture/Texture.hpp"


class Bitmap
{
private:
  struct VertexType
  {
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT2 texture;
  };

public:
  Bitmap();
  Bitmap(const Bitmap&);
  ~Bitmap();

  bool Initialize(ID3D11Device*, ID3D11DeviceContext*, long, long, const char*, int, int);
  long GetIndexCount();
  ID3D11ShaderResourceView* GetTexture();
  bool Render(ID3D11DeviceContext*);
  void SetRenderLocation(int, int);
  void Shutdown();

private:
  bool InitializeBuffers(ID3D11Device*);
  bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, const char*);
  void ReleaseTexture();
  void RenderBuffers(ID3D11DeviceContext*);
  void ShutdownBuffers();
  bool UpdateBuffers(ID3D11DeviceContext*);

  ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
  long m_vertexCount, m_indexCount, m_screenWidth, m_screenHeight, m_bitmapWidth, m_bitmapHeight;
  int m_renderX, m_renderY, m_prevPosX, m_prevPosY;
  std::unique_ptr<Texture> m_Texture;
};