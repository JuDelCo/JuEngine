// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "MeshNode.hpp"
#include "../App.hpp"
#include <GL/glew.h>
#include <algorithm>

namespace JuEngine
{
MeshNode::MeshNode() : IObject("meshNode")
{
}

auto MeshNode::GetMeshNodeList() -> std::vector<MeshNode*>&
{
	return mMeshNodeList;
}

auto MeshNode::AddMeshNode(MeshNode* meshNode) -> MeshNode*
{
	if(std::find(mMeshNodeList.begin(), mMeshNodeList.end(), meshNode) == mMeshNodeList.end())
	{
		mMeshNodeList.push_back(meshNode);
	}

	return this;
}

auto MeshNode::RemoveMeshNode(MeshNode* meshNode) -> MeshNode*
{
	mMeshNodeList.erase(std::find(mMeshNodeList.begin(), mMeshNodeList.end(), meshNode));

	return this;
}

auto MeshNode::GetMeshList() -> std::vector<Mesh*>&
{
	return mMeshList;
}

auto MeshNode::AddMesh(Mesh* mesh) -> MeshNode*
{
	if(std::find(mMeshList.begin(), mMeshList.end(), mesh) == mMeshList.end())
	{
		mMeshList.push_back(mesh);
	}

	return this;
}

auto MeshNode::RemoveMesh(Mesh* mesh) -> MeshNode*
{
	mMeshList.erase(std::find(mMeshList.begin(), mMeshList.end(), mesh));

	return this;
}
}
