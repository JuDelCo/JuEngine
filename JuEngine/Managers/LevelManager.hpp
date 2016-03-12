// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Services/ILevelService.hpp"
#include <unordered_map>
#include <memory>

namespace JuEngine
{
class JUENGINEAPI LevelManager : public ILevelService
{
	public:
		LevelManager();
		~LevelManager();

		void Update();
		void LoadLevel(const Identifier& id);
		void LoadLevelAdditive(const Identifier& id);
		void UnloadLevel();
		void DeleteAll();

	private:
		void Add(std::shared_ptr<Level> level, std::type_index type);
		void LoadLevel(Level* level);
		void LoadLevelAdditive(Level* level);
		auto Get(std::type_index type) -> Level*;

		std::unordered_map<std::type_index, std::shared_ptr<Level>> mLevels;
		std::type_index mRequestedLoadLevelType{typeid(void)};
		Identifier* mRequestedLoadLevelName{nullptr};
		bool mRequestedLoadLevel{false};
		bool mLoadAdditive{false};
};
}
