#include "Lighting/Light.hpp"

using namespace DirectX;
using namespace std;

Light::Light()
{
}


Light::Light(const Light& other)
{
  m_ambientColor = other.m_ambientColor;
  m_diffuseColor = other.m_diffuseColor;
  m_specularColor = other.m_specularColor;
  m_position = other.m_position;
  m_direction = other.m_direction;
  m_specularPower = other.m_specularPower;
}

Light& Light::operator=(const Light& other)
{
  m_ambientColor = other.m_ambientColor;
  m_diffuseColor = other.m_diffuseColor;
  m_specularColor = other.m_specularColor;
  m_position = other.m_position;
  m_direction = other.m_direction;
  m_specularPower = other.m_specularPower;

  return *this;
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

XMFLOAT4 Light::GetPosition()
{
  return m_position;
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

void Light::SetPosition(float x, float y, float z)
{
  m_position = XMFLOAT4(x, y, z, 1.0f);
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