// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "MeshManager.hpp"
#include "../Resources/DebugLog.hpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace JuEngine
{
MeshManager* MeshManager::mInstance = nullptr;

MeshManager::MeshManager() : IObject("meshManager")
{
	if(MeshManager::mInstance != nullptr)
	{
		ThrowRuntimeError("Error, there are a MeshManager instance created already");
	}

	MeshManager::mInstance = this;
}

MeshManager::~MeshManager()
{
	MeshManager::mInstance = nullptr;
}

void MeshManager::Unload()
{
	mMeshes.clear();
}

auto MeshManager::Add(const string& name, const string& fileName, const GLenum drawMode) -> Mesh*
{
	if(MeshManager::mInstance->mMeshes.count(name) != 0)
	{
		DebugLog::Write("Warning, attempted to load a mesh with name '%s'. The mesh name is already being used", name.c_str());

		return Get(name);
	}

	// ========================================================================

	Assimp::Importer modelImporter;

	auto modelsPath = "Assets/Models/";

	const aiScene* scene = modelImporter.ReadFile(modelsPath + fileName,
		aiProcess_Triangulate
		| aiProcess_RemoveComponent  // Removes animations, materials, light sources, cameras, textures, vertex components
		| aiProcess_JoinIdenticalVertices
		| aiProcess_GenNormals
		//| aiProcess_CalcTangentSpace
		//| aiProcess_GenUVCoords
		//| aiProcess_SortByPType
		//| aiProcess_ConvertToLeftHanded
		| aiProcess_ImproveCacheLocality // For huge models
	);

	if(! scene)
	{
		DebugLog::Write("%s", modelImporter.GetErrorString());

		modelImporter.FreeScene();

		return Get(name);
	}

	vector<float> vertexArray;
	vector<unsigned int> indexArray;
	unsigned int numVertices = 0;
	unsigned int numFaces = 0;

	for(size_t meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex)
	{
		aiMesh* mesh = scene->mMeshes[meshIndex];
		aiColor4D diffuseColor;

		vertexArray.resize((numVertices * Mesh::mNumVertexAttr) + (mesh->mNumVertices * Mesh::mNumVertexAttr), 0.f);

		for(size_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex)
		{
			auto vertexArrayIndex = (numVertices * Mesh::mNumVertexAttr) + (vertexIndex * Mesh::mNumVertexAttr);

			if(mesh->HasPositions())
			{
				vertexArray[vertexArrayIndex+0] = mesh->mVertices[vertexIndex].x; // X
				vertexArray[vertexArrayIndex+1] = mesh->mVertices[vertexIndex].y; // Y
				vertexArray[vertexArrayIndex+2] = mesh->mVertices[vertexIndex].z; // Z
			}

			if(mesh->HasNormals())
			{
				vertexArray[vertexArrayIndex+3] = mesh->mNormals[vertexIndex].x; // NX
				vertexArray[vertexArrayIndex+4] = mesh->mNormals[vertexIndex].y; // NY
				vertexArray[vertexArrayIndex+5] = mesh->mNormals[vertexIndex].z; // NZ
			}

			if(mesh->HasTextureCoords(0))
			{
				vertexArray[vertexArrayIndex+6] = mesh->mTextureCoords[0][vertexIndex].x; // U
				vertexArray[vertexArrayIndex+7] = mesh->mTextureCoords[0][vertexIndex].y; // V
			}

			if (AI_SUCCESS == aiGetMaterialColor(scene->mMaterials[mesh->mMaterialIndex], AI_MATKEY_COLOR_DIFFUSE, &diffuseColor))
			{
				vertexArray[vertexArrayIndex+8] =  diffuseColor.r; // R
				vertexArray[vertexArrayIndex+9] =  diffuseColor.g; // G
				vertexArray[vertexArrayIndex+10] = diffuseColor.b; // B
			}
		}

		indexArray.resize((numFaces * 3) + (mesh->mNumFaces * 3), 0.f); // Triangles only !

		for(size_t faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex)
		{
			auto indexArrayIndex = (numFaces * 3) + (faceIndex * 3);

			indexArray[indexArrayIndex+0] = numVertices + mesh->mFaces[faceIndex].mIndices[0];
			indexArray[indexArrayIndex+1] = numVertices + mesh->mFaces[faceIndex].mIndices[1];
			indexArray[indexArrayIndex+2] = numVertices + mesh->mFaces[faceIndex].mIndices[2];
		}

		numVertices += mesh->mNumVertices;
		numFaces += mesh->mNumFaces;
	}

	modelImporter.FreeScene();

	return Add(name, vertexArray, indexArray, drawMode);

	// ========================================================================

	return Get(name);
}

auto MeshManager::Add(const string& name, const vector<float>& vertexArray, const vector<unsigned int>& indexArray, const GLenum drawMode) -> Mesh*
{
	if(MeshManager::mInstance->mMeshes.count(name) != 0)
	{
		DebugLog::Write("Warning, attempted to load a mesh with name '%s'. The mesh name is already being used", name.c_str());

		return Get(name);
	}

	MeshManager::mInstance->mMeshes[name] = std::make_shared<Mesh>(vertexArray, indexArray, drawMode);
	MeshManager::mInstance->mMeshes.at(name)->SetName(name);

	return Get(name);
}

auto MeshManager::Get(const string& name) -> Mesh*
{
	if(MeshManager::mInstance->mMeshes.count(name) != 0)
	{
		return MeshManager::mInstance->mMeshes.at(name).get();
	}

	DebugLog::Write("Warning: MeshManager.Get: No mesh found with name %s", name.c_str());

	return nullptr;
}

void MeshManager::Use(const string& name)
{
	auto mesh = Get(name);

	if(mesh)
	{
		mesh->Use();
	}
}

void MeshManager::DisableMeshes()
{
	Mesh::DisableMeshes();
}
}
