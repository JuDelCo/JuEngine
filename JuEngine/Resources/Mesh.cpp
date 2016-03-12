// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "Mesh.hpp"
#include "../OpenGL.hpp"
#include "../App.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace JuEngine
{
/*Mesh::Mesh() : IObject("mesh")
{
	vector<float> quad = {
	//    X      Y      Z      NX    NY    NZ    U     V      R      G      B
		-0.5f, -0.5f,  0.5f,  1.f,  1.f,  1.f,  0.f,  0.f, // BL
		 0.5f, -0.5f,  0.5f,  1.f,  1.f,  1.f,  1.f,  0.f, // BR
		 0.5f,  0.5f,  0.5f,  1.f,  1.f,  1.f,  1.f,  1.f, // TR
		 0.5f,  0.5f,  0.5f,  1.f,  1.f,  1.f,  1.f,  1.f, // TR
		-0.5f,  0.5f,  0.5f,  1.f,  1.f,  1.f,  0.f,  1.f, // TL
		-0.5f, -0.5f,  0.5f,  1.f,  1.f,  1.f,  0.f,  0.f  // BL
	};

	Create(quad);
}*/

Mesh::Mesh(const std::string& fileName, const GLenum drawMode) : IObject("mesh")
{
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
		App::Log()->Error("Error loading mesh '%s': Message: %s", fileName.c_str(), modelImporter.GetErrorString());

		modelImporter.FreeScene();

		return;
	}

	std::vector<float> vertexArray;
	std::vector<unsigned int> indexArray;
	unsigned int numVertices = 0;
	unsigned int numFaces = 0;

	for(size_t meshIndex = 0; meshIndex < (scene->mNumMeshes); ++meshIndex)
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

	Create(vertexArray, indexArray, drawMode);
}

Mesh::Mesh(const std::vector<float>& vertexArray, const std::vector<unsigned int>& indexArray, const GLenum drawMode) : IObject("mesh")
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
