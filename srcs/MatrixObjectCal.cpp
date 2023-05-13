#include <matrix.h>

extern PyObject *PyExc_ShapeError;

int Matrix_copy(const PyMatrixObject *const self, PyMatrixObject *result)
{
    result->rows = self->rows;
    result->cols = self->cols;
    if (PyMatrixAlloc(result))
    {
        return -1;
    }
    memcpy(result->elements, self->elements, self->total_elements * sizeof(ComplexVar));
    return 0;
}

int Matrix_add(const PyMatrixObject *const x, const PyMatrixObject *const y, PyMatrixObject *result)
{
    if (!Matrix_sameshape(x, y))
    {
        PyErr_Format(PyExc_ShapeError, "cannot add matrix with shape (%ld,%ld) and matrix with shape (%ld,%ld)", x->rows, x->cols, y->rows, y->cols);
        return -1;
    }
    result->rows = x->rows;
    result->cols = x->cols;
    if (PyMatrixAlloc(result))
    {
        return -1;
    }
    for (Py_ssize_t i = 0; i < result->rows; i++)
    {
        for (Py_ssize_t j = 0; j < result->cols; j++)
        {
            PyMatrixAssign(result, i, j, ComplexVar_add(PyMatrixGetitem(x, i, j), PyMatrixGetitem(y, i, j)));
        }
    }
    return 0;
}

int Matrix_iadd(PyMatrixObject *x, const PyMatrixObject *const y)
{
    if (!Matrix_sameshape(x, y))
    {
        PyErr_Format(PyExc_ShapeError, "cannot add matrix with shape (%ld,%ld) and matrix with shape (%ld,%ld)", x->rows, x->cols, y->rows, y->cols);
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

int Matrix_sub(const PyMatrixObject *const x, const PyMatrixObject *const y, PyMatrixObject *result)
{
    if (!Matrix_sameshape(x, y))
    {
        PyErr_Format(PyExc_ShapeError, "cannot sub matrix with shape (%ld,%ld) and matrix with shape (%ld,%ld)", x->rows, x->cols, y->rows, y->cols);
        return -1;
    }
    result->rows = x->rows;
    result->cols = x->cols;
    if (PyMatrixAlloc(result))
    {
        return -1;
    }
    for (Py_ssize_t i = 0; i < result->rows; i++)
    {
        for (Py_ssize_t j = 0; j < result->cols; j++)
        {
            PyMatrixAssign(result, i, j, ComplexVar_sub(PyMatrixGetitem(x, i, j), PyMatrixGetitem(y, i, j)));
        }
    }
    return 0;
}

int Matrix_isub(PyMatrixObject *x, const PyMatrixObject *const y)
{
    if (!Matrix_sameshape(x, y))
    {
        PyErr_Format(PyExc_ShapeError, "cannot sub matrix with shape (%ld,%ld) and matrix with shape (%ld,%ld)", x->rows, x->cols, y->rows, y->cols);
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

int Matrix_mul(const PyMatrixObject *const x, const ComplexVar &y, PyMatrixObject *result)
{
    result->rows = x->rows;
    result->cols = x->cols;
    if (PyMatrixAlloc(result))
    {
        return -1;
    }
    for (Py_ssize_t i = 0; i < result->rows; i++)
    {
        for (Py_ssize_t j = 0; j < result->cols; j++)
        {
            PyMatrixAssign(result, i, j, ComplexVar_mul(PyMatrixGetitem(x, i, j), y));
        }
    }
    return 0;
}

int Matrix_imul(PyMatrixObject *x, const ComplexVar &y)
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
    return 0;
}

int Matrix_mul(const PyMatrixObject *const x, const PyMatrixObject *const y, PyMatrixObject *result)
{
    if (x->cols != y->rows)
    {
        PyErr_Format(PyExc_ShapeError, "cannot mul matrix with shape (%ld,%ld) and matrix with shape (%ld,%ld)", x->rows, x->cols, y->rows, y->cols);
        return -1;
    }
    Py_ssize_t L = x->cols;
    result->rows = x->rows;
    result->cols = y->cols;
    ComplexVar tmp;
    if (PyMatrixAlloc(result))
    {
        return -1;
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
    return 0;
}

int Matrix_div(const PyMatrixObject *const x, const ComplexVar &y, PyMatrixObject *result)
{
    result->rows = x->rows;
    result->cols = x->cols;
    if (PyMatrixAlloc(result))
    {
        return -1;
    }
    for (Py_ssize_t i = 0; i < result->rows; i++)
    {
        for (Py_ssize_t j = 0; j < result->cols; j++)
        {
            PyMatrixAssign(result, i, j, ComplexVar_div(PyMatrixGetitem(x, i, j), y));
        }
    }
    return 0;
}

int Matrix_idiv(PyMatrixObject *x, const ComplexVar &y)
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
    return 0;
}

int Matrix_div(const ComplexVar &x, const PyMatrixObject *const y, PyMatrixObject *result)
{
    result->rows = y->rows;
    result->cols = y->cols;
    if (PyMatrixAlloc(result))
    {
        return -1;
    }
    for (Py_ssize_t i = 0; i < result->rows; i++)
    {
        for (Py_ssize_t j = 0; j < result->cols; j++)
        {
            PyMatrixAssign(result, i, j, ComplexVar_div(x, PyMatrixGetitem(y, i, j)));
        }
    }
    return 0;
}

int Matrix_fdv(const PyMatrixObject *const x, const ComplexVar &y, PyMatrixObject *result)
{
    result->rows = x->rows;
    result->cols = x->cols;
    if (PyMatrixAlloc(result))
    {
        return -1;
    }
    for (Py_ssize_t i = 0; i < result->rows; i++)
    {
        for (Py_ssize_t j = 0; j < result->cols; j++)
        {
            PyMatrixAssign(result, i, j, ComplexVar_fdv(PyMatrixGetitem(x, i, j), y));
        }
    }
    return 0;
}

int Matrix_ifdv(PyMatrixObject *x, const ComplexVar &y)
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
    return 0;
}

int Matrix_fdv(const ComplexVar &x, const PyMatrixObject *const y, PyMatrixObject *result)
{
    result->rows = y->rows;
    result->cols = y->cols;
    if (PyMatrixAlloc(result))
    {
        return -1;
    }
    for (Py_ssize_t i = 0; i < result->rows; i++)
    {
        for (Py_ssize_t j = 0; j < result->cols; j++)
        {
            PyMatrixAssign(result, i, j, ComplexVar_fdv(x, PyMatrixGetitem(y, i, j)));
        }
    }
    return 0;
}

int Matrix_mod(const PyMatrixObject *const x, const ComplexVar &y, PyMatrixObject *result)
{
    result->rows = x->rows;
    result->cols = x->cols;
    if (PyMatrixAlloc(result))
    {
        return -1;
    }
    for (Py_ssize_t i = 0; i < result->rows; i++)
    {
        for (Py_ssize_t j = 0; j < result->cols; j++)
        {
            PyMatrixAssign(result, i, j, ComplexVar_mod(PyMatrixGetitem(x, i, j), y));
        }
    }
    return 0;
}

int Matrix_imod(PyMatrixObject *x, const ComplexVar &y)
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
    return 0;
}

int Matrix_mod(const ComplexVar &x, const PyMatrixObject *const y, PyMatrixObject *result)
{
    result->rows = y->rows;
    result->cols = y->cols;
    if (PyMatrixAlloc(result))
    {
        return -1;
    }
    for (Py_ssize_t i = 0; i < result->rows; i++)
    {
        for (Py_ssize_t j = 0; j < result->cols; j++)
        {
            PyMatrixAssign(result, i, j, ComplexVar_mod(x, PyMatrixGetitem(y, i, j)));
        }
    }
    return 0;
}

int Matrix_neg(const PyMatrixObject *const self, PyMatrixObject *result)
{
    result->rows = self->rows;
    result->cols = self->cols;
    if (PyMatrixAlloc(result))
    {
        return -1;
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
    return 0;
}

int Matrix_conj(const PyMatrixObject *const self, PyMatrixObject *result)
{
    result->rows = self->rows;
    result->cols = self->cols;
    if (PyMatrixAlloc(result))
    {
        return -1;
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
    return 0;
}

int Matrix_iconj(PyMatrixObject *self)
{
    for (Py_ssize_t i = 0; i < self->rows; i++)
    {
        for (Py_ssize_t j = 0; j < self->cols; j++)
        {
            ComplexVar_iconj(PyMatrixGetitem(self, i, j));
        }
    }
    return 0;
}

int Matrix_transpose(const PyMatrixObject *const self, PyMatrixObject *result)
{
    result->rows = self->cols;
    result->cols = self->rows;
    if (PyMatrixAlloc(result))
    {
        return -1;
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
    return 0;
}

void rorv(ComplexVar *a, int x)
{
    if (x == 1)
    {
        return;
    }
    ComplexVar tmp = a[x - 1];
    int j = x - 2;
    for (int i = x - 1; i >= 1; i--, j--)
    {
        std::swap(a[i], a[j]);
    }
    a[0] = tmp;
    return;
}

void itranspose(ComplexVar *a, int r, int c)
{
    int from;
    int to = 0;
    int offset = 0;
    int step = c;
    for (int i = 0; i < c; i++)
    {
        from = i + offset;
        for (int j = 0; j < r; j++)
        {
            rorv(a + to, from - to + 1);
            from += step;
            to++;
        }
        step--;
        offset += r - 1;
    }
    return;
}

int Matrix_itranspose(PyMatrixObject *self)
{
    itranspose(self->elements, self->rows, self->cols);
    std::swap(self->rows, self->cols);
    return 0;
}

int Matrix_hermite_transpose(const PyMatrixObject *const self, PyMatrixObject *result)
{
    result->rows = self->cols;
    result->cols = self->rows;
    if (PyMatrixAlloc(result))
    {
        return -1;
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
    return 0;
}

int Matrix_ihermite_transpose(PyMatrixObject *self)
{
    Matrix_itranspose(self);
    Matrix_iconj(self);
    return 0;
}

int Matrix_kronecker(const PyMatrixObject *const x, const PyMatrixObject *const y, PyMatrixObject *result)
{
    result->rows = x->rows * y->rows;
    result->cols = x->cols * y->cols;
    if (PyMatrixAlloc(result))
    {
        return -1;
    }
    for (Py_ssize_t i = 0; i < x->rows; i++)
    {
        for (Py_ssize_t j = 0; j < x->cols; j++)
        {
            for (Py_ssize_t p = 0; p < y->rows; p++)
            {
                for (Py_ssize_t q = 0; q < y->cols; q++)
                {
                    PyMatrixAssign(result, i * y->rows + p, j * y->cols + q, ComplexVar_mul(PyMatrixGetitem(x, i, j), PyMatrixGetitem(y, p, q)));
                }
            }
        }
    }
    return 0;
}

int Matrix_hadamard(const PyMatrixObject *const x, const PyMatrixObject *const y, PyMatrixObject *result)
{
    if (!Matrix_sameshape(x, y))
    {
        PyErr_Format(PyExc_ShapeError, "cannot do hadamard mul matrix with shape (%ld,%ld) and matrix with shape (%ld,%ld)", x->rows, x->cols, y->rows, y->cols);
        return -1;
    }
    result->rows = x->rows;
    result->cols = x->cols;
    if (PyMatrixAlloc(result))
    {
        return -1;
    }
    for (Py_ssize_t i = 0; i < result->rows; i++)
    {
        for (Py_ssize_t j = 0; j < result->cols; j++)
        {
            PyMatrixAssign(result, i, j, ComplexVar_mul(PyMatrixGetitem(x, i, j), PyMatrixGetitem(y, i, j)));
        }
    }
    return 0;
}