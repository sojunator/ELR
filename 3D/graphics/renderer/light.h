#ifndef LIGHT__H
#define LIGHT__H

#include "utils/D3D/d3d.h"

#include "SimpleMath.h"

namespace ENGINE
{
	using namespace DirectX;
	using namespace SimpleMath;

	class Light
	{
	public:

	private:
		 
 
	public:
		Light(D3D11_VIEWPORT port, Vector4 position, Vector4 colour, SimpleMath::Vector4 attributes = { 1.0f, 1.0f, 0.7f, 1.8f });

		Light();

		void update();
		void render(float dt = 0.0f);

		void setPosition(const Vector4& position);
		void setPosition(Vector4&& position);

		void setColour(const Vector4& colour);
		void setColour(Vector4&& colour);

		void move(Vector4&& direction);
		void move(Vector4& direction);
	private:
	};
}

#endif