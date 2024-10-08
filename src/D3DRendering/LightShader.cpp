#include "D3DRendering/LightShader.hpp"

#define USE_MULTIPLE_LIGHTS 1

using namespace DirectX;
using namespace std;

const wchar_t* light_vs_file_path = L"D:\\ProgettiPersonali\\RealTimeGraphicalEngine\\shaders\\Light.vs";
const wchar_t* light_ps_file_path = L"D:\\ProgettiPersonali\\RealTimeGraphicalEngine\\shaders\\Light.ps";
const wchar_t* multiple_lights_vs_file_path = L"D:\\ProgettiPersonali\\RealTimeGraphicalEngine\\shaders\\MultipleLights.vs";
const wchar_t* multiple_lights_ps_file_path = L"D:\\ProgettiPersonali\\RealTimeGraphicalEngine\\shaders\\MultipleLights.ps";

LightShader::LightShader()
{
  m_vertexShader = nullptr;
  m_pixelShader = nullptr;
  m_layout = nullptr;
  m_sampleState = nullptr;
  m_matrixBuffer = nullptr;
  m_lightBuffer = nullptr;
  m_cameraBuffer = nullptr;
  m_lightColorBuffer = nullptr;
  m_lightPositionBuffer = nullptr;
}

LightShader::LightShader(const LightShader& other)
{
}


LightShader::~LightShader()
{
}


bool LightShader::Initialize(ID3D11Device* device, HWND hwnd)
{
  wchar_t vsFilename[128];
  wchar_t psFilename[128];
  int error;
  bool result;

#if USE_MULTIPLE_LIGHTS == 0
  // Set the filename of the vertex shader.
  error = wcscpy_s(vsFilename, 128, light_vs_file_path);
  if (error != 0)
  {
    return false;
  }

  // Set the filename of the pixel shader.
  error = wcscpy_s(psFilename, 128, light_ps_file_path);
  if (error != 0)
  {
    return false;
  }
#else
  // Set the filename of the vertex shader.
  error = wcscpy_s(vsFilename, 128, multiple_lights_vs_file_path);
  if (error != 0)
  {
    return false;
  }

  // Set the filename of the pixel shader.
  error = wcscpy_s(psFilename, 128, multiple_lights_ps_file_path);
  if (error != 0)
  {
    return false;
  }
#endif

  // Initialize the vertex and pixel shaders.
  result = InitializeShader(device, hwnd, vsFilename, psFilename);
  if (!result)
  {
    return false;
  }

  return true;
}

bool LightShader::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* vsFilename, WCHAR* psFilename)
{
  HRESULT result;
  ID3D10Blob* errorMessage;
  ID3D10Blob* vertexShaderBuffer;
  ID3D10Blob* pixelShaderBuffer;

  D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
  unsigned int numElements;
  D3D11_SAMPLER_DESC samplerDesc;
  D3D11_BUFFER_DESC matrixBufferDesc;
#if USE_MULTIPLE_LIGHTS == 0
  D3D11_BUFFER_DESC cameraBufferDesc;
  D3D11_BUFFER_DESC lightBufferDesc;
#else
  D3D11_BUFFER_DESC lightColorBufferDesc;
  D3D11_BUFFER_DESC lightPositionBufferDesc;
#endif


  // Initialize the pointers this function will use to null.
  errorMessage = 0;
  vertexShaderBuffer = 0;
  pixelShaderBuffer = 0;

  // Compile the vertex shader code.
  result = D3DCompileFromFile(vsFilename, NULL, NULL, "LightVertexShader", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);
  if (FAILED(result))
  {
    // If the shader failed to compile it should have writen something to the error message.
    if (errorMessage)
    {
      OutputShaderErrorMessage(errorMessage, hwnd, vsFilename);
    }
    // If there was nothing in the error message then it simply could not find the shader file itself.
    else
    {
      MessageBox(hwnd, vsFilename, L"Missing Shader File", MB_OK);
    }

    return false;
  }

  // Compile the pixel shader code.
  result = D3DCompileFromFile(psFilename, NULL, NULL, "LightPixelShader", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);
  if (FAILED(result))
  {
    // If the shader failed to compile it should have writen something to the error message.
    if (errorMessage)
    {
      OutputShaderErrorMessage(errorMessage, hwnd, psFilename);
    }
    // If there was nothing in the error message then it simply could not find the file itself.
    else
    {
      MessageBox(hwnd, psFilename, L"Missing Shader File", MB_OK);
    }

    return false;
  }

  // Create the vertex shader from the buffer.
  result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), NULL, &m_vertexShader);
  if (FAILED(result))
  {
    return false;
  }

  // Create the pixel shader from the buffer.
  result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), NULL, &m_pixelShader);
  if (FAILED(result))
  {
    return false;
  }

  // Create the vertex input layout description.
  // This setup needs to match the VertexType stucture in the ModelClass and in the shader.
  polygonLayout[0].SemanticName = "POSITION";
  polygonLayout[0].SemanticIndex = 0;
  polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
  polygonLayout[0].InputSlot = 0;
  polygonLayout[0].AlignedByteOffset = 0;
  polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  polygonLayout[0].InstanceDataStepRate = 0;

  polygonLayout[1].SemanticName = "TEXCOORD";
  polygonLayout[1].SemanticIndex = 0;
  polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
  polygonLayout[1].InputSlot = 0;
  polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
  polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  polygonLayout[1].InstanceDataStepRate = 0;

  polygonLayout[2].SemanticName = "NORMAL";
  polygonLayout[2].SemanticIndex = 0;
  polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
  polygonLayout[2].InputSlot = 0;
  polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
  polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
  polygonLayout[2].InstanceDataStepRate = 0;

  // Get a count of the elements in the layout.
  numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);

  // Create the vertex input layout.
  result = device->CreateInputLayout(polygonLayout, numElements, vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(),
    &m_layout);
  if (FAILED(result))
  {
    return false;
  }

  // Release the vertex shader buffer and pixel shader buffer since they are no longer needed.
  vertexShaderBuffer->Release();
  vertexShaderBuffer = 0;

  pixelShaderBuffer->Release();
  pixelShaderBuffer = 0;

  // Create a texture sampler state description.
  samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
  samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
  samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
  samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
  samplerDesc.MipLODBias = 0.0f;
  samplerDesc.MaxAnisotropy = 1;
  samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
  samplerDesc.BorderColor[0] = 0;
  samplerDesc.BorderColor[1] = 0;
  samplerDesc.BorderColor[2] = 0;
  samplerDesc.BorderColor[3] = 0;
  samplerDesc.MinLOD = 0;
  samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

  // Create the texture sampler state.
  result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
  if (FAILED(result))
  {
    return false;
  }

  // Setup the description of the dynamic matrix constant buffer that is in the vertex shader.
  matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
  matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  matrixBufferDesc.MiscFlags = 0;
  matrixBufferDesc.StructureByteStride = 0;

  // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
  result = device->CreateBuffer(&matrixBufferDesc, NULL, &m_matrixBuffer);
  if (FAILED(result))
  {
    return false;
  }

#if USE_MULTIPLE_LIGHTS == 0
  // Setup the description of the camera dynamic constant buffer that is in the vertex shader.
  cameraBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  cameraBufferDesc.ByteWidth = sizeof(CameraBufferType);
  cameraBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  cameraBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  cameraBufferDesc.MiscFlags = 0;
  cameraBufferDesc.StructureByteStride = 0;

  // Create the camera constant buffer pointer so we can access the vertex shader constant buffer from within this class.
  result = device->CreateBuffer(&cameraBufferDesc, NULL, &m_cameraBuffer);
  if (FAILED(result))
  {
    return false;
  }

  // Setup the description of the light dynamic constant buffer that is in the pixel shader.
  // Note that ByteWidth always needs to be a multiple of 16 if using D3D11_BIND_CONSTANT_BUFFER or CreateBuffer will fail.
  lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  lightBufferDesc.ByteWidth = sizeof(LightBufferType);
  lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  lightBufferDesc.MiscFlags = 0;
  lightBufferDesc.StructureByteStride = 0;

  // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
  result = device->CreateBuffer(&lightBufferDesc, NULL, &m_lightBuffer);
  if (FAILED(result))
  {
    return false;
  }
#else
  // Setup the description of the dynamic constant buffer that is in the pixel shader.
  lightColorBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  lightColorBufferDesc.ByteWidth = sizeof(LightColorBufferType);
  lightColorBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  lightColorBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  lightColorBufferDesc.MiscFlags = 0;
  lightColorBufferDesc.StructureByteStride = 0;

  // Create the constant buffer pointer so we can access the pixel shader constant buffer from within this class.
  result = device->CreateBuffer(&lightColorBufferDesc, NULL, &m_lightColorBuffer);
  if (FAILED(result))
  {
    return false;
  }

  // Setup the description of the dynamic constant buffer that is in the vertex shader.
  lightPositionBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
  lightPositionBufferDesc.ByteWidth = sizeof(LightPositionBufferType);
  lightPositionBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
  lightPositionBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
  lightPositionBufferDesc.MiscFlags = 0;
  lightPositionBufferDesc.StructureByteStride = 0;

  // Create the constant buffer pointer so we can access the vertex shader constant buffer from within this class.
  result = device->CreateBuffer(&lightPositionBufferDesc, NULL, &m_lightPositionBuffer);
  if (FAILED(result))
  {
    return false;
  }
#endif

  return true;
}

void LightShader::OutputShaderErrorMessage(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
  char* compileErrors;
  unsigned __int64 bufferSize, i;
  ofstream fout;


  // Get a pointer to the error message text buffer.
  compileErrors = (char*)(errorMessage->GetBufferPointer());

  // Get the length of the message.
  bufferSize = errorMessage->GetBufferSize();

  // Open a file to write the error message to.
  fout.open("shader-error.txt");

  // Write out the error message.
  for (i = 0; i < bufferSize; i++)
  {
    fout << compileErrors[i];
  }

  // Close the file.
  fout.close();

  // Release the error message.
  errorMessage->Release();
  errorMessage = 0;

  // Pop a message up on the screen to notify the user to check the text file for compile errors.
  MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

  return;
}

bool LightShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix,
  ID3D11ShaderResourceView* texture, const vector<XMFLOAT4>& diffuseColor, const vector<XMFLOAT4>& lightPosition)
{
  bool result;

  // Set the shader parameters that it will use for rendering.
  result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture, diffuseColor, lightPosition);
  if (!result)
  {
    return false;
  }

  // Now render the prepared buffers with the shader.
  RenderShader(deviceContext, indexCount);

  return true;
}

bool LightShader::Render(ID3D11DeviceContext* deviceContext, int indexCount, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix,
  ID3D11ShaderResourceView* texture, const XMFLOAT3& lightDirection, const XMFLOAT4& ambientColor, const XMFLOAT4& diffuseColor,
  const XMFLOAT3& cameraPosition, const XMFLOAT4& specularColor, float specularPower)
{
  bool result;

  // Set the shader parameters that it will use for rendering.
  result = SetShaderParameters(deviceContext, worldMatrix, viewMatrix, projectionMatrix, texture, lightDirection, ambientColor, diffuseColor, cameraPosition,
    specularColor, specularPower);
  if (!result)
  {
    return false;
  }

  // Now render the prepared buffers with the shader.
  RenderShader(deviceContext, indexCount);

  return true;
}

void LightShader::RenderShader(ID3D11DeviceContext* deviceContext, int indexCount)
{
  // Set the vertex input layout.
  deviceContext->IASetInputLayout(m_layout);

  // Set the vertex and pixel shaders that will be used to render this triangle.
  deviceContext->VSSetShader(m_vertexShader, NULL, 0);
  deviceContext->PSSetShader(m_pixelShader, NULL, 0);

  // Set the sampler state in the pixel shader.
  deviceContext->PSSetSamplers(0, 1, &m_sampleState);

  // Render the triangle.
  deviceContext->DrawIndexed(indexCount, 0, 0);

  return;
}

bool LightShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, XMMATRIX& worldMatrix, XMMATRIX& viewMatrix, XMMATRIX& projectionMatrix,
  ID3D11ShaderResourceView* texture, const vector<XMFLOAT4>& diffuseColor, const vector<XMFLOAT4>& lightPosition)
{
  HRESULT result;
  D3D11_MAPPED_SUBRESOURCE mappedResource;
  unsigned int bufferNumber;
  MatrixBufferType* dataPtr;
  LightPositionBufferType* dataPtr2;
  LightColorBufferType* dataPtr3;


  // Transpose the matrices to prepare them for the shader.
  worldMatrix = XMMatrixTranspose(worldMatrix);
  viewMatrix = XMMatrixTranspose(viewMatrix);
  projectionMatrix = XMMatrixTranspose(projectionMatrix);

  // Lock the constant buffer so it can be written to.
  result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
  if (FAILED(result))
  {
    return false;
  }

  // Get a pointer to the data in the constant buffer.
  dataPtr = (MatrixBufferType*)mappedResource.pData;

  // Copy the matrices into the constant buffer.
  dataPtr->world = worldMatrix;
  dataPtr->view = viewMatrix;
  dataPtr->projection = projectionMatrix;

  // Unlock the constant buffer.
  deviceContext->Unmap(m_matrixBuffer, 0);

  // Set the position of the constant buffer in the vertex shader.
  bufferNumber = 0;

  // Now set the constant buffer in the vertex shader with the updated values.
  deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

  // Lock the light position constant buffer so it can be written to.
  result = deviceContext->Map(m_lightPositionBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
  if (FAILED(result))
  {
    return false;
  }

  // Get a pointer to the data in the constant buffer.
  dataPtr2 = (LightPositionBufferType*)mappedResource.pData;

  // Copy the light position variables into the constant buffer.
  dataPtr2->lightPosition[0] = lightPosition[0];
  dataPtr2->lightPosition[1] = lightPosition[1];
  dataPtr2->lightPosition[2] = lightPosition[2];
  dataPtr2->lightPosition[3] = lightPosition[3];

  // Unlock the constant buffer.
  deviceContext->Unmap(m_lightPositionBuffer, 0);

  // Set the position of the constant buffer in the vertex shader.
  bufferNumber = 1;

  // Finally set the constant buffer in the vertex shader with the updated values.
  deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_lightPositionBuffer);

  // Set shader texture resource in the pixel shader.
  deviceContext->PSSetShaderResources(0, 1, &texture);

  // Lock the light color constant buffer so it can be written to.
  result = deviceContext->Map(m_lightColorBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
  if (FAILED(result))
  {
    return false;
  }

  // Get a pointer to the data in the constant buffer.
  dataPtr3 = (LightColorBufferType*)mappedResource.pData;

  // Copy the light color variables into the constant buffer.
  dataPtr3->diffuseColor[0] = diffuseColor[0];
  dataPtr3->diffuseColor[1] = diffuseColor[1];
  dataPtr3->diffuseColor[2] = diffuseColor[2];
  dataPtr3->diffuseColor[3] = diffuseColor[3];

  // Unlock the constant buffer.
  deviceContext->Unmap(m_lightColorBuffer, 0);

  // Set the position of the constant buffer in the pixel shader.
  bufferNumber = 0;

  // Finally set the constant buffer in the pixel shader with the updated values.
  deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightColorBuffer);

  return true;
}

bool LightShader::SetShaderParameters(ID3D11DeviceContext* deviceContext, const XMMATRIX& worldMatrix, const XMMATRIX& viewMatrix, const XMMATRIX& projectionMatrix,
  ID3D11ShaderResourceView* texture, const XMFLOAT3& lightDirection, const XMFLOAT4& ambientColor, const XMFLOAT4& diffuseColor,
  const XMFLOAT3& cameraPosition, const XMFLOAT4& specularColor, float specularPower)
{
  HRESULT result;
  D3D11_MAPPED_SUBRESOURCE mappedResource;
  unsigned int bufferNumber;
  MatrixBufferType* dataPtr;
  LightBufferType* dataPtr2;
  CameraBufferType* dataPtr3;

  // Lock the constant buffer so it can be written to.
  result = deviceContext->Map(m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
  if (FAILED(result))
  {
    return false;
  }

  // Get a pointer to the data in the constant buffer.
  dataPtr = (MatrixBufferType*)mappedResource.pData;

  // Transpose and copy the matrices into the constant buffer.
  dataPtr->world = XMMatrixTranspose(worldMatrix);
  dataPtr->view = XMMatrixTranspose(viewMatrix);
  dataPtr->projection = XMMatrixTranspose(projectionMatrix);

  // Unlock the constant buffer.
  deviceContext->Unmap(m_matrixBuffer, 0);

  // Set the position of the constant buffer in the vertex shader.
  bufferNumber = WORLD_VIEW_PROJ_MATRIX_BUFFER;

  // Now set the constant buffer in the vertex shader with the updated values.
  deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_matrixBuffer);

  // Lock the camera constant buffer so it can be written to.
  result = deviceContext->Map(m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
  if (FAILED(result))
  {
    return false;
  }

  // Get a pointer to the data in the constant buffer.
  dataPtr3 = (CameraBufferType*)mappedResource.pData;

  // Copy the camera position into the constant buffer.
  dataPtr3->cameraPosition = cameraPosition;
  dataPtr3->padding = 0.0f;

  // Unlock the camera constant buffer.
  deviceContext->Unmap(m_cameraBuffer, 0);

  // Set the position of the camera constant buffer in the vertex shader.
  bufferNumber = CAMERA_BUFFER;

  // Now set the camera constant buffer in the vertex shader with the updated values.
  deviceContext->VSSetConstantBuffers(bufferNumber, 1, &m_cameraBuffer);

  // Set shader texture resource in the pixel shader.
  deviceContext->PSSetShaderResources(0, 1, &texture);

  // Lock the light constant buffer so it can be written to.
  result = deviceContext->Map(m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
  if (FAILED(result))
  {
    return false;
  }

  // Get a pointer to the data in the constant buffer.
  dataPtr2 = (LightBufferType*)mappedResource.pData;

  // Copy the lighting variables into the constant buffer.
  dataPtr2->ambientColor = ambientColor;
  dataPtr2->diffuseColor = diffuseColor;
  dataPtr2->lightDirection = lightDirection;
  dataPtr2->specularColor = specularColor;
  dataPtr2->specularPower = specularPower;

  // Unlock the constant buffer.
  deviceContext->Unmap(m_lightBuffer, 0);

  // Set the position of the light constant buffer in the pixel shader.
  bufferNumber = 0;

  // Finally set the light constant buffer in the pixel shader with the updated values.
  deviceContext->PSSetConstantBuffers(bufferNumber, 1, &m_lightBuffer);

  return true;
}

void LightShader::Shutdown()
{
  // Shutdown the vertex and pixel shaders as well as the related objects.
  ShutdownShader();

  return;
}

void LightShader::ShutdownShader()
{
  // Release the light constant buffers.
  if (m_lightColorBuffer)
  {
    m_lightColorBuffer->Release();
    m_lightColorBuffer = nullptr;
  }

  if (m_lightPositionBuffer)
  {
    m_lightPositionBuffer->Release();
    m_lightPositionBuffer = nullptr;
  }

  // Release the camera constant buffer.
  if (m_cameraBuffer)
  {
    m_cameraBuffer->Release();
    m_cameraBuffer = nullptr;
  }

  // Release the light constant buffer.
  if (m_lightBuffer)
  {
    m_lightBuffer->Release();
    m_lightBuffer = nullptr;
  }

  // Release the matrix constant buffer.
  if (m_matrixBuffer)
  {
    m_matrixBuffer->Release();
    m_matrixBuffer = nullptr;
  }

  // Release the sampler state.
  if (m_sampleState)
  {
    m_sampleState->Release();
    m_sampleState = nullptr;
  }

  // Release the layout.
  if (m_layout)
  {
    m_layout->Release();
    m_layout = nullptr;
  }

  // Release the pixel shader.
  if (m_pixelShader)
  {
    m_pixelShader->Release();
    m_pixelShader = nullptr;
  }

  // Release the vertex shader.
  if (m_vertexShader)
  {
    m_vertexShader->Release();
    m_vertexShader = nullptr;
  }

  return;
}
