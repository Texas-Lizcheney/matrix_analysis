#include <vector.h>

extern int doubleprecision;

static PyObject *PyVector_repr(PyVectorObject *self)
{
    std::stringstream repr;
    repr << std::setprecision(doubleprecision) << "[";
    for (int d = 0; d < self->matrix.total_elements; d++)
    {
        repr << PyVectorGetitem(self, d) << ',';
    }
    repr << "]";
    return PyUnicode_FromString(repr.str().c_str());
}

static int PyVector_init_from_df(PyVectorObject *self, PyObject *fill)
{
    ComplexVar tmp_value;
    if (assignComplexVar_withExc(fill, tmp_value))
    {
        return -1;
    }
    if (PyMatrixAlloc((PyMatrixObject *)self))
    {
        return -1;
    }
    for (int64_t i = 0; i < self->matrix.total_elements; i++)
    {
        self->matrix.elements[i] = tmp_value;
    }
    return 0;
}

static int PyVector_init_from_Vf(PyVectorObject *self, PyObject *vector)
{
    if (self->isHor)
    {
        self->matrix.cols = PyList_Size(vector);
    }
    else
    {
        self->matrix.rows = PyList_Size(vector);
    }
    if (PyMatrixAlloc((PyMatrixObject *)self))
    {
        return -1;
    }
    for (Py_ssize_t i = 0; i < self->matrix.total_elements; i++)
    {
        if (assignComplexVar_withExc(PyList_GetItem(vector, i), PyVectorGetitem(self, i)))
        {
            Addmessage("At index:%ld", i);
            return -1;
        }
    }
    return 0;
}

static int PyVector_init_from_sVf(PyVectorObject *self, PyObject *vector, PyObject *fill)
{
    int tmp_dims = 0;
    Py_ssize_t total = PyList_Size(vector);
    PyObject *tmp = nullptr;
    int pos;
    PyObject *pair;
    for (Py_ssize_t i = 0; i < total; i++)
    {
        tmp = PyList_GetItem(vector, i);
        if (!PyTuple_CheckExact(tmp))
        {
            PyErr_Format(PyExc_ValueError, "Inconsistency value format. On index:%ld", i);
            return -1;
        }
        if (!PyArg_ParseTuple(tmp, "iO", &pos, &pair))
        {
            Addmessage("On index:%ld", i);
            return -1;
        }
        if (pos > tmp_dims)
        {
            tmp_dims = pos;
        }
    }
    ++tmp_dims;
    if (self->isHor)
    {
        self->matrix.cols = tmp_dims;
    }
    else
    {
        self->matrix.rows = tmp_dims;
    }
    ComplexVar tmp_value;
    if (PyVector_init_from_df(self, fill))
    {
        return -1;
    }
    for (Py_ssize_t i = 0; i < total; i++)
    {
        tmp = PyList_GetItem(vector, i);
        PyArg_ParseTuple(tmp, "iO", &pos, &pair);
        if (assignComplexVar_withExc(pair, tmp_value))
        {
            Addmessage("On index:%ld", i);
            return -1;
        }
        PyVectorAssign(self, i, tmp_value);
    }
    return 0;
}

static void assign_from_np_R(ComplexVar *target, int d, npy_real auto *data)
{
    for (int i = 0; i < d; i++)
    {
        target[i].real = data[i];
        target[i].imag = 0;
        target[i].isArbitrary = false;
    }
    return;
}

static void assign_from_np_C(ComplexVar *target, int d, npy_complex auto *data)
{
    for (int i = 0; i < d; i++)
    {
        target[i].real = data[i].real;
        target[i].imag = data[i].imag;
        target[i].isArbitrary = false;
    }
    return;
}

static int assign_from_obj(ComplexVar *target, int d, PyObject **data)
{
    for (int i = 0; i < d; i++)
    {
        if (assignComplexVar_withExc(data[d], target[d]))
        {
            Addmessage("On index:%ld", i);
            return -1;
        }
    }
    return 0;
}

static void assign_from_half(ComplexVar *target, int d, npy_half *data)
{
    for (int i = 0; i < d; i++)
    {
        target[i].real = casthalf_to_double(data[i]);
        target[i].imag = 0;
        target[i].isArbitrary = false;
    }
    return;
}

static int PyVector_init_from_nparray(PyVectorObject *self, PyArrayObject *vector)
{
    if (PyArray_NDIM(vector) != 1)
    {
        PyErr_SetString(PyExc_ValueError, "Only take 1 dimension array");
        return -1;
    }
    void *data = PyArray_DATA(vector);
    if (self->isHor)
    {
        self->matrix.cols = PyArray_DIM(vector, 0);
    }
    else
    {
        self->matrix.rows = PyArray_DIM(vector, 0);
    }
    if (PyMatrixAlloc((PyMatrixObject *)self))
    {
        return -1;
    }
    switch (PyArray_DESCR(vector)->type_num)
    {
    case NPY_BYTE:
    {
        assign_from_np_R(self->matrix.elements, self->matrix.total_elements, (npy_byte *)data);
        break;
    }
    case NPY_UBYTE:
    {
        assign_from_np_R(self->matrix.elements, self->matrix.total_elements, (npy_ubyte *)data);
        break;
    }
    case NPY_SHORT:
    {
        assign_from_np_R(self->matrix.elements, self->matrix.total_elements, (npy_short *)data);
        break;
    }
    case NPY_USHORT:
    {
        assign_from_np_R(self->matrix.elements, self->matrix.total_elements, (npy_ushort *)data);
        break;
    }
    case NPY_INT:
    {
        assign_from_np_R(self->matrix.elements, self->matrix.total_elements, (npy_int *)data);
        break;
    }
    case NPY_UINT:
    {
        assign_from_np_R(self->matrix.elements, self->matrix.total_elements, (npy_uint *)data);
        break;
    }
    case NPY_LONG:
    {
        assign_from_np_R(self->matrix.elements, self->matrix.total_elements, (npy_long *)data);
        break;
    }
    case NPY_ULONG:
    {
        assign_from_np_R(self->matrix.elements, self->matrix.total_elements, (npy_ulong *)data);
        break;
    }
    case NPY_LONGLONG:
    {
        assign_from_np_R(self->matrix.elements, self->matrix.total_elements, (npy_longlong *)data);
        break;
    }
    case NPY_ULONGLONG:
    {
        assign_from_np_R(self->matrix.elements, self->matrix.total_elements, (npy_ulonglong *)data);
        break;
    }
    case NPY_FLOAT:
    {
        assign_from_np_R(self->matrix.elements, self->matrix.total_elements, (npy_float *)data);
        break;
    }
    case NPY_DOUBLE:
    {
        assign_from_np_R(self->matrix.elements, self->matrix.total_elements, (npy_double *)data);
        break;
    }
    case NPY_LONGDOUBLE:
    {
        assign_from_np_R(self->matrix.elements, self->matrix.total_elements, (npy_longdouble *)data);
        break;
    }
    case NPY_CFLOAT:
    {
        assign_from_np_C(self->matrix.elements, self->matrix.total_elements, (npy_cfloat *)data);
        break;
    }
    case NPY_CDOUBLE:
    {
        assign_from_np_C(self->matrix.elements, self->matrix.total_elements, (npy_cdouble *)data);
        break;
    }
    case NPY_CLONGDOUBLE:
    {
        assign_from_np_C(self->matrix.elements, self->matrix.total_elements, (npy_clongdouble *)data);
        break;
    }
    case NPY_OBJECT:
    {
        return assign_from_obj(self->matrix.elements, self->matrix.total_elements, (PyObject **)data);
    }
    case NPY_HALF:
    {
        assign_from_half(self->matrix.elements, self->matrix.total_elements, (npy_half *)data);
        break;
    }
    default:
    {
        PyErr_SetString(PyExc_ValueError, "Unsupport dtype");
        return -1;
    }
    }
    return 0;
}

static int PyVector_init(PyVectorObject *self, PyObject *args, PyObject *kwds)
{
    static char *kwlist_0[] = {
        (char *)"dims",
        (char *)"fill",
        (char *)"is_horizontal",
        nullptr,
    };
    static char *kwlist_1[] = {
        (char *)"vector",
        (char *)"is_horizontal",
        nullptr,
    };
    static char *kwlist_2[] = {
        (char *)"vector",
        (char *)"fill",
        (char *)"is_horizontal",
        nullptr,
    };
    int dims;
    PyObject *tmp = nullptr;
    PyObject *direction = Py_False;
    if (PyArg_ParseTupleAndKeywords(args, kwds, "i|$OO", kwlist_0, &dims, &tmp, &direction))
    {
        if (Py_IsFalse(direction))
        {
            self->isHor = false;
            self->matrix.rows = dims;
            self->matrix.cols = 1;
        }
        else if (Py_IsTrue(direction))
        {
            self->isHor = true;
            self->matrix.rows = 1;
            self->matrix.cols = dims;
        }
        else
        {
            return -1;
        }
        return PyVector_init_from_df(self, tmp);
    }
    PyObject *vector = nullptr;
    PyErr_Clear();
    vector = nullptr;
    direction = Py_False;
    if (PyArg_ParseTupleAndKeywords(args, kwds, "O|$O", kwlist_1, &vector, &direction))
    {
        if (Py_IsFalse(direction))
        {
            self->isHor = false;
            self->matrix.cols = 1;
        }
        else if (Py_IsTrue(direction))
        {
            self->isHor = true;
            self->matrix.rows = 1;
        }
        else
        {
            return -1;
        }
        if (PyList_CheckExact(vector))
        {
            if (!PyList_Size(vector))
            {
                PyErr_SetString(PyExc_ValueError, "Empty list.");
                return -1;
            }
            return PyVector_init_from_Vf(self, vector);
        }
        if (PyArray_Check(vector))
        {
            return PyVector_init_from_nparray(self, (PyArrayObject *)vector);
        }
    }
    PyErr_Clear();
    vector = nullptr;
    tmp = nullptr;
    direction = Py_False;
    if (PyArg_ParseTupleAndKeywords(args, kwds, "O|$OO", kwlist_2, &vector, &tmp, &direction))
    {
        if (Py_IsFalse(direction))
        {
            self->isHor = false;
            self->matrix.cols = 1;
        }
        else if (Py_IsTrue(direction))
        {
            self->isHor = true;
            self->matrix.rows = 1;
        }
        else
        {
            return -1;
        }
        if (PyList_CheckExact(vector))
        {
            if (!PyList_Size(vector))
            {
                PyErr_SetString(PyExc_ValueError, "Empty list.");
                return -1;
            }
            return PyVector_init_from_sVf(self, vector, tmp);
        }
    }
    PyErr_SetString(PyExc_TypeError, "Fail to match any overload");
    return -1;
}

PyTypeObject PyVector_Type = {
    .ob_base = PyVarObject_HEAD_INIT(&PyType_Type, 0).tp_name = "matrixcore.vector",
    .tp_basicsize = sizeof(PyVectorObject),
    .tp_repr = (reprfunc)PyVector_repr,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_init = (initproc)PyVector_init,
};