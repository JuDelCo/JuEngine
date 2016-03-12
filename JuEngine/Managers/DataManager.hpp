// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Services/IDataService.hpp"
#include <unordered_map>

namespace JuEngine
{
class JUENGINEAPI DataManager : public IDataService
{
	public:
		DataManager();

		void DeleteAll();

	private:
		auto Add(const std::type_index type, const Identifier& id, void* asset) -> void*;
		auto Get(const std::type_index type) -> void*;
		auto Get(const std::type_index type, const Identifier& id) -> void*;
		auto GetAll(const std::type_index type) -> std::vector<void*>;
		void Delete(const std::type_index type, const Identifier& id);
		void DeleteAll(const std::type_index type);

		std::unordered_map<std::type_index, std::unordered_map<Identifier, std::shared_ptr<void>>> mAssets;
};
}
