#pragma once
#include <Python.h>
#include <structmember.h>
#include <complexvar.h>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>

PyObject *SetFastPrint(PyObject *, PyObject *);

struct PyMatrixObject
{
    PyObject_HEAD;
    ComplexVar *elements;
    int rows;
    int cols;
    int64_t total_elements;
};

void PyMatrix_dealloc(PyMatrixObject *);
PyObject *PyMatrix_repr(PyMatrixObject *);
PyObject *PyMatrix_str(PyMatrixObject *);
int PyComplexVar_init(PyMatrixObject *, PyObject *, PyObject *);
PyObject *PyMatrix_new(PyTypeObject *, PyObject *, PyObject *);