// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/IObject.hpp"
#include "../Resources/Material.hpp"
#include <unordered_map>
#include <memory>

namespace JuEngine
{
class JUENGINEAPI MaterialManager : public IObject
{
	public:
		MaterialManager();
		~MaterialManager();

		void Unload();
		static auto Add(const Identifier& id, const std::string& textureName, const std::string& shaderName) -> Material*;
		static auto Get(const Identifier& id) -> Material*;
		static void Use(const Identifier& id);
		static void DisableMaterials();

	private:
		std::unordered_map<Identifier, std::shared_ptr<Material>> mMaterials;

		// Singleton
		static MaterialManager* mInstance;
};
}
