// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "MeshLoader.hpp"
#include "Material.hpp"
#include "Texture.hpp"
#include "../App.hpp"
#include "../Services/IDataService.hpp"
#include <sstream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace JuEngine
{
auto ProcessNode(aiNode* node, const aiScene* scene) -> MeshNode*;
auto ProcessMesh(aiMesh* mesh, const aiScene* scene) -> Mesh*;
void LoadTextures(aiMaterial* mat, aiTextureType type, Material* material, const std::string& textureName);

auto MeshLoader::Load(const std::string& filePath, const MeshDrawMode drawMode) -> MeshNode*
{
	static auto modelsPath = "Assets/Models/";

	Assimp::Importer modelImporter;

	const aiScene* scene = modelImporter.ReadFile(modelsPath + filePath,
		aiProcess_Triangulate
		//| aiProcess_RemoveComponent  // Removes animations, materials, light sources, cameras, textures, vertex components
		//| aiProcess_JoinIdenticalVertices
		//| aiProcess_GenNormals
		//| aiProcess_CalcTangentSpace
		//| aiProcess_GenUVCoords
		//| aiProcess_SortByPType
		//| aiProcess_ConvertToLeftHanded
		//| aiProcess_FlipUVs
		| aiProcess_ImproveCacheLocality // For huge models
	);

	if(! scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || ! scene->mRootNode)
	{
		App::Log()->Error("Error loading mesh '%s': Message: %s", filePath.c_str(), modelImporter.GetErrorString());
		modelImporter.FreeScene();

		return nullptr;
	}

	auto rootMeshNode = ProcessNode(scene->mRootNode, scene);

	modelImporter.FreeScene();

	return rootMeshNode;
}

auto ProcessNode(aiNode* node, const aiScene* scene) -> MeshNode*
{
	auto meshNode = new MeshNode();
	meshNode->SetId(node->mName.C_Str());

	for(unsigned int meshIndex = 0; meshIndex < node->mNumMeshes; ++meshIndex)
	{
		meshNode->AddMesh(ProcessMesh(scene->mMeshes[node->mMeshes[meshIndex]], scene));
	}

	for(unsigned int subNodeIndex = 0; subNodeIndex < node->mNumChildren; ++subNodeIndex)
	{
		meshNode->AddMeshNode(ProcessNode(node->mChildren[subNodeIndex], scene));
	}

	return meshNode;
}

auto ProcessMesh(aiMesh* mesh, const aiScene* scene) -> Mesh*
{
	unsigned int NumVertexAttr = 11; // 11 Attributes = [x3 position, x3 normals, x2 texCoords, x3 Color]
	std::vector<float> vertexArray;
	std::vector<GLuint> indexArray;
	aiColor4D diffuseColor;

	vertexArray.resize((mesh->mNumVertices * NumVertexAttr), 0.f);

	for(unsigned int vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex)
	{
		unsigned int offset = vertexIndex * NumVertexAttr;

		if(mesh->HasPositions())
		{
			vertexArray[offset+0] = mesh->mVertices[vertexIndex].x; // X
			vertexArray[offset+1] = mesh->mVertices[vertexIndex].y; // Y
			vertexArray[offset+2] = mesh->mVertices[vertexIndex].z; // Z
		}

		if(mesh->HasNormals())
		{
			vertexArray[offset+3] = mesh->mNormals[vertexIndex].x; // NX
			vertexArray[offset+4] = mesh->mNormals[vertexIndex].y; // NY
			vertexArray[offset+5] = mesh->mNormals[vertexIndex].z; // NZ
		}

		if(mesh->HasTextureCoords(0)) // Up to 8 different texture coordinates !
		{
			vertexArray[offset+6] = mesh->mTextureCoords[0][vertexIndex].x; // U
			vertexArray[offset+7] = mesh->mTextureCoords[0][vertexIndex].y; // V
		}

		if (AI_SUCCESS == aiGetMaterialColor(scene->mMaterials[mesh->mMaterialIndex], AI_MATKEY_COLOR_DIFFUSE, &diffuseColor))
		{
			vertexArray[offset+8] =  diffuseColor.r; // R
			vertexArray[offset+9] =  diffuseColor.g; // G
			vertexArray[offset+10] = diffuseColor.b; // B
		}
		else
		{
			diffuseColor.r = 255;
			diffuseColor.g = 0;
			diffuseColor.b = 255;
		}
	}

	indexArray.resize((mesh->mNumFaces * 3), 0.f);

	for(unsigned int faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex)
	{
		aiFace face = mesh->mFaces[faceIndex];

		for(unsigned int indexIndex = 0; indexIndex < face.mNumIndices; ++indexIndex)
		{
			indexArray[(faceIndex*3)+indexIndex] = face.mIndices[indexIndex];
		}
	}

	Material* material = nullptr;

	if(mesh->mMaterialIndex >= 0)
	{
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
		material = new Material();
		material->SetDiffuseColor(vec3(diffuseColor.r, diffuseColor.g, diffuseColor.b));

		LoadTextures(mat, aiTextureType_DIFFUSE,      material, "diffuse");
		LoadTextures(mat, aiTextureType_NORMALS,      material, "normal");
		LoadTextures(mat, aiTextureType_SPECULAR,     material, "specular");
		LoadTextures(mat, aiTextureType_EMISSIVE,     material, "emissive");
		LoadTextures(mat, aiTextureType_DISPLACEMENT, material, "displacement");
		LoadTextures(mat, aiTextureType_HEIGHT,       material, "height");
		LoadTextures(mat, aiTextureType_AMBIENT,      material, "ambient");
		LoadTextures(mat, aiTextureType_LIGHTMAP,     material, "lightmap");
		LoadTextures(mat, aiTextureType_OPACITY,      material, "opacity");
		LoadTextures(mat, aiTextureType_REFLECTION,   material, "reflection");
		LoadTextures(mat, aiTextureType_SHININESS,    material, "shininess");
	}

	auto loadedMesh = new Mesh(vertexArray, indexArray, MeshDrawMode::Triangles, material);
	loadedMesh->SetId(mesh->mName.C_Str());

	return loadedMesh;
}

void LoadTextures(aiMaterial* mat, aiTextureType type, Material* material, const std::string& textureName)
{
	std::vector<Texture*> materialTextures;

	for(unsigned int textureIndex = 0; textureIndex < mat->GetTextureCount(type); ++textureIndex)
	{
		aiString str;
		mat->GetTexture(type, textureIndex, &str);
		bool skip = false;

		auto loadedTextures = App::Data()->GetAll<Texture>();

		for(auto &loadedTexture : loadedTextures)
		{
			if(! skip && loadedTexture->GetPath() == str.C_Str())
			{
				materialTextures.push_back(loadedTexture);
				skip = true;
				break;
			}
		}

		if(! skip)
		{
			materialTextures.push_back(new Texture(std::string(str.C_Str())));
		}
	}

	if(! materialTextures.empty())
	{
		std::stringstream ss;

		for(unsigned int textureIndex = 0; textureIndex < materialTextures.size(); ++textureIndex)
		{
			ss.str(std::string());
			ss << textureName << textureIndex;
			material->SetTexture(ss.str(), materialTextures[textureIndex]);
		}
	}
}

auto MeshLoader::GenerateQuad(Material* material) -> MeshNode*
{
	std::vector<float> vertexArray = {
	//    X      Y      Z      NX    NY    NZ    U     V     R     G     B
		-0.5f, -0.5f,  0.5f,  1.f,  1.f,  1.f,  0.f,  0.f,  1.f,  1.f,  1.f, // BL
		 0.5f, -0.5f,  0.5f,  1.f,  1.f,  1.f,  1.f,  0.f,  1.f,  1.f,  1.f, // BR
		 0.5f,  0.5f,  0.5f,  1.f,  1.f,  1.f,  1.f,  1.f,  1.f,  1.f,  1.f, // TR
		 0.5f,  0.5f,  0.5f,  1.f,  1.f,  1.f,  1.f,  1.f,  1.f,  1.f,  1.f, // TR
		-0.5f,  0.5f,  0.5f,  1.f,  1.f,  1.f,  0.f,  1.f,  1.f,  1.f,  1.f, // TL
		-0.5f, -0.5f,  0.5f,  1.f,  1.f,  1.f,  0.f,  0.f,  1.f,  1.f,  1.f  // BL
	};

	std::vector<unsigned int> indexArray = {
		0, 1, 2,
		3, 4, 5
	};

	return (new MeshNode())->AddMesh(new Mesh(vertexArray, indexArray, MeshDrawMode::Triangles, material));
}
}
