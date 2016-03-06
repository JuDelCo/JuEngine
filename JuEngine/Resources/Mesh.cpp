// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "Mesh.hpp"

namespace JuEngine
{
Mesh::Mesh() : IObject("mesh")
{
	/*vector<float> quad = {
	//    X      Y      Z      NX    NY    NZ    U     V      R      G      B
		-0.5f, -0.5f,  0.5f,  1.f,  1.f,  1.f,  0.f,  0.f, // BL
		 0.5f, -0.5f,  0.5f,  1.f,  1.f,  1.f,  1.f,  0.f, // BR
		 0.5f,  0.5f,  0.5f,  1.f,  1.f,  1.f,  1.f,  1.f, // TR
		 0.5f,  0.5f,  0.5f,  1.f,  1.f,  1.f,  1.f,  1.f, // TR
		-0.5f,  0.5f,  0.5f,  1.f,  1.f,  1.f,  0.f,  1.f, // TL
		-0.5f, -0.5f,  0.5f,  1.f,  1.f,  1.f,  0.f,  0.f  // BL
	};

	Create(quad);*/
}

Mesh::Mesh(const std::vector<float>& vertexArray, const std::vector<unsigned int>& indexArray, const GLenum drawMode) :
	IObject("mesh")
{
	Create(vertexArray, indexArray, drawMode);
}

Mesh::~Mesh()
{
	glDeleteBuffers(1, &mVBO);
	glDeleteVertexArrays(1, &mVAO);
}

void Mesh::Create(const std::vector<float>& vertexArray, const std::vector<unsigned int>& indexArray, const GLenum drawMode)
{
	glGenBuffers(1, &mVBO);
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mIBO);

	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);

	glBufferData(GL_ARRAY_BUFFER, vertexArray.size() * sizeof(float), vertexArray.data(), GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexArray.size() * sizeof(unsigned int), indexArray.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, mNumVertexAttr * sizeof(float), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, mNumVertexAttr * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, mNumVertexAttr * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, mNumVertexAttr * sizeof(float), (void*)(8 * sizeof(float)));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	mVertexCount = vertexArray.size() / mNumVertexAttr;
	mIndexCount = indexArray.size();
	mDrawMode = drawMode;
}

void Mesh::Use()
{
	glBindVertexArray(mVAO);
}

void Mesh::DisableMeshes()
{
	glBindVertexArray(0);
}

auto Mesh::GetVertexCount() const -> const unsigned int&
{
	return mVertexCount;
}

auto Mesh::GetIndexCount() const -> const unsigned int&
{
	return mIndexCount;
}

auto Mesh::GetDrawMode() const -> const GLenum&
{
	return mDrawMode;
}
}
