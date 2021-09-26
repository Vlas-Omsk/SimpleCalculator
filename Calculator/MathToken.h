#pragma once
#include "MathTokenType.h"

#include <string>

class MathToken
{
private:
	void ThrowNonConvertible(MathTokenType tokenType);
public:
	MathTokenType Type;
	int Position;
	int Length;
	void* Value;

	MathToken();
	MathToken(MathTokenType type, int position, int length, void* value);
	std::string GetStringValue();
	double GetNumberValue();
};