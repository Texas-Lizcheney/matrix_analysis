#pragma once
#define NPY_NO_DEPRECATED_API NPY_1_24_API_VERSION
#include <Python.h>
#include <structmember.h>
#include <complexvar.h>
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

#define Matrix_sameshape(x, y) ((x->rows == y->rows) && (x->cols == y->cols))
PyMatrixObject *MatrixAdd(const PyMatrixObject *const, const PyMatrixObject *const);
PyMatrixObject *MatrixSub(const PyMatrixObject *const, const PyMatrixObject *const);
PyMatrixObject *MatrixMulConstant(const PyMatrixObject *const, const ComplexVar &);
PyMatrixObject *MatrixMatmul(const PyMatrixObject *const, const PyMatrixObject *const);

int PyMatrixAlloc(PyMatrixObject *);
void PyMatrixAssign(PyMatrixObject *, int, int, const ComplexVar &);
ComplexVar PyMatrixGetitem(const PyMatrixObject *const, int, int);
int PyMatrixAssign_withcheck(PyMatrixObject *, int, int, const ComplexVar &);
int PyMatrixGet_withcheck(const PyMatrixObject *const, int, int, ComplexVar &);
void PyMatrix_dealloc(PyMatrixObject *);
PyObject *PyMatrix_repr(PyMatrixObject *);
PyObject *PyMatrix_str(PyMatrixObject *);
int PyMatrix_init(PyMatrixObject *, PyObject *, PyObject *);
PyObject *PyMatrix_new(PyTypeObject *, PyObject *, PyObject *);

PyObject *PyMatrix_add(PyObject *, PyObject *);
PyObject *PyMatrix_subtract(PyObject *, PyObject *);
PyObject *PyMatrix_multiply(PyObject *, PyObject *);
PyObject *PyMatrix_matrix_multiply(PyObject *, PyObject *);

Py_ssize_t PyMatrix_length(PyMatrixObject *);
PyObject *PyMatrix_subscript(PyMatrixObject *, PyObject *);
int PyMatrix_ass_subscript(PyMatrixObject *, PyObject *, PyObject *);

PyObject *PyMatrix_get_shape(PyMatrixObject *, void *);

extern PyTypeObject PyMatrixType;
#define PyMatrix_Check(op) PyObject_TypeCheck(op, &PyMatrixType)
#define PyMatrix_CheckExact(op) Py_IS_TYPE(((PyObject *)((a))), &PyMatrixType)