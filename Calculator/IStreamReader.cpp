#include "pch.h"
#include "IStreamReader.h"

long IStreamReader::GetPosition()
{
	return Position;
}

void IStreamReader::SetPosition(long value)
{
	Position = value;
}