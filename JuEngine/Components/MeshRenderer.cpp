// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "MeshRenderer.hpp"
#include "../Managers/MeshManager.hpp"
#include "../Managers/MaterialManager.hpp"

namespace JuEngine
{
void MeshRenderer::Reset(const std::string& meshName, const std::string& materialName, const bool forceDraw)
{
	mMesh = MeshManager::Get(meshName);
	mMaterial = MaterialManager::Get(materialName);
	mForceDraw = forceDraw;
}

auto MeshRenderer::GetMesh() -> Mesh*
{
	return mMesh;
}

auto MeshRenderer::GetMaterial() -> Material*
{
	return mMaterial;
}

auto MeshRenderer::GetForceDraw() -> const bool&
{
	return mForceDraw;
}
}
