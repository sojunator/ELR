#ifndef __RENDERER_H
#define __RENDERER_H
#include <wrl/client.h>
#include <DirectXMath.h>

#include "utils/D3D/d3d.h"
#include "common.h"
#include "camera.h"
#include "shader.h" 
#include "LightManager.h"
#include "utils/primitives/Primtive.h"

using namespace Microsoft::WRL;

namespace ENGINE
{
	class Renderer
	{
	private:
		utils::D3D d3d;

		ComPtr<IDXGISwapChain4> swapChain;
		ComPtr<ID3D11Debug> debug;
		ComPtr<ID3D11RenderTargetView> backBuffer;


		// geopass
		ComPtr<ID3D11DepthStencilState> state;
		ComPtr<ID3D11DepthStencilView> stencilView;
		ComPtr<ID3D11Texture2D> depthStencil;

		// deferred
		std::vector<ID3D11RenderTargetView*> g_RTV;
		std::vector<ID3D11ShaderResourceView*> g_SRV;
		std::vector<ID3D11Texture2D*> g_tex;

		Skybox skybox;
		Camera camera;
		D3D11_VIEWPORT viewport;

		Shader deferred;
		std::map<std::string, Shader> shaders;
		LightManager lights;

		// TODO: This should be moved into it's own class
		Light light;
		Plane test;

		 
		DWORD oldTime = 0.0f;

	public: 
		Renderer(HWND handle); 
		void render();

	private:
		void createGbuffer();
	};
}

#endif
