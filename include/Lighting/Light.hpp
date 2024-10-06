#pragma once

#include <directxmath.h>

class Light
{
public:
  Light();
  Light(const Light&);
  ~Light() = default;

  Light& operator=(const Light&);

  DirectX::XMFLOAT4 GetAmbientColor();
  DirectX::XMFLOAT4 GetDiffuseColor();
  DirectX::XMFLOAT3 GetDirection();
  DirectX::XMFLOAT4 GetPosition();
  DirectX::XMFLOAT4 GetSpecularColor();
  float GetSpecularPower();

  void SetAmbientColor(float, float, float, float);
  void SetDiffuseColor(float, float, float, float);
  void SetDirection(float, float, float);
  void SetPosition(float, float, float);
  void SetSpecularColor(float, float, float, float);
  void SetSpecularPower(float);  

private:
  DirectX::XMFLOAT4 m_ambientColor;
  DirectX::XMFLOAT4 m_diffuseColor;
  DirectX::XMFLOAT4 m_specularColor;
  DirectX::XMFLOAT4 m_position;
  DirectX::XMFLOAT3 m_direction;
  float m_specularPower;
};