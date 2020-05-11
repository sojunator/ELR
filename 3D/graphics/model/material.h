#ifndef __MATERIAL__H
#define __MATERIAL__H

#include "../renderer/utils/D3D/d3d.h"

#include "tiny_obj_loader.h"
#include "SimpleMath.h"
namespace ENGINE
{

	using namespace DirectX;
	using namespace SimpleMath;
	class Material
	{
	private:
		struct mtr
		{
			Vector4 specular;
			Vector4 ambient;
			Vector4 diffuse;
			Vector4 shininess;
		};

		ComPtr<ID3D11Buffer> gpuMtr;
		ComPtr<ID3D11ShaderResourceView> diffusetexture;
		ComPtr<ID3D11ShaderResourceView> normaltexture;
		std::string name;
		mtr cpuMtr;
	public:

		void render() const;
		Material(tinyobj::material_t data);
		Material();
	};
}




#endif
