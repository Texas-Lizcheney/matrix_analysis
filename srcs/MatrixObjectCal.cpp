#include <matrix.h>

extern PyObject *PyExc_ShapeError;

PyMatrixObject *MatrixAdd(const PyMatrixObject *const x, const PyMatrixObject *const y)
{
    if (!Matrix_sameshape(x, y))
    {
        PyErr_SetNone(PyExc_ShapeError);
        return nullptr;
    }
    PyMatrixObject *result = nullptr;
    result = PyObject_New(PyMatrixObject, &PyMatrixType);
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

PyMatrixObject *MatrixAddConstant(const PyMatrixObject *const x, const ComplexVar y)
{
    PyMatrixObject *result = nullptr;
    result = PyObject_New(PyMatrixObject, &PyMatrixType);
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
            PyMatrixAssign(result, i, j, ComplexVar_add(PyMatrixGetitem(x, i, j), y));
        }
    }
    return result;
}