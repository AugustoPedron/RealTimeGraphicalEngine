#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>

#define WORLD_VIEW_PROJ_MATRIX_BUFFER 0
#define CAMERA_BUFFER 1

class LightShader
{
private:
  struct MatrixBufferType
  {
    DirectX::XMMATRIX world;
    DirectX::XMMATRIX view;
    DirectX::XMMATRIX projection;
  };

  struct CameraBufferType
  {
    DirectX::XMFLOAT3 cameraPosition;
    float padding;
  };

  struct LightBufferType
  {
    DirectX::XMFLOAT4 ambientColor;
    DirectX::XMFLOAT4 diffuseColor;
    DirectX::XMFLOAT3 lightDirection;
    float specularPower;
    DirectX::XMFLOAT4 specularColor;
  };

public:
  LightShader();
  LightShader(const LightShader&);
  ~LightShader();

  bool Initialize(ID3D11Device*, HWND);
  void Shutdown();
  bool Render(ID3D11DeviceContext*, int, DirectX::XMMATRIX&, DirectX::XMMATRIX&, DirectX::XMMATRIX&, ID3D11ShaderResourceView*, 
    const DirectX::XMFLOAT3&, const DirectX::XMFLOAT4&, const DirectX::XMFLOAT4&, const DirectX::XMFLOAT3&, const DirectX::XMFLOAT4&, float);

private:
  bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
  void ShutdownShader();
  void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);

  bool SetShaderParameters(ID3D11DeviceContext*, const DirectX::XMMATRIX&, const DirectX::XMMATRIX&, const DirectX::XMMATRIX&, ID3D11ShaderResourceView*, 
     const DirectX::XMFLOAT3&, const DirectX::XMFLOAT4&, const DirectX::XMFLOAT4&, const DirectX::XMFLOAT3&, const DirectX::XMFLOAT4&, float);
  void RenderShader(ID3D11DeviceContext*, int);

private:
  ID3D11VertexShader* m_vertexShader;
  ID3D11PixelShader* m_pixelShader;
  ID3D11InputLayout* m_layout;
  ID3D11SamplerState* m_sampleState;
  ID3D11Buffer* m_matrixBuffer;
  ID3D11Buffer* m_lightBuffer;
  ID3D11Buffer* m_cameraBuffer;
};