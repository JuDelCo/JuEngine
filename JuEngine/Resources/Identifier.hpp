// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#pragma once

#include "../DllExport.hpp"
#include <string>

namespace JuEngine
{
class JUENGINEAPI Identifier
{
	public:
		Identifier(const int& r);
		Identifier(const char* r);
		Identifier(const std::string& r);

		bool IsInteger() const;
		auto GetIntRef() const -> const int&;
		auto GetStringRef() const -> const std::string&;

		bool operator ==(const Identifier right) const;
		bool operator !=(const Identifier right) const;
		bool operator ==(const int& right) const;
		bool operator !=(const int& right) const;
		bool operator ==(const std::string& right) const;
		bool operator !=(const std::string& right) const;
		bool operator ==(const char* right) const;
		bool operator !=(const char* right) const;

	protected:
		Identifier();

	private:
		bool mIsInteger{true};
		int mIntRef;
		std::string mStringRef;
};
}

namespace std
{
template <>
struct hash<JuEngine::Identifier>
{
	std::size_t operator()(const JuEngine::Identifier& id) const
	{
		if(id.IsInteger())
		{
			return hash<int>()(id.GetIntRef());
		}

		return hash<string>()(id.GetStringRef());
	}
};
}
