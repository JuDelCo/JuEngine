// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "MeshRenderer.hpp"
#include "../App.hpp"
#include "../Services/IDataService.hpp"

namespace JuEngine
{
void MeshRenderer::Reset(const std::string& meshNodeName, const std::string& shaderName)
{
	mMeshNode = App::Data()->Get<MeshNode>(meshNodeName);
	mShader = App::Data()->Get<Shader>(shaderName);
}

auto MeshRenderer::GetMeshNode() -> MeshNode*
{
	return mMeshNode;
}

auto MeshRenderer::GetShader() -> Shader*
{
	return mShader;
}
}
