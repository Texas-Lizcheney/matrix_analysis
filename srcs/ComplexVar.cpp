#include <complexvar.h>

ComplexVar One = {1, 0, false};
ComplexVar I = {0, 1, false};
ComplexVar negI = {0, -1, false};
extern int doubleprecision;

ComplexVar::ComplexVar(double x, double y, bool z) : real(x), imag(y), isArbitrary(z)
{
}

ComplexVar::ComplexVar(const ComplexVar &n) noexcept : real(n.real), imag(n.imag), isArbitrary(n.isArbitrary)
{
}

void setvalue_frompolar(double r, double a, ComplexVar &x)
{
    x.real = cos(a) * r;
    x.imag = sin(a) * r;
    x.isArbitrary = false;
    return;
}

std::stringstream ComplexVar_repr(const ComplexVar &x)
{
    std::stringstream tmp;
    if (x.isArbitrary)
    {
        tmp << "Unsure";
    }
    else
    {
        tmp << std::setprecision(doubleprecision) << x.real;
        if (!std::signbit(x.imag) || std::isnan(x.imag))
        {
            tmp << '+';
        }
        tmp << x.imag << 'j';
    }
    return tmp;
}

std::stringstream ComplexVar_str(const ComplexVar &x)
{
    std::stringstream tmp;
    if (x.isArbitrary)
    {
        tmp << "undefined";
    }
    else
    {
        tmp << std::setprecision(doubleprecision) << x.real;
        if (!std::signbit(x.imag) || std::isnan(x.imag))
        {
            tmp << '+';
        }
        tmp << x.imag << 'i';
    }
    return tmp;
}

ComplexVar ComplexVar_conj(const ComplexVar &x)
{
    ComplexVar result = x;
    result.imag = -result.imag;
    return result;
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
    if (ComplexVar_iszero(x) || ComplexVar_iszero(y))
    {
        result.real = 0;
        result.imag = 0;
        result.isArbitrary = false;
    }
    else if (x.isArbitrary || y.isArbitrary)
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
    if (ComplexVar_iszero(x))
    {
        result.real = 0;
        result.imag = 0;
        result.isArbitrary = false;
    }
    else if (ComplexVar_iszero(y))
    {
        result.real = std::nan("");
        result.imag = std::nan("");
        result.isArbitrary = false;
        PyErr_WarnEx(PyExc_RuntimeWarning, "Divide by 0.", 2);
    }
    else if (x.isArbitrary || y.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        double l = ComplexVar_L2(y);
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
        result.real = std::round(result.real);
        result.imag = std::round(result.imag);
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
            result.real = std::nan("");
            result.imag = std::nan("");
            result.isArbitrary = false;
            return result;
        }
        result.real = x.real / l;
        result.imag = -x.imag / l;
        result.isArbitrary = false;
    }
    return result;
}

ComplexVar ComplexVar_sqrt(const ComplexVar &x)
{
    ComplexVar result;
    if (x.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        setvalue_frompolar(sqrt(ComplexVar_length(x)), ComplexVar_arg(x) / 2, result);
    }
    return result;
}

ComplexVar ComplexVar_sin(const ComplexVar &x)
{
    ComplexVar result;
    static ComplexVar a = {0, -0.5, false};
    if (x.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        ComplexVar tmp = ComplexVar_exp(ComplexVar_mul(I, x));
        result = ComplexVar_mul(ComplexVar_sub(tmp, ComplexVar_ivt(tmp)), a);
    }
    return result;
}

ComplexVar ComplexVar_cos(const ComplexVar &x)
{
    ComplexVar result;
    static ComplexVar a = {0.5, 0, false};
    if (x.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        ComplexVar tmp = ComplexVar_exp(ComplexVar_mul(I, x));
        result = ComplexVar_mul(ComplexVar_add(tmp, ComplexVar_ivt(tmp)), a);
    }
    return result;
}

ComplexVar ComplexVar_tan(const ComplexVar &x)
{
    ComplexVar result;
    static ComplexVar i2 = {0, 2, false};
    if (x.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        ComplexVar tmp = ComplexVar_exp(ComplexVar_mul(i2, x));
        result = ComplexVar_mul(ComplexVar_div(ComplexVar_sub(tmp, One), ComplexVar_add(tmp, One)), negI);
    }
    return result;
}

ComplexVar ComplexVar_cot(const ComplexVar &x)
{
    ComplexVar result;
    if (x.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        result = ComplexVar_ivt(ComplexVar_tan(x));
    }
    return result;
}

ComplexVar ComplexVar_sec(const ComplexVar &x)
{
    ComplexVar result;
    if (x.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        result = ComplexVar_ivt(ComplexVar_cos(x));
    }
    return result;
}

ComplexVar ComplexVar_csc(const ComplexVar &x)
{
    ComplexVar result;
    if (x.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        result = ComplexVar_ivt(ComplexVar_sin(x));
    }
    return result;
}

ComplexVar ComplexVar_arcsin(const ComplexVar &x)
{
    ComplexVar result;
    if (x.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        result = ComplexVar_mul(negI, ComplexVar_ln(ComplexVar_add(ComplexVar_mul(I, x), ComplexVar_sqrt(ComplexVar_sub(One, ComplexVar_mul(x, x))))));
    }
    return result;
}

ComplexVar ComplexVar_arccos(const ComplexVar &x)
{
    ComplexVar result;
    if (x.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        result = ComplexVar_mul(negI, ComplexVar_ln(ComplexVar_sub(x, ComplexVar_sqrt(ComplexVar_sub(ComplexVar_mul(x, x), One)))));
        if ((x.real >= 0 && x.imag >= 0) || (x.real < 0 && x.imag < 0))
        {
            result.real = -result.real;
            result.imag = -result.imag;
        }
    }
    return result;
}

ComplexVar ComplexVar_arctan(const ComplexVar &x)
{
    ComplexVar result;
    static ComplexVar a = {0, -0.5, false};
    if (x.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        ComplexVar tmp = ComplexVar_mul(I, x);
        result = ComplexVar_mul(a, ComplexVar_ln(ComplexVar_div(ComplexVar_add(One, tmp), ComplexVar_sub(One, tmp))));
    }
    return result;
}

ComplexVar ComplexVar_arccot(const ComplexVar &x)
{
    ComplexVar result;
    if (x.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        if (ComplexVar_iszero(x))
        {
            result.real = std::numbers::pi / 2;
            result.imag = 0;
            result.isArbitrary = false;
        }
        else
        {
            result = ComplexVar_arctan(ComplexVar_ivt(x));
        }
    }
    return result;
}

ComplexVar ComplexVar_arcsec(const ComplexVar &x)
{
    ComplexVar result;
    if (x.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        if (ComplexVar_iszero(x))
        {
            result.real = std::nan("");
            result.imag = std::nan("");
            result.isArbitrary = false;
        }
        else
        {
            result = ComplexVar_arccos(ComplexVar_ivt(x));
        }
    }
    return result;
}

ComplexVar ComplexVar_arccsc(const ComplexVar &x)
{
    ComplexVar result;
    if (x.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        if (ComplexVar_iszero(x))
        {
            result.real = std::nan("");
            result.imag = std::nan("");
            result.isArbitrary = false;
        }
        else
        {
            result = ComplexVar_arcsin(ComplexVar_ivt(x));
        }
    }
    return result;
}

ComplexVar ComplexVar_sinh(const ComplexVar &x)
{
    ComplexVar result;
    static ComplexVar a = {0.5, 0, false};
    if (x.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        ComplexVar tmp = ComplexVar_exp(x);
        result = ComplexVar_mul(ComplexVar_sub(tmp, ComplexVar_ivt(tmp)), a);
    }
    return result;
}

ComplexVar ComplexVar_cosh(const ComplexVar &x)
{
    ComplexVar result;
    static ComplexVar a = {0.5, 0, false};
    if (x.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        ComplexVar tmp = ComplexVar_exp(x);
        result = ComplexVar_mul(ComplexVar_add(tmp, ComplexVar_ivt(tmp)), a);
    }
    return result;
}

ComplexVar ComplexVar_tanh(const ComplexVar &x)
{
    ComplexVar result;
    static ComplexVar i2 = {2, 0, false};
    if (x.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        ComplexVar tmp = ComplexVar_exp(ComplexVar_mul(i2, x));
        result = ComplexVar_div(ComplexVar_sub(tmp, One), ComplexVar_add(tmp, One));
    }
    return result;
}

ComplexVar ComplexVar_coth(const ComplexVar &x)
{
    ComplexVar result;
    if (x.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        result = ComplexVar_ivt(ComplexVar_tanh(x));
    }
    return result;
}

ComplexVar ComplexVar_sech(const ComplexVar &x)
{
    ComplexVar result;
    if (x.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        result = ComplexVar_ivt(ComplexVar_cosh(x));
    }
    return result;
}

ComplexVar ComplexVar_csch(const ComplexVar &x)
{
    ComplexVar result;
    if (x.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        result = ComplexVar_ivt(ComplexVar_sinh(x));
    }
    return result;
}

ComplexVar ComplexVar_arcsinh(const ComplexVar &x)
{
    ComplexVar result;
    if (x.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        result = ComplexVar_ln(ComplexVar_add(x, ComplexVar_sqrt(ComplexVar_add(One, ComplexVar_mul(x, x)))));
    }
    return result;
}

ComplexVar ComplexVar_arccosh(const ComplexVar &x)
{
    ComplexVar result;
    if (x.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        result = ComplexVar_ln(ComplexVar_sub(x, ComplexVar_sqrt(ComplexVar_sub(ComplexVar_mul(x, x), One))));
    }
    if (x.real >= 0)
    {
        result.real = -result.real;
        result.imag = -result.imag;
    }
    return result;
}

ComplexVar ComplexVar_arctanh(const ComplexVar &x)
{
    ComplexVar result;
    static ComplexVar a = {0.5, 0, false};
    if (x.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        result = ComplexVar_mul(a, ComplexVar_ln(ComplexVar_div(ComplexVar_add(One, x), ComplexVar_sub(One, x))));
    }
    return result;
}

ComplexVar ComplexVar_arccoth(const ComplexVar &x)
{
    ComplexVar result;
    if (x.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        if (ComplexVar_iszero(x))
        {
            result.real = 0;
            result.imag = std::nan("");
            result.isArbitrary = false;
        }
        else
        {
            result = ComplexVar_arctanh(ComplexVar_ivt(x));
        }
    }
    return result;
}

ComplexVar ComplexVar_arcsech(const ComplexVar &x)
{
    ComplexVar result;
    if (x.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        if (ComplexVar_iszero(x))
        {
            result.real = std::nan("");
            result.imag = std::nan("");
            result.isArbitrary = false;
        }
        else
        {
            result = ComplexVar_arccosh(ComplexVar_ivt(x));
        }
    }
    return result;
}

ComplexVar ComplexVar_arccsch(const ComplexVar &x)
{
    ComplexVar result;
    if (x.isArbitrary)
    {
        result.isArbitrary = true;
    }
    else
    {
        if (ComplexVar_iszero(x))
        {
            result.real = std::nan("");
            result.imag = std::nan("");
            result.isArbitrary = false;
        }
        else
        {
            result = ComplexVar_arcsinh(ComplexVar_ivt(x));
        }
    }
    return result;
}