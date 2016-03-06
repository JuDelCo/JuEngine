// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/IObject.hpp"
#include "../Resources/Mesh.hpp"
#include <unordered_map>
#include <memory>

namespace JuEngine
{
class JUENGINEAPI MeshManager : public IObject
{
	public:
		MeshManager();
		~MeshManager();

		void Unload();
		static auto Add(const Identifier& id, const std::string& fileName, const GLenum drawMode = GL_TRIANGLES) -> Mesh*;
		static auto Add(const Identifier& id, const std::vector<float>& vertexArray, const std::vector<unsigned int>& indexArray, const GLenum drawMode) -> Mesh*;
		static auto Get(const Identifier& id) -> Mesh*;
		static void Use(const Identifier& id);
		static void DisableMeshes();

	private:
		std::unordered_map<Identifier, std::shared_ptr<Mesh>> mMeshes;

		// Singleton
		static MeshManager* mInstance;
};
}
