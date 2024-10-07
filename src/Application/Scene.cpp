#include "Application/Scene.hpp"

using namespace std;
using namespace DirectX;

Scene::Scene(std::shared_ptr<D3DHandler> D3DHandler): m_D3DHandler(D3DHandler) 
{
  m_camera = nullptr;
  m_LightShader = nullptr;
  m_TextureShader = nullptr;
}

Scene::Scene(const Scene&) 
{

}

Scene::~Scene() 
{

}

bool Scene::AddBitmap(const char* bitmapFilePath)
{
  bool result = false;

  long pos = m_bitmaps.size();

  // Create and initialize the bitmap object.
  m_bitmaps.emplace_back(make_unique<Bitmap>());

  result = m_bitmaps[pos]->Initialize(m_D3DHandler->GetDevice(), m_D3DHandler->GetDeviceContext(), m_D3DHandler->GetScreenWidth(), m_D3DHandler->GetScreenHeight(), bitmapFilePath, 150, 50);
  if (!result)
  {
    return false;
  }

  return result;
}

bool Scene::AddCamera(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& rotation) 
{
  if (m_camera) {
    m_camera.reset();
    m_camera = nullptr;
  }

  m_camera = make_unique<Camera>();
  m_camera->SetPosition(XMFLOAT3(0.0f, 0.0f, -10.0f));
  m_camera->Render();
  m_camera->GetViewMatrix(m_orthoViewMatrix);
  m_camera->SetPosition(position);
  m_camera->SetRotation(rotation);
  m_camera->Render();

  return true;
}

bool Scene::AddLightSource(const DirectX::XMFLOAT3& position, const DirectX::XMFLOAT3& direction, const DirectX::XMFLOAT4& diffuseColor)
{
  long pos = m_lights.size();

  m_lights.emplace_back(make_unique<Light>());

  m_lights[pos]->SetDiffuseColor(diffuseColor);
  m_lights[pos]->SetPosition(position);
  m_lights[pos]->SetDirection(direction);

  return true;
}

bool Scene::AddModel(const char* modelFilePath, const char* textureFilePath, const ShaderType shaderType) 
{
  bool result = false;

  long pos = m_models[shaderType].size();

  // Create and initialize the model object.
  m_models[shaderType].emplace_back(make_unique<Model>());

  result = m_models[shaderType][pos]->Initialize(m_D3DHandler->GetDevice(), m_D3DHandler->GetDeviceContext(), modelFilePath, textureFilePath);

  return result;
}

bool Scene::AddSprite(const char* spriteFilePath)
{
  bool result = false;

  long pos = m_sprites.size();

  m_sprites.emplace_back(make_unique<Sprite>());

  result = m_sprites[pos]->Initialize(m_D3DHandler->GetDevice(), m_D3DHandler->GetDeviceContext(), m_D3DHandler->GetScreenWidth(), m_D3DHandler->GetScreenHeight(), spriteFilePath, 50, 50);

  return result;
}

bool Scene::Initialize(HWND hwnd) 
{
  bool result = false;
  // Create and initialize the light shader object.
  m_LightShader = make_unique<LightShader>();

  result = m_LightShader->Initialize(m_D3DHandler->GetDevice(), hwnd);
  if (!result)
  {
    MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
    return false;
  }

  // Create and initialize the texture shader object.
  m_TextureShader = make_unique<TextureShader>();

  result = m_TextureShader->Initialize(m_D3DHandler->GetDevice(), hwnd);
  if (!result)
  {
    MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
    return false;
  }

  m_models.resize(ShaderType::Count);
}

bool Scene::Render(float frameTime)
{
  XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix, rotateMatrix, translateMatrix, scaleMatrix, srMatrix;
  vector<XMFLOAT4> diffuseColors, lightPositions;

  bool result;

  Update(frameTime);

  // Clear the buffers to begin the scene.
  m_D3DHandler->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);

  // Get the world, view, and projection matrices from the camera and d3d objects.
  m_D3DHandler->GetWorldMatrix(worldMatrix);
  m_D3DHandler->GetProjectionMatrix(projectionMatrix);
  m_D3DHandler->GetOrthoMatrix(orthoMatrix);
  m_camera->GetViewMatrix(viewMatrix);

  diffuseColors.reserve(m_lights.size());
  lightPositions.reserve(m_lights.size());

  // Get the light properties.
  for (long i = 0; i < m_lights.size(); i++)
  {
    // Create the diffuse color array from the four light colors.
    diffuseColors.push_back(m_lights[i]->GetDiffuseColor());

    // Create the light position array from the four light positions.
    lightPositions.push_back(m_lights[i]->GetPosition());
  }

  vector<unique_ptr<Model>>& modelsWithLightShaderIt = m_models[ShaderType::LightShaderType];

  // Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
  for (long i = 0; i < m_models[ShaderType::LightShaderType].size(); i++)
  {
    modelsWithLightShaderIt[i]->Render(m_D3DHandler->GetDeviceContext());

    // Render the model using the light shader.
    result = m_LightShader->Render(m_D3DHandler->GetDeviceContext(), modelsWithLightShaderIt[i]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix, modelsWithLightShaderIt[i]->GetTexture(),
      diffuseColors, lightPositions);

    if (!result)
    {
      return false;
    }
  }

  // Turn off the Z buffer to begin all 2D rendering.
  m_D3DHandler->TurnZBufferOff();

  for (long i = 0; i < m_bitmaps.size(); i++)
  {
    // Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
    result = m_bitmaps[i]->Render(m_D3DHandler->GetDeviceContext());
    if (!result)
    {
      return false;
    }

    // Render the bitmap with the texture shader.
    result = m_TextureShader->Render(m_D3DHandler->GetDeviceContext(), m_bitmaps[i]->GetIndexCount(), worldMatrix, m_orthoViewMatrix, orthoMatrix, m_bitmaps[i]->GetTexture());
    if (!result)
    {
      return false;
    }
  }

  for (long i = 0; i < m_sprites.size(); i++)
  {
    // Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
    result = m_sprites[i]->Render(m_D3DHandler->GetDeviceContext());
    if (!result)
    {
      return false;
    }

    // Render the bitmap with the texture shader.
    result = m_TextureShader->Render(m_D3DHandler->GetDeviceContext(), m_sprites[i]->GetIndexCount(), worldMatrix, m_orthoViewMatrix, orthoMatrix, m_sprites[i]->GetTexture());
    if (!result)
    {
      return false;
    }
  }

  // Turn the Z buffer back on now that all 2D rendering has completed.
  m_D3DHandler->TurnZBufferOn();

  // Present the rendered scene to the screen.
  m_D3DHandler->EndScene();

  return true;
}

void Scene::Shutdown() 
{
  if (m_camera)
  {
    m_camera.reset();
    m_camera = nullptr;
  }

  for (long j = 0; j < ShaderType::Count; j++) {
    for (long i = 0; i < m_models[j].size(); i++) {
      m_models[j][i]->Shutdown();
    }
  }

  for (long i = 0; i < m_lights.size(); i++) {
    m_lights[i].reset();
  }

  // Release the light shader object.
  if (m_LightShader)
  {
    m_LightShader->Shutdown();
    m_LightShader.reset();
    m_LightShader = nullptr;
  }

  // Release the bitmap objects.
  for (long i = 0; i < m_bitmaps.size(); i++) {
    m_bitmaps[i]->Shutdown();
  }

  // Release the texture shader object.
  if (m_TextureShader)
  {
    m_TextureShader->Shutdown();
    m_TextureShader.reset();
    m_TextureShader = nullptr;
  }
}

void Scene::Update(float frameTime)
{
  for (long i = 0; i < m_sprites.size(); i++)
  {
    m_sprites[i]->Update(frameTime);
  }
}