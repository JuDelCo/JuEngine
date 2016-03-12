// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Resources/IObject.hpp"

namespace JuEngine
{
class JUENGINEAPI ILogService : public IObject
{
	public:
		virtual void Log(const std::string& message, ...) = 0;
		virtual void Debug(const std::string& message, ...) = 0;
		virtual void Info(const std::string& message, ...) = 0;
		virtual void Notice(const std::string& message, ...) = 0;
		virtual void Warning(const std::string& message, ...) = 0;
		virtual void Error(const std::string& message, ...) = 0;

		virtual void RunTimeError(const std::string& function, int line) = 0;
};
}
