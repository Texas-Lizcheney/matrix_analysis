#include <matrix.h>

extern PyObject *PyExc_ShapeError;

PyMatrixObject *Matrix_add(const PyMatrixObject *const x, const PyMatrixObject *const y)
{
    if (!Matrix_sameshape(x, y))
    {
        PyErr_SetNone(PyExc_ShapeError);
        return nullptr;
    }
    PyMatrixObject *result = nullptr;
    result = PyObject_New(PyMatrixObject, &PyMatrix_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->rows = x->rows;
    result->cols = x->cols;
    if (PyMatrixAlloc(result))
    {
        Py_DECREF(result);
        return nullptr;
    }
    for (Py_ssize_t i = 0; i < result->rows; i++)
    {
        for (Py_ssize_t j = 0; j < result->cols; j++)
        {
            PyMatrixAssign(result, i, j, ComplexVar_add(PyMatrixGetitem(x, i, j), PyMatrixGetitem(y, i, j)));
        }
    }
    return result;
}

int Matrix_iadd(PyMatrixObject *x, const PyMatrixObject *const y)
{
    if (!Matrix_sameshape(x, y))
    {
        PyErr_SetNone(PyExc_ShapeError);
        return -1;
    }
    ComplexVar tmp;
    for (Py_ssize_t i = 0; i < x->rows; i++)
    {
        for (Py_ssize_t j = 0; j < x->cols; j++)
        {
            tmp = ComplexVar_add(PyMatrixGetitem(x, i, j), PyMatrixGetitem(y, i, j));
            PyMatrixAssign(x, i, j, tmp);
        }
    }
    return 0;
}

PyMatrixObject *Matrix_sub(const PyMatrixObject *const x, const PyMatrixObject *const y)
{
    if (!Matrix_sameshape(x, y))
    {
        PyErr_SetNone(PyExc_ShapeError);
        return nullptr;
    }
    PyMatrixObject *result = nullptr;
    result = PyObject_New(PyMatrixObject, &PyMatrix_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->rows = x->rows;
    result->cols = x->cols;
    if (PyMatrixAlloc(result))
    {
        Py_DECREF(result);
        return nullptr;
    }
    for (Py_ssize_t i = 0; i < result->rows; i++)
    {
        for (Py_ssize_t j = 0; j < result->cols; j++)
        {
            PyMatrixAssign(result, i, j, ComplexVar_sub(PyMatrixGetitem(x, i, j), PyMatrixGetitem(y, i, j)));
        }
    }
    return result;
}

int Matrix_isub(PyMatrixObject *x, const PyMatrixObject *const y)
{
    if (!Matrix_sameshape(x, y))
    {
        PyErr_SetNone(PyExc_ShapeError);
        return -1;
    }
    ComplexVar tmp;
    for (Py_ssize_t i = 0; i < x->rows; i++)
    {
        for (Py_ssize_t j = 0; j < x->cols; j++)
        {
            tmp = ComplexVar_sub(PyMatrixGetitem(x, i, j), PyMatrixGetitem(y, i, j));
            PyMatrixAssign(x, i, j, tmp);
        }
    }
    return 0;
}

PyMatrixObject *Matrix_mul(const PyMatrixObject *const x, const ComplexVar &y)
{
    PyMatrixObject *result = nullptr;
    result = PyObject_New(PyMatrixObject, &PyMatrix_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->rows = x->rows;
    result->cols = x->cols;
    if (PyMatrixAlloc(result))
    {
        Py_DECREF(result);
        return nullptr;
    }
    for (Py_ssize_t i = 0; i < result->rows; i++)
    {
        for (Py_ssize_t j = 0; j < result->cols; j++)
        {
            PyMatrixAssign(result, i, j, ComplexVar_mul(PyMatrixGetitem(x, i, j), y));
        }
    }
    return result;
}

void Matrix_imul(PyMatrixObject *x, const ComplexVar &y)
{
    ComplexVar tmp;
    for (Py_ssize_t i = 0; i < x->rows; i++)
    {
        for (Py_ssize_t j = 0; j < x->cols; j++)
        {
            tmp = ComplexVar_mul(PyMatrixGetitem(x, i, j), y);
            PyMatrixAssign(x, i, j, tmp);
        }
    }
    return;
}

PyMatrixObject *Matrix_mul(const PyMatrixObject *const x, const PyMatrixObject *const y)
{
    if (x->cols != y->rows)
    {
        PyErr_SetNone(PyExc_ShapeError);
        return nullptr;
    }
    Py_ssize_t L = x->cols;
    PyMatrixObject *result = nullptr;
    result = PyObject_New(PyMatrixObject, &PyMatrix_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->rows = x->rows;
    result->cols = y->cols;
    ComplexVar tmp;
    if (PyMatrixAlloc(result))
    {
        Py_DECREF(result);
        return nullptr;
    }
    for (Py_ssize_t i = 0; i < result->rows; i++)
    {
        for (Py_ssize_t j = 0; j < result->cols; j++)
        {
            tmp = {0, 0, false};
            for (Py_ssize_t k = 0; k < L; k++)
            {
                tmp = ComplexVar_add(tmp, ComplexVar_mul(PyMatrixGetitem(x, i, k), PyMatrixGetitem(y, k, j)));
            }
            PyMatrixAssign(result, i, j, tmp);
        }
    }
    return result;
}

PyMatrixObject *Matrix_div(const PyMatrixObject *const x, const ComplexVar &y)
{
    PyMatrixObject *result = nullptr;
    result = PyObject_New(PyMatrixObject, &PyMatrix_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->rows = x->rows;
    result->cols = x->cols;
    if (PyMatrixAlloc(result))
    {
        Py_DECREF(result);
        return nullptr;
    }
    for (Py_ssize_t i = 0; i < result->rows; i++)
    {
        for (Py_ssize_t j = 0; j < result->cols; j++)
        {
            PyMatrixAssign(result, i, j, ComplexVar_div(PyMatrixGetitem(x, i, j), y));
        }
    }
    return result;
}

void Matrix_idiv(PyMatrixObject *x, const ComplexVar &y)
{
    ComplexVar tmp;
    for (Py_ssize_t i = 0; i < x->rows; i++)
    {
        for (Py_ssize_t j = 0; j < x->cols; j++)
        {
            tmp = ComplexVar_div(PyMatrixGetitem(x, i, j), y);
            PyMatrixAssign(x, i, j, tmp);
        }
    }
    return;
}

PyMatrixObject *Matrix_div(const ComplexVar &x, const PyMatrixObject *const y)
{
    PyMatrixObject *result = nullptr;
    result = PyObject_New(PyMatrixObject, &PyMatrix_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->rows = y->rows;
    result->cols = y->cols;
    if (PyMatrixAlloc(result))
    {
        Py_DECREF(result);
        return nullptr;
    }
    for (Py_ssize_t i = 0; i < result->rows; i++)
    {
        for (Py_ssize_t j = 0; j < result->cols; j++)
        {
            PyMatrixAssign(result, i, j, ComplexVar_div(x, PyMatrixGetitem(y, i, j)));
        }
    }
    return result;
}

PyMatrixObject *Matrix_fdv(const PyMatrixObject *const x, const ComplexVar &y)
{
    PyMatrixObject *result = nullptr;
    result = PyObject_New(PyMatrixObject, &PyMatrix_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->rows = x->rows;
    result->cols = x->cols;
    if (PyMatrixAlloc(result))
    {
        Py_DECREF(result);
        return nullptr;
    }
    for (Py_ssize_t i = 0; i < result->rows; i++)
    {
        for (Py_ssize_t j = 0; j < result->cols; j++)
        {
            PyMatrixAssign(result, i, j, ComplexVar_fdv(PyMatrixGetitem(x, i, j), y));
        }
    }
    return result;
}

void Matrix_ifdv(PyMatrixObject *x, const ComplexVar &y)
{
    ComplexVar tmp;
    for (Py_ssize_t i = 0; i < x->rows; i++)
    {
        for (Py_ssize_t j = 0; j < x->cols; j++)
        {
            tmp = ComplexVar_fdv(PyMatrixGetitem(x, i, j), y);
            PyMatrixAssign(x, i, j, tmp);
        }
    }
    return;
}

PyMatrixObject *Matrix_fdv(const ComplexVar &x, const PyMatrixObject *const y)
{
    PyMatrixObject *result = nullptr;
    result = PyObject_New(PyMatrixObject, &PyMatrix_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->rows = y->rows;
    result->cols = y->cols;
    if (PyMatrixAlloc(result))
    {
        Py_DECREF(result);
        return nullptr;
    }
    for (Py_ssize_t i = 0; i < result->rows; i++)
    {
        for (Py_ssize_t j = 0; j < result->cols; j++)
        {
            PyMatrixAssign(result, i, j, ComplexVar_fdv(x, PyMatrixGetitem(y, i, j)));
        }
    }
    return result;
}

PyMatrixObject *Matrix_mod(const PyMatrixObject *const x, const ComplexVar &y)
{
    PyMatrixObject *result = nullptr;
    result = PyObject_New(PyMatrixObject, &PyMatrix_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->rows = x->rows;
    result->cols = x->cols;
    if (PyMatrixAlloc(result))
    {
        Py_DECREF(result);
        return nullptr;
    }
    for (Py_ssize_t i = 0; i < result->rows; i++)
    {
        for (Py_ssize_t j = 0; j < result->cols; j++)
        {
            PyMatrixAssign(result, i, j, ComplexVar_mod(PyMatrixGetitem(x, i, j), y));
        }
    }
    return result;
}

void Matrix_imod(PyMatrixObject *x, const ComplexVar &y)
{
    ComplexVar tmp;
    for (Py_ssize_t i = 0; i < x->rows; i++)
    {
        for (Py_ssize_t j = 0; j < x->cols; j++)
        {
            tmp = ComplexVar_mod(PyMatrixGetitem(x, i, j), y);
            PyMatrixAssign(x, i, j, tmp);
        }
    }
    return;
}

PyMatrixObject *Matrix_mod(const ComplexVar &x, const PyMatrixObject *const y)
{
    PyMatrixObject *result = nullptr;
    result = PyObject_New(PyMatrixObject, &PyMatrix_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->rows = y->rows;
    result->cols = y->cols;
    if (PyMatrixAlloc(result))
    {
        Py_DECREF(result);
        return nullptr;
    }
    for (Py_ssize_t i = 0; i < result->rows; i++)
    {
        for (Py_ssize_t j = 0; j < result->cols; j++)
        {
            PyMatrixAssign(result, i, j, ComplexVar_mod(x, PyMatrixGetitem(y, i, j)));
        }
    }
    return result;
}

PyMatrixObject *Matrix_neg(const PyMatrixObject *const self)
{
    PyMatrixObject *result = nullptr;
    result = PyObject_New(PyMatrixObject, &PyMatrix_Type);
    result->elements = nullptr;
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->rows = self->rows;
    result->cols = self->cols;
    if (PyMatrixAlloc(result))
    {
        Py_DECREF(result);
        return nullptr;
    }
    int64_t pos;
    for (Py_ssize_t i = 0; i < result->rows; i++)
    {
        for (Py_ssize_t j = 0; j < result->cols; j++)
        {
            pos = i * self->cols + j;
            result->elements[pos].real = -self->elements[pos].real;
            result->elements[pos].imag = -self->elements[pos].imag;
            result->elements[pos].isArbitrary = self->elements[pos].isArbitrary;
        }
    }
    return result;
}

PyMatrixObject *Matrix_conj(const PyMatrixObject *const self)
{
    PyMatrixObject *result = nullptr;
    result = PyObject_New(PyMatrixObject, &PyMatrix_Type);
    result->elements = nullptr;
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->rows = self->rows;
    result->cols = self->cols;
    if (PyMatrixAlloc(result))
    {
        Py_DECREF(result);
        return nullptr;
    }
    int64_t pos;
    for (Py_ssize_t i = 0; i < result->rows; i++)
    {
        for (Py_ssize_t j = 0; j < result->cols; j++)
        {
            pos = i * self->cols + j;
            result->elements[pos].real = self->elements[pos].real;
            result->elements[pos].imag = -self->elements[pos].imag;
            result->elements[pos].isArbitrary = self->elements[pos].isArbitrary;
        }
    }
    return result;
}

PyMatrixObject *Matrix_transpose(const PyMatrixObject *const self)
{
    PyMatrixObject *result = nullptr;
    result = PyObject_New(PyMatrixObject, &PyMatrix_Type);
    result->elements = nullptr;
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->rows = self->cols;
    result->cols = self->rows;
    if (PyMatrixAlloc(result))
    {
        Py_DECREF(result);
        return nullptr;
    }
    int64_t rpos;
    int64_t spos;
    for (Py_ssize_t i = 0; i < self->cols; i++)
    {
        for (Py_ssize_t j = 0; j < result->cols; j++)
        {
            rpos = i * result->cols + j;
            spos = j * self->cols + i;
            result->elements[rpos].real = self->elements[spos].real;
            result->elements[rpos].imag = self->elements[spos].imag;
            result->elements[rpos].isArbitrary = self->elements[spos].isArbitrary;
        }
    }
    return result;
}

PyMatrixObject *Matrix_hermite_transpose(const PyMatrixObject *const self)
{
    PyMatrixObject *result = nullptr;
    result = PyObject_New(PyMatrixObject, &PyMatrix_Type);
    result->elements = nullptr;
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->rows = self->cols;
    result->cols = self->rows;
    if (PyMatrixAlloc(result))
    {
        Py_DECREF(result);
        return nullptr;
    }
    int64_t rpos;
    int64_t spos;
    for (Py_ssize_t i = 0; i < self->cols; i++)
    {
        for (Py_ssize_t j = 0; j < result->cols; j++)
        {
            rpos = i * result->cols + j;
            spos = j * self->cols + i;
            result->elements[rpos].real = self->elements[spos].real;
            result->elements[rpos].imag = -self->elements[spos].imag;
            result->elements[rpos].isArbitrary = self->elements[spos].isArbitrary;
        }
    }
    return result;
}

PyMatrixObject *Matrix_hadamard(const PyMatrixObject *const x, const PyMatrixObject *const y)
{
    if (!Matrix_sameshape(x, y))
    {
        PyErr_SetNone(PyExc_ShapeError);
        return nullptr;
    }
    PyMatrixObject *result = nullptr;
    result = PyObject_New(PyMatrixObject, &PyMatrix_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->rows = x->rows;
    result->cols = x->cols;
    if (PyMatrixAlloc(result))
    {
        Py_DECREF(result);
        return nullptr;
    }
    for (Py_ssize_t i = 0; i < result->rows; i++)
    {
        for (Py_ssize_t j = 0; j < result->cols; j++)
        {
            PyMatrixAssign(result, i, j, ComplexVar_mul(PyMatrixGetitem(x, i, j), PyMatrixGetitem(y, i, j)));
        }
    }
    return result;
}