#pragma once
#include "IEnumerator.h"
#include "IStreamReader.h"
#include "Lexer.h"
#include "Math.h"
#include "MathToken.h"
#include "MathTokenType.h"
#include "MathTokenTypes.h"
#include "MemoryStream.h"
#include "Operators.h"

#include <string>
#include <vector>
#include <algorithm>

class Calculator
{
private:
	int _generation;
	std::vector<MathToken> _tokens;

	void ReplaceTokenRange(int begin, int end, double value);
	void RecursiveCalculate(long position, bool calledFromFunction);
	void RecursiveCalculateFunction(long position);
	bool GetConstant(std::string constantName, double* value);
	bool CalculateFunction(std::string functionName, std::vector<double> arguments, double* value);
	void CalculateOperators(long startPosition, long& endPosition);
	short int CalculateBinaryOperator(long index, long startPosition, long endPosition);
	short int CalculateUnaryOperator(long index, long startPosition, long endPosition);
public:
	Calculator();
	double Calculate(std::string str);
	double Calculate(IStreamReader* streamReader);
	double Calculate(Lexer* lexer);
	double Calculate(std::vector<MathToken> tokens);
};