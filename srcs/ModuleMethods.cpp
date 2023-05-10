#include <matrix.h>

extern PyObject *PyExc_ShapeError;

PyObject *PyMatrix_reshape(PyObject *self, PyObject *args, PyObject *kwds)
{
    static char *kwlist0[] = {
        (char *)"matrix",
        (char *)"rows",
        (char *)"cols",
        nullptr,
    };
    static char *kwlist1[] = {
        (char *)"matrix",
        (char *)"shape",
        nullptr,
    };
    PyObject *matrix = nullptr;
    PyObject *tmp = nullptr;
    int r = 0;
    int c = 0;
    if (PyArg_ParseTupleAndKeywords(args, kwds, "Oii|", kwlist0, &matrix, &r, &c))
    {
        goto unpack_success;
    }
    PyErr_Clear();
    matrix = nullptr;
    if (PyArg_ParseTupleAndKeywords(args, kwds, "OO|", kwlist1, &matrix, &tmp))
    {
        if (PyArg_ParseTuple(tmp, "ii", &r, &c))
        {
            goto unpack_success;
        }
        return nullptr;
    }
    PyErr_SetString(PyExc_TypeError, "Fail to match any overload");
    return nullptr;
unpack_success:
    if (!PyMatrix_CheckExact(matrix))
    {
        PyErr_Format(PyExc_TypeError, "cannot reshape %s object", matrix->ob_type->tp_name);
        return nullptr;
    }
    if (r * c != ((PyMatrixObject *)matrix)->total_elements)
    {
        PyErr_Format(PyExc_ShapeError, "cannot reshape matrix of size %lld into rows:%ld cols:%d", ((PyMatrixObject *)matrix)->total_elements, r, c);
        return nullptr;
    }
    ((PyMatrixObject *)matrix)->rows = r;
    ((PyMatrixObject *)matrix)->cols = c;
    Py_RETURN_NONE;
}