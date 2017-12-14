#include "utils.h"


int approx_equal(double d1, double d2)
{
    const double epsilon = 0.0000001;  // To compare double values
    if (fabs(d1-d2) < epsilon)
        return 1;
    else
        return 0;
}
