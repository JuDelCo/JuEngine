// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/IObject.hpp"
#include <vector>

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

enum class MeshVertexFormat
{
	PositionColor,
	PositionTextureColor,
	PositionNormalColor,
	PositionNormalTexture,
	PositionTexture
};

class Shader;
class Material;

class JUENGINEAPI Mesh : public IObject
{
	public:
		Mesh(const std::vector<float>& vertexArray, const std::vector<unsigned int>& indexArray, const MeshDrawMode drawMode, const MeshVertexFormat meshVertexFormat, Material* material);
		~Mesh();

		void Use(Shader* shader);
		static void DisableMeshes();
		static auto GetNumVertexAttr(MeshVertexFormat meshVertexFormat) -> unsigned int;
		static auto GetDrawModeGL(MeshDrawMode meshDrawMode) -> const uint32_t;

		auto GetNumVertexAttr() const -> const unsigned int;
		auto GetVertexCount() const -> const unsigned int;
		auto GetIndexCount() const -> const unsigned int;
		auto GetDrawMode() const -> const MeshDrawMode;

		auto GetMaterial() const -> Material*;
		auto SetMaterial(Material* material) -> Mesh*;

	private:
		uint32_t mVBO; // Vertex buffer object
		uint32_t mVAO; // Vertex array object
		uint32_t mEBO; // Element buffer object | Index buffer object
		unsigned int mNumVertexAttr{0};
		unsigned int mVertexCount{0};
		unsigned int mIndexCount{0};
		MeshDrawMode mDrawMode;
		Material* mMaterial;
};
}
