#include "material.h"

#include "tiny_obj_loader.h"
namespace ENGINE
{

	void Material::render() const
	{
		utils::D3D::getContext()->PSSetConstantBuffers(0, 1, gpuMtr.GetAddressOf());
 
		utils::D3D::getContext()->PSSetShaderResources(0, 1, diffusetexture.GetAddressOf());
		utils::D3D::getContext()->PSSetShaderResources(1, 1, normaltexture.GetAddressOf());

	}

	Material::Material(tinyobj::material_t data)
	{
		cpuMtr.ambient = Vector4(data.ambient[0], data.ambient[1], data.ambient[2], 1.0f);
		cpuMtr.diffuse = Vector4(data.diffuse[0], data.diffuse[1], data.diffuse[2], 1.0f);
		cpuMtr.specular = Vector4(data.specular[0], data.specular[1], data.specular[2], 1.0f);
		cpuMtr.shininess = Vector4(32, 32, 32, 1.0f);

		name = data.name;

		std::wstring wsTmp(data.diffuse_texname.begin(), data.diffuse_texname.end());

		utils::D3D::createBuffer(&cpuMtr, gpuMtr, sizeof(mtr), sizeof(mtr), D3D11_BIND_CONSTANT_BUFFER);

		if (data.diffuse_texname != "")
			utils::D3D::loadTextureFromFile(L"resources/models/" + wsTmp, diffusetexture);

		if ((data.normal_texname != ""))
		{
			std::wstring wsTmp(data.normal_texname.begin(), data.normal_texname.end());
			utils::D3D::loadTextureFromFile(L"resources/models/" + wsTmp, normaltexture);
		}
		else
		{
			utils::D3D::loadTextureFromFile(L"resources/models/textures/blank.jpg" , normaltexture);
		}
	}
	Material::Material()
	{
		cpuMtr.ambient = Vector4::One;
		cpuMtr.diffuse = Vector4::One;
		cpuMtr.specular = Vector4::One;


		utils::D3D::createBuffer(&cpuMtr, gpuMtr, sizeof(mtr), sizeof(mtr), D3D11_BIND_CONSTANT_BUFFER);
	}
}
