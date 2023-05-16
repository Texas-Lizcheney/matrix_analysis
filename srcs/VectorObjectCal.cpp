#include <vector.h>

extern PyObject *PyExc_ShapeError;

int Vector_dot(const PyVectorObject *const x, const PyVectorObject *const y, ComplexVar &result)
{
    if (x->matrix.total_elements != y->matrix.total_elements)
    {
        PyErr_Format(PyExc_ShapeError, "cannot dot vector with dimensions %ld and vector with dimension %ld", x->matrix.total_elements, y->matrix.total_elements);
        return -1;
    }
    result = 0;
    for (int i = 0; i < x->matrix.total_elements; i++)
    {
        result += PyVectorGetitem(x, i) * ComplexVar_conj(PyVectorGetitem(y, i));
    }
    return 0;
}