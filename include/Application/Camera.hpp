#pragma once

#include <d3d11.h>
#include <directxmath.h>

class Camera
{
public:
	Camera();
	Camera(const Camera&);
	~Camera();

	void SetPosition(const DirectX::XMFLOAT3&);
	void SetRotation(const DirectX::XMFLOAT3&);

	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();

	void Render();
	void GetViewMatrix(DirectX::XMMATRIX&);

private:
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_rotation;
	DirectX::XMMATRIX m_viewMatrix;
};