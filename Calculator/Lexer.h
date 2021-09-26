#pragma once
#include "IStreamReader.h"
#include "IEnumerator.h"
#include "MathToken.h"

#include <string>

class Lexer: public IEnumerator<MathToken>
{
private:
	IStreamReader* _streamReader;
	int _position;
	int _startPosition;
	MathToken _token;
	std::string _buffer;
	int _current;

	bool HasNext();
	char GetNextChar();
	bool HasCurrent();
	char GetCurrentChar();
	void ReadNext();
	void Get();
	void ReadWhiteSpace();
	void ReadNumber();
	void ReadString();
public:
	bool SkipInvisible;

	Lexer(IStreamReader* streamReader);
	IStreamReader* GetStreamReader();
	bool MoveNext() override;
	void Reset() override;
	MathToken GetCurrent() override;
};