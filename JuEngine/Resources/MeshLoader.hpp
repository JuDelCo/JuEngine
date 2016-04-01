// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "MeshNode.hpp"
#include "Mesh.hpp"

namespace JuEngine
{
class Material;

class JUENGINEAPI MeshLoader : public IObject
{
	public:
		static auto Load(const std::string& filePath, const MeshVertexFormat meshVertexFormat, const MeshDrawMode drawMode = MeshDrawMode::Triangles) -> MeshNode*;
		static auto GenerateQuad(Material* material) -> MeshNode*;

	protected:
		MeshLoader();
};
}
