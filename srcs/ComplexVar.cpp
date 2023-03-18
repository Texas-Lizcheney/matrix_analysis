#include <complexvar.h>

void setvalue_frompolar(double r, double a, ComplexVar &x)
{
    x.real = cos(a) * r;
    x.imag = sin(a) * r;
    x.isArbitrary = false;
    return;
}
