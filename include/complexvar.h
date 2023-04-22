#pragma once
#include <sstream>
#include <string>
#include <iomanip>
#include <math.h>
#include <numbers>
#include <Python.h>
#include <structmember.h>
#include <utilities.h>
#include <sstream>

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
#define ComplexVar_iszero(x) (x.real == 0 && x.imag == 0 && !x.isArbitrary)
void setvalue_frompolar(double r, double a, ComplexVar &);
std::stringstream ComplexVar_repr(const ComplexVar &);
std::stringstream ComplexVar_str(const ComplexVar &);
ComplexVar ComplexVar_conj(const ComplexVar &);
ComplexVar ComplexVar_add(const ComplexVar &, const ComplexVar &);
ComplexVar ComplexVar_sub(const ComplexVar &, const ComplexVar &);
ComplexVar ComplexVar_mul(const ComplexVar &, const ComplexVar &);
ComplexVar ComplexVar_div(const ComplexVar &, const ComplexVar &);
ComplexVar ComplexVar_fdv(const ComplexVar &, const ComplexVar &);
ComplexVar ComplexVar_mod(const ComplexVar &, const ComplexVar &);
ComplexVar ComplexVar_exp(const ComplexVar &);
ComplexVar ComplexVar_ln(const ComplexVar &);
ComplexVar ComplexVar_log(const ComplexVar &, const ComplexVar &);
ComplexVar ComplexVar_pow(const ComplexVar &, const ComplexVar &);
ComplexVar ComplexVar_neg(const ComplexVar &);
ComplexVar ComplexVar_ivt(const ComplexVar &);
ComplexVar ComplexVar_sqrt(const ComplexVar &);
ComplexVar ComplexVar_sin(const ComplexVar &);
ComplexVar ComplexVar_cos(const ComplexVar &);
ComplexVar ComplexVar_tan(const ComplexVar &);
ComplexVar ComplexVar_cot(const ComplexVar &);
ComplexVar ComplexVar_sec(const ComplexVar &);
ComplexVar ComplexVar_csc(const ComplexVar &);
ComplexVar ComplexVar_arcsin(const ComplexVar &);
ComplexVar ComplexVar_arccos(const ComplexVar &);
ComplexVar ComplexVar_arctan(const ComplexVar &);
ComplexVar ComplexVar_arccot(const ComplexVar &);
ComplexVar ComplexVar_arcsec(const ComplexVar &);
ComplexVar ComplexVar_arccsc(const ComplexVar &);
ComplexVar ComplexVar_sinh(const ComplexVar &);
ComplexVar ComplexVar_cosh(const ComplexVar &);
ComplexVar ComplexVar_tanh(const ComplexVar &);
ComplexVar ComplexVar_coth(const ComplexVar &);
ComplexVar ComplexVar_sech(const ComplexVar &);
ComplexVar ComplexVar_csch(const ComplexVar &);
ComplexVar ComplexVar_arcsinh(const ComplexVar &);
ComplexVar ComplexVar_arccosh(const ComplexVar &);
ComplexVar ComplexVar_arctanh(const ComplexVar &);
ComplexVar ComplexVar_arccoth(const ComplexVar &);
ComplexVar ComplexVar_arcsech(const ComplexVar &);
ComplexVar ComplexVar_arccsch(const ComplexVar &);

struct PyComplexVarObject
{
    PyObject_HEAD;
    ComplexVar num;
};

int assignComplexVar(PyObject *, ComplexVar &);
extern PyTypeObject PyComplexVarType;
#define PyComplexVar_Check(op) PyObject_TypeCheck(op, &PyComplexVarType)
#define PyComplexVar_CheckExact(op) Py_IS_TYPE(op, &PyComplexVarType)
#define CanBeComplexVar(op) (PyComplexVar_CheckExact(op) || Py_IsNone(op) || PyLong_CheckExact(op) || PyFloat_CheckExact(op) || PyComplex_CheckExact(op))

void PyComplexVar_dealloc(PyComplexVarObject *);
PyObject *PyComplexVar_repr(PyComplexVarObject *);
PyObject *PyComplexVar_str(PyComplexVarObject *);
PyObject *PyComplexVar_richcompare(PyComplexVarObject *, PyObject *, int);
int PyComplexVar_init(PyComplexVarObject *, PyObject *, PyObject *);
PyObject *PyComplexVar_new(PyTypeObject *, PyObject *, PyObject *);

PyObject *PyComplexVar_add(PyObject *, PyObject *);
PyObject *PyComplexVar_subtract(PyObject *, PyObject *);
PyObject *PyComplexVar_multiply(PyObject *, PyObject *);
PyObject *PyComplexVar_remainder(PyObject *, PyObject *);
PyObject *PyComplexVar_divmod(PyObject *, PyObject *);
PyObject *PyComplexVar_power(PyObject *, PyObject *, PyObject *);
PyObject *PyComplexVar_negative(PyObject *);
PyObject *PyComplexVar_positive(PyObject *);
PyObject *PyComplexVar_absolute(PyObject *);
int PyComplexVar_bool(PyObject *);
PyObject *PyComplexVar_invert(PyObject *);
PyObject *PyComplexVar_inplace_add(PyComplexVarObject *, PyObject *);
PyObject *PyComplexVar_inplace_subtract(PyComplexVarObject *, PyObject *);
PyObject *PyComplexVar_inplace_multiply(PyComplexVarObject *, PyObject *);
PyObject *PyComplexVar_inplace_remainder(PyComplexVarObject *, PyObject *);
PyObject *PyComplexVar_inplace_power(PyComplexVarObject *, PyObject *, PyObject *);
PyObject *PyComplexVar_floor_divide(PyObject *, PyObject *);
PyObject *PyComplexVar_true_divide(PyObject *, PyObject *);
PyObject *PyComplexVar_inplace_floor_divide(PyComplexVarObject *, PyObject *);
PyObject *PyComplexVar_inplace_true_divide(PyComplexVarObject *, PyObject *);

PyObject *PyComplexVar_conj(PyComplexVarObject *);
PyObject *PyComplexVar_exp(PyComplexVarObject *, PyObject *);
PyObject *PyComplexVar_ln(PyComplexVarObject *, PyObject *);
PyObject *PyComplexVar_log(PyComplexVarObject *, PyObject *);
PyObject *PyComplexVar_logasbase(PyComplexVarObject *, PyObject *);
PyObject *PyComplexVar_sqrt(PyComplexVarObject *);
PyObject *PyComplexVar_sin(PyComplexVarObject *);
PyObject *PyComplexVar_cos(PyComplexVarObject *);
PyObject *PyComplexVar_tan(PyComplexVarObject *);
PyObject *PyComplexVar_cot(PyComplexVarObject *);
PyObject *PyComplexVar_sec(PyComplexVarObject *);
PyObject *PyComplexVar_csc(PyComplexVarObject *);
PyObject *PyComplexVar_arcsin(PyComplexVarObject *);
PyObject *PyComplexVar_arccos(PyComplexVarObject *);
PyObject *PyComplexVar_arctan(PyComplexVarObject *);
PyObject *PyComplexVar_arccot(PyComplexVarObject *);
PyObject *PyComplexVar_arcsec(PyComplexVarObject *);
PyObject *PyComplexVar_arccsc(PyComplexVarObject *);
PyObject *PyComplexVar_sinh(PyComplexVarObject *);
PyObject *PyComplexVar_cosh(PyComplexVarObject *);
PyObject *PyComplexVar_tanh(PyComplexVarObject *);
PyObject *PyComplexVar_coth(PyComplexVarObject *);
PyObject *PyComplexVar_sech(PyComplexVarObject *);
PyObject *PyComplexVar_csch(PyComplexVarObject *);
PyObject *PyComplexVar_arcsinh(PyComplexVarObject *);
PyObject *PyComplexVar_arccosh(PyComplexVarObject *);
PyObject *PyComplexVar_arctanh(PyComplexVarObject *);
PyObject *PyComplexVar_arccoth(PyComplexVarObject *);
PyObject *PyComplexVar_arcsech(PyComplexVarObject *);
PyObject *PyComplexVar_arccsch(PyComplexVarObject *);

PyObject *PyComplexVar_get_len(PyComplexVarObject *, void *);
int PyComplexVar_set_len(PyComplexVarObject *, PyObject *, void *);
PyObject *PyComplexVar_get_arg(PyComplexVarObject *, void *);
int PyComplexVar_set_arg(PyComplexVarObject *, PyObject *, void *);
PyObject *PyComplexVar_get_recpair(PyComplexVarObject *, void *);
int PyComplexVar_set_recpair(PyComplexVarObject *, PyObject *, void *);
PyObject *PyComplexVar_get_polarpair(PyComplexVarObject *, void *);
int PyComplexVar_set_polarpair(PyComplexVarObject *, PyObject *, void *);