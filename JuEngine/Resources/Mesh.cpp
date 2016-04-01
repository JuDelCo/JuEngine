// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "Mesh.hpp"
#include "Material.hpp"
#include "../App.hpp"
#include <GL/glew.h>

namespace JuEngine
{
Mesh::Mesh(const std::vector<float>& vertexArray, const std::vector<unsigned int>& indexArray, const MeshDrawMode drawMode, const MeshVertexFormat meshVertexFormat, Material* material) : IObject("mesh")
{
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mEBO);

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, vertexArray.size() * sizeof(float), vertexArray.data(), GL_STATIC_DRAW); // GL_DYNAMIC_DRAW, GL_STREAM_DRAW
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArray.size() * sizeof(unsigned int), indexArray.data(), GL_STATIC_DRAW); // GL_DYNAMIC_DRAW, GL_STREAM_DRAW

	mNumVertexAttr = GetNumVertexAttr(meshVertexFormat);

	if(meshVertexFormat == MeshVertexFormat::PositionColor)
	{
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, mNumVertexAttr * sizeof(float), 0);
		glEnableVertexAttribArray(0); // Positions
		glVertexAttribPointer(3, 4, GL_UNSIGNED_INT_2_10_10_10_REV, GL_FALSE, mNumVertexAttr * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(3); // Colors
	}
	else if(meshVertexFormat == MeshVertexFormat::PositionTextureColor)
	{
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, mNumVertexAttr * sizeof(float), 0);
		glEnableVertexAttribArray(0); // Positions
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, mNumVertexAttr * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2); // Texture Coordinates
		glVertexAttribPointer(3, 4, GL_UNSIGNED_INT_2_10_10_10_REV, GL_FALSE, mNumVertexAttr * sizeof(float), (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(3); // Colors
	}
	else if(meshVertexFormat == MeshVertexFormat::PositionNormalColor)
	{
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, mNumVertexAttr * sizeof(float), 0);
		glEnableVertexAttribArray(0); // Positions
		glVertexAttribPointer(1, 4, GL_INT_2_10_10_10_REV, GL_FALSE, mNumVertexAttr * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1); // Normals
		glVertexAttribPointer(3, 4, GL_UNSIGNED_INT_2_10_10_10_REV, GL_FALSE, mNumVertexAttr * sizeof(float), (void*)(4 * sizeof(float)));
		glEnableVertexAttribArray(3); // Colors
	}
	else if(meshVertexFormat == MeshVertexFormat::PositionNormalTexture)
	{
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, mNumVertexAttr * sizeof(float), 0);
		glEnableVertexAttribArray(0); // Positions
		glVertexAttribPointer(1, 4, GL_INT_2_10_10_10_REV, GL_FALSE, mNumVertexAttr * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1); // Normals
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, mNumVertexAttr * sizeof(float), (void*)(4 * sizeof(float)));
		glEnableVertexAttribArray(2); // Texture Coordinates
	}
	else if(meshVertexFormat == MeshVertexFormat::PositionTexture)
	{
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, mNumVertexAttr * sizeof(float), 0);
		glEnableVertexAttribArray(0); // Positions
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, mNumVertexAttr * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2); // Texture Coordinates
	}

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	mVertexCount = vertexArray.size() / mNumVertexAttr;
	mIndexCount = indexArray.size();

	mDrawMode = drawMode;

	SetMaterial(material);
}

Mesh::~Mesh()
{
	glDeleteVertexArrays(1, &mVAO);
	glDeleteBuffers(1, &mVBO);
	glDeleteBuffers(1, &mEBO);
}

void Mesh::Use(Shader* shader)
{
	glBindVertexArray(mVAO);

	if(mMaterial != nullptr)
	{
		mMaterial->Use(shader);
	}
}

void Mesh::DisableMeshes()
{
	glBindVertexArray(0);
}

auto Mesh::GetNumVertexAttr(MeshVertexFormat meshVertexFormat) -> unsigned int
{
	unsigned int numVertexAttr = 0;

	switch (meshVertexFormat)
	{
		case MeshVertexFormat::PositionColor:
			numVertexAttr = 4;
			break;
		case MeshVertexFormat::PositionTextureColor:
			numVertexAttr = 6;
			break;
		case MeshVertexFormat::PositionNormalColor:
			numVertexAttr = 5;
			break;
		case MeshVertexFormat::PositionNormalTexture:
			numVertexAttr = 6;
			break;
		case MeshVertexFormat::PositionTexture:
			numVertexAttr = 5;
			break;
	}

	return numVertexAttr;
}

auto Mesh::GetDrawModeGL(MeshDrawMode meshDrawMode) -> const uint32_t
{
	switch (meshDrawMode)
	{
		case MeshDrawMode::Points:
			return GL_POINTS;
			break;
		case MeshDrawMode::Lines:
			return GL_LINES;
			break;
		case MeshDrawMode::LineLoop:
			return GL_LINE_LOOP;
			break;
		case MeshDrawMode::LineStrip:
			return GL_LINE_STRIP;
			break;
		case MeshDrawMode::Triangles:
			return GL_TRIANGLES;
			break;
		case MeshDrawMode::TriangleStrip:
			return GL_TRIANGLE_STRIP;
			break;
		case MeshDrawMode::TriangleFan:
			return GL_TRIANGLE_FAN;
			break;
		case MeshDrawMode::Quads:
			return GL_QUADS;
			break;
		case MeshDrawMode::QuadStrip:
			return GL_QUAD_STRIP;
			break;
		case MeshDrawMode::Polygon:
			return GL_POLYGON;
			break;
	}

	return GL_TRIANGLES;
}

auto Mesh::GetNumVertexAttr() const -> const unsigned int
{
	return mNumVertexAttr;
}

auto Mesh::GetVertexCount() const -> const unsigned int
{
	return mVertexCount;
}

auto Mesh::GetIndexCount() const -> const unsigned int
{
	return mIndexCount;
}

auto Mesh::GetDrawMode() const -> const MeshDrawMode
{
	return mDrawMode;
}

auto Mesh::GetMaterial() const -> Material*
{
	return mMaterial;
}

auto Mesh::SetMaterial(Material* material) -> Mesh*
{
	mMaterial = material;

	return this;
}
}
