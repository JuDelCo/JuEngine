// Copyright (c) 2016 Juan Delgado (JuDelCo)
// License: GPLv3 License
// GPLv3 License web page: http://www.gnu.org/licenses/gpl.txt

#include "Identifier.hpp"

namespace JuEngine
{
Identifier::Identifier(const int& r)
{
	mIntRef = r;

	mStringRef = std::to_string(mIntRef);
}

Identifier::Identifier(const char* r) : Identifier(std::string(r))
{
}

Identifier::Identifier(const std::string& r) : mStringRef(r)
{
	mIsInteger = false;

	std::hash<std::string> hash_fn;
	mIntRef = hash_fn(mStringRef);
}

bool Identifier::IsInteger() const
{
	return mIsInteger;
}

auto Identifier::GetIntRef() const -> const int&
{
	return mIntRef;
}

auto Identifier::GetStringRef() const -> const std::string&
{
	return mStringRef;
}

bool Identifier::operator ==(const Identifier right) const
{
	if(this->IsInteger() != right.IsInteger())
	{
		return false;
	}

	if(this->IsInteger())
	{
		return this->GetIntRef() == right.GetIntRef();
	}
	else
	{
		return this->GetStringRef() == right.GetStringRef();
	}
}

bool Identifier::operator !=(const Identifier right) const
{
	return !(*this == right);
}

bool Identifier::operator ==(const int& right) const
{
	if(this->IsInteger())
	{
		return this->GetIntRef() == right;
	}

	return false;
}

bool Identifier::operator !=(const int& right) const
{
	return !(*this == right);
}

bool Identifier::operator ==(const std::string& right) const
{
	if(! this->IsInteger())
	{
		return this->GetStringRef() == right;
	}

	return false;
}

bool Identifier::operator !=(const std::string& right) const
{
	return !(*this == right);
}

bool Identifier::operator ==(const char* right) const
{
	return (*this == std::string(right));
}

bool Identifier::operator !=(const char* right) const
{
	return !(*this == right);
}

Identifier::Identifier()
{
}
}
