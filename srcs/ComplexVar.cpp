#include <complexvar.h>

void setvalue_frompolar(double r, double a, ComplexVar &x)
{
    x.real = cos(a) * r;
    x.imag = sin(a) * r;
    x.isArbitrary = false;
    return;
}

ComplexVar ComplexVar_add(const ComplexVar &x, const ComplexVar &y)
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

ComplexVar ComplexVar_sub(const ComplexVar &x, const ComplexVar &y)
{
    ComplexVar result;
    if (x.isArbitrary || y.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        result.real = x.real - y.real;
        result.imag = x.imag - y.imag;
        result.isArbitrary = false;
    }
    return result;
}

ComplexVar ComplexVar_mul(const ComplexVar &x, const ComplexVar &y)
{
    ComplexVar result;
    if (x.isArbitrary || y.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        result.real = x.real * y.real - x.imag * y.imag;
        result.imag = x.imag * y.real + x.real * y.imag;
        result.isArbitrary = false;
    }
    return result;
}

ComplexVar ComplexVar_div(const ComplexVar &x, const ComplexVar &y)
{
    ComplexVar result;
    if (x.isArbitrary || y.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        double l = ComplexVar_L2(y);
        if (l == 0)
        {
            PyErr_SetString(PyExc_ZeroDivisionError, "Devide by zero.");
        }
        result.real = (x.real * y.real + x.imag * y.imag) / l;
        result.imag = (x.imag * y.real - x.real * y.imag) / l;
        result.isArbitrary = false;
    }
    return result;
}

ComplexVar ComplexVar_fdv(const ComplexVar &x, const ComplexVar &y)
{
    ComplexVar result;
    if (x.isArbitrary || y.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        result = ComplexVar_div(x, y);
        result.real = std::floor(result.real);
        result.imag = std::floor(result.imag);
    }
    return result;
}

ComplexVar ComplexVar_mod(const ComplexVar &x, const ComplexVar &y)
{
    ComplexVar result;
    if (x.isArbitrary || y.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        ComplexVar tmp = ComplexVar_fdv(x, y);
        result = ComplexVar_sub(x, ComplexVar_mul(tmp, y));
    }
    return result;
}

ComplexVar ComplexVar_exp(const ComplexVar &x)
{
    ComplexVar result;
    if (x.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        setvalue_frompolar(std::exp(x.real), x.imag, result);
    }
    return result;
}

ComplexVar ComplexVar_ln(const ComplexVar &x)
{
    ComplexVar result;
    if (x.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        result.isArbitrary = false;
        if (ComplexVar_iszero(x))
        {
            PyErr_SetString(PyExc_ValueError, "Ln domain error.");
            result.real = std::nan("");
            result.imag = std::nan("");
        }
        else
        {
            result.real = std::log(ComplexVar_length(x));
            result.imag = ComplexVar_arg(x);
        }
    }
    return result;
}

ComplexVar ComplexVar_log(const ComplexVar &x, const ComplexVar &y)
{
    return ComplexVar_div(ComplexVar_ln(x), ComplexVar_ln(y));
}

ComplexVar ComplexVar_pow(const ComplexVar &x, const ComplexVar &y)
{
    ComplexVar result;
    if (x.isArbitrary || y.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        if (ComplexVar_iszero(x))
        {
            result = {1, 0, false};
        }
        else
        {
            result = ComplexVar_exp(ComplexVar_mul(y, ComplexVar_ln(x)));
            result.isArbitrary = false;
        }
    }
    return result;
}

ComplexVar ComplexVar_neg(const ComplexVar &x)
{
    ComplexVar result;
    if (x.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        result.real = -x.real;
        result.imag = -x.imag;
        result.isArbitrary = false;
    }
    return result;
}

ComplexVar ComplexVar_ivt(const ComplexVar &x)
{
    ComplexVar result;
    if (x.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        double l = ComplexVar_L2(x);
        if (l == 0)
        {
            PyErr_SetString(PyExc_ZeroDivisionError, "Devide by zero.");
        }
        result.real = (x.real + x.imag) / l;
        result.imag = (x.real - x.imag) / l;
        result.isArbitrary = false;
    }
    return result;
}