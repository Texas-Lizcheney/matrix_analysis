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
};