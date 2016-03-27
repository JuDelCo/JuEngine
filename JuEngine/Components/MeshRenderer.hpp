// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Entity/IComponent.hpp"
#include "../Resources/MeshNode.hpp"
#include "../Resources/Shader.hpp"

namespace JuEngine
{
class JUENGINEAPI MeshRenderer : public IComponent
{
	public:
		auto GetMeshNode() -> MeshNode*;
		auto GetShader() -> Shader*;

		void Reset(const std::string& meshNodeName, const std::string& shaderName);

	private:
		MeshNode* mMeshNode;
		Shader* mShader;

};
}
