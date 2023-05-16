#pragma once
#include <matrix.h>

struct PyVectorObject
{
    PyMatrixObject matrix;
    bool isHor;
};

int Vector_dot(const PyVectorObject *const, const PyVectorObject *const, ComplexVar &);

extern PyTypeObject PyVector_Type;
#define PyVector_Check(op) PyObject_TypeCheck(op, &PyVector_Type)
#define PyVector_CheckExact(op) Py_IS_TYPE(op, &PyVector_Type)
#define PyVectorGetitem(self, pos) (((self)->matrix).elements[(pos)])
PyVectorObject* internal_new_PyVector();

