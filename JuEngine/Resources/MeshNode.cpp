// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "MeshNode.hpp"
#include "Material.hpp"
#include "Mesh.hpp"
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

auto GetMeshNodeListRecursive(MeshNode* meshNode) -> std::vector<MeshNode*>
{
	std::vector<MeshNode*> meshNodeList;
	auto meshNodeListTemp = meshNode->GetMeshNodeList();

	meshNodeList.push_back(meshNode);
	meshNodeList.insert(meshNodeList.end(), meshNodeListTemp.begin(), meshNodeListTemp.end());

	return meshNodeList;
}

auto MeshNode::GetMeshList() -> std::vector<Mesh*>
{
	auto meshNodeList = GetMeshNodeListRecursive(this);
	std::vector<Mesh*> meshList;

	for(auto &meshNode : meshNodeList)
	{
		meshList.insert(meshList.end(), meshNode->mMeshList.begin(), meshNode->mMeshList.end());
	}

	return meshList;
}

auto MeshNode::GetMaterialList() -> std::vector<Material*>
{
	auto meshList = GetMeshList();
	std::vector<Material*> materialList;

	for(auto &mesh : meshList)
	{
		auto material = mesh->GetMaterial();

		if(material != nullptr)
		{
			materialList.push_back(material);
		}
	}

	return materialList;
}

auto MeshNode::GetTextureList() -> std::vector<Texture*>
{
	auto meshList = GetMeshList();
	std::vector<Texture*> textureList;

	for(auto &mesh : meshList)
	{
		auto material = mesh->GetMaterial();

		if(material != nullptr)
		{
			auto textureListTemp = material->GetTextureList();
			textureList.insert(textureList.end(), textureListTemp.begin(), textureListTemp.end());
		}
	}

	return textureList;
}
}
