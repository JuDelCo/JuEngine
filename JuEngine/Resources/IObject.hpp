// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "INonCopyable.hpp"
#include "Identifier.hpp"

namespace JuEngine
{
class JUENGINEAPI IObject : public INonCopyable
{
	public:
		IObject() : mId("unnamed object") {}
		IObject(const Identifier& id) : mId(id) {}

		auto GetId() const -> const Identifier& { return mId; }
		void SetId(const Identifier& id) { mId = id; }

	protected:
		Identifier mId;
};
}
