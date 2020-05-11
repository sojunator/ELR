#ifndef __D3D_H
#define __D3D_H


#include <d3d11_4.h>

#include <d3dcompiler.h>


#pragma comment (lib, "d3d11.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "d3dCompiler.lib")
#include <atlbase.h>
#include <atlconv.h>
#include <comdef.h>
#include <wrl/client.h>
#include <DirectXMath.h>
#include <DDSTextureLoader.h>
#include <SimpleMath.h>
#include "../../common.h"


using namespace Microsoft::WRL; 

namespace ENGINE
{
	namespace utils
	{
		using namespace DirectX;
		using namespace SimpleMath;
		class D3D
		{
		private:
			static ComPtr<ID3D11Device5>  device;
			static ComPtr<ID3D11DeviceContext4> context;

		public:
			D3D() {};
			~D3D();
			D3D(HWND handle, ComPtr<IDXGISwapChain4>& pSwapChain);

			static void createViewPort(D3D11_VIEWPORT& viewport);

			static void createDebug();

			static void setDebugName(const ComPtr<ID3D11DeviceChild> child, const std::string& name);

			static void createBackBuffer(const ComPtr<IDXGISwapChain>& swapChain, ComPtr<ID3D11RenderTargetView>& rendertarget);

			static void createTexture2D(ID3D11Texture2D*& texture, uint32_t flags, bool dynamic = false,
				DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT, float height = 1904, float width = 1041);

			static void createRenderTargetView(ID3D11RenderTargetView*& rtv, ID3D11Texture2D*& texture, DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT);

			static void createShaderResourceView(ID3D11ShaderResourceView*& srv, ID3D11Texture2D*& texture, DXGI_FORMAT format = DXGI_FORMAT_R32G32B32A32_FLOAT);

			static void createBuffer(void* data, ComPtr<ID3D11Buffer>& buffer, const uint32_t& size,
				const uint32_t& stride, const uint32_t& bindflags, bool dynamic = false);

			static void CompileShader(ComPtr<ID3DBlob>& shaderBlob, const std::wstring& path, const std::string& entryPoint, const std::string& target);

			static void createVertexShader(ComPtr<ID3D11VertexShader>& vs, const std::wstring& path, ComPtr<ID3D11InputLayout>& inputLayer,
				const std::string& entryPoint = "VSmain", const std::string& target = "vs_5_0");

			static void createPixelShader(ComPtr<ID3D11PixelShader>& shader, const std::wstring& path,
				const std::string& entryPoint = "PSmain", const std::string& target = "ps_5_0");

			static void createDepthStencil(ComPtr<ID3D11Texture2D>& depthStencil,
				ComPtr<ID3D11DepthStencilState>& state, ComPtr<ID3D11DepthStencilView>& deptStencilView,
				uint32_t dim = 1,
				DXGI_FORMAT format = DXGI_FORMAT_D24_UNORM_S8_UINT,
				int32_t bind = D3D11_BIND_DEPTH_STENCIL,
				bool dynamic = false);

			static void loadTextureFromFile(const std::wstring& filename, ComPtr<ID3D11ShaderResourceView>& textureView);

			static void createRasterizerState(ComPtr<ID3D11RasterizerState>& raster, const D3D11_FILL_MODE& fillMode = D3D11_FILL_SOLID, const D3D11_CULL_MODE& cullMode = D3D11_CULL_BACK);

			static void createSampler(ComPtr<ID3D11SamplerState>& sampler, D3D11_TEXTURE_ADDRESS_MODE mode = D3D11_TEXTURE_ADDRESS_MIRROR);

			static void updateResource();

			static void createCubeMap(const std::string& filename, ComPtr<ID3D11Resource>& texture, ComPtr<ID3D11ShaderResourceView>& srv);

			static ComPtr<ID3D11DeviceContext4>& getContext();

			static ComPtr<ID3D11Device5>& getDevice();

			template <typename T>
			static void mapResource(ComPtr<ID3D11Buffer> dst, T* src)
			{
				D3D11_MAPPED_SUBRESOURCE desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.RowPitch = 0;
				desc.DepthPitch = 0;


				HRESULT hr = context->Map(dst.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &desc);
				if (FAILED(hr))
				{
					_com_error err(hr);
					LPCTSTR errMsg = err.ErrorMessage();
					std::cout << "Map resource: " << errMsg << std::endl;
				}
				else
				{
					memcpy(desc.pData, src, sizeof(T));
					context->Unmap(dst.Get(), 0);
				}
			}


		};
	}
}



#endif