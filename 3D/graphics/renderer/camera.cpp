#include "camera.h"

namespace ENGINE
{
	const float ROTATION_GAIN = 0.002f;
	const float MOVEMENT_GAIN = 0.07f;

	Vector3 move = Vector3::Zero;

	void Camera::update(float dt)
	{
		auto kb = keyboard->GetState();
 		if (kb.Home)
		{
			//position = Vector3::Zero;
			pitch = yaw = 0;
		}



		if (kb.Up || kb.W)
			move.z += 1.f;

		if (kb.Down || kb.S)
			move.z -= 1.f;

		if (kb.Left || kb.A)
			move.x -= 1.f;

		if (kb.Right || kb.D)
			move.x += 1.f;

		 
 
		 
		auto mouse_state = mouse->GetState();  

		if (mouse_state.positionMode == DirectX::Mouse::MODE_RELATIVE)
		{
			Vector3 delta = Vector3(float(mouse_state.x), float(mouse_state.y), 0.f)
				* ROTATION_GAIN;

			pitch += delta.y;
			yaw += delta.x;

			// limit pitch to straight up or straight down
			// with a little fudge-factor to avoid gimbal lock
			float limit = DirectX::XM_PI / 2.0f - 0.01f;
			pitch = max(-limit, pitch);
			pitch = min(+limit, pitch);

			// keep longitude in sane range by wrapping
			if (yaw > DirectX::XM_PI)
			{
				yaw -= DirectX::XM_PI * 2.0f; 
			}
			else if (yaw < -DirectX::XM_PI)
			{
				yaw += DirectX::XM_PI * 2.0f;
			}
		}

		mouse->SetMode(mouse_state.leftButton ? DirectX::Mouse::MODE_RELATIVE : DirectX::Mouse::MODE_ABSOLUTE);

		cBufferCpu.view = getView(dt);
		if (kb.Space)
			std::cout << "(" << pos.x << ", " << pos.y << ", " << pos.z << ")" << std::endl;
 	}

	void Camera::render()
	{


		Vector4 pos = getPosition();
		utils::D3D::mapResource(cBufferGpu, &cBufferCpu);
		utils::D3D::mapResource(pixelBuffer, &pos);
		utils::D3D::getContext()->VSSetConstantBuffers(0, 1, cBufferGpu.GetAddressOf());
		utils::D3D::getContext()->PSSetConstantBuffers(3, 1, pixelBuffer.GetAddressOf());
	}

	void Camera::render(const ComPtr<ID3D11Buffer>& view)
	{
		// Render with somebody else eyes

		Vector4 pos = getPosition();
		utils::D3D::mapResource(cBufferGpu, &cBufferCpu);
		utils::D3D::mapResource(pixelBuffer, &pos);
		utils::D3D::getContext()->VSSetConstantBuffers(0, 1, view.GetAddressOf());
		utils::D3D::getContext()->PSSetConstantBuffers(4, 1, pixelBuffer.GetAddressOf());
	}

	Camera::Camera()
	{

	}

	Camera::Camera(const Vector3& position, const Vector3& target, HWND handle)
	{
	
		keyboard = std::make_unique<DirectX::Keyboard>();
		mouse = std::make_unique<DirectX::Mouse>();
		mouse->SetWindow(handle);
		pos = { position.x, position.y, position.z };

		cBufferCpu.proj = getPerspective();
		 
		update(1.0f);

		Vector4 pos = getPosition();

		uint32_t size = sizeof(CameraConstant);
		utils::D3D::createBuffer(&cBufferCpu, cBufferGpu, size, size, D3D11_BIND_CONSTANT_BUFFER, true);
		utils::D3D::createBuffer(&pos, pixelBuffer, sizeof(Vector4), sizeof(Vector4), D3D11_BIND_CONSTANT_BUFFER, true);
	}

	Vector4 Camera::getPosition() const
	{
		return Vector4(pos.x, pos.y, pos.z, 1.0f);
	}


	Matrix Camera::getView(float dt)
	{  
		using namespace DirectX;
		//compute rotation matrix
		XMMATRIX mRot = XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f);

		XMVECTOR camMovement = XMVectorSet(0, 0, 0, 0);

		//forward
		float speedForward = GetAsyncKeyState('W') ? 1.0f : GetAsyncKeyState('S') ? -1.0f : 0.0f;
		XMVECTOR forward = XMVectorSet(0, 0, 1, 0);
		camMovement += XMVector3TransformNormal(forward, mRot) * speedForward;

		//strafe
		float speedStrafe = GetAsyncKeyState('D') ? 1.0f : GetAsyncKeyState('A') ? -1.0f : 0.0f;
		XMVECTOR right = XMVectorSet(1, 0, 0, 0);
		camMovement += XMVector3TransformNormal(right, mRot) * speedStrafe;

		//up
		float speedUp = GetAsyncKeyState('E') ? 1.0f : GetAsyncKeyState('Q') ? -1.0f : 0.0f;
		XMVECTOR up = XMVectorSet(0, 1, 0, 0);
		camMovement += XMVector3TransformNormal(up, mRot) * speedUp;

		//normazlie final movement vector
		camMovement = XMVector3Normalize(camMovement);

		float movementSpeed = 1.0f;
		XMVECTOR newCamPos = XMLoadFloat3(&pos) + camMovement * movementSpeed * dt;

		//store camera pos
		XMStoreFloat3(&pos, newCamPos);

		//compute translation matrix
		XMMATRIX mTrans = XMMatrixTranslation(pos.x, pos.y, pos.z);


		//calculate view matrix inv(rot * trans)
		Matrix view = XMMatrixInverse(nullptr, mRot * mTrans);

		return view.Transpose();
	}

	Matrix Camera::getPerspective()
	{  
		return DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(0.7854f,
			float(SCREEN_WIDTH) / float(SCREEN_HEIGHT), 0.0001f, 100000.f).Transpose();
	}
}