// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Entity/IComponent.hpp"
#include "../Resources/Mesh.hpp"
#include "../Resources/Material.hpp"

namespace JuEngine
{
class JUENGINEAPI MeshRenderer : public IComponent
{
	public:
		auto GetMesh() -> Mesh*;
		auto GetMaterial() -> Material*;
		auto GetForceDraw() -> const bool&;

		void Reset(const std::string& meshName, const std::string& materialName, const bool forceDraw = false);

	private:
		Mesh* mMesh;
		Material* mMaterial;
		bool mForceDraw{false};
};
}
