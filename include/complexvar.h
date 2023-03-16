#pragma once
#include <Python.h>

struct ComplexVar
{
    double real;
    double imag;
    bool isArbitrary;
};

struct PyComplexVarObject
{
    PyObject_HEAD;
    ComplexVar num;
};