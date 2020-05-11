#ifndef __PLANE__H
#define __PLANE__H

#include "../../renderer.h"
#include "../D3D/d3d.h"
#include "../graphics/model/model.h"

#include "SimpleMath.h"

namespace ENGINE
{
	using namespace DirectX; 
	namespace utils
	{
		class Primitive
		{
		private:
			Primitive() {};

		public:
			static Model PlaneXZ(const uint32_t& size, const uint32_t detail, const float& start = 0);
			static Model PlaneXY(const uint32_t& size, const uint32_t detail, const float& start = 0);
			static Model Quad();
		};
	}
}



#endif