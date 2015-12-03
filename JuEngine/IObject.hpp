// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "DllExport.hpp"
#include <memory>
#include <string>

namespace JuEngine
{
using std::unique_ptr;
using std::shared_ptr;
using std::weak_ptr;
using std::string;

class JUENGINEAPI IObject
{
public:
	IObject() : mObjectName("unnamed object") {}
	IObject(const string& name) : mObjectName(name) {}
	virtual ~IObject() = default;

	auto GetName() const -> const string& { return mObjectName; }
	void SetName(const string& name) { mObjectName = name; }

protected:
	string mObjectName;

private:
	// NonCopyable
	IObject(const IObject&);
	IObject& operator =(const IObject&);
};
}
