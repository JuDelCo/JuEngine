// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../IObject.hpp"
#include "../OpenGL.hpp"
#include <vector>

namespace JuEngine
{
using std::vector;

class JUENGINEAPI Mesh : public IObject
{
public:
	Mesh();
	Mesh(const vector<float>& vertexArray, const vector<unsigned int>& indexArray, const GLenum drawMode);
	virtual ~Mesh();

	void Use();
	static void DisableMeshes();
	auto GetVertexCount() const -> const unsigned int&;
	auto GetIndexCount() const -> const unsigned int&;
	auto GetDrawMode() const -> const GLenum&;

	static const unsigned int mNumVertexAttr{11};

private:
	void Create(const vector<float>& vertexArray, const vector<unsigned int>& indexArray, const GLenum drawMode);

	GLuint mVBO; // Vertex buffer object
	GLuint mVAO; // Vertex array object
	GLuint mIBO; // Index buffer object
	unsigned int mVertexCount;
	unsigned int mIndexCount;
	GLenum mDrawMode;
};
}
