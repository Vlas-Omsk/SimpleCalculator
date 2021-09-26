#pragma once
#include "IStreamReader.h"

#include <string>

class MemoryStream: public IStreamReader
{
private:
	const char* _buffer;
	long _length;
public:
	MemoryStream(const char* buffer, long length);
	MemoryStream(std::string buffer);
	long GetLength();
	int Peek() override;
	int Read() override;
	bool IsEndOfStream() override;
};