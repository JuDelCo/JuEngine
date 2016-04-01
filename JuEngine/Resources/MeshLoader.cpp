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
auto ProcessNode(aiNode* node, const aiScene* scene, const MeshVertexFormat meshVertexFormat, const std::string& folderPath) -> MeshNode*;
auto ProcessMesh(aiMesh* mesh, const aiScene* scene, const MeshVertexFormat meshVertexFormat, const std::string& folderPath) -> Mesh*;
void LoadTextures(aiMaterial* mat, aiTextureType type, Material* material, const std::string& textureName, const std::string& folderPath);

auto MeshLoader::Load(const std::string& filePath, const MeshVertexFormat meshVertexFormat, const MeshDrawMode drawMode) -> MeshNode*
{
	std::string folderPath = filePath.substr(0, filePath.find_last_of('/') + 1);

	Assimp::Importer modelImporter;

	const aiScene* scene = modelImporter.ReadFile(filePath,
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

	auto rootMeshNode = ProcessNode(scene->mRootNode, scene, meshVertexFormat, folderPath);

	modelImporter.FreeScene();

	return rootMeshNode;
}

auto ProcessNode(aiNode* node, const aiScene* scene, const MeshVertexFormat meshVertexFormat, const std::string& folderPath) -> MeshNode*
{
	auto meshNode = new MeshNode();
	meshNode->SetId(node->mName.C_Str());

	for(unsigned int meshIndex = 0; meshIndex < node->mNumMeshes; ++meshIndex)
	{
		meshNode->AddMesh(ProcessMesh(scene->mMeshes[node->mMeshes[meshIndex]], scene, meshVertexFormat, folderPath));
	}

	for(unsigned int subNodeIndex = 0; subNodeIndex < node->mNumChildren; ++subNodeIndex)
	{
		meshNode->AddMeshNode(ProcessNode(node->mChildren[subNodeIndex], scene, meshVertexFormat, folderPath));
	}

	return meshNode;
}

auto ProcessMesh(aiMesh* mesh, const aiScene* scene, const MeshVertexFormat meshVertexFormat, const std::string& folderPath) -> Mesh*
{
	unsigned int NumVertexAttr = Mesh::GetNumVertexAttr(meshVertexFormat);
	std::vector<float> vertexArray;
	std::vector<uint32_t> indexArray;
	aiColor4D diffuseColor;
	float* floatPackedValue;
	uint32_t packedValue;
	int16_t nx, ny, nz;

	vertexArray.resize((mesh->mNumVertices * NumVertexAttr), 0.f);

	for(unsigned int vertexIndex = 0; vertexIndex < mesh->mNumVertices; ++vertexIndex)
	{
		unsigned int offset = vertexIndex * NumVertexAttr;

		if(mesh->HasPositions())
		{
			vertexArray[offset+0] = mesh->mVertices[vertexIndex].x; // X
			vertexArray[offset+1] = mesh->mVertices[vertexIndex].y; // Y
			vertexArray[offset+2] = mesh->mVertices[vertexIndex].z; // Z

			offset += 3;
		}
		else
		{
			ThrowRuntimeError("Error, mesh %s's' vertices doesn't have positions (in %s)", mesh->mName.C_Str(), folderPath.c_str());
		}

		if((meshVertexFormat == MeshVertexFormat::PositionNormalColor || meshVertexFormat == MeshVertexFormat::PositionNormalTexture))
		{
			if(mesh->HasNormals())
			{
				nz = Math::ConvertRange(-1, 1, -512, 511, mesh->mNormals[vertexIndex].z);
				ny = Math::ConvertRange(-1, 1, -512, 511, mesh->mNormals[vertexIndex].y);
				nx = Math::ConvertRange(-1, 1, -512, 511, mesh->mNormals[vertexIndex].x);;

				packedValue = 0;
				packedValue = packedValue | ((nz >= 0 ? (nz & 0x1FF) : (0x3FF &~ ((abs(nz) - 1) & 0x1FF))) << 20); // NZ
				packedValue = packedValue | ((ny >= 0 ? (ny & 0x1FF) : (0x3FF &~ ((abs(ny) - 1) & 0x1FF))) << 10); // NY
				packedValue = packedValue | ((nx >= 0 ? (nx & 0x1FF) : (0x3FF &~ ((abs(nx) - 1) & 0x1FF))) << 0);  // NX

				floatPackedValue = (float*)&packedValue;
				vertexArray[offset+0] = *floatPackedValue;
			}

			offset += 1;
		}

		if((meshVertexFormat == MeshVertexFormat::PositionTextureColor || meshVertexFormat == MeshVertexFormat::PositionNormalTexture || meshVertexFormat == MeshVertexFormat::PositionTexture))
		{
			if(mesh->HasTextureCoords(0)) // Up to 8 different texture coordinates !
			{
				vertexArray[offset+0] = mesh->mTextureCoords[0][vertexIndex].x; // U
				vertexArray[offset+1] = mesh->mTextureCoords[0][vertexIndex].y; // V
			}

			offset += 2;
		}

		if ((meshVertexFormat == MeshVertexFormat::PositionColor || meshVertexFormat == MeshVertexFormat::PositionTextureColor || meshVertexFormat == MeshVertexFormat::PositionNormalColor))
		{
			if(AI_SUCCESS == aiGetMaterialColor(scene->mMaterials[mesh->mMaterialIndex], AI_MATKEY_COLOR_DIFFUSE, &diffuseColor))
			{
				packedValue = 0;
				packedValue = packedValue | (((unsigned int)(diffuseColor.a * 3.f)) << 30);    // A
				packedValue = packedValue | (((unsigned int)(diffuseColor.b * 1023.f)) << 20); // B
				packedValue = packedValue | (((unsigned int)(diffuseColor.g * 1023.f)) << 10); // G
				packedValue = packedValue | (((unsigned int)(diffuseColor.r * 1023.f)) << 0);  // R

				floatPackedValue = (float*)&packedValue;
				vertexArray[offset+0] = *floatPackedValue;
			}
			else
			{
				packedValue = 0;
				packedValue = packedValue | (((unsigned int)(3)) << 30);    // A
				packedValue = packedValue | (((unsigned int)(1023)) << 20); // B
				packedValue = packedValue | (((unsigned int)(0)) << 10);    // G
				packedValue = packedValue | (((unsigned int)(1023)) << 0);  // R

				floatPackedValue = (float*)&packedValue;
				vertexArray[offset+0] = *floatPackedValue;
			}

			offset += 1;
		}
	}

	indexArray.resize((mesh->mNumFaces * 3), 0.f); // Triangles only !

	for(unsigned int faceIndex = 0; faceIndex < mesh->mNumFaces; ++faceIndex)
	{
		aiFace face = mesh->mFaces[faceIndex];

		for(unsigned int indexIndex = 0; indexIndex < face.mNumIndices; ++indexIndex)
		{
			indexArray[(faceIndex*3)+indexIndex] = face.mIndices[indexIndex]; // Triangles only !
		}
	}

	Material* material = nullptr;

	if(mesh->mMaterialIndex >= 0)
	{
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];
		material = new Material();
		material->SetDiffuseColor(vec3(diffuseColor.r, diffuseColor.g, diffuseColor.b));

		LoadTextures(mat, aiTextureType_DIFFUSE,      material, "diffuse", folderPath);
		LoadTextures(mat, aiTextureType_NORMALS,      material, "normal", folderPath);
		LoadTextures(mat, aiTextureType_SPECULAR,     material, "specular", folderPath);
		LoadTextures(mat, aiTextureType_EMISSIVE,     material, "emissive", folderPath);
		LoadTextures(mat, aiTextureType_DISPLACEMENT, material, "displacement", folderPath);
		LoadTextures(mat, aiTextureType_HEIGHT,       material, "height", folderPath);
		LoadTextures(mat, aiTextureType_AMBIENT,      material, "ambient", folderPath);
		LoadTextures(mat, aiTextureType_LIGHTMAP,     material, "lightmap", folderPath);
		LoadTextures(mat, aiTextureType_OPACITY,      material, "opacity", folderPath);
		LoadTextures(mat, aiTextureType_REFLECTION,   material, "reflection", folderPath);
		LoadTextures(mat, aiTextureType_SHININESS,    material, "shininess", folderPath);
	}

	auto loadedMesh = new Mesh(vertexArray, indexArray, MeshDrawMode::Triangles, meshVertexFormat, material);
	loadedMesh->SetId(mesh->mName.C_Str());

	return loadedMesh;
}

void LoadTextures(aiMaterial* mat, aiTextureType type, Material* material, const std::string& textureName, const std::string& folderPath)
{
	std::vector<Texture*> materialTextures;

	for(unsigned int textureIndex = 0; textureIndex < mat->GetTextureCount(type); ++textureIndex)
	{
		aiString str;
		mat->GetTexture(type, textureIndex, &str);

		auto texturePath = std::string(str.C_Str());
		std::replace(texturePath.begin(), texturePath.end(), '\\', '/');
		texturePath = folderPath + texturePath;
		bool skip = false;

		for(auto &texture : App::Data()->GetAll<Texture>())
		{
			if(texture->GetPath() == texturePath)
			{
				materialTextures.push_back(texture);
				skip = true;
				break;
			}
		}

		if(! skip)
		{
			materialTextures.push_back(App::Data()->Set<Texture>(texturePath, new Texture(texturePath)));
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
	//    X      Y      Z      U     V
		-0.5f, -0.5f,  0.5f,  0.f,  0.f, // BL
		 0.5f, -0.5f,  0.5f,  1.f,  0.f, // BR
		 0.5f,  0.5f,  0.5f,  1.f,  1.f, // TR
		 0.5f,  0.5f,  0.5f,  1.f,  1.f, // TR
		-0.5f,  0.5f,  0.5f,  0.f,  1.f, // TL
		-0.5f, -0.5f,  0.5f,  0.f,  0.f  // BL
	};

	std::vector<unsigned int> indexArray = {
		0, 1, 2,
		3, 4, 5
	};

	return (new MeshNode())->AddMesh(new Mesh(vertexArray, indexArray, MeshDrawMode::Triangles, MeshVertexFormat::PositionTexture, material));
}
}
