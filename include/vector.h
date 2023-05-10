#pragma once
#include <matrix.h>

struct PyVectorObject
{
    PyMatrixObject matrix;
    bool isHor;
};

extern PyTypeObject PyVector_Type;
#define PyVector_Check(op) PyObject_TypeCheck(op, &PyVector_Type)
#define PyVector_CheckExact(op) Py_IS_TYPE(op, &PyVector_Type)
#define PyVectorAssign(self, pos, value) ((self)->matrix).elements[(pos)] = (value)
#define PyVectorGetitem(self, pos) (((self)->matrix).elements[(pos)])