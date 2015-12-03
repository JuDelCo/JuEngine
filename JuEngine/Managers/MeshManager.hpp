// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../IObject.hpp"
#include "../Resources/Mesh.hpp"
#include <unordered_map>

namespace JuEngine
{
using std::unordered_map;

class JUENGINEAPI MeshManager : public IObject
{
public:
	MeshManager();
	virtual ~MeshManager();

	void Unload();
	static auto Add(const string& name, const string& fileName, const GLenum drawMode = GL_TRIANGLES) -> Mesh*;
	static auto Add(const string& name, const vector<float>& vertexArray, const vector<unsigned int>& indexArray, const GLenum drawMode) -> Mesh*;
	static auto Get(const string& name) -> Mesh*;
	static void Use(const string& name);
	static void DisableMeshes();

private:
	unordered_map<string, shared_ptr<Mesh>> mMeshes;

	// Singleton
	static MeshManager* mInstance;
};
}
