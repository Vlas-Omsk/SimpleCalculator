#pragma once

class IStreamReader
{
public:
	virtual int Peek() = 0;
	virtual int Read() = 0;
	virtual bool IsEndOfStream() = 0;
	virtual long GetPosition();
	virtual void SetPosition(long value);
protected:
	long Position = 0;
};