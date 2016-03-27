// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "Mesh.hpp"
#include "Material.hpp"
#include "../App.hpp"
#include <GL/glew.h>

namespace JuEngine
{
Mesh::Mesh(const std::vector<float>& vertexArray, const std::vector<unsigned int>& indexArray, const MeshDrawMode drawMode, Material* material) : IObject("mesh")
{
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mEBO);

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, vertexArray.size() * sizeof(float), vertexArray.data(), GL_STATIC_DRAW); // GL_DYNAMIC_DRAW, GL_STREAM_DRAW
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArray.size() * sizeof(unsigned int), indexArray.data(), GL_STATIC_DRAW); // GL_DYNAMIC_DRAW, GL_STREAM_DRAW

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, mNumVertexAttr * sizeof(float), 0);
	glEnableVertexAttribArray(0); // Vertex Positions
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, mNumVertexAttr * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1); // Vertex Normals
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, mNumVertexAttr * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2); // Texture Coordinates
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, mNumVertexAttr * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3); // Vertex Color

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	mVertexCount = vertexArray.size() / mNumVertexAttr;
	mIndexCount = indexArray.size();

	switch (drawMode)
	{
		case MeshDrawMode::Points:
			mDrawMode = GL_POINTS;
			break;
		case MeshDrawMode::Lines:
			mDrawMode = GL_LINES;
			break;
		case MeshDrawMode::LineLoop:
			mDrawMode = GL_LINE_LOOP;
			break;
		case MeshDrawMode::LineStrip:
			mDrawMode = GL_LINE_STRIP;
			break;
		case MeshDrawMode::Triangles:
			mDrawMode = GL_TRIANGLES;
			break;
		case MeshDrawMode::TriangleStrip:
			mDrawMode = GL_TRIANGLE_STRIP;
			break;
		case MeshDrawMode::TriangleFan:
			mDrawMode = GL_TRIANGLE_FAN;
			break;
		case MeshDrawMode::Quads:
			mDrawMode = GL_QUADS;
			break;
		case MeshDrawMode::QuadStrip:
			mDrawMode = GL_QUAD_STRIP;
			break;
		case MeshDrawMode::Polygon:
			mDrawMode = GL_POLYGON;
			break;
		default:
			mDrawMode = GL_TRIANGLES;
	}

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

auto Mesh::GetDrawMode() const -> const GLenum
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
