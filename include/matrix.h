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
PyMatrixObject *Matrix_add(const PyMatrixObject *const, const PyMatrixObject *const);
int Matrix_iadd(PyMatrixObject *, const PyMatrixObject *const);
PyMatrixObject *Matrix_sub(const PyMatrixObject *const, const PyMatrixObject *const);
int Matrix_isub(PyMatrixObject *, const PyMatrixObject *const);
PyMatrixObject *Matrix_mul(const PyMatrixObject *const, const ComplexVar &);
void Matrix_imul(PyMatrixObject *, const ComplexVar &);
PyMatrixObject *Matrix_mul(const PyMatrixObject *const, const PyMatrixObject *const);
PyMatrixObject *Matrix_div(const PyMatrixObject *const, const ComplexVar &);
void Matrix_idiv(PyMatrixObject *, const ComplexVar &);
PyMatrixObject *Matrix_div(const ComplexVar &, const PyMatrixObject *const);
PyMatrixObject *Matrix_fdv(const PyMatrixObject *const, const ComplexVar &);
void Matrix_ifdv(PyMatrixObject *, const ComplexVar &);
PyMatrixObject *Matrix_fdv(const ComplexVar &, const PyMatrixObject *const);
PyMatrixObject *Matrix_mod(const PyMatrixObject *const, const ComplexVar &);
void Matrix_imod(PyMatrixObject *, const ComplexVar &);
PyMatrixObject *Matrix_mod(const ComplexVar &, const PyMatrixObject *const);
PyMatrixObject *Matrix_neg(const PyMatrixObject *const);
PyMatrixObject *Matrix_conj(const PyMatrixObject *const);
PyMatrixObject *Matrix_transpose(const PyMatrixObject *const);
PyMatrixObject *Matrix_hermite_transpose(const PyMatrixObject *const);
PyMatrixObject *Matrix_hadamard(const PyMatrixObject *const, const PyMatrixObject *const);

void Matrix_row_switch(PyMatrixObject *, int, int);
void Matrix_row_multiply(PyMatrixObject *, int, const ComplexVar &);
void Matrix_row_add(PyMatrixObject *, int, int, const ComplexVar &);
void Matrix_col_switch(PyMatrixObject *, int, int);
void Matrix_col_multiply(PyMatrixObject *, int, const ComplexVar &);
void Matrix_col_add(PyMatrixObject *, int, int, const ComplexVar &);
int Matrix_rank(const PyMatrixObject *const);

int PyMatrixAlloc(PyMatrixObject *);
#define PyMatrixAssign(self, r, c, value) (self)->elements[r * (self)->cols + c] = value
#define PyMatrixGetitem(self, r, c) (self)->elements[r * (self)->cols + c]
int PyMatrixAssign_withcheck(PyMatrixObject *, int, int, const ComplexVar &);
int PyMatrixGet_withcheck(const PyMatrixObject *const, int, int, ComplexVar &);
PyObject *PyMatrix_copy(const PyMatrixObject *const);
void PyMatrix_dealloc(PyMatrixObject *);
PyObject *PyMatrix_repr(PyMatrixObject *);
PyObject *PyMatrix_str(PyMatrixObject *);
int PyMatrix_init(PyMatrixObject *, PyObject *, PyObject *);
PyObject *PyMatrix_new(PyTypeObject *, PyObject *, PyObject *);

PyObject *PyMatrix_add(PyObject *, PyObject *);
PyObject *PyMatrix_subtract(PyObject *, PyObject *);
PyObject *PyMatrix_multiply(PyObject *, PyObject *);
PyObject *PyMatrix_remainder(PyObject *, PyObject *);
PyObject *PyMatrix_divmod(PyObject *, PyObject *);
PyObject *PyMatrix_negative(PyObject *);
PyObject *PyMatrix_positive(PyObject *);
PyObject *PyMatrix_inplace_add(PyMatrixObject *, PyObject *);
PyObject *PyMatrix_inplace_subtract(PyMatrixObject *, PyObject *);
PyObject *PyMatrix_inplace_multiply(PyMatrixObject *, PyObject *);
PyObject *PyMatrix_inplace_remainder(PyMatrixObject *, PyObject *);
PyObject *PyMatrix_floor_divide(PyObject *, PyObject *);
PyObject *PyMatrix_true_divide(PyObject *, PyObject *);
PyObject *PyMatrix_inplace_floor_divide(PyMatrixObject *, PyObject *);
PyObject *PyMatrix_inplace_true_divide(PyMatrixObject *, PyObject *);
PyObject *PyMatrix_matrix_multiply(PyObject *, PyObject *);

PyObject *PyMatrix_conj(PyMatrixObject *, PyObject *);
PyObject *PyMatrix_T(PyMatrixObject *, PyObject *);
PyObject *PyMatrix_H(PyMatrixObject *, PyObject *);

Py_ssize_t PyMatrix_length(PyMatrixObject *);
PyObject *PyMatrix_subscript(PyMatrixObject *, PyObject *);
int PyMatrix_ass_subscript(PyMatrixObject *, PyObject *, PyObject *);

PyObject *PyMatrix_get_shape(PyMatrixObject *, void *);
PyObject *PyMatrix_get_rank(PyMatrixObject *, void *);

extern PyTypeObject PyMatrixType;
#define PyMatrix_Check(op) PyObject_TypeCheck(op, &PyMatrixType)
#define PyMatrix_CheckExact(op) Py_IS_TYPE(((PyObject *)((a))), &PyMatrixType)