#ifndef __MODEL__H
#define __MODEL__H

#include <vector>

#include "mesh.h"

#include "material.h"
namespace ENGINE
{
	class Model
	{
	public:
		struct Vertex
		{
			DirectX::XMFLOAT4 pos;
			DirectX::XMFLOAT4 color;
			DirectX::XMFLOAT4 norm;
			DirectX::XMFLOAT4 tang;
			DirectX::XMFLOAT4 bitang;
			DirectX::XMFLOAT2 texCoord;
		};


	private:
		DirectX::SimpleMath::Matrix worldCpu;
		ComPtr<ID3D11Buffer> worldGpu;
		ComPtr<ID3D11ShaderResourceView> textureview;
		std::vector<ENGINE::Mesh> meshes; 
		

	public:	
		Model();
		Model(const std::string& modelName, const std::wstring& textureName, DirectX::SimpleMath::Vector3 positon);
		Model(std::vector<Vertex> verts);
		Model(std::vector<Vertex> points, std::vector<uint32_t> indices);
		Model(const uint32_t& size, ComPtr<ID3D11Buffer>& vertexBuffer, ComPtr<ID3D11Buffer>& indexBuffer);
		void rotateY(float rad);
		void rotateX(float rad);
		void rotateZ(float rad);

		void scale(float scale);
		void setPosition(DirectX::SimpleMath::Vector3 position);
		void move(DirectX::SimpleMath::Vector3 direction, float unit);

		void render() const;
		void update(float dt);
	private:

	};
}

#endif