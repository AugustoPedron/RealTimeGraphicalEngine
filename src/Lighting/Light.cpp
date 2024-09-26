#include "Lighting/Light.hpp"

using namespace DirectX;
using namespace std;

Light::Light()
{
}


Light::Light(const Light& other)
{
}


Light::~Light()
{
}

XMFLOAT4 Light::GetAmbientColor()
{
  return m_ambientColor;
}

XMFLOAT4 Light::GetDiffuseColor()
{
  return m_diffuseColor;
}


XMFLOAT3 Light::GetDirection()
{
  return m_direction;
}

XMFLOAT4 Light::GetSpecularColor()
{
  return m_specularColor;
}


float Light::GetSpecularPower()
{
  return m_specularPower;
}

void Light::SetAmbientColor(float red, float green, float blue, float alpha)
{
  m_ambientColor = XMFLOAT4(red, green, blue, alpha);
  return;
}

void Light::SetDiffuseColor(float red, float green, float blue, float alpha)
{
  m_diffuseColor = XMFLOAT4(red, green, blue, alpha);
  return;
}


void Light::SetDirection(float x, float y, float z)
{
  m_direction = XMFLOAT3(x, y, z);
  return;
}

void Light::SetSpecularColor(float red, float green, float blue, float alpha)
{
  m_specularColor = XMFLOAT4(red, green, blue, alpha);
  return;
}


void Light::SetSpecularPower(float power)
{
  m_specularPower = power;
  return;
}