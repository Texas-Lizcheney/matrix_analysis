#pragma once
#include <sstream>
#include <string>
#include <iomanip>
#include <Python.h>

PyObject *SetDoublePrecision(PyObject *, PyObject *);

struct ComplexVar
{
    double real;
    double imag;
    bool isArbitrary;
};

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