#include <matrix.h>

void Matrix_row_switch(PyMatrixObject *self, int r1, int r2)
{
    for (Py_ssize_t i = 0; i < self->cols; i++)
    {
        std::swap(PyMatrixGetitem(self, r1, i), PyMatrixGetitem(self, r2, i));
    }
    return;
}

void Matrix_row_multiply(PyMatrixObject *, int, int, const ComplexVar &);
void Matrix_row_add(PyMatrixObject *, int, const ComplexVar &);
void Matrix_row_switch(PyMatrixObject *, int, int);
void Matrix_row_multiply(PyMatrixObject *, int, int, const ComplexVar &);
void Matrix_row_add(PyMatrixObject *, int, const ComplexVar &);