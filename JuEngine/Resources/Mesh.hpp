// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/IObject.hpp"
#include "../OpenGL.hpp"
#include <vector>

namespace JuEngine
{
class JUENGINEAPI Mesh : public IObject
{
	public:
		Mesh();
		Mesh(const std::vector<float>& vertexArray, const std::vector<unsigned int>& indexArray, const GLenum drawMode);
		~Mesh();

		void Use();
		static void DisableMeshes();
		auto GetVertexCount() const -> const unsigned int&;
		auto GetIndexCount() const -> const unsigned int&;
		auto GetDrawMode() const -> const GLenum&;

		static const unsigned int mNumVertexAttr{11};

	private:
		void Create(const std::vector<float>& vertexArray, const std::vector<unsigned int>& indexArray, const GLenum drawMode);

		GLuint mVBO; // Vertex buffer object
		GLuint mVAO; // Vertex array object
		GLuint mIBO; // Index buffer object
		unsigned int mVertexCount{0};
		unsigned int mIndexCount{0};
		GLenum mDrawMode;
};
}
