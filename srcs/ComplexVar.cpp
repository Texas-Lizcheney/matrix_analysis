#include <complexvar.h>

void setvalue_frompolar(double r, double a, ComplexVar &x)
{
    x.real = cos(a) * r;
    x.imag = sin(a) * r;
    x.isArbitrary = false;
    return;
}

ComplexVar ComplexVar_add(ComplexVar &x, ComplexVar &y)
{
    ComplexVar result;
    if (x.isArbitrary || y.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        result.real = x.real + y.real;
        result.imag = x.imag + y.imag;
        result.isArbitrary = false;
    }
    return result;
}