// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/IObject.hpp"
#include <typeindex>
#include <vector>
#include <memory>

namespace JuEngine
{
class JUENGINEAPI IDataService : public IObject
{
	public:
		template <typename T, typename... TArgs> inline auto Add(const Identifier& id, TArgs&&... args) -> T*;
		template <typename T, typename RealT, typename... TArgs> inline auto Add(const Identifier& id, TArgs&&... args) -> T*;
		template <typename T> inline auto Set(const Identifier& id, T* data) -> T*;
		template <typename T> inline auto Get() -> T*;
		template <typename T> inline auto Get(const Identifier& id) -> T*;
		template <typename T> inline auto GetAll() -> std::vector<T*>;
		template <typename T> inline void Delete(const Identifier& id);
		template <typename T> inline void DeleteAll();
		template <typename T> inline void ForEach(const std::function<void(T*)> function);
		virtual void DeleteAll() = 0;

	private:
		virtual auto Add(const std::type_index type, const Identifier& id, void* asset) -> void* = 0;
		virtual auto Get(const std::type_index type) -> void* = 0;
		virtual auto Get(const std::type_index type, const Identifier& id) -> void* = 0;
		virtual auto GetAll(const std::type_index type) -> std::vector<void*> = 0;
		virtual void Delete(const std::type_index type, const Identifier& id) = 0;
		virtual void DeleteAll(const std::type_index type) = 0;
};

template <typename T, typename... TArgs>
auto IDataService::Add(const Identifier& id, TArgs&&... args) -> T*
{
	return static_cast<T*>(Add(typeid(T), id, new T(std::forward<TArgs>(args)...)));
}

template <typename T, typename RealT, typename... TArgs>
auto IDataService::Add(const Identifier& id, TArgs&&... args) -> T*
{
	return static_cast<T*>(Add(typeid(T), id, new RealT(std::forward<TArgs>(args)...)));
}

template <typename T>
auto IDataService::Set(const Identifier& id, T* data) -> T*
{
	return static_cast<T*>(Add(typeid(T), id, static_cast<T*>(data)));
}

template <typename T>
auto IDataService::Get() -> T*
{
	return static_cast<T*>(Get(typeid(T)));
}

template <typename T>
auto IDataService::Get(const Identifier& id) -> T*
{
	return static_cast<T*>(Get(typeid(T), id));
}

template <typename T>
auto IDataService::GetAll() -> std::vector<T*>
{
	std::vector<T*> data;

	for(auto &obj : GetAll(typeid(T)))
	{
		data.push_back(static_cast<T*>(obj));
	}

	return data;
}

template <typename T>
void IDataService::Delete(const Identifier& id)
{
	Delete(typeid(T), id);
}

template <typename T>
void IDataService::DeleteAll()
{
	DeleteAll(typeid(T));
}

template <typename T>
void IDataService::ForEach(const std::function<void(T*)> function)
{
	for(void* obj : GetAll(typeid(T)))
	{
		function(static_cast<T*>(obj));
	}
}
}
