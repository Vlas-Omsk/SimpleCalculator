#include "pch.h"
#include "MathTokenTypes.h"

#include <stdexcept>

std::string MathTokenTypes::GetName(MathTokenType tokenType)
{
	switch (tokenType)
	{
		case MathTokenType::Invalid:
			return "Invalid";
		case MathTokenType::Invisible:
			return "Invisible";
		case MathTokenType::String:
			return "String";
		case MathTokenType::Number:
			return "Number";
		case MathTokenType::OpenBracket:
			return "OpenBracket";
		case MathTokenType::CloseBracket:
			return "CloseBracket";
		case MathTokenType::Add:
			return "Add";
		case MathTokenType::Sub:
			return "Sub";
		case MathTokenType::Div:
			return "Div";
		case MathTokenType::Mul:
			return "Mul";
		case MathTokenType::Equ:
			return "Equ";
		case MathTokenType::Pow:
			return "Pow";
		case MathTokenType::Fact:
			return "Fact";
		default:
			throw std::runtime_error("Unknown token type");
	}
}
