#include "pch.h"
#include "MemoryStream.h"

MemoryStream::MemoryStream(const char* buffer, long length)
{
	_buffer = buffer;
	_length = length;
}

MemoryStream::MemoryStream(std::string buffer)
{
	_buffer = (new std::string(buffer))->c_str();
	_length = buffer.length();
}

long MemoryStream::GetLength()
{
	return _length;
}

int MemoryStream::Peek()
{
	if (IsEndOfStream())
		return -1;
	else
		return _buffer[Position];
}

int MemoryStream::Read()
{
	int value = Peek();
	Position++;
	return value;
}

bool MemoryStream::IsEndOfStream()
{
	return Position >= _length;
}