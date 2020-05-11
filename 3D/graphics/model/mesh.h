#ifndef __MESH__H
#define __MESH__H
 

#include <vector>

#include "../renderer/utils/D3D/d3d.h"

#include "material.h"

namespace ENGINE
{
	class Mesh
	{
	public: 

		ComPtr<ID3D11Buffer> vBuffer;
		ComPtr<ID3D11Buffer> iBuffer;
		bool hasMtr;
		uint32_t size;
		Material mat;
	private:

	public:	
		Mesh(uint32_t size, const ComPtr<ID3D11Buffer>& vBuffer, const ComPtr<ID3D11Buffer>& iBuffer);
		Mesh(uint32_t size, const ComPtr<ID3D11Buffer>& vBuffer, const ComPtr<ID3D11Buffer>& iBuffer, Material mat);
		void render() const;
	private:


	};
}
#endif