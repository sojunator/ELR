#ifndef SHADER__H
#define SHADER__H

#include "utils/D3D/d3d.h"
#include "../model/model.h"
#include "skybox.h"

namespace ENGINE
{
	class Shader
	{
	private:
		ComPtr<ID3D11VertexShader> vs;
		ComPtr<ID3D11PixelShader> ps;
		ComPtr<ID3D11InputLayout> inputLayout;
		std::vector<Model> models;
		Skybox skybox;
		bool hasSkybox = false;
		ComPtr<ID3D11SamplerState> sampler;
		ComPtr<ID3D11RasterizerState> rasterizer;
		 

	public:
		Shader() {};
		Shader(const std::wstring& name, const D3D11_FILL_MODE& fillMode = D3D11_FILL_SOLID, const D3D11_CULL_MODE& cullMode = D3D11_CULL_BACK);
		void addModel(Model&& model);
		void addModel(Model& model);
		void createSkybox(Skybox skybox);
		void render();
	private:
	};
}

#endif