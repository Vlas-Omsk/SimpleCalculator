#include "pch.h"
#include "Operators.h"

int Operators::GetPriority(MathTokenType tokenType)
{
	switch (tokenType)
	{
		case MathTokenType::Add:
		case MathTokenType::Sub:
			return 0;
		case MathTokenType::Div:
		case MathTokenType::Mul:
			return 1;
		case MathTokenType::Pow:
		case MathTokenType::Fact:
			return 2;
		default:
			return -1;
	}
}

bool Operators::IsBinary(MathTokenType tokenType)
{
	switch (tokenType)
	{
		case MathTokenType::Add:
		case MathTokenType::Sub:
		case MathTokenType::Div:
		case MathTokenType::Mul:
		case MathTokenType::Pow:
			return true;
		default:
			return false;
	}
}

bool Operators::IsUnary(MathTokenType tokenType)
{
	switch (tokenType)
	{
		case MathTokenType::Add:
		case MathTokenType::Sub:
		case MathTokenType::Fact:
			return true;
		default:
			return false;
	}
}