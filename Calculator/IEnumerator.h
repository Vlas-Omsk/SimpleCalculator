#pragma once
#include <vector>

template <class T>
class IEnumerator
{
public:
	std::vector<T> ToVector()
	{
		std::vector<T> vector;
		while (MoveNext())
			vector.push_back(GetCurrent());
		return vector;
	}
	virtual bool MoveNext() = 0;
	virtual void Reset() = 0;
	virtual T GetCurrent() = 0;
};