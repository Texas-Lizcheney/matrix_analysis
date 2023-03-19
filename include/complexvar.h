#pragma once
#include <sstream>
#include <string>
#include <iomanip>
#include <math.h>
#include <numbers>
#include <Python.h>
#include <structmember.h>
#include <utilities.h>

PyObject *SetDoublePrecision(PyObject *, PyObject *);
PyObject *SetArgFormat(PyObject *, PyObject *);

struct ComplexVar
{
    double real;
    double imag;
    bool isArbitrary;
};

#define ComplexVar_L2(x) (x.real * x.real + x.imag * x.imag)
#define ComplexVar_length(x) sqrt(ComplexVar_L2(x))
#define ComplexVar_arg(x) atan2(x.imag, x.real)
#define ComplexVar_iszero(x) (ComplexVar_L2(x) == 0)
void setvalue_frompolar(double r, double a, ComplexVar &);
ComplexVar ComplexVar_add(const ComplexVar &, const ComplexVar &);
ComplexVar ComplexVar_sub(const ComplexVar &, const ComplexVar &);
ComplexVar ComplexVar_mul(const ComplexVar &, const ComplexVar &);
ComplexVar ComplexVar_div(const ComplexVar &, const ComplexVar &);
ComplexVar ComplexVar_fdv(const ComplexVar &, const ComplexVar &);
ComplexVar ComplexVar_mod(const ComplexVar &, const ComplexVar &);
ComplexVar ComplexVar_exp(const ComplexVar &);
ComplexVar ComplexVar_ln(const ComplexVar &);
ComplexVar ComplexVar_pow(const ComplexVar &, const ComplexVar &);
ComplexVar ComplexVar_neg(const ComplexVar &);

int assignComplexVar(PyObject *, ComplexVar &);

struct PyComplexVarObject
{
    PyObject_HEAD;
    ComplexVar num;
};
extern PyTypeObject PyComplexVarType;
#define PyComplexVar_CheckExact(op) Py_IS_TYPE(op, &PyComplexVarType)

void PyComplexVar_dealloc(PyComplexVarObject *);
PyObject *PyComplexVar_repr(PyComplexVarObject *);
PyObject *PyComplexVar_str(PyComplexVarObject *);
PyObject *PyComplexVar_richcompare(PyComplexVarObject *, PyObject *, int);
int PyComplexVar_init(PyComplexVarObject *, PyObject *, PyObject *);
PyObject *PyComplexVar_new(PyTypeObject *, PyObject *, PyObject *);

PyObject *PyComplexVar_add(PyComplexVarObject *, PyObject *);
PyObject *PyComplexVar_subtract(PyComplexVarObject *, PyObject *);
PyObject *PyComplexVar_multiply(PyComplexVarObject *, PyObject *);
PyObject *PyComplexVar_remainder(PyComplexVarObject *, PyObject *);
PyObject *PyComplexVar_divmod(PyComplexVarObject *, PyObject *);
PyObject *PyComplexVar_power(PyComplexVarObject *, PyObject *, PyObject *);
PyObject *PyComplexVar_negative(PyComplexVarObject *);
PyObject *PyComplexVar_floor_divide(PyComplexVarObject *, PyObject *);
PyObject *PyComplexVar_true_divide(PyComplexVarObject *, PyObject *);

PyObject *PyComplexVar_get_len(PyComplexVarObject *, void *);
int PyComplexVar_set_len(PyComplexVarObject *, PyObject *, void *);
PyObject *PyComplexVar_get_arg(PyComplexVarObject *, void *);
int PyComplexVar_set_arg(PyComplexVarObject *, PyObject *, void *);
PyObject *PyComplexVar_get_recpair(PyComplexVarObject *, void *);
int PyComplexVar_set_recpair(PyComplexVarObject *, PyObject *, void *);
PyObject *PyComplexVar_get_polarpair(PyComplexVarObject *, void *);
int PyComplexVar_set_polarpair(PyComplexVarObject *, PyObject *, void *);