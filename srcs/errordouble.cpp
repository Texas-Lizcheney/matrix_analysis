#include <errordouble.h>
extern bool print_error;
error_double edHalfPi = {1.5707963267948966192313216916397514420985846996875529104874722961539};
error_double edPi = {std::numbers::pi_v<double>};

error_double::error_double() noexcept
    : value(0),
      error(0)
{
}

template <is_double T>
error_double::error_double(T value) noexcept
    : value(value),
      error(cal_original_error(value))
{
}

error_double::error_double(int64_t v) noexcept
    : value(v),
      error(0)
{
    if (abs(value) >= (1LL << 54))
    {
        error = cal_original_error(value);
    }
}

error_double::error_double(double value, double error) noexcept
    : value(value),
      error(error)
{
}

error_double::error_double(const error_double &x) noexcept
    : value(x.value),
      error(x.error)
{
}

error_double error_double::operator+(const error_double &other) const
{
    return error_double(value + other.value, error + other.error);
}

error_double operator+(const double &self, const error_double &other)
{
    return error_double(self + other.value, cal_original_error(self) + other.error);
}

error_double &error_double::operator+=(const error_double &other)
{
    value += other.value;
    error += other.error;
    return *this;
}

error_double error_double::operator+() const
{
    return error_double(value, error);
}

error_double error_double::operator-(const error_double &other) const
{
    return error_double(value - other.value, error + other.error);
}

error_double operator-(const double &self, const error_double &other)
{
    return error_double(self - other.value, cal_original_error(self) + other.error);
}

error_double error_double::operator-() const
{
    return error_double(-value, error);
}

error_double &error_double::operator-=(const error_double &other)
{
    value -= other.value;
    error += other.error;
    return *this;
}

error_double error_double::operator*(const error_double &other) const
{
    return error_double(value * other.value, other.value * error + value * other.error);
}

error_double operator*(const double &self, const error_double &other)
{
    return error_double(self * other.value, other.value * cal_original_error(self) + self * other.error);
}

error_double &error_double::operator*=(const error_double &other)
{
    error = error * other.value + other.error * value;
    value *= other.value;
    return *this;
}

error_double error_double::operator/(const error_double &other) const
{
    if (other == 0)
    {
        PyErr_WarnEx(PyExc_RuntimeWarning, "Dividing 0.", 2);
        return error_double(std::nan(""));
    }
    double tmp = value / other.value;
    return error_double(tmp, (error + tmp * other.error) / other.value);
}

error_double operator/(const double &self, const error_double &other)
{
    if (other == 0)
    {
        PyErr_WarnEx(PyExc_RuntimeWarning, "Dividing 0.", 2);
        return error_double(std::nan(""));
    }
    double tmp = self / other.value;
    return error_double(tmp, (cal_original_error(self) + tmp * other.error) / other.value);
}

error_double &error_double::operator/=(const error_double &other)
{
    if (other == 0)
    {
        PyErr_WarnEx(PyExc_RuntimeWarning, "Dividing 0.", 2);
        value = std::nan("");
        error = cal_original_error(value);
    }
    else
    {
        value /= other.value;
        error = (error + value * other.error) / other.value;
    }
    return *this;
}

error_double error_double::operator%(const error_double &other) const
{
    return *this - floor(*this / other) * other;
}

error_double operator%(const double &self, const error_double &other)
{
    return self - floor(self / other) * other;
}

error_double &error_double::operator%=(const error_double &other)
{
    *this -= floor(*this / other) * other;
    return *this;
}

bool error_double::operator==(const error_double &x) const
{
    return ((*this) == x.value) || (x == value);
}

bool error_double::operator==(const double &x) const
{
    if ((value == x) || (abs(value - x) < error))
    {
        return true;
    }
    return false;
}

int error_double::operator>(const double &x) const
{
    double tmp = cal_original_error(x);
    if ((value - error) > (x + tmp))
    {
        return 2;
    }
    else if ((value + error) > (x - tmp))
    {
        return 1;
    }
    return 0;
}

void error_double::operator=(const error_double &x)
{
    value = x.value;
    error = x.error;
    return;
}

std::ostream &operator<<(std::ostream &os, const error_double &x)
{
    os << x.value;
    if (print_error)
    {
        os << "(±" << x.error << ")";
    }
    return os;
}

error_double sin(const error_double &x)
{
    return error_double(sin(x.value), abs(cos(x.value) * x.error));
}

error_double cos(const error_double &x)
{
    return error_double(cos(x.value), abs(sin(x.value) * x.error));
}

error_double tan(const error_double &x)
{
    return error_double(tan(x.value), abs(x.error / pow(cos(x.value), 2)));
}

error_double cot(const error_double &x)
{
    return 1 / tan(x);
}

error_double sec(const error_double &x)
{
    return 1 / cos(x);
}

error_double csc(const error_double &x)
{
    return 1 / sin(x);
}

error_double arcsin(const error_double &x)
{
    if (abs(x.value) > 1)
    {
        PyErr_WarnEx(PyExc_RuntimeWarning, "Out of domain", 2);
        return error_double(std::nan(""));
    }
    return error_double(asin(x.value), x.error / sqrt(1 - x.value * x.value));
}

error_double arccos(const error_double &x)
{
    if (abs(x.value) > 1)
    {
        PyErr_WarnEx(PyExc_RuntimeWarning, "Out of domain", 2);
        return error_double(std::nan(""));
    }
    return error_double(acos(x.value), x.error / sqrt(1 - x.value * x.value));
}

error_double arctan(const error_double &x)
{
    return error_double(atan(x.value), x.error / (1 + x.value * x.value));
}

error_double arccot(const error_double &x)
{
    return edHalfPi - arctan(x);
}

error_double arcsec(const error_double &x)
{
    if (abs(x.value) < 1)
    {
        PyErr_WarnEx(PyExc_RuntimeWarning, "Out of domain", 2);
        return error_double(std::nan(""));
    }
    return edHalfPi - arccsc(x);
}

error_double arccsc(const error_double &x)
{
    if (abs(x.value) < 1)
    {
        PyErr_WarnEx(PyExc_RuntimeWarning, "Out of domain", 2);
        return error_double(std::nan(""));
    }
    return arcsin(1 / x);
}

error_double sinh(const error_double &x)
{
    return (exp(x) - exp(-x)) / 2;
}

error_double cosh(const error_double &x)
{
    return (exp(x) + exp(-x)) / 2;
}

error_double tanh(const error_double &x)
{
    return sinh(x) / cosh(x);
}

error_double coth(const error_double &x)
{
    return cosh(x) / sinh(x);
}

error_double sech(const error_double &x)
{
    return 1 / cosh(x);
}

error_double csch(const error_double &x)
{
    return 1 / sinh(x);
}

error_double arcsinh(const error_double &x)
{
    return log(x + sqrt(x * x + 1));
}

error_double arccosh(const error_double &x)
{
    if (x.value < 1)
    {
        PyErr_WarnEx(PyExc_RuntimeWarning, "Out of domain", 2);
        return error_double(std::nan(""));
    }
    return log(x + sqrt(x * x - 1));
}

error_double arctanh(const error_double &x)
{
    if (abs(x.value) > 1)
    {
        PyErr_WarnEx(PyExc_RuntimeWarning, "Out of domain", 2);
        return error_double(std::nan(""));
    }
    return log((1 + x) / (1 - x)) / 2;
}

error_double arccoth(const error_double &x)
{
    if (abs(x.value) < 1)
    {
        PyErr_WarnEx(PyExc_RuntimeWarning, "Out of domain", 2);
        return error_double(std::nan(""));
    }
    return log((x + 1) / (x - 1)) / 2;
}

error_double arcsech(const error_double &x)
{
    if (x.value <= 0 && x.value > 1)
    {
        PyErr_WarnEx(PyExc_RuntimeWarning, "Out of domain", 2);
        return error_double(std::nan(""));
    }
    return arccosh(1 / x);
}

error_double arccsch(const error_double &x)
{
    if (x.value == 0)
    {
        PyErr_WarnEx(PyExc_RuntimeWarning, "Out of domain", 2);
        return error_double(std::nan(""));
    }
    return arcsinh(1 / x);
}

error_double exp(const error_double &x)
{
    double t = exp(x.value);
    return error_double(t, t * x.error);
}

error_double log(const error_double &x)
{
    if (x.value <= 0)
    {
        PyErr_WarnEx(PyExc_RuntimeWarning, "Out of domain", 2);
        return error_double(std::nan(""));
    }
    return error_double(log(x.value), x.error / x.value);
}

error_double pow(const error_double &x, const error_double &y)
{
    double tmp = pow(x.value, y.value);
    return error_double(tmp, log(x.value) * tmp * x.error + y.value * pow(x.value, y.value - 1) * y.error);
}

error_double fastpow(const error_double &x, const int exp)
{
    if (exp == 0)
    {
        return {0, 0};
    }
    int tmp_exp = abs(exp);
    error_double tmp_x = x;
    error_double result = {1, 0};
    if (exp > 0)
    {
        while (tmp_exp)
        {
            if (tmp_exp & 1)
            {
                result *= tmp_x;
            }
            tmp_x *= tmp_x;
            tmp_exp >>= 1;
        }
    }
    else
    {
        while (tmp_exp)
        {
            if (tmp_exp & 1)
            {
                result /= tmp_x;
            }
            tmp_x *= tmp_x;
            tmp_exp >>= 1;
        }
    }
    return result;
}

error_double abs(const error_double &x)
{
    return error_double(abs(x.value), x.error);
}

error_double sqrt(const error_double &x)
{
    if (x.value < 0)
    {
        PyErr_WarnEx(PyExc_RuntimeWarning, "Out of domain", 2);
        return error_double(std::nan(""));
    }
    double t = sqrt(x.value);
    return error_double(t, 0.5 * x.error / t);
}

error_double atan2(const error_double &y, const error_double &x)
{
    error_double tmp = y / x;
    return error_double(atan2(y.value, x.value), tmp.error / (1 + tmp.value * tmp.value));
}

error_double round(const error_double &x)
{
    return error_double(round(x.value));
}

error_double floor(const error_double &x)
{
    return error_double(floor(x.value));
}

int Errordouble_compare(const error_double &x, const error_double &y, int opid)
{
    double xl = x.value - x.error;
    double xr = x.value + x.error;
    double yl = y.value - y.error;
    double yr = y.value + y.error;
    switch (opid)
    {
    case Py_LT:
    {
        if (xr < yl)
        {
            return 2;
        }
        else if (xl < yr)
        {
            return 1;
        }
        else
        {
            return 0;
        }
        break;
    }
    case Py_LE:
    {
        if (xr <= yl)
        {
            return 2;
        }
        else if (xl <= yr)
        {
            return 1;
        }
        else
        {
            return 0;
        }
        break;
    }
    case Py_EQ:
    {
        if ((xl <= y.value) && (y.value <= xr) && (yl <= x.value) && ((x.value <= yr)))
        {
            return 2;
        }
        else if ((xr < yl) || (yr < xl))
        {
            return 0;
        }
        else
        {
            return 1;
        }
        break;
    }
    case Py_NE:
    {
        if ((xr < yl) || (yr < xl))
        {
            return 2;
        }
        else if ((xl <= y.value) && (y.value <= xr) && (yl <= x.value) && ((x.value <= yr)))
        {
            return 0;
        }
        else
        {
            return 1;
        }
        break;
    }
    case Py_GT:
    {
        if (yr < xl)
        {
            return 2;
        }
        else if (yl < xr)
        {
            return 1;
        }
        else
        {
            return 0;
        }
        break;
    }
    case Py_GE:
    {
        if (yr <= xl)
        {
            return 2;
        }
        else if (yl <= xr)
        {
            return 1;
        }
        else
        {
            return 0;
        }
        break;
    }
    default:
    {
        return -1;
    }
    }
    return -1;
}