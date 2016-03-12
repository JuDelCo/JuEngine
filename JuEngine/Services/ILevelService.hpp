// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/IObject.hpp"
#include <typeindex>
#include <memory>

namespace JuEngine
{
class Level;

class JUENGINEAPI ILevelService : public IObject
{
	public:
		virtual void Update() = 0;
		template <typename T> void Add();
		template <typename T> void LoadLevel();
		template <typename T> void LoadLevelAdditive();
		virtual void LoadLevel(const Identifier& id) = 0;
		virtual void LoadLevelAdditive(const Identifier& id) = 0;
		virtual void UnloadLevel() = 0;
		virtual void DeleteAll() = 0;

	private:
		template <typename T> auto Get() -> T*;
		virtual void Add(std::shared_ptr<Level> level, std::type_index type) = 0;
		virtual void LoadLevel(Level* level) = 0;
		virtual void LoadLevelAdditive(Level* level) = 0;
		virtual auto Get(std::type_index type) -> Level* = 0;
};

template <typename T>
void ILevelService::Add()
{
	Add(std::make_shared<T>(), typeid(T));
}

template <typename T>
void ILevelService::LoadLevel()
{
	LoadLevel(Get(typeid(T)));
}

template <typename T>
void ILevelService::LoadLevelAdditive()
{
	LoadLevelAdditive(Get(typeid(T)));
}

template<typename T>
auto ILevelService::Get() -> T*
{
	return static_cast<T*>(Get(typeid(T)));
}
}
