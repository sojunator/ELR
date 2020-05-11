#include "mesh.h"
#include "model.h"

namespace ENGINE
{
	Mesh::Mesh(uint32_t size, const ComPtr<ID3D11Buffer>& vBuffer, const ComPtr<ID3D11Buffer>& iBuffer)
	{
		this->vBuffer = vBuffer;
		this->iBuffer = iBuffer;
		this->size = size;
		this->mat = Material();
		hasMtr = false;
	}

	Mesh::Mesh(uint32_t size, const ComPtr<ID3D11Buffer>& vBuffer, const ComPtr<ID3D11Buffer>& iBuffer, Material mat) 
	{
		this->vBuffer = vBuffer;
		this->iBuffer = iBuffer;
		this->size = size;
		this->mat = mat;
		hasMtr = true;
	}

	void Mesh::render() const
	{
		uint32_t stride = sizeof(Model::Vertex);
		uint32_t offset = 0;

		if (hasMtr)
			mat.render();

		utils::D3D::getContext()->IASetVertexBuffers(0, 1,  vBuffer.GetAddressOf(), &stride, &offset);
		utils::D3D::getContext()->IASetIndexBuffer(iBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);

		utils::D3D::getContext()->DrawIndexed(size, 0, 0);
	}
 

}