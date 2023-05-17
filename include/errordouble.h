#pragma once
#include <Python.h>
#include <structmember.h>
#include <utilities.h>
#include <math.h>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <exception>

class error_double
{
public:
    double value;
    double error;

    error_double() noexcept;
    template <is_double T>
    error_double(T) noexcept;
    error_double(int64_t) noexcept;
    error_double(double, double) noexcept;
    error_double(const error_double &) noexcept;
    error_double operator+(const error_double &) const;
    friend error_double operator+(const double &, const error_double &);
    error_double &operator+=(const error_double &);
    error_double operator+() const;
    error_double operator-(const error_double &) const;
    friend error_double operator-(const double &, const error_double &);
    error_double &operator-=(const error_double &);
    error_double operator-() const;
    error_double operator*(const error_double &) const;
    friend error_double operator*(const double &, const error_double &);
    error_double &operator*=(const error_double &);
    error_double operator/(const error_double &) const;
    friend error_double operator/(const double &, const error_double &);
    error_double &operator/=(const error_double &);
    error_double operator%(const error_double &) const;
    friend error_double operator%(const double &, const error_double &);
    error_double &operator%=(const error_double &);
    bool operator==(const error_double &) const;
    bool operator==(const double &) const;
    int operator>(const double &) const;
    void operator=(const error_double &);
    friend std::ostream &operator<<(std::ostream &, const error_double &);
};

error_double operator+(const double &, const error_double &);
error_double operator-(const double &, const error_double &);
error_double operator*(const double &, const error_double &);
error_double operator/(const double &, const error_double &);
error_double operator%(const double &, const error_double &);
std::ostream &operator<<(std::ostream &, const error_double &);
error_double sin(const error_double &);
error_double cos(const error_double &);
error_double tan(const error_double &);
error_double cot(const error_double &);
error_double sec(const error_double &);
error_double csc(const error_double &);
error_double arcsin(const error_double &);
error_double arccos(const error_double &);
error_double arctan(const error_double &);
error_double arccot(const error_double &);
error_double arcsec(const error_double &);
error_double arccsc(const error_double &);
error_double sinh(const error_double &);
error_double cosh(const error_double &);
error_double tanh(const error_double &);
error_double coth(const error_double &);
error_double sech(const error_double &);
error_double csch(const error_double &);
error_double arcsinh(const error_double &);
error_double arccosh(const error_double &);
error_double arctanh(const error_double &);
error_double arccoth(const error_double &);
error_double arcsech(const error_double &);
error_double arccsch(const error_double &);
error_double exp(const error_double &);
error_double log(const error_double &);
error_double pow(const error_double &, const error_double &);
error_double fastpow(const error_double &, const int);
error_double abs(const error_double &);
error_double sqrt(const error_double &);
error_double atan2(const error_double &, const error_double &);
error_double round(const error_double &);
error_double floor(const error_double &);
int Errordouble_compare(const error_double &, const error_double &, int);

struct PyErrordoubleObject
{
    PyObject_HEAD;
    error_double num;
    error_double *parent;
};

extern PyTypeObject PyErrordouble_Type;
#define PyErrordouble_Check(op) PyObject_TypeCheck(op, &PyErrordouble_Type)
#define PyErrordouble_CheckExact(op) Py_IS_TYPE(op, &PyErrordouble_Type)
int assignErrordouble(PyObject *, error_double &);
PyErrordoubleObject *internal_new_PyErrordouble();
