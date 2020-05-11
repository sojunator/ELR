#include "Primtive.h"
namespace ENGINE
{
	namespace utils
	{
		Model Primitive::PlaneXZ(const uint32_t& size, const uint32_t detail, const float& start)
		{
			std::vector<Model::Vertex> points;
			std::vector<uint32_t> indices;

			float width = (float)size * detail;
			float height = width;

			for (float y = start; y < height; y++)
			{
				for (float x = start; x < width; x++)
				{
					Model::Vertex point;
					point.pos = XMFLOAT4(x, 0.0f, y, 1.0f);
					point.norm = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
					point.color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
					point.texCoord = XMFLOAT2(x / width, y / height);

					points.push_back(std::move(point));
				}
			}
			// triangulate points
			for (int i = 0; i < (size * detail) - 1; i++)
			{
				int index = size * detail * (i + 1);
				for (int j = 0; j < (size * detail) - 1; j++)
				{

					indices.push_back(j + i * size * detail);
					indices.push_back(index);
					indices.push_back(index + 1);

					indices.push_back(j + i * size * detail);
					indices.push_back(index + 1);
					indices.push_back((j + i * size * detail) + 1);

					index++;
				}
			}

			return Model(points, indices);
		}

		Model Primitive::PlaneXY(const uint32_t& size, const uint32_t detail, const float& start)
		{
			std::vector<Model::Vertex> points;
			std::vector<uint32_t> indices;

			float width = (float)size * detail;
			float height = width;

			for (float y = start; y < height; y++)
			{
				for (float x = start; x < width; x++)
				{
					Model::Vertex point;
					point.pos = XMFLOAT4(x, y, 0, 1.0f);
					point.norm = XMFLOAT4(0.0f, 1.0f, 0.0f, 0.0f);
					point.color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
					point.texCoord = XMFLOAT2(x / width, y / height);

					points.push_back(std::move(point));
				}
			}
			// triangulate points
			for (int i = 0; i < (size * detail) - 1; i++)
			{
				int index = size * detail * (i + 1);
				for (int j = 0; j < (size * detail) - 1; j++)
				{

					indices.push_back(j + i * size * detail);
					indices.push_back(index);
					indices.push_back(index + 1);

					indices.push_back(j + i * size * detail);
					indices.push_back(index + 1);
					indices.push_back((j + i * size * detail) + 1);

					index++;
				}
			}

			return Model(points, indices);
		}
		Model Primitive::Quad()
		{
			// You fucking hack lord
			Model::Vertex vertss[6];
			vertss[0].pos.x = -1; // upper left corner
			vertss[0].pos.y = 1;
			vertss[0].pos.z = 0;
			vertss[1].pos.x = 1; // upper right corner
			vertss[1].pos.y = 1;
			vertss[1].pos.z = 0;
			vertss[2].pos.x = 1; // lower right corner;
			vertss[2].pos.y = -1;
			vertss[2].pos.z = 0;
			vertss[3] = vertss[2];
			vertss[4].pos.x = -1; // lower left corner;
			vertss[4].pos.y = -1;
			vertss[4].pos.z = 0;
			vertss[5] = vertss[0];

			std::vector<Model::Vertex> square(std::begin(vertss), std::end(vertss));

			return Model(square);
		}
	}

}