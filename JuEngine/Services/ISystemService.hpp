// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/IObject.hpp"
#include <memory>

namespace JuEngine
{
class ISystem;

class JUENGINEAPI ISystemService : public IObject
{
	friend class AppController;
	friend class LevelManager;

	public:
		virtual void Add(std::shared_ptr<ISystem> system) = 0;
		template <typename T> inline void Add();

	protected:
		virtual void Initialize() = 0;
		virtual void Execute() = 0;
		virtual void FixedExecute() = 0;
		virtual void Reset() = 0;
};

template <typename T>
void ISystemService::Add()
{
	Add(std::shared_ptr<T>(new T()));
}
}
