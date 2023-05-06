#include <matrix.h>

void Matrix_row_switch(PyMatrixObject *self, int r1, int r2)
{
    if (r1 == r2)
    {
        return;
    }
    for (Py_ssize_t i = 0; i < self->cols; i++)
    {
        std::swap(PyMatrixGetitem(self, r1, i), PyMatrixGetitem(self, r2, i));
    }
    return;
}

void Matrix_row_multiply(PyMatrixObject *self, int r, const ComplexVar &num)
{
    for (Py_ssize_t i = 0; i < self->cols; i++)
    {
        PyMatrixAssign(self, r, i, ComplexVar_mul(PyMatrixGetitem(self, r, i), num));
    }
    return;
}

void Matrix_row_add(PyMatrixObject *self, int r1, int r2, const ComplexVar &num)
{
    if (r1 == r2)
    {
        Matrix_row_multiply(self, r1, ComplexVar_add(num, {1, 0, false}));
        return;
    }
    for (Py_ssize_t i = 0; i < self->cols; i++)
    {
        PyMatrixAssign(self, r1, i, ComplexVar_add(PyMatrixGetitem(self, r1, i), ComplexVar_mul(PyMatrixGetitem(self, r2, i), num)));
    }
    return;
}

void Matrix_col_switch(PyMatrixObject *self, int c1, int c2)
{
    if (c1 == c2)
    {
        return;
    }
    for (Py_ssize_t i = 0; i < self->rows; i++)
    {
        std::swap(PyMatrixGetitem(self, i, c1), PyMatrixGetitem(self, i, c2));
    }
    return;
}

void Matrix_col_multiply(PyMatrixObject *self, int c, const ComplexVar &num)
{
    for (Py_ssize_t i = 0; i < self->rows; i++)
    {
        PyMatrixAssign(self, i, c, ComplexVar_mul(PyMatrixGetitem(self, i, c), num));
    }
    return;
}

void Matrix_col_add(PyMatrixObject *self, int c1, int c2, const ComplexVar &num)
{
    if (c1 == c2)
    {
        Matrix_col_multiply(self, c1, ComplexVar_add(num, {1, 0, false}));
        return;
    }
    for (Py_ssize_t i = 0; i < self->rows; i++)
    {
        PyMatrixAssign(self, i, c1, ComplexVar_add(PyMatrixGetitem(self, i, c1), ComplexVar_mul(PyMatrixGetitem(self, i, c2), num)));
    }
    return;
}

int Matrix_rank(const PyMatrixObject *const self)
{
    PyMatrixObject *tmp = nullptr;
    int rank = 0;
    tmp = (PyMatrixObject *)PyMatrix_copy(self);
    if (!tmp)
    {
        return FAIL_RANK;
    }
    for (Py_ssize_t r = 0; r < std::min(tmp->rows, tmp->cols); r++)
    {
        Py_ssize_t maxr = r;
        Py_ssize_t maxc = r;
        double L = 0;
        error_double l;
        bool valid_value = false;
        ComplexVar tmp_item;
        for (Py_ssize_t R = r; R < tmp->rows; R++)
        {
            for (Py_ssize_t C = r; C < tmp->cols; C++)
            {
                tmp_item = PyMatrixGetitem(tmp, R, C);
                l = ComplexVar_L1(tmp_item);
                if (!tmp_item.isArbitrary)
                {
                    valid_value = true;
                    if ((l > L) == 2)
                    {
                        maxr = R;
                        maxc = C;
                        L = l.value;
                    }
                }
            }
        }
        if (!valid_value)
        {
            Py_DECREF(tmp);
            return UNSURE_RANK;
        }
        if (L == 0)
        {
            break;
        }
        Matrix_row_switch(tmp, r, maxr);
        Matrix_col_switch(tmp, r, maxc);
        tmp_item = ComplexVar_ivt(PyMatrixGetitem(tmp, r, r));
        PyMatrixGetitem(tmp, r, r) = {1, 0, false};
        rank++;
        for (Py_ssize_t subc = r + 1; subc < tmp->cols; subc++)
        {
            PyMatrixAssign(tmp, r, subc, ComplexVar_mul(tmp_item, PyMatrixGetitem(tmp, r, subc)));
        }
        for (Py_ssize_t subr = r + 1; subr < tmp->rows; subr++)
        {
            tmp_item = PyMatrixGetitem(tmp, subr, r);
            PyMatrixGetitem(tmp, subr, r) = {0, 0, false};
            for (Py_ssize_t subc = r + 1; subc < tmp->cols; subc++)
            {
                PyMatrixAssign(tmp, subr, subc, ComplexVar_sub(PyMatrixGetitem(tmp, subr, subc), ComplexVar_mul(tmp_item, PyMatrixGetitem(tmp, r, subc))));
            }
        }
    }
    Py_DECREF(tmp);
    return rank;
}