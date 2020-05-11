#include "renderer.h"

#include <iostream>
#include <filesystem>
#include <mmsystem.h>
#include <timeapi.h>

#pragma comment(lib, "Winmm.lib")
namespace ENGINE
{
	Renderer::Renderer(HWND handle) 
	{
		// init 
		utils::D3D(utils::D3D::D3D(handle, swapChain));
		utils::D3D::createBackBuffer(swapChain, backBuffer);
		utils::D3D::createViewPort(viewport);
		utils::D3D::createDepthStencil(depthStencil, state, stencilView);

		createGbuffer();
		

		shaders["skybox"] = Shader(L"skybox", D3D11_FILL_SOLID, D3D11_CULL_NONE);
		//shaders["skybox"].createSkybox(Skybox("daybreak.dds"));

		

		shaders["geometrypass"] = Shader(L"geometrypass");

		shaders["plane"] = Shader(L"plane", D3D11_FILL_WIREFRAME, D3D11_CULL_NONE);
		//shaders["plane"].addModel(utils::Primitive::PlaneXZ(32, 32));

		Model sponza = Model("sponza.obj", L"", Vector3(0.0f, 0.0f, 0.0f));
		sponza.scale(0.001f);
		shaders["geometrypass"].addModel(std::move(sponza));
		deferred = Shader(L"phong", D3D11_FILL_SOLID, D3D11_CULL_NONE);




		// setup lights
		lights.init(); // This cannot be done in default constructor, as no device has been created at that time.

		lights.addPointLight(Vector4(0.693207f, 1.08236f, -0.0571255f, 1.0f), Vector4(1.f, 1.f, 1.f, 1.0f), Vector3(0.693207f, 0.58236f, -0.0571255f), Vector4(7.0f, 1.0f, 0.7f, 1.8f));

		lights.addPointLight(Vector4(-0.403163f, 1.30713f, 0.00869122f, 1.0f), Vector4(1.f, 1.f, 1.f, 1.0f), Vector3(0.693207f, 0.58236f, -0.0571255f), Vector4(7.0f, 1.0f, 0.7f, 1.8f));

		lights.addPointLight(Vector4(-0.898538f, 0.227895f, -0.0870534f, 1.0f), Vector4(1.f, 1.f, 1.f, 1.0f), Vector3(0.693207f, 0.58236f, -0.0571255f),Vector4(7.0f, 1.0f, 0.7f, 1.8f));



		// Camera
		camera = Camera(Vector3(0.0202607, 0.238587, 0.00577667), Vector3(0.0f, 0.0f, 0.0f), handle);

		// Setup models
		deferred.addModel(utils::Primitive::Quad());
	}


	void Renderer::render()
	{ 
		float color[4] = { 0.0f, 0.2f, 0.4f, 1.0f };

		for (auto& rtv : g_RTV)
			utils::D3D::getContext()->ClearRenderTargetView(rtv, color);

		utils::D3D::getContext()->ClearRenderTargetView(backBuffer.Get(), color);
		utils::D3D::getContext()->ClearDepthStencilView(stencilView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0);

		ID3D11ShaderResourceView* shvs[4] = { nullptr };
		utils::D3D::getContext()->PSSetShaderResources(0, 4, shvs);
		utils::D3D::getContext()->OMSetRenderTargets(4, g_RTV.data(), stencilView.Get());


		camera.update(0.1);
		camera.render();
		 

		utils::D3D::getContext()->RSSetViewports(1, &viewport);
	
		for (auto& shader : shaders)
			shader.second.render();



		utils::D3D::getContext()->OMSetRenderTargets(1, backBuffer.GetAddressOf(), nullptr);
		utils::D3D::getContext()->PSSetShaderResources(0, 4, g_SRV.data());

		lights.render();
		deferred.render(); 


		HRESULT hr = swapChain->Present(0, 0);

		if (FAILED(hr))
		{
			hr = utils::D3D::getDevice()->GetDeviceRemovedReason();
			if (FAILED(hr))
			{
				_com_error err(hr);
				LPCTSTR errMsg = err.ErrorMessage();
				std::cout << "Could not swap frame: " << errMsg << std::endl;
			}
			assert(false);
		}
	}
	void Renderer::createGbuffer()
	{
		for (uint32_t i = 0; i < 4; i++)
		{
			ID3D11Texture2D* tempTexPtr;
			ID3D11ShaderResourceView* tempSrvView;
			ID3D11RenderTargetView*tempRtvView;

			utils::D3D::createTexture2D(tempTexPtr, D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET);
			utils::D3D::createRenderTargetView(tempRtvView, tempTexPtr);
			utils::D3D::createShaderResourceView(tempSrvView, tempTexPtr);

			g_RTV.push_back(std::move(tempRtvView));
			g_SRV.push_back(std::move(tempSrvView));
			g_tex.push_back(std::move(tempTexPtr));
		}
	}
}