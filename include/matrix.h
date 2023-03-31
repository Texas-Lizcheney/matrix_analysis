#pragma once
#include <Python.h>
#include <structmember.h>
#include <complexvar.h>

struct PyMatrixObject
{
    PyObject_HEAD;
    ComplexVar *elements;
    int rows;
    int cols;
    int64_t total_elements;
};

void PyMatrix_dealloc(PyMatrixObject *);
int PyComplexVar_init(PyMatrixObject *, PyObject *, PyObject *);
PyObject *PyMatrix_new(PyTypeObject *, PyObject *, PyObject *);