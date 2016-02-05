// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../ECS/Component.hpp"
#include "../Resources/Mesh.hpp"
#include "../Resources/Material.hpp"

namespace JuEngine
{
class JUENGINEAPI MeshRenderer : public Component
{
friend class WindowManager;

public:
	MeshRenderer() = default;
	MeshRenderer(const string& meshName);
	MeshRenderer(const string& meshName, const string& materialName);
	virtual ~MeshRenderer() = default;

	void SetMesh(const string& name);
	void SetMaterial(const string& name);
	auto GetMesh() -> Mesh*;
	auto GetMaterial() -> Material*;
	auto GetForceDraw() -> const bool&;
	void SetForceDraw(const bool force);

private:
	Mesh* mMesh;
	Material* mMaterial;
	bool mForceDraw{false};
};
}
