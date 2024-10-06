#pragma once

#include <d3d11.h>
#include <d3dcompiler.h>
#include <directxmath.h>
#include <fstream>

#define WORLD_VIEW_PROJ_MATRIX_BUFFER 0
#define CAMERA_BUFFER 1

const int NUM_LIGHTS = 4;

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

  struct LightColorBufferType
  {
    DirectX::XMFLOAT4 diffuseColor[NUM_LIGHTS];
  };

  struct LightPositionBufferType
  {
    DirectX::XMFLOAT4 lightPosition[NUM_LIGHTS];
  };

public:
  LightShader();
  LightShader(const LightShader&);
  ~LightShader();

  bool Initialize(ID3D11Device*, HWND);  
  bool Render(ID3D11DeviceContext*, int, DirectX::XMMATRIX&, DirectX::XMMATRIX&, DirectX::XMMATRIX&, ID3D11ShaderResourceView*, const DirectX::XMFLOAT4[], const DirectX::XMFLOAT4[]);
  bool Render(ID3D11DeviceContext*, int, DirectX::XMMATRIX&, DirectX::XMMATRIX&, DirectX::XMMATRIX&, ID3D11ShaderResourceView*, 
    const DirectX::XMFLOAT3&, const DirectX::XMFLOAT4&, const DirectX::XMFLOAT4&, const DirectX::XMFLOAT3&, const DirectX::XMFLOAT4&, float);
  void Shutdown();
  

private:
  bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*);
  void OutputShaderErrorMessage(ID3D10Blob*, HWND, WCHAR*);
  void RenderShader(ID3D11DeviceContext*, int);
  bool SetShaderParameters(ID3D11DeviceContext*, DirectX::XMMATRIX&, DirectX::XMMATRIX&, DirectX::XMMATRIX&, ID3D11ShaderResourceView*, const DirectX::XMFLOAT4[], const DirectX::XMFLOAT4[]);
  bool SetShaderParameters(ID3D11DeviceContext*, const DirectX::XMMATRIX&, const DirectX::XMMATRIX&, const DirectX::XMMATRIX&, ID3D11ShaderResourceView*, 
     const DirectX::XMFLOAT3&, const DirectX::XMFLOAT4&, const DirectX::XMFLOAT4&, const DirectX::XMFLOAT3&, const DirectX::XMFLOAT4&, float);
  void ShutdownShader();
  

private:
  ID3D11VertexShader* m_vertexShader;
  ID3D11PixelShader* m_pixelShader;
  ID3D11InputLayout* m_layout;
  ID3D11SamplerState* m_sampleState;
  ID3D11Buffer* m_matrixBuffer;
  ID3D11Buffer* m_lightBuffer;
  ID3D11Buffer* m_cameraBuffer;
  ID3D11Buffer* m_lightColorBuffer;
  ID3D11Buffer* m_lightPositionBuffer;
};