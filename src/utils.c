#include "utils.h"


int approx_equal(long double d1, long double d2)
{
    const long double epsilon = 0.000000000001;  // To compare long double values
    if (fabsl(d1-d2) < epsilon)
        return 1;
    else
        return 0;
}
