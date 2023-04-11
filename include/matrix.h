#pragma once
#define NPY_NO_DEPRECATED_API NPY_1_24_API_VERSION
#include <Python.h>
#include <structmember.h>
#include <complexvar.h>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <arrayobject.h>
#include <ndarraytypes.h>

PyObject *SetFastPrint(PyObject *, PyObject *);
PyObject *SetPrintArea(PyObject *, PyObject *, PyObject *);

struct PyMatrixObject
{
    PyObject_HEAD;
    ComplexVar *elements;
    int rows;
    int cols;
    int64_t total_elements;
};

int PyMatrixAlloc(PyMatrixObject *);
void PyMatrixAssign(PyMatrixObject *, int, int, const ComplexVar &);
void PyMatrix_dealloc(PyMatrixObject *);
PyObject *PyMatrix_repr(PyMatrixObject *);
PyObject *PyMatrix_str(PyMatrixObject *);
int PyCMatrix_init(PyMatrixObject *, PyObject *, PyObject *);
PyObject *PyMatrix_new(PyTypeObject *, PyObject *, PyObject *);