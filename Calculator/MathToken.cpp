#include "pch.h"
#include "MathToken.h"
#include "MathTokenTypes.h"

#include <stdexcept>

MathToken::MathToken()
{
	Type = MathTokenType::Invalid;
	Position = 0;
	Length = 0;
	Value = nullptr;
}

MathToken::MathToken(MathTokenType type, int position, int length, void* value)
{
	Type = type;
	Position = position;
	Length = length;
	Value = value;
}

void MathToken::ThrowNonConvertible(MathTokenType tokenType)
{
	throw std::runtime_error(("Unable to convert MathTokenType::" + MathTokenTypes::GetName(Type) + " to MathTokenType::" + MathTokenTypes::GetName(tokenType)).c_str());
}

double MathToken::GetNumberValue()
{
	if (Type != MathTokenType::Number)
		ThrowNonConvertible(MathTokenType::Number);
	return *(double*)Value;
}

std::string MathToken::GetStringValue()
{
	if (Type != MathTokenType::String)
		ThrowNonConvertible(MathTokenType::String);
	return *(std::string*)Value;
}
