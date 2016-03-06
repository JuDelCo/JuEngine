// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../DllExport.hpp"
#include "Matcher.hpp"
#include "GroupEventType.hpp"

namespace JuEngine
{
struct JUENGINEAPI TriggerOnEvent
{
	public:
		TriggerOnEvent(const Matcher trigger, const GroupEventType eventType)
		{
			this->trigger = trigger;
			this->eventType = eventType;
		}

		Matcher trigger;
		GroupEventType eventType;
};
}
