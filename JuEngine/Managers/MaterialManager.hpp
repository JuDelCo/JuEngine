// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../IObject.hpp"
#include "../Resources/Material.hpp"
#include <unordered_map>

namespace JuEngine
{
using std::unordered_map;

class JUENGINEAPI MaterialManager : public IObject
{
public:
	MaterialManager();
	virtual ~MaterialManager();

	void Unload();
	static auto Add(const string& name, const string& textureName, const string& shaderName) -> Material*;
	static auto Get(const string& name) -> Material*;
	static void Use(const string& name);
	static void DisableMaterials();

private:
	unordered_map<string, shared_ptr<Material>> mMaterials;

	// Singleton
	static MaterialManager* mInstance;
};
}
