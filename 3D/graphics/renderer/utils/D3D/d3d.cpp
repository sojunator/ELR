#include "d3d.h"
#include <stdint.h>
#include <WICTextureLoader.h>
 
#include "../../renderer.h"

namespace ENGINE
{
	namespace utils
	{

		ComPtr<ID3D11DeviceContext4> D3D::context;
		ComPtr<ID3D11Device5> D3D::device;

		D3D::D3D(HWND handle, ComPtr<IDXGISwapChain4>& pSwapChain)
		{
			if (handle != 0)
			{
			}

			uint32_t deviceFlags = 0;

#if defined(DEBUG) || defined(_DEBUG)
			deviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

			HRESULT hr;

			D3D_FEATURE_LEVEL levels[] =
			{
				D3D_FEATURE_LEVEL_11_1,
				D3D_FEATURE_LEVEL_11_0,
				D3D_FEATURE_LEVEL_9_1,
				D3D_FEATURE_LEVEL_9_2,
				D3D_FEATURE_LEVEL_9_3,
				D3D_FEATURE_LEVEL_10_0,
				D3D_FEATURE_LEVEL_10_1
			};


			D3D_FEATURE_LEVEL supported;

			// Create device for level 1
			ComPtr<ID3D11Device> temp_device;
			ComPtr<ID3D11DeviceContext> temp_context;
			ComPtr<IDXGISwapChain> temp_swapChain;

			hr = D3D11CreateDevice(nullptr,
				D3D_DRIVER_TYPE_HARDWARE,
				0,
				deviceFlags,
				levels,
				7,
				D3D11_SDK_VERSION,
				temp_device.GetAddressOf(),
				&supported,
				temp_context.GetAddressOf()
			);


			if (FAILED(hr))
			{
				MessageBox(NULL, L"Could not create Context", L"Fatal error", MB_OK);
			}


			DXGI_SWAP_CHAIN_DESC desc;
			ZeroMemory(&desc, sizeof(DXGI_SWAP_CHAIN_DESC));
			desc.Windowed = TRUE; // Sets the initial state of full-screen mode.
			desc.BufferCount = 2;
			desc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
			desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			desc.SampleDesc.Count = 1;      //multisampling setting
			desc.SampleDesc.Quality = 0;    //vendor-specific flag
			desc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
			desc.OutputWindow = handle;

			ComPtr<IDXGIDevice3>  pDXGIDevice;
			hr = (temp_device)->QueryInterface(__uuidof(IDXGIDevice3), (void**)&pDXGIDevice);

			if (FAILED(hr))
			{

				_com_error err(hr);
				LPCTSTR errMsg = err.ErrorMessage();
				std::cout << "Failed to create DXGI device: " << errMsg << std::endl;
			}

			// Create swap chain.
			ComPtr<IDXGIAdapter> adapter = 0;
			ComPtr<IDXGIFactory> factory = 0;

			hr = pDXGIDevice->GetAdapter(&adapter);

			if (SUCCEEDED(hr))
			{
				adapter->GetParent(IID_PPV_ARGS(&factory));

				hr = factory->CreateSwapChain(
					temp_device.Get(),
					&desc,
					temp_swapChain.GetAddressOf()
				);

				if (FAILED(hr))
				{
					_com_error err(hr);
					LPCTSTR errMsg = err.ErrorMessage();
					std::cout << "Could not create swapchain: " << errMsg << std::endl;
				}
			}

			// There should be some sort of fail check here, but we _know_ that it will always work
			// So who gives a shit?
			hr = temp_device->QueryInterface(__uuidof(ID3D11Device5), (void**)(device.GetAddressOf()));

			if (FAILED(hr))
			{
				std::cout << "Could not create level 5" << std::endl;
			}
			else
			{
				hr = temp_context->QueryInterface(__uuidof(ID3D11DeviceContext4), (void**)(context.GetAddressOf()));

				hr = temp_swapChain->QueryInterface(__uuidof(IDXGISwapChain4), (void**)(pSwapChain.GetAddressOf()));
			}



#if defined(DEBUG) || defined(_DEBUG)
			utils::D3D::createDebug();
#endif

		}

		D3D::~D3D()
		{

		}



		void D3D::createViewPort(D3D11_VIEWPORT& viewport)
		{
			// Viewport 
			ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));
			viewport.TopLeftX = 0;
			viewport.TopLeftY = 0;
			viewport.Width = SCREEN_WIDTH;
			viewport.Height = SCREEN_HEIGHT;
			viewport.MinDepth = 0.0f;
			viewport.MaxDepth = 1.0f;
		}

		void D3D::createDebug()
		{
			ComPtr<ID3D11Debug> pDebug;
			HRESULT hr = device->QueryInterface(IID_PPV_ARGS(pDebug.GetAddressOf()));

			if (FAILED(hr))
			{
				_com_error err(hr);
				LPCTSTR errMsg = err.ErrorMessage();
				std::cout << "Could not create debug device: " << errMsg << std::endl;
			}
			else
			{
				pDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
				pDebug = nullptr;
			}

		}

		void D3D::setDebugName(const ComPtr<ID3D11DeviceChild> child, const std::string& name)
		{
			if (child != nullptr && name != "")
				child->SetPrivateData(WKPDID_D3DDebugObjectName, name.size(), name.c_str());
		}

		void D3D::createBackBuffer(const ComPtr<IDXGISwapChain>& swapChain, ComPtr<ID3D11RenderTargetView>& rendertarget)
		{

			ComPtr<ID3D11Texture2D> backbuffer;
			swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), &backbuffer);


			// create a render target pointing to the back buffer
			HRESULT hr = device->CreateRenderTargetView(backbuffer.Get(), nullptr, &rendertarget);
			if (FAILED(hr))
			{
				_com_error err(hr);
				LPCTSTR errMsg = err.ErrorMessage();
				std::cout << "Failed to create back buffer render target: " << errMsg << std::endl;
			}
		}



		void D3D::createBuffer(void* data, ComPtr<ID3D11Buffer>& buffer, const uint32_t& size, const uint32_t& stride, const uint32_t& bindflags, bool dynamic)
		{

			D3D11_BUFFER_DESC bufferDesc;
			ZeroMemory(&bufferDesc, sizeof(bufferDesc));

			bufferDesc.ByteWidth = size;
			bufferDesc.StructureByteStride = stride;
			bufferDesc.MiscFlags = 0;
			bufferDesc.BindFlags = bindflags;


			if (data == nullptr || dynamic)
			{
				bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
				bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			}
			else
			{
				bufferDesc.CPUAccessFlags = 0;
				bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
			}

			D3D11_SUBRESOURCE_DATA initData;
			initData.pSysMem = data;
			initData.SysMemPitch = 0;
			initData.SysMemSlicePitch = 0;


			HRESULT hr = device->CreateBuffer(&bufferDesc, &initData, buffer.GetAddressOf());

			if (FAILED(hr))
			{
				_com_error err(hr);
				LPCTSTR errMsg = err.ErrorMessage();
				std::cout << "Could not create Buffer: " << errMsg << std::endl;
			}
		}

		void D3D::CompileShader(ComPtr<ID3DBlob>& shaderBlob, const std::wstring& path, const std::string& entryPoint, const std::string& target)
		{
			uint32_t flags = 0;

#if defined(DEBUG) || defined(_DEBUG)
			flags |= 0x1;
#endif
			flags |= (1 << 15);


			ComPtr<ID3DBlob> errorBlob = nullptr;
			HRESULT hr = D3DCompileFromFile(path.c_str(), 0, 0, entryPoint.c_str(), target.c_str(), flags, 0, shaderBlob.GetAddressOf(), errorBlob.GetAddressOf());

			if (FAILED(hr))
			{
				if (errorBlob)
				{
					std::cout << "Failed to compile: " << (char*)_bstr_t(path.c_str()) << std::endl;
					std::cout << (char*)errorBlob->GetBufferPointer() << std::endl;
					errorBlob->Release();
				}
				else
				{
					_com_error err(hr);
					LPCTSTR errMsg = err.ErrorMessage();
					std::cout << "Could not compile shader: " << errMsg << std::endl;
				}
			}
		}

		void D3D::createPixelShader(ComPtr<ID3D11PixelShader>& shader, const std::wstring& path,
			const std::string& entryPoint, const std::string& target)
		{
			ComPtr<ID3DBlob> compiledShader;

			utils::D3D::CompileShader(compiledShader, path, entryPoint, target);
			HRESULT hr = device->CreatePixelShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), nullptr, &shader);


			if (FAILED(hr))
			{
				_com_error err(hr);
				LPCTSTR errMsg = err.ErrorMessage();
				std::cout << "Failed to create pixel shader: " << errMsg << std::endl;
			}

		}

		void D3D::createDepthStencil(ComPtr<ID3D11Texture2D>& depthStencil, ComPtr<ID3D11DepthStencilState>& state,
			ComPtr<ID3D11DepthStencilView>& deptStencilView,
			uint32_t dim,
			DXGI_FORMAT format,
			int32_t bind,
			bool dynamic)
		{
			D3D11_TEXTURE2D_DESC descDepth;
			descDepth.Width = 1904 * dim;
			descDepth.Height = 1041 * dim;
			descDepth.MipLevels = 1;
			descDepth.ArraySize = 1;
			descDepth.Format = format;
			descDepth.SampleDesc.Count = 1;
			descDepth.SampleDesc.Quality = 0;

			descDepth.BindFlags = bind;
			descDepth.MiscFlags = 0;

			if (dynamic)
			{
				descDepth.Usage = D3D11_USAGE_DYNAMIC;
				descDepth.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			}
			else
			{
				descDepth.Usage = D3D11_USAGE_DEFAULT;
				descDepth.CPUAccessFlags = 0;
			}

			HRESULT hr = device->CreateTexture2D(&descDepth, NULL, depthStencil.GetAddressOf());

			if (FAILED(hr))
			{
				_com_error err(hr);
				LPCTSTR errMsg = err.ErrorMessage();
				std::cout << "Failed to create depth stencil texture: " << errMsg << std::endl;
			}

			D3D11_DEPTH_STENCIL_DESC dsDesc;

			// Depth test parameters
			dsDesc.DepthEnable = true;
			dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
			dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

			// Stencil test parameters
			dsDesc.StencilEnable = true;
			dsDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
			dsDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;

			// Stencil operations if pixel is front-facing
			dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
			dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

			// Stencil operations if pixel is back-facing
			dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
			dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
			dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
			dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

			// Create depth stencil state 
			device->CreateDepthStencilState(&dsDesc, state.GetAddressOf());

			if (FAILED(hr))
			{
				_com_error err(hr);
				LPCTSTR errMsg = err.ErrorMessage();
				std::cout << "Failed to create depth state: " << errMsg << std::endl;
			}

			D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
			ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));
			depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
			depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
			depthStencilViewDesc.Texture2D.MipSlice = 0;

			hr = device->CreateDepthStencilView(depthStencil.Get(), nullptr, deptStencilView.GetAddressOf());

			if (FAILED(hr))
			{
				_com_error err(hr);
				LPCTSTR errMsg = err.ErrorMessage();
				std::cout << "Failed to create deptStencilView with error: " << errMsg << std::endl;
			}

		}

		void D3D::loadTextureFromFile(const std::wstring& filename, ComPtr<ID3D11ShaderResourceView>& textureView)
		{
			HRESULT hr = DirectX::CreateWICTextureFromFile((ID3D11Device*)device.Get(), filename.c_str(), nullptr, textureView.GetAddressOf());

			if (FAILED(hr))
			{
				_com_error err(hr);
				LPCTSTR errMsg = err.ErrorMessage();
				std::cout << "Failed to load texture: " << filename.c_str() << " with error: " << errMsg << std::endl;
			}
		}

		void D3D::createRasterizerState(ComPtr<ID3D11RasterizerState>& raster, const D3D11_FILL_MODE& fillMode, const D3D11_CULL_MODE& cullMode)
		{
			D3D11_RASTERIZER_DESC desc;
			ZeroMemory(&desc, sizeof(desc));

			desc.FillMode = fillMode;
			desc.CullMode = cullMode;
			desc.FrontCounterClockwise = false;
			desc.DepthBias = 0.0f;
			desc.DepthBiasClamp = 0.0f;
			desc.DepthClipEnable = false;
			desc.MultisampleEnable = false;
			desc.ScissorEnable = false;
			desc.SlopeScaledDepthBias = 0.0f;

			HRESULT hr = device->CreateRasterizerState(&desc, raster.GetAddressOf());

			if (FAILED(hr))
			{
				_com_error err(hr);
				LPCTSTR errMsg = err.ErrorMessage();
				std::cout << "Failed to rasterizer: " << errMsg << std::endl;
			}

		}

		void D3D::createSampler(ComPtr<ID3D11SamplerState>& sampler, D3D11_TEXTURE_ADDRESS_MODE mode)
		{
			D3D11_SAMPLER_DESC sampDesc;
			ZeroMemory(&sampDesc, sizeof(sampDesc));
			sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			sampDesc.AddressU = mode;
			sampDesc.AddressV = mode;
			sampDesc.AddressW = mode;

			sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			sampDesc.MinLOD = 0;
			sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

			HRESULT hr = device->CreateSamplerState(&sampDesc, sampler.GetAddressOf());

			if (FAILED(hr))
			{
				assert(false);
				_com_error err(hr);
				LPCTSTR errMsg = err.ErrorMessage();
				std::cout << "Failed to create sampler state: " << errMsg << std::endl;
			}
		}

		void D3D::updateResource()
		{

		}

		void D3D::createCubeMap(const std::string& filename, ComPtr<ID3D11Resource>& texture, ComPtr<ID3D11ShaderResourceView>& srv)
		{
			HRESULT hr = CreateDDSTextureFromFileEx((ID3D11Device*)device.Get(), CA2W(("resources/textures/" + filename).c_str()), 0, D3D11_USAGE_IMMUTABLE, D3D11_BIND_SHADER_RESOURCE, 0, 0, false, texture.GetAddressOf(), srv.GetAddressOf());
			if (FAILED(hr))
			{
				_com_error err(hr);
				LPCTSTR errMsg = err.ErrorMessage();
				std::cout << "Failed to create cubemap texture: " << errMsg << std::endl;
			}
		}

		ComPtr<ID3D11DeviceContext4>& D3D::getContext()
		{
			return context;
		}

		ComPtr<ID3D11Device5>& D3D::getDevice()
		{
			return device;
		}

		void D3D::createTexture2D(ID3D11Texture2D*& texture, uint32_t flags, bool dynamic, DXGI_FORMAT format, float height, float width)
		{
			D3D11_TEXTURE2D_DESC textureDesc;
			ZeroMemory(&textureDesc, sizeof(textureDesc));

			textureDesc.Width = height;
			textureDesc.Height = width;
			textureDesc.MipLevels = 1;
			textureDesc.ArraySize = 1;
			textureDesc.Format = format;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE; flags; // first pass its a rt, second its a shader resource
			textureDesc.MiscFlags = 0;

			if (dynamic)
			{
				textureDesc.Usage = D3D11_USAGE_DYNAMIC;
				textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
			}
			else
			{
				textureDesc.Usage = D3D11_USAGE_DEFAULT;
				textureDesc.CPUAccessFlags = 0;
			}

			HRESULT hr = device->CreateTexture2D(&textureDesc, nullptr, &texture);
			if FAILED(hr)
			{
				_com_error err(hr);
				LPCTSTR errMsg = err.ErrorMessage();
				std::cout << "Failed to create texture: " << errMsg << std::endl;
			}
		}

		void D3D::createRenderTargetView(ID3D11RenderTargetView*& rtv, ID3D11Texture2D*& texture, DXGI_FORMAT format)
		{
			D3D11_RENDER_TARGET_VIEW_DESC rtvd;
			ZeroMemory(&rtvd, sizeof(rtvd));

			rtvd.Format = format;
			rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			rtvd.Texture2D.MipSlice = 0;

			HRESULT hr = device->CreateRenderTargetView(texture, &rtvd, &rtv);
			if (FAILED(hr))
			{
				_com_error err(hr);
				LPCTSTR errMsg = err.ErrorMessage();
				std::cout << "Failed to create rendertargetview: " << errMsg << std::endl;
			}
		}

		void D3D::createShaderResourceView(ID3D11ShaderResourceView*& srv, ID3D11Texture2D*& texture, DXGI_FORMAT format)
		{
			D3D11_SHADER_RESOURCE_VIEW_DESC srvd;
			ZeroMemory(&srvd, sizeof(srvd));

			srvd.Format = format;
			srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
			srvd.Texture2D.MostDetailedMip = 0;
			srvd.Texture2D.MipLevels = 1;

			HRESULT hr = device->CreateShaderResourceView(texture, &srvd, &srv);
			if (FAILED(hr))
			{
				_com_error err(hr);
				LPCTSTR errMsg = err.ErrorMessage();
				std::cout << "Failed to create shader resource view: " << errMsg << std::endl;
			}
		}


		void D3D::createVertexShader(ComPtr<ID3D11VertexShader>& shader, const std::wstring& path,
			ComPtr<ID3D11InputLayout>& inputLayer, const std::string& entryPoint, const std::string& target)
		{
			ComPtr<ID3DBlob> compiledShader;

			utils::D3D::CompileShader(compiledShader, path, entryPoint, target);
			HRESULT hr = device->CreateVertexShader(compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), nullptr, shader.GetAddressOf());


			if (FAILED(hr))
			{
				std::cout << "Failed to create vertex shader" << std::endl;
			}


			D3D11_INPUT_ELEMENT_DESC ied[] =
			{
				{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"NORMAL", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"BITANGENT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
				{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,  D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
			};

			hr = device->CreateInputLayout(ied, sizeof(ied) / sizeof(D3D11_INPUT_ELEMENT_DESC), compiledShader->GetBufferPointer(), compiledShader->GetBufferSize(), &inputLayer);

			if (FAILED(hr))
			{
				std::cout << "Failed to create input layer" << std::endl;
			}
		}
	}
}