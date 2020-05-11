#ifndef __SKYBOX__H
#define __SKYBOX__H

#include "common.h"
#include "utils/D3D/d3d.h"
#include "../model/model.h"

namespace ENGINE
{	 
	class Skybox
	{
	private:
		ComPtr<ID3D11RasterizerState> raster;
		ComPtr<ID3D11Resource> texture;
		ComPtr<ID3D11ShaderResourceView> srv;
		Model box;
		ComPtr<ID3D11SamplerState> sampler;

	public:
		Skybox(const std::string& filename);
		Skybox() {};
		void render();
	private:


	public:


	};
}

#endif
