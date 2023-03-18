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

#define ComplexVar_length(x) sqrt(x.real *x.real + x.imag * x.imag)
#define ComplexVar_arg(x) atan2(x.imag, x.real)
void setvalue_frompolar(double r, double a, ComplexVar &);

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
int PyComplexVar_init(PyComplexVarObject *, PyObject *, PyObject *);
PyObject *PyComplexVar_new(PyTypeObject *, PyObject *, PyObject *);

PyObject *PyComplexVar_get_len(PyComplexVarObject *, void *);
int PyComplexVar_set_len(PyComplexVarObject *, PyObject *, void *);
PyObject *PyComplexVar_get_arg(PyComplexVarObject *, void *);
int PyComplexVar_set_arg(PyComplexVarObject *, PyObject *, void *);
PyObject *PyComplexVar_get_recpair(PyComplexVarObject *, void *);
int PyComplexVar_set_recpair(PyComplexVarObject *, PyObject *, void *);
PyObject *PyComplexVar_get_polarpair(PyComplexVarObject *, void *);
int PyComplexVar_set_polarpair(PyComplexVarObject *, PyObject *, void *);