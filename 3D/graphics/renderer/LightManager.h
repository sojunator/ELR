#ifndef __LIGHT_MANANGER__H
#define __LIGHT_MANANGER__H


#include "light.h"
#include "utils/D3D/d3d.h"


#include <array>

constexpr auto maxLights = 256;

namespace ENGINE
{
	class LightManager
	{
	private:
		struct lightBuffer
		{
			lightBuffer(Vector4 position, Vector4 colour, Vector4 lighattrib) : position(position), colour(colour), attributes(lighattrib) {};
			lightBuffer() {};
			Vector4 position;
			Vector4 colour;
			Vector4 attributes; // x = diffuse y = constant z = linear w = quadratic
		};


		struct depthBuffer
		{
			Vector4 viewport;
			Matrix view;
		};

		struct dirBuffer
		{
			lightBuffer lights[maxLights];
			uint32_t nrOfLights = 0;
			Vector3 padding;
		};

		struct pointBuffer
		{
			lightBuffer lights[maxLights];
			uint32_t nrOfLights = 0;
			Vector3 padding;
		};

		struct spotBuffer
		{
			lightBuffer lights[maxLights];
			uint32_t nrOfLights = 0;
			Vector3 padding;
		};
		dirBuffer dir;
		pointBuffer point;
		spotBuffer spot;

		ComPtr<ID3D11Buffer> dirGpu;
		ComPtr<ID3D11Buffer> spotGpu;
		ComPtr<ID3D11Buffer> pointGpu;

	public:
		LightManager();
		void init();
		void render();

		void addDirLight(const Vector4& position, const Vector4& colour, const Vector3& target);
		void addPointLight(const Vector4& position, const Vector4& colour, const Vector3& target, const Vector4& lightattr);
		void addSpotLight(const Vector4& position, const Vector4& colour, const Vector4& direction);

	private:

	public:


	};
}

#endif