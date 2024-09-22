#pragma once

#include <directxmath.h>

class Light
{
public:
  Light();
  Light(const Light&);
  ~Light();

  void SetDiffuseColor(float, float, float, float);
  void SetDirection(float, float, float);

  DirectX::XMFLOAT4 GetDiffuseColor();
  DirectX::XMFLOAT3 GetDirection();

private:
  DirectX::XMFLOAT4 m_diffuseColor;
  DirectX::XMFLOAT3 m_direction;
};