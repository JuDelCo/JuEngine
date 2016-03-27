// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/IObject.hpp"
#include <vector>

typedef unsigned int GLenum;
typedef unsigned int GLuint;

namespace JuEngine
{
enum class MeshDrawMode
{
	Points,
	Lines,
	LineLoop,
	LineStrip,
	Triangles,
	TriangleStrip,
	TriangleFan,
	Quads,
	QuadStrip,
	Polygon
};

class Shader;
class Material;

class JUENGINEAPI Mesh : public IObject
{
	public:
		Mesh(const std::vector<float>& vertexArray, const std::vector<unsigned int>& indexArray, const MeshDrawMode drawMode, Material* material = nullptr);
		~Mesh();

		void Use(Shader* shader);
		static void DisableMeshes();

		auto GetNumVertexAttr() const -> const unsigned int;
		auto GetVertexCount() const -> const unsigned int;
		auto GetIndexCount() const -> const unsigned int;
		auto GetDrawMode() const -> const GLenum;

		auto GetMaterial() const -> Material*;
		auto SetMaterial(Material* material) -> Mesh*;

	private:
		GLuint mVBO; // Vertex buffer object
		GLuint mVAO; // Vertex array object
		GLuint mEBO; // Element buffer object | Index buffer object
		unsigned int mNumVertexAttr{11};
		unsigned int mVertexCount{0};
		unsigned int mIndexCount{0};
		GLenum mDrawMode;
		Material* mMaterial;
};
}
