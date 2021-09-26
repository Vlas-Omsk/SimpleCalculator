#include "pch.h"
#include "Lexer.h"
#include "Math.h"
#include "MathTokenType.h"

#include <algorithm>

Lexer::Lexer(IStreamReader* streamReader)
{
	_streamReader = streamReader;
	_position = 0;
	_startPosition = 0;
	_current = -1;
	SkipInvisible = true;
}

bool Lexer::HasNext()
{
	return _streamReader->Peek() != -1;
}

char Lexer::GetNextChar()
{
	return _streamReader->Peek();
}

bool Lexer::HasCurrent()
{
	return _current != -1;
}

char Lexer::GetCurrentChar()
{
	return _current;
}

void Lexer::ReadNext()
{
	_position++;
	_current = _streamReader->Read();
	if (HasCurrent())
		_buffer += GetCurrentChar();
}

void Lexer::Get()
{
	_token = MathToken();
	_buffer.clear();
	ReadNext();
	_token.Position = _startPosition = _position;

	switch (_current) {
		case '+':
			_token.Type = MathTokenType::Add;
			break;
		case '-':
			_token.Type = MathTokenType::Sub;
			break;
		case '/':
			_token.Type = MathTokenType::Div;
			break;
		case '*':
			_token.Type = MathTokenType::Mul;
			break;
		case '^':
			_token.Type = MathTokenType::Pow;
			break;
		case '!':
			_token.Type = MathTokenType::Fact;
			break;
		case '=':
			_token.Type = MathTokenType::Equ;
			break;
		case ',':
			_token.Type = MathTokenType::Comma;
			break;
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
		case '0':
			ReadNumber();
			break;
		case '(':
			_token.Type = MathTokenType::OpenBracket;
			break;
		case ')':
			_token.Type = MathTokenType::CloseBracket;
			break;
		default:
			if (isspace(_current))
				ReadWhiteSpace();
			else
				ReadString();
			break;
	}
}

void Lexer::ReadWhiteSpace()
{
	while (HasNext() && isspace(GetNextChar()))
		ReadNext();
	_token.Type = MathTokenType::Invisible;
}

const char numberChars[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '.', 'X', 'x', 'E', 'e' },
hexadecimalChars[] = { 'a', 'b', 'c', 'd', 'f', 'A', 'B', 'C', 'D', 'F' };

bool IsNumberChar(char value)
{
	const char* element = std::find(std::begin(numberChars), std::end(numberChars), value);
	return element != std::end(numberChars);
}

bool IsHexadecimalChar(char value)
{
	const char* element = std::find(std::begin(hexadecimalChars), std::end(hexadecimalChars), value);
	return element != std::end(hexadecimalChars);
}

void Lexer::ReadNumber()
{
	bool isDouble = false, isEnumber = false, isHexadecimal = false;
	int previous = _current;
	while (HasNext() && (IsNumberChar(GetNextChar()) || (isHexadecimal && IsHexadecimalChar(GetNextChar()))))
	{
		previous = _current;
		ReadNext();

		if (GetCurrentChar() == '.' && !isHexadecimal)
		{
			if (isDouble)
				throw std::exception("Invalid double number");
			else
				isDouble = true;
		}
		if ((GetCurrentChar() == 'e' || GetCurrentChar() == 'E') && !isHexadecimal)
		{
			if (isEnumber)
				throw std::exception("Invalid e number");
			else
				isEnumber = true;
		}
		if (GetCurrentChar() == 'x' || GetCurrentChar() == 'X')
		{
			if (isHexadecimal || previous != '0' || _buffer.size() > 2)
				throw std::exception("Invalid hexadecimal number");
			else
				isHexadecimal = true;
		}
	}

	_token.Value = new double();
	std::replace(_buffer.begin(), _buffer.end(), '.', ',');
	if (!Math::TryParseDouble(_buffer.c_str(), (double*)_token.Value))
	{
		delete _token.Value;
		throw std::exception(("Invalid or too big number " + _buffer).c_str());
	}
	_token.Type = MathTokenType::Number;
}

void Lexer::ReadString()
{
	while (isalpha(GetNextChar()))
		ReadNext();
	_token.Value = new std::string(_buffer);
	_token.Type = MathTokenType::String;
}

IStreamReader* Lexer::GetStreamReader()
{
	return _streamReader;
}

bool Lexer::MoveNext()
{
	if (!_streamReader->IsEndOfStream())
	{
		Get();
		if (_token.Type == MathTokenType::Invalid)
			throw std::exception("InvalidTokenException");
		if (SkipInvisible && _token.Type == MathTokenType::Invisible)
			MoveNext();
		return true;
	}
	return false;
}

void Lexer::Reset()
{
	_streamReader->SetPosition(0);
	_position = 0;
	_startPosition = 0;
}

MathToken Lexer::GetCurrent()
{
	return _token;
}