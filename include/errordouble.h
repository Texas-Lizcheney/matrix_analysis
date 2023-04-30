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

    error_double(double = 0) noexcept;
    error_double(double, double) noexcept;
    error_double(const error_double &) noexcept;
    error_double(PyObject *);
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
    void operator=(const double &);
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
error_double exp(const error_double &);
error_double log(const error_double &);
error_double pow(const error_double &, const error_double &);
error_double abs(const error_double &);
error_double sqrt(const error_double &);
error_double atan2(const error_double &, const error_double &);
error_double round(const error_double &);
error_double floor(const error_double &);

struct PyErrordoubleObject
{
    PyObject_HEAD;
    error_double num;
    error_double *parent;
};

extern PyTypeObject PyErrordouble_Type;
#define PyErrordouble_Check(op) PyObject_TypeCheck(op, &PyErrordouble_Type)
#define PyErrordouble_CheckExact(op) Py_IS_TYPE(op, &PyErrordouble_Type)