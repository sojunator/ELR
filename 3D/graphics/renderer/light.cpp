#include "light.h"

namespace ENGINE
{
	Light::Light(D3D11_VIEWPORT port, SimpleMath::Vector4 position, SimpleMath::Vector4 colour, SimpleMath::Vector4 attributes) //: cpuBuffer(position, colour, attributes)
	{
	//	this->port = port;
	}

	Light::Light()
	{
		
	}

	void Light::update()
	{
		//utils::D3D::mapResource(gpuBuffer, &cpuBuffer);
	}

	void Light::render(float dt)
	{
		///utils::D3D::getContext()->RSSetViewports(1, &port);
	}

	void Light::setPosition(const Vector4& position)
	{
		//cpuBuffer.position = position;
	}

	void Light::setPosition(Vector4&& position)
	{
		//cpuBuffer.position = Vector4(std::move(position));
	}

	void Light::setColour(const Vector4& colour)
	{
		//cpuBuffer.colour = colour;
	}

	void Light::setColour(Vector4&& colour)
	{
		//cpuBuffer.colour = Vector4(std::move(colour));
	}
	void Light::move(Vector4&& direction)
	{
		//cpuBuffer.position += direction * 0.00001f;
	}
	void Light::move(Vector4& direction)
	{
		//cpuBuffer.position += direction * 0.001f;
	}
}