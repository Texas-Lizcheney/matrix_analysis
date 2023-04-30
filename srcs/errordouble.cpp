#include <errordouble.h>
extern bool print_error;

error_double::error_double(double value) noexcept : value(value),
                                                    error(cal_original_error(value))
{
}

error_double::error_double(double value, double error) noexcept : value(value),
                                                                  error(error)
{
}

error_double::error_double(const error_double &x) noexcept : value(x.value),
                                                             error(x.error)
{
}

error_double::error_double(PyObject *x)
{
    if (PyErrordouble_Check(x))
    {
        value = ((PyErrordoubleObject *)x)->num.value;
        error = ((PyErrordoubleObject *)x)->num.error;
    }
    value = PyFloat_AsDouble(x);
    if (value == -1 && PyErr_Occurred())
    {
        throw std::exception();
    }
    error = cal_original_error(value);
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
    double tmp = value / other.value;
    return error_double(tmp, (error + tmp * other.error) / other.value);
}

error_double operator/(const double &self, const error_double &other)
{
    double tmp = self / other.value;
    return error_double(tmp, (cal_original_error(self) + tmp * other.error) / other.value);
}

error_double &error_double::operator/=(const error_double &other)
{
    value /= other.value;
    error = (error + value * other.error) / other.value;
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
    if ((value - error) > x)
    {
        return 2;
    }
    else if (value > x)
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

void error_double::operator=(const double &x)
{
    value = x;
    error = cal_original_error(x);
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

error_double exp(const error_double &x)
{
    double t = exp(x.value);
    return error_double(t, t * x.error);
}

error_double log(const error_double &x)
{
    return error_double(log(x.value), x.error / x.value);
}

error_double pow(const error_double &x, const error_double &y)
{
    double tmp = pow(x.value, y.value);
    return error_double(tmp, log(x.value) * tmp * x.error + y.value * pow(x.value, y.value - 1) * y.error);
}

error_double abs(const error_double &x)
{
    return error_double(abs(x.value), x.error);
}

error_double sqrt(const error_double &x)
{
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