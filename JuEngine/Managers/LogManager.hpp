// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../Services/ILogService.hpp"

namespace JuEngine
{
class JUENGINEAPI LogManager : public ILogService
{
	public:
		LogManager();
		~LogManager();

		void Log(const std::string& message, ...);
		void Debug(const std::string& message, ...);
		void Info(const std::string& message, ...);
		void Notice(const std::string& message, ...);
		void Warning(const std::string& message, ...);
		void Error(const std::string& message, ...);

		void RunTimeError(const std::string& function, int line);

	private:
		void RemoveLogIfEmpty(const std::string& fileName);
};
}
