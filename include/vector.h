#pragma once
#include <matrix.h>

struct PyVectorObject
{
    PyMatrixObject matrix;
    bool isHor;
};

int Vector_dot(const PyVectorObject *const, const PyVectorObject *const, ComplexVar &);
error_double Vector_minkowsi_distance(const PyVectorObject *const, int);
error_double Vector_L1(const PyVectorObject *const);
error_double Vector_L2(const PyVectorObject *const);
error_double Vector_Linf(const PyVectorObject *const);

extern PyTypeObject PyVector_Type;
#define PyVector_Check(op) PyObject_TypeCheck(op, &PyVector_Type)
#define PyVector_CheckExact(op) Py_IS_TYPE(op, &PyVector_Type)
#define PyVectorGetitem(self, pos) (((self)->matrix).elements[(pos)])
PyVectorObject *internal_new_PyVector();