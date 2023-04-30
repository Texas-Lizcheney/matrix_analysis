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

void PyErrordoubleObject_dealloc(PyErrordoubleObject *);
PyObject *PyErrordoubleObject_repr(PyErrordoubleObject *);
int PyErrordoubleObject_init(PyErrordoubleObject *, PyObject *, PyObject *);
PyObject *PyErrordoubleObject_new(PyTypeObject *, PyObject *, PyObject *);

PyObject *PyErrordouble_add(PyObject *, PyObject *);
PyObject *PyErrordouble_subtract(PyObject *, PyObject *);
PyObject *PyErrordouble_multiply(PyObject *, PyObject *);
PyObject *PyErrordouble_remainder(PyObject *, PyObject *);
PyObject *PyErrordouble_divmod(PyObject *, PyObject *);
PyObject *PyErrordouble_power(PyObject *, PyObject *, PyObject *);
PyObject *PyErrordouble_negative(PyObject *);
PyObject *PyErrordouble_positive(PyObject *);
PyObject *PyErrordouble_absolute(PyObject *);
int PyErrordouble_bool(PyObject *);
PyObject *PyErrordouble_int(PyObject *);
PyObject *PyErrordouble_float(PyObject *);
PyObject *PyErrordouble_inplace_add(PyErrordoubleObject *, PyObject *);
PyObject *PyErrordouble_inplace_subtract(PyErrordoubleObject *, PyObject *);
PyObject *PyErrordouble_inplace_multiply(PyErrordoubleObject *, PyObject *);
PyObject *PyErrordouble_inplace_remainder(PyErrordoubleObject *, PyObject *);
PyObject *PyErrordouble_inplace_power(PyErrordoubleObject *, PyObject *, PyObject *);
PyObject *PyErrordouble_floor_divide(PyObject *, PyObject *);
PyObject *PyErrordouble_true_divide(PyObject *, PyObject *);
PyObject *PyErrordouble_inplace_floor_divide(PyErrordoubleObject *, PyObject *);
PyObject *PyErrordouble_inplace_true_divide(PyErrordoubleObject *, PyObject *);

PyObject *PyErrordouble_round(PyErrordoubleObject *, PyObject *const *, Py_ssize_t);

PyObject *PyErrordouble_get_value(PyErrordoubleObject *, void *);
int PyErrordouble_set_value(PyErrordoubleObject *, PyObject *, void *);
PyObject *PyErrordouble_get_error(PyErrordoubleObject *, void *);
int PyErrordouble_set_error(PyErrordoubleObject *, PyObject *, void *);