#ifndef __CAMERA__H
#define __CAMERA__H

#include "utils/D3D/d3d.h"
#include "common.h"

#include "SimpleMath.h"
 
#include "Keyboard.h" 
#include "Mouse.h" 

namespace ENGINE
{
	using namespace DirectX::SimpleMath;
	class Camera
	{
	public:
		struct CameraConstant
		{
			DirectX::SimpleMath::Matrix view;
			DirectX::SimpleMath::Matrix proj;
		};


	private:
		DirectX::XMFLOAT3 pos;
		//DirectX::SimpleMath::Vector3 target;
		float pitch = 0.0f;
		float yaw = 0.0f;

		CameraConstant cBufferCpu;
		ComPtr<ID3D11Buffer> cBufferGpu;
		ComPtr<ID3D11Buffer> viewPos;
		ComPtr<ID3D11Buffer> pixelBuffer;

		std::unique_ptr<DirectX::Keyboard> keyboard;
		std::unique_ptr<DirectX::Mouse> mouse;
	public: 
		void update(float dt);
		void render();
		void render(const ComPtr<ID3D11Buffer>& view);
		Camera(); 
		Camera(const Vector3& position, const Vector3& target, HWND handle);

		Vector4 getPosition() const;
		
		Matrix getView(float dt);
		Matrix getPerspective();
	private:

	};
}

#endif
