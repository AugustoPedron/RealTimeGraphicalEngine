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

void Light::SetAmbientColor(const XMFLOAT4& ambientColor)
{
  m_ambientColor = ambientColor;
  return;
}

void Light::SetDiffuseColor(const XMFLOAT4& diffuseColor)
{
  m_diffuseColor = diffuseColor;
  return;
}


void Light::SetDirection(const XMFLOAT3& direction)
{
  m_direction = direction;
  return;
}

void Light::SetPosition(const XMFLOAT3& position)
{
  m_position = XMFLOAT4(position.x, position.y, position.z, 1.0f);
  return;
}

void Light::SetSpecularColor(const XMFLOAT4& specularColor)
{
  m_specularColor = specularColor;
  return;
}


void Light::SetSpecularPower(float power)
{
  m_specularPower = power;
  return;
}