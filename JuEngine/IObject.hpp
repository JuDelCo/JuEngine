// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "INonCopyable.hpp"
#include "StdDefines.hpp"

namespace JuEngine
{
class JUENGINEAPI IObject : public INonCopyable
{
public:
	IObject() : mObjectName("unnamed object") {}
	IObject(const string& name) : mObjectName(name) {}
	virtual ~IObject() = default;

	auto GetName() const -> const string& { return mObjectName; }
	void SetName(const string& name) { mObjectName = name; }

protected:
	string mObjectName;
};
}
