// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "MeshRenderer.hpp"
#include "../Managers/MeshManager.hpp"
#include "../Managers/MaterialManager.hpp"

namespace JuEngine
{
MeshRenderer::MeshRenderer(const string& meshName)
{
	SetMesh(meshName);
}

MeshRenderer::MeshRenderer(const string& meshName, const string& materialName)
{
	SetMesh(meshName);
	SetMaterial(materialName);
}

void MeshRenderer::SetMesh(const string& name)
{
	mMesh = MeshManager::Get(name);
}

void MeshRenderer::SetMaterial(const string& name)
{
	mMaterial = MaterialManager::Get(name);
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

void MeshRenderer::SetForceDraw(const bool force)
{
	mForceDraw = force;
}
}
