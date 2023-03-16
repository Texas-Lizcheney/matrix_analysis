#pragma once
#include <Python.h>

struct Complex
{
    double real;
    double imag;
    bool isArbitrarily;
};

struct PyMatrixObject
{
    PyObject_HEAD;
    Complex* elements;
    int rows;
    int cols;
};