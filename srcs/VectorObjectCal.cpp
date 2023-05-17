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

error_double Vector_minkowsi_distance(const PyVectorObject *const x, int p)
{
    error_double result = 0;
    for (int i = 0; i < x->matrix.total_elements; i++)
    {
        result += fastpow(ComplexVar_L2(PyVectorGetitem(x, i)), p);
    }
    result = pow(result, 1.0 / p);
    return result;
}

error_double Vector_L1(const PyVectorObject *const x)
{
    error_double result = 0;
    for (int i = 0; i < x->matrix.total_elements; i++)
    {
        result += ComplexVar_L2(PyVectorGetitem(x, i));
    }
    return result;
}

error_double Vector_L2(const PyVectorObject *const x)
{
    error_double result = 0;
    for (int i = 0; i < x->matrix.total_elements; i++)
    {
        result += ComplexVar_squaredL2(PyVectorGetitem(x, i));
    }
    result = sqrt(result);
    return result;
}

error_double Vector_Linf(const PyVectorObject *const x)
{
    error_double result = 0;
    error_double tmp;
    for (int i = 0; i < x->matrix.total_elements; i++)
    {
        tmp = ComplexVar_L2(PyVectorGetitem(x, i));
        if (result.value < tmp.value)
        {
            result = tmp;
        }
    }
    return result;
}