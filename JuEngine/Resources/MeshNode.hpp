// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/IObject.hpp"
#include <vector>

namespace JuEngine
{
class Mesh;

class JUENGINEAPI MeshNode : public IObject
{
	public:
		MeshNode();

		auto GetMeshNodeList() -> std::vector<MeshNode*>&;
		auto AddMeshNode(MeshNode* meshNode) -> MeshNode*;
		auto RemoveMeshNode(MeshNode* meshNode) -> MeshNode*;

		auto GetMeshList() -> std::vector<Mesh*>&;
		auto AddMesh(Mesh* mesh) -> MeshNode*;
		auto RemoveMesh(Mesh* mesh) -> MeshNode*;

	private:
		std::vector<MeshNode*> mMeshNodeList;
		std::vector<Mesh*> mMeshList;
};
}
