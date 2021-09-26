#include "pch.h"
#include "Calculator.h"
#include "MemoryStream.h"
#include "Operators.h"
#include "Math.h"

#define _USE_MATH_DEFINES
#include <math.h>

Calculator::Calculator()
{
	_generation = 0;
}

double Calculator::Calculate(std::string str)
{
	return Calculate(new MemoryStream(str));
}

double Calculator::Calculate(IStreamReader* streamReader)
{
	return Calculate(new Lexer(streamReader));
}

double Calculator::Calculate(Lexer* lexer)
{
	return Calculate(lexer->ToVector());
}

double Calculator::Calculate(std::vector<MathToken> tokens)
{
	_tokens = tokens;
	_generation = 0;

	RecursiveCalculate(0, false);
	if (_tokens.size() != 1 || _tokens[0].Type != MathTokenType::Number)
		throw std::exception("Syntax error");
	double result = _tokens[0].GetNumberValue();

	_tokens.clear();

	return result;
}

void Calculator::ReplaceTokenRange(int begin, int end, double value)
{
	_tokens[begin] = MathToken(MathTokenType::Number, 0, 0, new double(value));

	auto first = _tokens.cbegin() + begin + 1;
	auto last = _tokens.cbegin() + end;

	_tokens.erase(first, last);
}

void Calculator::RecursiveCalculate(long position, bool calledFromFunction)
{
	_generation++;
	long startPosition = position;
	long endPosition = 0;

	for (long i = position; i < (long)_tokens.size(); i++)
	{
		if (_tokens[i].Type == MathTokenType::String)
			RecursiveCalculateFunction(i);
		if (_tokens[i].Type == MathTokenType::OpenBracket)
			RecursiveCalculate(i + 1, false);
		if (_tokens[i].Type == MathTokenType::CloseBracket || (calledFromFunction && _tokens[i].Type == MathTokenType::Comma))
		{
			if (_generation == 1)
				throw std::exception("Syntax error");
			endPosition = i;
			break;
		}
	}
	if (_generation != 1)
	{
		if (endPosition == 0)
			throw std::exception("Syntax error");
	}
	else
		endPosition = _tokens.size() - 1;

	CalculateOperators(startPosition, endPosition);

	if (!calledFromFunction && _generation != 1)
		ReplaceTokenRange(startPosition - 1, startPosition + 2, _tokens[startPosition].GetNumberValue());

	_generation--;
}

void Calculator::RecursiveCalculateFunction(long position)
{
	_generation++;
	long startPosition = position;
	long endPosition = 0;
	std::vector<double> arguments;

	double value = 0;
	bool hasValue = false;
	std::string functionName = _tokens[position].GetStringValue();

	hasValue = GetConstant(functionName, &value);
	if (hasValue)
		endPosition = startPosition;

	if (!hasValue)
	{
		if (position + 1 >= (long)_tokens.size())
			throw std::exception("Syntax error");

		if (_tokens[position + 1].Type == MathTokenType::Number || _tokens[position + 1].Type == MathTokenType::String)
		{
			if (_tokens[position + 1].Type == MathTokenType::String)
			{
				if (GetConstant(_tokens[position + 1].GetStringValue(), &value))
					arguments.push_back(value);
				else
					throw std::exception("Syntax error");
			}
			else
				arguments.push_back(_tokens[position + 1].GetNumberValue());
			endPosition = position + 1;
		}
		else
		{
			for (long i = position + 1; i < (long)_tokens.size(); i++)
			{
				if (_tokens[i].Type == MathTokenType::Comma || _tokens[i].Type == MathTokenType::OpenBracket)
				{
					if (_tokens[i].Type == MathTokenType::OpenBracket && i != position + 1)
						throw std::exception("Syntax error");
					RecursiveCalculate(i + 1, true);
					if (_tokens[i + 1].Type != MathTokenType::Number)
						throw std::exception("Internal error");
					arguments.push_back(_tokens[i + 1].GetNumberValue());
				}
				if (_tokens[i].Type == MathTokenType::CloseBracket)
				{
					endPosition = i;
					break;
				}
			}
			if (endPosition == 0)
				throw std::exception("Syntax error");
		}

		hasValue = CalculateFunction(functionName, arguments, &value);
	}

	if (!hasValue)
		throw std::exception("No function found with such parameters");

	ReplaceTokenRange(startPosition, endPosition + 1, value);

	_generation--;
}

bool Calculator::GetConstant(std::string constantName, double* value)
{
	if (constantName == "pi")
	{
		*value = M_PI;
		return true;
	}
	else if (constantName == "e")
	{
		*value = M_E;
		return true;
	}
	return false;
}

double Calculator::CalculateFunction(std::string functionName, std::vector<double> arguments, double* value)
{
	if (arguments.size() == 1)
	{
		if (functionName == "sqrt")
		{
			*value = sqrt(arguments[0]);
			return true;
		}
	}
	else if (arguments.size() >= 1)
	{
		if (functionName == "min")
		{
			*value = *std::min_element(arguments.begin(), arguments.end());
			return true;
		}
	}
	return false;
}

void Calculator::CalculateOperators(long startPosition, long& endPosition)
{
	double value = 0;
	long length = 0;

	for (int priority = Operators::MaxPriority; priority >= 0; priority--)
		for (long i = startPosition; i <= endPosition; i++)
			if (Operators::IsUnary(_tokens[i].Type) && Operators::GetPriority(_tokens[i].Type) == priority)
			{
				long offset = CalculateUnaryOperator(i, startPosition, endPosition);
				i -= offset;
				endPosition -= offset;
			}

	for (int priority = Operators::MaxPriority; priority >= 0; priority--)
		for (long i = startPosition; i <= endPosition; i++)
			if (Operators::IsBinary(_tokens[i].Type) && Operators::GetPriority(_tokens[i].Type) == priority)
			{
				long offset = CalculateBinaryOperator(i, startPosition, endPosition);
				i -= offset;
				endPosition -= offset;
			}
}

short int Calculator::CalculateBinaryOperator(long index, long startPosition, long endPosition)
{
	if (index + 1 > endPosition || _tokens[index + 1].Type != MathTokenType::Number)
		throw std::exception("Syntax error");
	if (index - 1 < startPosition || _tokens[index - 1].Type != MathTokenType::Number)
		throw std::exception("Syntax error");

	double value = 0;

	double left = _tokens[index - 1].GetNumberValue();
	double right = _tokens[index + 1].GetNumberValue();

	switch (_tokens[index].Type)
	{
		case MathTokenType::Add:
			value = left + right;
			break;
		case MathTokenType::Sub:
			value = left - right;
			break;
		case MathTokenType::Div:
			value = left / right;
			break;
		case MathTokenType::Mul:
			value = left * right;
			break;
		case MathTokenType::Pow:
			value = pow(left, right);
			break;
		default:
			throw std::exception("Internal error");
	}

	ReplaceTokenRange(index - 1, index + 2, value);

	return 2;
}

short int Calculator::CalculateUnaryOperator(long index, long startPosition, long endPosition)
{
	double value = 0;
	long length = 0;

	switch (_tokens[index].Type)
	{
		case MathTokenType::Add:
		case MathTokenType::Sub:
			if (index - 1 >= startPosition && _tokens[index - 1].Type == MathTokenType::Number)
				return 0;
			if (index + 1 > endPosition || _tokens[index + 1].Type != MathTokenType::Number)
				throw std::exception("Syntax error");

			value = _tokens[index + 1].GetNumberValue();
			length = 2;

			switch (_tokens[index].Type)
			{
				case MathTokenType::Add:
					break;
				case MathTokenType::Sub:
					value = -value;
					break;
			}
			break;
		case MathTokenType::Fact:
			if (index - 1 < startPosition || _tokens[index - 1].Type != MathTokenType::Number ||
				(index + 1 <= endPosition && _tokens[index + 1].Type == MathTokenType::Number))
				throw std::exception("Syntax error");

			value = Math::Fact((int)_tokens[index - 1].GetNumberValue());
			index--;
			length = 2;
			break;
		default:
			throw std::exception("Internal error");
	}

	ReplaceTokenRange(index, index + length, value);

	return 1;
}