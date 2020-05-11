#include "LightManager.h"

namespace ENGINE
{
	LightManager::LightManager()
	{

	}
	void LightManager::init()
	{
		utils::D3D::createBuffer(&dir, dirGpu, sizeof(dirBuffer), sizeof(dirBuffer), D3D11_BIND_CONSTANT_BUFFER, true);
		utils::D3D::createBuffer(&point, pointGpu, sizeof(pointBuffer), sizeof(pointBuffer), D3D11_BIND_CONSTANT_BUFFER, true);
		utils::D3D::createBuffer(&spot, spotGpu, sizeof(spotBuffer), sizeof(spotBuffer), D3D11_BIND_CONSTANT_BUFFER, true);

	}
	void LightManager::render()
	{
		utils::D3D::getContext()->PSSetConstantBuffers(0, 1, dirGpu.GetAddressOf());

		utils::D3D::getContext()->PSSetConstantBuffers(1, 1, spotGpu.GetAddressOf());

		utils::D3D::getContext()->PSSetConstantBuffers(2, 1, pointGpu.GetAddressOf());
	}



	void LightManager::addDirLight(const Vector4& position, const Vector4& colour, const Vector3& target)
	{

		//dir.lights[dir.nrOfLights++] = lightBuffer(position, colour, lightattr);

		utils::D3D::mapResource(dirGpu, &dir);
	}

	void LightManager::addPointLight(const Vector4& position, const Vector4& colour, const Vector3& target, const Vector4& lightattr)
	{
		point.lights[point.nrOfLights++] = lightBuffer(position, colour, lightattr);
		utils::D3D::mapResource(pointGpu, &point);
	}

	void LightManager::addSpotLight(const Vector4& position, const Vector4& colour, const Vector4& direction)
	{
		//spot.lights[spot.nrOfLights++] = lightBuffer(position, colour, lightattr);
		
		utils::D3D::mapResource(spotGpu, &spot);
	}
}