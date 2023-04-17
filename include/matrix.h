#pragma once
#define NPY_NO_DEPRECATED_API NPY_1_24_API_VERSION
#include <Python.h>
#include <structmember.h>
#include <complexvar.h>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>
#include <type_traits>
#define PY_ARRAY_UNIQUE_SYMBOL np_array_api
#define NO_IMPORT
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
int PyMatrix_init(PyMatrixObject *, PyObject *, PyObject *);
PyObject *PyMatrix_new(PyTypeObject *, PyObject *, PyObject *);

Py_ssize_t PyMatrix_length(PyMatrixObject *);
PyObject *PyMatrix_subscript(PyMatrixObject *, PyObject *);