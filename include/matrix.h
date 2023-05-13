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

struct PyMatrixObject
{
    PyObject_HEAD;
    ComplexVar *elements;
    int rows;
    int cols;
    int64_t total_elements;
};

extern PyTypeObject PyMatrix_Type;
#define PyMatrix_Check(op) PyObject_TypeCheck(op, &PyMatrix_Type)
#define PyMatrix_CheckExact(op) Py_IS_TYPE(op, &PyMatrix_Type)
#define Matrix_sameshape(x, y) (((x)->rows == (y)->rows) && ((x)->cols == (y)->cols))
#define PyMatrixAssign(self, r, c, value) (self)->elements[(r) * (self)->cols + (c)] = (value)
#define PyMatrixGetitem(self, r, c) ((self)->elements[(r) * (self)->cols + (c)])
int Matrix_copy(const PyMatrixObject *const, PyMatrixObject *);
int Matrix_add(const PyMatrixObject *const, const PyMatrixObject *const, PyMatrixObject *);
int Matrix_iadd(PyMatrixObject *, const PyMatrixObject *const);
int Matrix_sub(const PyMatrixObject *const, const PyMatrixObject *const, PyMatrixObject *);
int Matrix_isub(PyMatrixObject *, const PyMatrixObject *const);
int Matrix_mul(const PyMatrixObject *const, const ComplexVar &, PyMatrixObject *);
int Matrix_imul(PyMatrixObject *, const ComplexVar &);
int Matrix_mul(const PyMatrixObject *const, const PyMatrixObject *const, PyMatrixObject *);
int Matrix_div(const PyMatrixObject *const, const ComplexVar &, PyMatrixObject *);
int Matrix_idiv(PyMatrixObject *, const ComplexVar &);
int Matrix_div(const ComplexVar &, const PyMatrixObject *const, PyMatrixObject *);
int Matrix_fdv(const PyMatrixObject *const, const ComplexVar &, PyMatrixObject *);
int Matrix_ifdv(PyMatrixObject *, const ComplexVar &);
int Matrix_fdv(const ComplexVar &, const PyMatrixObject *const, PyMatrixObject *);
int Matrix_mod(const PyMatrixObject *const, const ComplexVar &, PyMatrixObject *);
int Matrix_imod(PyMatrixObject *, const ComplexVar &);
int Matrix_mod(const ComplexVar &, const PyMatrixObject *const, PyMatrixObject *);
int Matrix_neg(const PyMatrixObject *const, PyMatrixObject *);
int Matrix_conj(const PyMatrixObject *const, PyMatrixObject *);
int Matrix_iconj(PyMatrixObject *);
int Matrix_transpose(const PyMatrixObject *const, PyMatrixObject *);
int Matrix_itranspose(PyMatrixObject *);
int Matrix_hermite_transpose(const PyMatrixObject *const, PyMatrixObject *);
int Matrix_ihermite_transpose(PyMatrixObject *);
int Matrix_kronecker(const PyMatrixObject *const, const PyMatrixObject *const, PyMatrixObject *);
int Matrix_hadamard(const PyMatrixObject *const, const PyMatrixObject *const, PyMatrixObject *);

enum RankState
{
    FAIL_RANK = -2,
    UNSURE_RANK = -1,
};
void Matrix_row_switch(PyMatrixObject *, int, int);
void Matrix_row_multiply(PyMatrixObject *, int, const ComplexVar &);
void Matrix_row_add(PyMatrixObject *, int, int, const ComplexVar &);
void Matrix_col_switch(PyMatrixObject *, int, int);
void Matrix_col_multiply(PyMatrixObject *, int, const ComplexVar &);
void Matrix_col_add(PyMatrixObject *, int, int, const ComplexVar &);
int Matrix_rank(const PyMatrixObject *const);

int PyMatrixAlloc(PyMatrixObject *);
int PyMatrixAssign_withcheck(PyMatrixObject *, int, int, const ComplexVar &);
int PyMatrixGet_withcheck(const PyMatrixObject *const, int, int, ComplexVar &);
PyObject *PyMatrix_copy(const PyMatrixObject *const);

// Matrix module methods
PyObject *PyMatrix_reshape(PyObject *, PyObject *, PyObject *);