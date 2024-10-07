#pragma once

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include <d3d11.h>
#include <directxmath.h>

class D3DHandler
{
public:
  D3DHandler();
  D3DHandler(const D3DHandler&);
  ~D3DHandler();

  bool Initialize(long, long, bool, HWND, bool, float, float);
  void Shutdown();

  void BeginScene(float, float, float, float);
  void EndScene();

  ID3D11Device* GetDevice();
  ID3D11DeviceContext* GetDeviceContext();

  void GetOrthoMatrix(DirectX::XMMATRIX&);
  void GetProjectionMatrix(DirectX::XMMATRIX&);
  long GetScreenHeight();
  long GetScreenWidth();  
  void GetVideoCardInfo(char*, int&);
  void GetWorldMatrix(DirectX::XMMATRIX&);

  void ResetViewport();
  void SetBackBufferRenderTarget();  

  void TurnZBufferOn();
  void TurnZBufferOff();

private:
  bool m_vsync_enabled;
  long m_videoCardMemory, m_screenWidth, m_screenHeight;
  char m_videoCardDescription[128];
  IDXGISwapChain* m_swapChain;
  ID3D11Device* m_device;
  ID3D11DeviceContext* m_deviceContext;
  ID3D11RenderTargetView* m_renderTargetView;
  ID3D11Texture2D* m_depthStencilBuffer;
  ID3D11DepthStencilState* m_depthStencilState;
  ID3D11DepthStencilView* m_depthStencilView;
  ID3D11DepthStencilState* m_depthDisabledStencilState;
  ID3D11RasterizerState* m_rasterState;
  D3D11_VIEWPORT m_viewport;
  DirectX::XMMATRIX m_projectionMatrix;
  DirectX::XMMATRIX m_worldMatrix;
  DirectX::XMMATRIX m_orthoMatrix;
};