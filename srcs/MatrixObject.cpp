#include <matrix.h>

extern int doubleprecision;
bool fastprint = false;
int escape_rows_from = 3;
int escape_rows_to = 3;
int escape_cols_from = 3;
int escape_cols_to = 3;

PyObject *SetFastPrint(PyObject *self, PyObject *value)
{
    if (!PyBool_Check(value))
    {
        PyErr_SetString(PyExc_ValueError, "Fail to set value! Only accept bool.");
    }
    else
    {
        if (Py_IsTrue(value))
        {
            fastprint = true;
        }
        else
        {
            fastprint = false;
        }
    }
    Py_RETURN_NONE;
}

PyObject *SetPrintArea(PyObject *self, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {
        (char *)"left",
        (char *)"right",
        (char *)"up",
        (char *)"down",
        nullptr,
    };
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|$iiii", kwlist, &escape_cols_from, &escape_cols_to, &escape_rows_from, &escape_rows_to))
    {
        PyErr_SetString(PyExc_ValueError, "Fail to set value! Only accept ints.");
    }
    Py_RETURN_NONE;
}

int PyMatrixAlloc(PyMatrixObject *self)
{
    self->total_elements = ((int64_t)self->rows) * ((int64_t)self->cols);
    try
    {
        self->elements = new ComplexVar[self->total_elements];
    }
    catch (const std::bad_array_new_length &e)
    {
        PyErr_Format(PyExc_MemoryError, "Fail to allocate %lld numbers.", self->total_elements);
        self->elements = nullptr;
        return -1;
    }
    catch (const std::bad_alloc &e)
    {
        PyErr_Format(PyExc_MemoryError, "Fail to allocate %lld numbers.", self->total_elements);
        self->elements = nullptr;
        return -1;
    }
    return 0;
}

void PyMatrixAssign(PyMatrixObject *self, int r, int c, const ComplexVar &value)
{
    self->elements[r * self->cols + c] = value;
}

void PyMatrix_dealloc(PyMatrixObject *self)
{
    if (self->elements)
    {
        delete[] self->elements;
    }
    Py_TYPE(self)->tp_free((PyObject *)self);
}

PyObject *PyMatrix_repr(PyMatrixObject *self)
{
    std::string repr;
    repr += "[";
    if (fastprint)
    {
        bool rowskipped = false;
        bool colskipped;
        int rowd = self->rows - escape_rows_to;
        int cold = self->cols - escape_cols_to;
        for (int r = 0; r < self->rows; r++)
        {
            if (r < escape_rows_from || rowd <= r)
            {
                repr += "[";
                colskipped = false;
                for (int c = 0; c < self->cols; c++)
                {
                    if (c < escape_cols_from || cold <= c)
                    {
                        repr += ComplexVar_repr(self->elements[r * self->cols + c]).str().c_str();
                        repr += ",";
                    }
                    else if (!colskipped)
                    {
                        repr += "...,";
                        colskipped = true;
                    }
                }
                repr += "],";
            }
            else if (!rowskipped)
            {
                repr += "[";
                for (int c = 0; c < std::min(self->cols, escape_cols_from + escape_cols_to + 1); c++)
                {
                    repr += "...,";
                }
                repr += "],";
                rowskipped = true;
            }
        }
        repr += "]";
    }
    else
    {
        for (int r = 0; r < self->rows; r++)
        {
            repr += "[";
            for (int c = 0; c < self->cols; c++)
            {
                repr += ComplexVar_repr(self->elements[r * self->cols + c]).str().c_str();
                repr += ",";
            }
            repr += "],";
        }
        repr += "]";
    }
    return PyUnicode_FromString(repr.c_str());
}

PyObject *PyMatrix_str(PyMatrixObject *self)
{
    std::string repr;
    if (fastprint)
    {
        bool rowskipped = false;
        bool colskipped;
        int rowd = self->rows - escape_rows_to;
        int cold = self->cols - escape_cols_to;
        for (int r = 0; r < self->rows; r++)
        {
            if (r < escape_rows_from || rowd <= r)
            {
                repr += "[";
                colskipped = false;
                for (int c = 0; c < self->cols; c++)
                {
                    if (c < escape_cols_from || cold <= c)
                    {
                        repr += ComplexVar_str(self->elements[r * self->cols + c]).str().c_str();
                        if (c != self->cols - 1)
                        {
                            repr += "\t";
                        }
                    }
                    else if (!colskipped)
                    {
                        repr += "...";
                        if (c != self->cols)
                        {
                            repr += "\t";
                        }
                        colskipped = true;
                    }
                }
                repr += "]";
                if (r != self->rows - 1)
                {
                    repr += "\n";
                }
            }
            else if (!rowskipped)
            {
                repr += "[";
                for (int c = 0; c < std::min(self->cols, escape_cols_from + escape_cols_to + 1); c++)
                {
                    repr += "...";
                    if (c != self->cols - 1)
                    {
                        repr += "\t";
                    }
                }
                repr += "]";
                if (r != self->rows - 1)
                {
                    repr += "\n";
                }
                rowskipped = true;
            }
        }
    }
    else
    {
        for (int r = 0; r < self->rows; r++)
        {
            repr += "[";
            for (int c = 0; c < self->cols; c++)
            {
                repr += ComplexVar_str(self->elements[r * self->cols + c]).str().c_str();
                if (c != self->cols - 1)
                {
                    repr += "\t";
                }
            }
            repr += "]";
            if (r != self->rows - 1)
            {
                repr += "\n";
            }
        }
    }
    return PyUnicode_FromString(repr.c_str());
}

static int PyMatrix_init_from_rcf(PyMatrixObject *self, PyObject *fill)
{
    if (self->rows <= 0 || self->cols <= 0)
    {
        PyErr_Format(PyExc_ValueError, "Invalid size. rows: %ld, cols: %ld", self->rows, self->cols);
        return -1;
    }
    ComplexVar tmp_value;
    if (assignComplexVar(fill, tmp_value))
    {
        PyErr_Format(PyExc_ValueError, "Unsupported type: %s", fill->ob_type->tp_name);
        return -1;
    }
    if (PyMatrixAlloc(self))
    {
        return -1;
    }
    for (int64_t i = 0; i < self->total_elements; i++)
    {
        self->elements[i] = tmp_value;
    }
    return 0;
}

static int PyMatrix_init_from_Mf(PyMatrixObject *self, PyObject *matrix, PyObject *fill, PyObject *&tmp)
{
    self->rows = PyList_Size(matrix);
    self->cols = PyList_Size(tmp);
    for (Py_ssize_t i = 1; i < self->rows; i++)
    {
        tmp = PyList_GetItem(matrix, i);
        if (!PyList_CheckExact(tmp))
        {
            PyErr_Format(PyExc_ValueError, "Inconsistency value format. On index:%ld", i);
            return -1;
        }
        if (self->cols < PyList_Size(tmp))
        {
            self->cols = PyList_Size(tmp);
        }
    }
    ComplexVar tmp_value;
    if (assignComplexVar(fill, tmp_value))
    {
        PyErr_Format(PyExc_ValueError, "Unsupported type: %s", fill->ob_type->tp_name);
        return -1;
    }
    if (PyMatrixAlloc(self))
    {
        return -1;
    }
    int cc;
    ComplexVar tmp_matrix_value;
    for (Py_ssize_t i = 0; i < self->rows; i++)
    {
        tmp = PyList_GetItem(matrix, i);
        cc = PyList_Size(tmp);
        for (Py_ssize_t j = 0; j < self->cols; j++)
        {
            if (j < cc)
            {
                if (assignComplexVar(PyList_GetItem(tmp, j), tmp_matrix_value))
                {
                    PyErr_Format(PyExc_ValueError, "Unsupported type: at row %ld col %ld", i, j);
                    return -1;
                }
                PyMatrixAssign(self, i, j, tmp_matrix_value);
            }
            else
            {
                PyMatrixAssign(self, i, j, tmp_value);
            }
        }
    }
    return 0;
}

static int PyMatrix_init_from_sMf(PyMatrixObject *self, PyObject *smatrix, PyObject *fill, PyObject *&tmp)
{
    self->rows = 0;
    self->cols = 0;
    Py_ssize_t total = PyList_Size(smatrix);
    PyObject *pair;
    int r;
    int c;
    for (Py_ssize_t i = 0; i < total; i++)
    {
        tmp = PyList_GetItem(smatrix, i);
        if (!PyTuple_CheckExact(tmp))
        {
            PyErr_Format(PyExc_ValueError, "Inconsistency value format. On index:%ld", i);
            return -1;
        }
        if (PyTuple_Size(tmp) != 3)
        {
            PyErr_Format(PyExc_ValueError, "Only take 3 items. On index:%ld", i);
            return -1;
        }
        if (!PyArg_ParseTuple(tmp, "iiO", &r, &c, &pair))
        {
            PyErr_Format(PyExc_ValueError, "Fail to parse tuple. On index:%ld", i);
            return -1;
        }
        if (r > self->rows)
        {
            self->rows = r;
        }
        if (c > self->cols)
        {
            self->cols = c;
        }
    }
    ++self->rows;
    ++self->cols;
    ComplexVar tmp_value;
    if (assignComplexVar(fill, tmp_value))
    {
        PyErr_Format(PyExc_ValueError, "Unsupported type: %s", fill->ob_type->tp_name);
        return -1;
    }
    if (PyMatrixAlloc(self))
    {
        return -1;
    }
    for (Py_ssize_t i = 0; i < self->rows; i++)
    {
        for (Py_ssize_t j = 0; j < self->cols; j++)
        {
            PyMatrixAssign(self, i, j, tmp_value);
        }
    }
    ComplexVar tmp_matrix_value;
    for (Py_ssize_t i = 0; i < total; i++)
    {
        PyArg_ParseTuple(PyList_GetItem(smatrix, i), "iiO", &r, &c, &pair);
        if (assignComplexVar(pair, tmp_matrix_value))
        {
            PyErr_Format(PyExc_ValueError, "Unsupport type: %s On index:%ld", pair->ob_type->tp_name, i);
            return -1;
        }
        PyMatrixAssign(self, r, c, tmp_matrix_value);
    }
    return 0;
}

template <typename T>
concept npy_real = std::is_integral<T>::value || std::is_floating_point<T>::value;
static void assign_from_np_R(ComplexVar *target, int rows, int cols, npy_real auto *data)
{
    int64_t pos;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            pos = i * cols + j;
            target[pos].real = data[pos];
            target[pos].imag = 0;
            target[pos].isArbitrary = false;
        }
    }
}

template <typename T>
concept npy_complex = requires(T a) {
                          a.real;
                          a.imag;
                      };
static void assign_from_np_C(ComplexVar *target, int rows, int cols, npy_complex auto *data)
{
    int64_t pos;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            pos = i * cols + j;
            target[pos].real = data[pos].real;
            target[pos].imag = data[pos].imag;
            target[pos].isArbitrary = false;
        }
    }
}

static void assign_from_half(ComplexVar *target, int rows, int cols, npy_half *data)
{
    int64_t pos;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            pos = i * cols + j;
            target[pos].real = casthalf_to_double(data[pos]);
            target[pos].imag = 0;
            target[pos].isArbitrary = false;
        }
    }
}

static int assign_from_obj(ComplexVar *target, int rows, int cols, PyObject **data)
{
    int64_t pos;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            pos = i * cols + j;
            if (assignComplexVar(data[pos], target[pos]))
            {
                PyObject *type;
                PyObject *value;
                PyObject *traceback;
                PyErr_Fetch(&type, &value, &traceback);
                PyObject *newvalue = PyUnicode_FromFormat("%S\nAt row:%ld\tcol%ld", value, i, j);
                PyErr_Restore(type, newvalue, traceback);
                Py_DECREF(value);
                return -1;
            }
        }
    }
    return 0;
}

static int PyMatrix_init_from_nparray(PyMatrixObject *self, PyArrayObject *array)
{
    if (PyArray_NDIM(array) != 2)
    {
        PyErr_SetString(PyExc_ValueError, "Only take 2 dimension array");
        return -1;
    }
    void *data = PyArray_DATA(array);
    self->rows = PyArray_DIM(array, 0);
    self->cols = PyArray_DIM(array, 1);
    if (PyMatrixAlloc(self))
    {
        return -1;
    }
    switch (PyArray_DESCR(array)->type_num)
    {
    case NPY_BYTE:
    {
        npy_byte *A = (npy_byte *)data;
        assign_from_np_R(self->elements, self->rows, self->cols, A);
        break;
    }
    case NPY_UBYTE:
    {
        npy_ubyte *A = (npy_ubyte *)data;
        assign_from_np_R(self->elements, self->rows, self->cols, A);
        break;
    }
    case NPY_SHORT:
    {
        npy_short *A = (npy_short *)data;
        assign_from_np_R(self->elements, self->rows, self->cols, A);
        break;
    }
    case NPY_USHORT:
    {
        npy_ushort *A = (npy_ushort *)data;
        assign_from_np_R(self->elements, self->rows, self->cols, A);
        break;
    }
    case NPY_INT:
    {
        npy_int *A = (npy_int *)data;
        assign_from_np_R(self->elements, self->rows, self->cols, A);
        break;
    }
    case NPY_UINT:
    {
        npy_uint *A = (npy_uint *)data;
        assign_from_np_R(self->elements, self->rows, self->cols, A);
        break;
    }
    case NPY_LONG:
    {
        npy_long *A = (npy_long *)data;
        assign_from_np_R(self->elements, self->rows, self->cols, A);
        break;
    }
    case NPY_ULONG:
    {
        npy_ulong *A = (npy_ulong *)data;
        assign_from_np_R(self->elements, self->rows, self->cols, A);
        break;
    }
    case NPY_LONGLONG:
    {
        npy_longlong *A = (npy_longlong *)data;
        assign_from_np_R(self->elements, self->rows, self->cols, A);
        break;
    }
    case NPY_ULONGLONG:
    {
        npy_ulonglong *A = (npy_ulonglong *)data;
        assign_from_np_R(self->elements, self->rows, self->cols, A);
        break;
    }
    case NPY_FLOAT:
    {
        npy_float *A = (npy_float *)data;
        assign_from_np_R(self->elements, self->rows, self->cols, A);
        break;
    }
    case NPY_DOUBLE:
    {
        npy_double *A = (npy_double *)data;
        assign_from_np_R(self->elements, self->rows, self->cols, A);
        break;
    }
    case NPY_LONGDOUBLE:
    {
        npy_longdouble *A = (npy_longdouble *)data;
        assign_from_np_R(self->elements, self->rows, self->cols, A);
        break;
    }
    case NPY_CFLOAT:
    {
        npy_cfloat *A = (npy_cfloat *)data;
        assign_from_np_C(self->elements, self->rows, self->cols, A);
        break;
    }
    case NPY_CDOUBLE:
    {
        npy_cdouble *A = (npy_cdouble *)data;
        assign_from_np_C(self->elements, self->rows, self->cols, A);
        break;
    }
    case NPY_CLONGDOUBLE:
    {
        npy_clongdouble *A = (npy_clongdouble *)data;
        assign_from_np_C(self->elements, self->rows, self->cols, A);
        break;
    }
    case NPY_OBJECT:
    {
        PyObject **A = (PyObject **)data;
        if (assign_from_obj(self->elements, self->rows, self->cols, A))
        {
            return -1;
        }
        break;
    }
    case NPY_HALF:
    {
        npy_half *A = (npy_half *)data;
        assign_from_half(self->elements, self->rows, self->cols, A);
        break;
    }
    default:
    {
        PyErr_SetString(PyExc_ValueError, "Unsupport dtype.");
        return -1;
    }
    }
    return 0;
}

int PyMatrix_init(PyMatrixObject *self, PyObject *args, PyObject *kwds)
{
    self->elements = nullptr;
    static char *kwlist_0[] = {
        (char *)"rows",
        (char *)"cols",
        (char *)"fill",
        nullptr,
    };
    static char *kwlist_1[] = {
        (char *)"matrix",
        (char *)"fill",
        nullptr,
    };
    static char *kwlist_2[] = {
        (char *)"matrix",
        nullptr,
    };
    PyObject *tmp_object = nullptr;
    PyObject *tmp_matrix = nullptr;
    if (PyArg_ParseTupleAndKeywords(args, kwds, "ii|O", kwlist_0, &self->rows, &self->cols, &tmp_object))
    {
        if (PyMatrix_init_from_rcf(self, tmp_object))
        {
            return -1;
        }
        goto PyMatrix_init_done;
    }
    if (PyArg_ParseTupleAndKeywords(args, kwds, "O|O", kwlist_1, &tmp_matrix, &tmp_object))
    {
        if (PyList_CheckExact(tmp_matrix))
        {
            if (!PyList_Size(tmp_matrix))
            {
                PyErr_SetString(PyExc_ValueError, "Empty list.");
                return -1;
            }
            PyObject *tmp;
            tmp = PyList_GetItem(tmp_matrix, 0);
            if (PyList_CheckExact(tmp))
            {
                if (PyMatrix_init_from_Mf(self, tmp_matrix, tmp_object, tmp))
                {
                    return -1;
                }
                goto PyMatrix_init_done;
            }
            else if (PyTuple_CheckExact(tmp))
            {
                if (PyMatrix_init_from_sMf(self, tmp_matrix, tmp_object, tmp))
                {
                    return -1;
                }
                goto PyMatrix_init_done;
            }
        }
    }
    if (PyArg_ParseTupleAndKeywords(args, kwds, "O", kwlist_2, &tmp_matrix))
    {
        if (PyArray_Check(tmp_matrix))
        {
            if (PyMatrix_init_from_nparray(self, (PyArrayObject *)tmp_matrix))
            {
                return -1;
            }
            goto PyMatrix_init_done;
        }
    }
    PyErr_SetString(PyExc_ValueError, "Fail to match init arguments.");
    return -1;
PyMatrix_init_done:
    PyErr_Clear();
    return 0;
}

PyObject *PyMatrix_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyObject *self = type->tp_alloc(type, 0);
    if (!self)
    {
        Py_RETURN_NONE;
    }
    ((PyMatrixObject *)self)->elements = nullptr;
    return self;
}

static PyMemberDef PyMatrixMember[] = {
    {"rows", T_INT, offsetof(PyMatrixObject, rows), READONLY, nullptr},
    {"cols", T_INT, offsetof(PyMatrixObject, cols), READONLY, nullptr},
    {"total", T_LONGLONG, offsetof(PyMatrixObject, total_elements), READONLY, nullptr},
    nullptr,
};

PyTypeObject PyMatrixType = {
    .ob_base = PyVarObject_HEAD_INIT(&PyType_Type, 0).tp_name = "matrixcore.matrix",
    .tp_basicsize = sizeof(PyMatrixObject),
    .tp_dealloc = (destructor)PyMatrix_dealloc,
    .tp_repr = (reprfunc)PyMatrix_repr,
    .tp_str = (reprfunc)PyMatrix_str,
    .tp_members = PyMatrixMember,
    .tp_init = (initproc)PyMatrix_init,
    .tp_new = (newfunc)PyMatrix_new,
};