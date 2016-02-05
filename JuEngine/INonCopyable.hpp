// Copyright (c) 2015 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "DllExport.hpp"

namespace JuEngine
{
class JUENGINEAPI INonCopyable
{
public:
	INonCopyable() {}
	virtual ~INonCopyable() = default;

private:
	// NonCopyable
	INonCopyable(const INonCopyable&);
	INonCopyable& operator =(const INonCopyable&);
};
}
