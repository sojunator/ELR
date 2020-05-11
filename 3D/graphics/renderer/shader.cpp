#include "shader.h"

namespace ENGINE
{
	Shader::Shader(const std::wstring& name, const D3D11_FILL_MODE& fillMode, const D3D11_CULL_MODE& cullMode)
	{
		std::wstring shaderfolder = L"resources/shaders/";
		utils::D3D::createSampler(sampler);
		utils::D3D::createVertexShader(vs, shaderfolder + name + L".hlsl", inputLayout);
		utils::D3D::createPixelShader(ps, shaderfolder + name + L".hlsl");
		utils::D3D::createRasterizerState(rasterizer, fillMode, cullMode);
	}

	void Shader::addModel(Model&& model)
	{
		models.push_back(std::move(model));
	}

	void Shader::addModel(Model& model)
	{
		models.push_back(model);
	}

	void Shader::createSkybox(Skybox skybox)
	{
		hasSkybox = true;
		this->skybox = skybox;
	}



	void Shader::render()
	{
		utils::D3D::getContext()->PSSetShader(ps.Get(), 0, 0);
		utils::D3D::getContext()->VSSetShader(vs.Get(), 0, 0);
		utils::D3D::getContext()->PSSetSamplers(0, 1, sampler.GetAddressOf());
		utils::D3D::getContext()->IASetInputLayout(inputLayout.Get());
		utils::D3D::getContext()->RSSetState(rasterizer.Get());
		// Render the meshes that belongs to the  pipeline
		// for now, this is specfic to model instead of mesh

		if (hasSkybox)
			skybox.render();

		for (auto& model : models)
		{ 
			model.update(1.0f); // this should be called on by a model handler or something
			model.render();
		}
	}
}