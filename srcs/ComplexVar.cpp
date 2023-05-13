#include <complexvar.h>

ComplexVar One = {1, 0, false};
ComplexVar I = {0, 1, false};
ComplexVar negI = {0, -1, false};

ComplexVar::ComplexVar(const double &x, const double &y, bool z) : real(x),
                                                                   imag(y),
                                                                   isArbitrary(z)
{
}

ComplexVar::ComplexVar(const error_double &x, const error_double &y, bool z) : real(x),
                                                                               imag(y),
                                                                               isArbitrary(z)
{
}

ComplexVar::ComplexVar(const ComplexVar &n) noexcept : real(n.real),
                                                       imag(n.imag),
                                                       isArbitrary(n.isArbitrary)
{
}

void ComplexVar::operator=(const ComplexVar &x)
{
    real = x.real;
    imag = x.imag;
    isArbitrary = x.isArbitrary;
}

void ComplexVar::operator=(const double &x)
{
    real = x;
    imag = 0;
    isArbitrary = false;
}

ComplexVar &ComplexVar::operator+=(const ComplexVar &x)
{
    if (isArbitrary || x.isArbitrary)
    {
        real = 0;
        imag = 0;
        isArbitrary = true;
    }
    else
    {
        real += x.real;
        imag += x.imag;
    }
    return *this;
}

ComplexVar &ComplexVar::operator-=(const ComplexVar &x)
{
    if (isArbitrary || x.isArbitrary)
    {
        real = 0;
        imag = 0;
        isArbitrary = true;
    }
    else
    {
        real -= x.real;
        imag -= x.imag;
    }
    return *this;
}

ComplexVar &ComplexVar::operator*=(const ComplexVar &x)
{
    if (ComplexVar_iszero((*this)) || ComplexVar_iszero(x))
    {
        real = 0;
        imag = 0;
        isArbitrary = false;
    }
    else if (isArbitrary || x.isArbitrary)
    {
        real = 0;
        imag = 0;
        isArbitrary = true;
    }
    else
    {
        error_double tmpr = real * x.real - imag * x.imag;
        error_double tmpi = imag * x.real + real * x.imag;
        real = tmpr;
        imag = tmpi;
    }
    return *this;
}

ComplexVar &ComplexVar::operator/=(const ComplexVar &x)
{
    if (ComplexVar_iszero((*this)))
    {
        real = 0;
        imag = 0;
        isArbitrary = false;
    }
    else if (ComplexVar_iszero(x))
    {
        real = std::nan("");
        imag = std::nan("");
        isArbitrary = false;
        PyErr_WarnEx(PyExc_RuntimeWarning, "Dividing 0.", 2);
    }
    else if (isArbitrary || x.isArbitrary)
    {
        real = 0;
        imag = 0;
        isArbitrary = true;
    }
    else
    {
        error_double l = ComplexVar_squaredL2(x);
        error_double tmpr = real * x.real + imag * x.imag;
        error_double tmpi = imag * x.real - real * x.imag;
        real = tmpr / l;
        imag = tmpi / l;
    }
    return *this;
}

ComplexVar &ComplexVar::operator%=(const ComplexVar &x)
{
    ComplexVar tmp = (*this) - ComplexVar_fdv((*this), x) * x;
    real = tmp.real;
    imag = tmp.imag;
    isArbitrary = tmp.isArbitrary;
    return *this;
}

error_double ComplexVar_L1(const ComplexVar &x)
{
    return abs(x.real) + abs(x.imag);
}

error_double ComplexVar_squaredL2(const ComplexVar &x)
{
    return x.real * x.real + x.imag * x.imag;
}

error_double ComplexVar_L2(const ComplexVar &x)
{
    return sqrt(ComplexVar_squaredL2(x));
}

error_double ComplexVar_arg(const ComplexVar &x)
{
    return atan2(x.imag, x.real);
}

void setvalue_frompolar(const error_double &r, const error_double &a, ComplexVar &x)
{
    x.real = cos(a) * r;
    x.imag = sin(a) * r;
    return;
}

std::ostream &operator<<(std::ostream &s, const ComplexVar &x)
{
    if (x.isArbitrary)
    {
        s << "Unsure";
    }
    else
    {
        s << x.real;
        if (!std::signbit(x.imag.value) || std::isnan(x.imag.value))
        {
            s << '+';
        }
        s << x.imag << 'j';
    }
    return s;
}

std::ostream &operator<(std::ostream &s, const ComplexVar &x)
{
    if (x.isArbitrary)
    {
        s << "undefined";
    }
    else
    {
        s << x.real;
        if (!std::signbit(x.imag.value) || std::isnan(x.imag.value))
        {
            s << '+';
        }
        s << x.imag << 'i';
    }
    return s;
}

ComplexVar ComplexVar_conj(const ComplexVar &x)
{
    ComplexVar result = x;
    result.imag = -result.imag;
    return result;
}

void ComplexVar_iconj(ComplexVar &x)
{
    x.imag = -x.imag;
    return;
}

ComplexVar operator+(const ComplexVar &x, const ComplexVar &y)
{
    ComplexVar result;
    if (x.isArbitrary || y.isArbitrary)
    {
        result = {0, 0, true};
    }
    else
    {
        result.real = x.real + y.real;
        result.imag = x.imag + y.imag;
        result.isArbitrary = false;
    }
    return result;
}

ComplexVar operator-(const ComplexVar &x, const ComplexVar &y)
{
    ComplexVar result;
    if (x.isArbitrary || y.isArbitrary)
    {
        result = {0, 0, true};
    }
    else
    {
        result.real = x.real - y.real;
        result.imag = x.imag - y.imag;
        result.isArbitrary = false;
    }
    return result;
}

ComplexVar operator*(const ComplexVar &x, const ComplexVar &y)
{
    ComplexVar result;
    if (ComplexVar_iszero(x) || ComplexVar_iszero(y))
    {
        result = {0, 0, false};
    }
    else if (x.isArbitrary || y.isArbitrary)
    {
        result = {0, 0, true};
    }
    else
    {
        result.real = x.real * y.real - x.imag * y.imag;
        result.imag = x.imag * y.real + x.real * y.imag;
        result.isArbitrary = false;
    }
    return result;
}

ComplexVar operator/(const ComplexVar &x, const ComplexVar &y)
{
    ComplexVar result;
    if (ComplexVar_iszero(x))
    {
        result = {0, 0, false};
    }
    else if (ComplexVar_iszero(y))
    {
        result = {std::nan(""), std::nan(""), false};
        PyErr_WarnEx(PyExc_RuntimeWarning, "Dividing 0.", 2);
    }
    else if (x.isArbitrary || y.isArbitrary)
    {
        result = {0, 0, true};
    }
    else
    {
        error_double l = ComplexVar_squaredL2(y);
        result.real = (x.real * y.real + x.imag * y.imag) / l;
        result.imag = (x.imag * y.real - x.real * y.imag) / l;
        result.isArbitrary = false;
    }
    return result;
}

ComplexVar ComplexVar_fdv(const ComplexVar &x, const ComplexVar &y)
{
    ComplexVar result;
    result = x / y;
    result.real = round(result.real);
    result.imag = round(result.imag);
    return result;
}

ComplexVar operator%(const ComplexVar &x, const ComplexVar &y)
{
    ComplexVar result;
    ComplexVar tmp = ComplexVar_fdv(x, y);
    result = x - tmp * y;
    return result;
}

ComplexVar ComplexVar_exp(const ComplexVar &x)
{
    ComplexVar result;
    if (x.isArbitrary)
    {
        result = {0, 0, true};
    }
    else
    {
        setvalue_frompolar(exp(x.real), x.imag, result);
        result.isArbitrary = false;
    }
    return result;
}

ComplexVar ComplexVar_ln(const ComplexVar &x)
{
    ComplexVar result;
    if (x.isArbitrary)
    {
        result = {0, 0, true};
    }
    else
    {
        if (ComplexVar_iszero(x))
        {
            result = {std::nan(""), std::nan(""), false};
            PyErr_WarnEx(PyExc_RuntimeWarning, "Out of domain", 2);
        }
        else
        {
            result.isArbitrary = false;
            error_double l = ComplexVar_L2(x);
            result.real = log(l);
            result.imag = ComplexVar_arg(x);
        }
    }
    return result;
}

ComplexVar ComplexVar_log(const ComplexVar &x, const ComplexVar &y)
{
    return ComplexVar_ln(x) / ComplexVar_ln(y);
}

ComplexVar ComplexVar_pow(const ComplexVar &x, const ComplexVar &y)
{
    ComplexVar result;
    if (x.isArbitrary || y.isArbitrary)
    {
        result = {0, 0, true};
    }
    else
    {
        if (ComplexVar_iszero(x))
        {
            result = One;
        }
        else
        {
            result = ComplexVar_exp(y * ComplexVar_ln(x));
        }
    }
    return result;
}

ComplexVar ComplexVar_neg(const ComplexVar &x)
{
    ComplexVar result;
    if (x.isArbitrary)
    {
        result = {0, 0, true};
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
        result = {0, 0, true};
    }
    else
    {
        error_double l = ComplexVar_squaredL2(x);
        if (l == 0)
        {
            result = {std::nan(""), std::nan(""), false};
            PyErr_WarnEx(PyExc_RuntimeWarning, "Divide by 0.", 2);
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
        setvalue_frompolar(sqrt(ComplexVar_L2(x)), ComplexVar_arg(x) / 2, result);
        result.isArbitrary = false;
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
        ComplexVar tmp = ComplexVar_exp(I * x);
        result = (tmp - ComplexVar_ivt(tmp)) * a;
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
        ComplexVar tmp = ComplexVar_exp(I * x);
        result = (tmp + ComplexVar_ivt(tmp)) * a;
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
        ComplexVar tmp = ComplexVar_exp(i2 * x);
        result = (tmp - One) / (tmp + One) * negI;
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
        result = negI * ComplexVar_ln(I * x + ComplexVar_sqrt(One - x * x));
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
        result = negI * ComplexVar_ln(x - ComplexVar_sqrt(x * x - One));
        if ((x.real.value >= 0 && x.imag.value >= 0) || (x.real.value < 0 && x.imag.value < 0))
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
        ComplexVar tmp = I * x;
        result = a * ComplexVar_ln((One + tmp) / (One - tmp));
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
        result = (tmp - ComplexVar_ivt(tmp)) * a;
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
        result = (tmp + ComplexVar_ivt(tmp)) * a;
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
        ComplexVar tmp = ComplexVar_exp(i2 * x);
        result = (tmp - One) / (tmp + One);
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
        result = ComplexVar_ln(x + ComplexVar_sqrt(One + x * x));
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
        result = ComplexVar_ln(x - ComplexVar_sqrt(x * x - One));
    }
    if (x.real.value >= 0)
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
        result = a * ComplexVar_ln((One + x) / (One - x));
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