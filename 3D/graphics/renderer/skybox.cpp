#include "skybox.h"

namespace ENGINE
{
	Skybox::Skybox(const std::string& filename)
	{
		box = Model("cube.obj", L"", Vector3(0.0f, 0.0f, 0.0f));
		// Texture
		utils::D3D::createCubeMap(filename, texture, srv);
		utils::D3D::createSampler(sampler, D3D11_TEXTURE_ADDRESS_WRAP);
	}
	void Skybox::render()
	{
		utils::D3D::getContext()->PSSetShaderResources(2, 1, srv.GetAddressOf());

		utils::D3D::getContext()->PSSetSamplers(0, 1, sampler.GetAddressOf());
		//box.scale(400);
		box.render();
	}
}