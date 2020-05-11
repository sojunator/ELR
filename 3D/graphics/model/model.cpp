#include "model.h"


#define TINYOBJLOADER_IMPLEMENTATION 
#include <tiny_obj_loader.h>

namespace ENGINE
{
	Model::Model()
	{
	}
	Model::Model(const std::string& modelName, const std::wstring& textureName, DirectX::SimpleMath::Vector3 position)
	{
		const std::string MODEL_PATH = "resources/models/";
		const std::wstring TEXTURE_PATH = L"resources/models/";

		std::vector<Vertex> vertices;
		std::vector<uint32_t> indices;


		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;

		std::vector<ENGINE::Material> loadedmaterials;

		std::string warn, err;

		if (textureName != L"")
			utils::D3D::loadTextureFromFile(TEXTURE_PATH + textureName, textureview);

		if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, (MODEL_PATH + modelName).c_str(),
								MODEL_PATH.c_str())) {
			std::cout << (warn + err);
		}
		else
		{
			for (auto& material : materials)
			{
				loadedmaterials.push_back(ENGINE::Material(material));
			}
 

			for (const auto& shape : shapes)
			{
				for (const auto& index : shape.mesh.indices)
				{
					Vertex vertex = {};

					vertex.pos =
					{
						attrib.vertices[3 * index.vertex_index + 0],
						attrib.vertices[3 * index.vertex_index + 1],
						attrib.vertices[3 * index.vertex_index + 2],
						1
					};
					 

					if (!attrib.texcoords.empty())
						vertex.texCoord =
						{
							attrib.texcoords[2 * index.texcoord_index + 0],
							1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
						
						};

					if (!attrib.normals.empty())
						vertex.norm = {
							attrib.normals[3 * index.normal_index + 0],
							attrib.normals[3 * index.normal_index + 1],
							attrib.normals[3 * index.normal_index + 2],
							0
						};

					vertices.push_back(vertex);
					indices.push_back(indices.size());
				}



				for (uint32_t i = 0; i < vertices.size(); i += 3)
				{

					Vector4 v0 = vertices[i + (uint32_t)0].pos;
					Vector4 v1 = vertices[i + (uint32_t)1].pos;
					Vector4 v2 = vertices[i + (uint32_t)2].pos;

					Vector2 uv0 = vertices[i + (uint32_t)0].texCoord;
					Vector2 uv1 = vertices[i + (uint32_t)1].texCoord;
					Vector2 uv2 = vertices[i + (uint32_t)2].texCoord;

					Vector4 edge1 = v1 - v0;
					Vector4 edge2 = v2 - v0;
					Vector2 deltaUV1 = uv1 - uv0;
					Vector2 deltaUV2 = uv2 - uv0;


					float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

					Vector4 tangent;
					tangent.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
					tangent.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
					tangent.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);


					Vector4 bitangent;
					bitangent.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
					bitangent.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
					bitangent.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);


					bitangent.Normalize();
					tangent.Normalize();

					vertices[i + (uint32_t)0].tang = tangent;
					vertices[i + (uint32_t)1].tang = tangent;
					vertices[i + (uint32_t)2].tang = tangent;
					vertices[i + (uint32_t)0].bitang = bitangent;
					vertices[i + (uint32_t)1].bitang = bitangent;
					vertices[i + (uint32_t)2].bitang = bitangent;
				}



				ComPtr<ID3D11Buffer> vbuffer;
				utils::D3D::createBuffer(vertices.data(), vbuffer, vertices.size() * sizeof(Vertex), sizeof(Vertex), D3D11_BIND_VERTEX_BUFFER);


				ComPtr<ID3D11Buffer> ibuffer;
				utils::D3D::createBuffer(indices.data(), ibuffer,  indices.size() * sizeof(uint32_t), sizeof(uint32_t), D3D11_BIND_INDEX_BUFFER);

				int matId = shape.mesh.material_ids[0];

				if (matId != -1)
					meshes.push_back(std::move(Mesh(indices.size(), vbuffer, ibuffer, loadedmaterials[matId])));
				else
				{
					// This will asign the mesh a material without texture
					meshes.push_back(std::move(Mesh(indices.size(), vbuffer, ibuffer)));
				}
				vertices.clear();
				indices.clear();
			}


		}


		// Create world matrix
		uint32_t size = sizeof(worldCpu);
		worldCpu = DirectX::SimpleMath::Matrix::CreateTranslation(position).Transpose();//DirectX::SimpleMath::Matrix::Identity;
		utils::D3D::createBuffer(&worldCpu, worldGpu, size, size, D3D11_BIND_CONSTANT_BUFFER, true);
	}

	Model::Model(std::vector<Vertex> verts)
	{
		std::vector<int> indices;

		for (uint32_t i = 0; i < verts.size(); i++)
			indices.push_back(i);

		ComPtr<ID3D11Buffer> vbuffer;
		utils::D3D::createBuffer(verts.data(), vbuffer, verts.size() * sizeof(Vertex), sizeof(Vertex), D3D11_BIND_VERTEX_BUFFER);


		ComPtr<ID3D11Buffer> ibuffer;
		utils::D3D::createBuffer(indices.data(), ibuffer, indices.size() * sizeof(uint32_t), sizeof(uint32_t), D3D11_BIND_INDEX_BUFFER);

		meshes.push_back(std::move(Mesh(indices.size(), vbuffer, ibuffer)));

		uint32_t size = sizeof(worldCpu);
		worldCpu = DirectX::SimpleMath::Matrix::Identity.Transpose();//DirectX::SimpleMath::Matrix::Identity;
		utils::D3D::createBuffer(&worldCpu, worldGpu, size, size, D3D11_BIND_CONSTANT_BUFFER, true);
	}

	Model::Model(std::vector<Vertex> points, std::vector<uint32_t> indices)
	{

		ComPtr<ID3D11Buffer> vbuffer;
		ComPtr<ID3D11Buffer> ibuffer;
		utils::D3D::createBuffer(points.data(), vbuffer, sizeof(Model::Vertex) * points.size(), sizeof(Model::Vertex), D3D11_BIND_VERTEX_BUFFER);
		utils::D3D::createBuffer(indices.data(), ibuffer, sizeof(uint32_t) * indices.size(), sizeof(uint32_t), D3D11_BIND_INDEX_BUFFER);
		uint32_t size = sizeof(worldCpu);
		worldCpu = DirectX::SimpleMath::Matrix::Identity.Transpose();//DirectX::SimpleMath::Matrix::Identity;
		utils::D3D::createBuffer(&worldCpu, worldGpu, size, size, D3D11_BIND_CONSTANT_BUFFER, true);
		meshes.push_back(std::move(Mesh(indices.size(), vbuffer, ibuffer)));

	}

	Model::Model(const uint32_t& size, ComPtr<ID3D11Buffer>& vertexBuffer, ComPtr<ID3D11Buffer>& indexBuffer)
	{
		meshes.push_back(std::move(Mesh(size, vertexBuffer, indexBuffer)));
	}

	void Model::rotateY(float rad)
	{
		worldCpu *= DirectX::SimpleMath::Matrix::CreateTranslation(0.0f, 0.0f, 0.0f).Transpose();
		worldCpu *= DirectX::SimpleMath::Matrix::CreateRotationY(rad);
	}

	void Model::rotateX(float rad)
	{

		worldCpu *= DirectX::SimpleMath::Matrix::CreateTranslation(0.0f, 0.0f, 0.0f).Transpose();
		worldCpu *= DirectX::SimpleMath::Matrix::CreateRotationX(rad);
	}

	void Model::rotateZ(float rad)
	{
		worldCpu *= DirectX::SimpleMath::Matrix::CreateTranslation(0.0f, 0.0f, 0.0f).Transpose();
		worldCpu *= DirectX::SimpleMath::Matrix::CreateRotationZ(rad);
	}

	void Model::move(DirectX::SimpleMath::Vector3 direction, float unit)
	{
		worldCpu *= DirectX::SimpleMath::Matrix::CreateTranslation(direction * unit).Transpose();
	}

	void Model::scale(float scale)
	{
		worldCpu = DirectX::SimpleMath::Matrix::CreateScale(scale);
	}

	void Model::setPosition(DirectX::SimpleMath::Vector3 position)
	{
		worldCpu = DirectX::SimpleMath::Matrix::CreateTranslation(position).Transpose();
	}

	void Model::render() const
	{
		utils::D3D::mapResource(worldGpu, &worldCpu);

		utils::D3D::getContext()->VSSetConstantBuffers(1, 1, worldGpu.GetAddressOf());
		utils::D3D::getContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
 


		for (const auto& mesh : meshes)
		{ 
			mesh.render();
		}
	}
	void Model::update(float dt)
	{

	}
}

