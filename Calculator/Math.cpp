#include "pch.h"
#include "Math.h"

#include <yvals_core.h>
#include <stdlib.h>

bool Math::TryParseDouble(const char* str, double* value)
{
    int& _Errno_ref = errno; // Nonzero cost, pay it once
    char* _Eptr;
    _Errno_ref = 0;
    *value = _CSTD strtod(str, &_Eptr);

    if (str == _Eptr || _Errno_ref == ERANGE)
        return false;

    return true;
}

// copied
double Math::Fact(int n)
{
    if (n < 0)
        return 0;
    if (n == 0)
        return 1;
    else
        return n * Fact(n - 1);
}