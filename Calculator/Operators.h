#pragma once
#include "MathTokenType.h"

class Operators
{
public:
	static const int MaxPriority = 2;
	static int GetPriority(MathTokenType tokenType);
	static bool IsBinary(MathTokenType tokenType);
	static bool IsUnary(MathTokenType tokenType);
};