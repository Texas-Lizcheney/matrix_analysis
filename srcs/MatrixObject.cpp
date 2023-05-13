#include <matrix.h>

extern int doubleprecision;
extern bool fastprint;
extern int escape_rows_from;
extern int escape_rows_to;
extern int escape_cols_from;
extern int escape_cols_to;
extern PyObject *PyExc_ShapeError;

int PyMatrixAlloc(PyMatrixObject *self)
{
    if (self->rows <= 0 || self->cols <= 0)
    {
        PyErr_Format(PyExc_ValueError, "Invalid size. rows: %ld, cols: %ld", self->rows, self->cols);
        return -1;
    }
    self->total_elements = ((int64_t)self->rows) * ((int64_t)self->cols);
    try
    {
        self->elements = new ComplexVar[self->total_elements];
    }
    catch (const std::bad_array_new_length &e)
    {
        PyErr_Format(PyExc_MemoryError, "Fail to allocate %lld numbers", self->total_elements);
        self->elements = nullptr;
        return -1;
    }
    catch (const std::bad_alloc &e)
    {
        PyErr_Format(PyExc_MemoryError, "Fail to allocate %lld numbers", self->total_elements);
        self->elements = nullptr;
        return -1;
    }
    return 0;
}

int PyMatrixAssign_withcheck(PyMatrixObject *self, int r, int c, const ComplexVar &value)
{
    int R = r;
    int C = c;
    if (r < 0)
    {
        R = self->rows + r;
    }
    if (c < 0)
    {
        C = self->cols + c;
    }
    if ((R >= self->rows) || (C >= self->cols) || (R < 0) || (C < 0))
    {
        PyErr_SetString(PyExc_IndexError, "Index out of range");
        return -1;
    }
    self->elements[R * self->cols + C] = value;
    return 0;
}

int PyMatrixGet_withcheck(const PyMatrixObject *const self, int r, int c, ComplexVar &value)
{
    int R = r;
    int C = c;
    if (r < 0)
    {
        R = self->rows + r;
    }
    if (c < 0)
    {
        C = self->cols + c;
    }
    if ((R >= self->rows) || (C >= self->cols) || (R < 0) || (C < 0))
    {
        PyErr_SetString(PyExc_IndexError, "Index out of range");
        return -1;
    }
    value = self->elements[R * self->cols + C];
    return 0;
}

PyObject *PyMatrix_copy(const PyMatrixObject *const self)
{
    PyMatrixObject *result = nullptr;
    result = PyObject_New(PyMatrixObject, &PyMatrix_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->elements = nullptr;
    if (Matrix_copy(self, result))
    {
        Py_DECREF(result);
        return nullptr;
    }
    return (PyObject *)result;
}

static void PyMatrix_dealloc(PyMatrixObject *self)
{
    if (self->elements)
    {
        delete[] self->elements;
    }
    Py_TYPE(self)->tp_free((PyObject *)self);
}

static PyObject *PyMatrix_repr(PyMatrixObject *self)
{
    std::stringstream repr;
    repr << std::setprecision(doubleprecision) << "[";
    for (int r = 0; r < self->rows; r++)
    {
        repr << "[";
        for (int c = 0; c < self->cols; c++)
        {
            repr << PyMatrixGetitem(self, r, c) << ',';
        }
        repr << "],";
    }
    repr << "]";
    return PyUnicode_FromString(repr.str().c_str());
}

static PyObject *PyMatrix_str(PyMatrixObject *self)
{
    std::stringstream repr;
    repr << std::setprecision(doubleprecision);
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
                repr << "[";
                colskipped = false;
                for (int c = 0; c < self->cols; c++)
                {
                    if (c < escape_cols_from || cold <= c)
                    {
                        repr < PyMatrixGetitem(self, r, c);
                        if (c != self->cols - 1)
                        {
                            repr << "\t";
                        }
                    }
                    else if (!colskipped)
                    {
                        repr << "...";
                        if (c != self->cols)
                        {
                            repr << "\t";
                        }
                        colskipped = true;
                    }
                }
                repr << "]";
                if (r != self->rows - 1)
                {
                    repr << "\n";
                }
            }
            else if (!rowskipped)
            {
                repr << "[";
                for (int c = 0; c < std::min(self->cols, escape_cols_from + escape_cols_to + 1); c++)
                {
                    repr << "...";
                    if (c != self->cols - 1)
                    {
                        repr << "\t";
                    }
                }
                repr << "]";
                if (r != self->rows - 1)
                {
                    repr << "\n";
                }
                rowskipped = true;
            }
        }
    }
    else
    {
        for (int r = 0; r < self->rows; r++)
        {
            repr << "[";
            for (int c = 0; c < self->cols; c++)
            {
                repr < PyMatrixGetitem(self, r, c);
                if (c != self->cols - 1)
                {
                    repr << "\t";
                }
            }
            repr << "]";
            if (r != self->rows - 1)
            {
                repr << "\n";
            }
        }
    }
    return PyUnicode_FromString(repr.str().c_str());
}

static int PyMatrix_init_from_rcf(PyMatrixObject *self, PyObject *fill)
{
    ComplexVar tmp_value;
    if (assignComplexVar_withExc(fill, tmp_value))
    {
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

static int PyMatrix_init_from_Mf(PyMatrixObject *self, PyObject *matrix, PyObject *fill)
{
    PyObject *tmp = nullptr;
    self->rows = PyList_Size(matrix);
    self->cols = 0;
    for (Py_ssize_t i = 0; i < self->rows; i++)
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
    if (assignComplexVar_withExc(fill, tmp_value))
    {
        return -1;
    }
    if (PyMatrixAlloc(self))
    {
        return -1;
    }
    int cc;
    for (Py_ssize_t i = 0; i < self->rows; i++)
    {
        tmp = PyList_GetItem(matrix, i);
        cc = PyList_Size(tmp);
        for (Py_ssize_t j = 0; j < self->cols; j++)
        {
            if (j < cc)
            {
                if (assignComplexVar_withExc(PyList_GetItem(tmp, j), PyMatrixGetitem(self, i, j)))
                {
                    Addmessage("At row:%ld\tcol:%ld", i, j);
                    return -1;
                }
            }
            else
            {
                PyMatrixAssign(self, i, j, tmp_value);
            }
        }
    }
    return 0;
}

static int PyMatrix_init_from_sMf(PyMatrixObject *self, PyObject *smatrix, PyObject *fill)
{
    PyObject *tmp = nullptr;
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
        if (!PyArg_ParseTuple(tmp, "iiO", &r, &c, &pair))
        {
            Addmessage("On index:%ld", i);
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
    if (PyMatrix_init_from_rcf(self, fill))
    {
        return -1;
    }
    for (Py_ssize_t i = 0; i < total; i++)
    {
        PyArg_ParseTuple(PyList_GetItem(smatrix, i), "iiO", &r, &c, &pair);
        if (assignComplexVar_withExc(pair, PyMatrixGetitem(self, r, c)))
        {
            Addmessage("On index:%ld", i);
            return -1;
        }
    }
    return 0;
}

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
    return;
}

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
    return;
}

static int assign_from_obj(ComplexVar *target, int rows, int cols, PyObject **data)
{
    int64_t pos;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            pos = i * cols + j;
            if (assignComplexVar_withExc(data[pos], target[pos]))
            {
                Addmessage("At row:%ld\tcol%ld", i, j);
                return -1;
            }
        }
    }
    return 0;
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
    return;
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
        assign_from_np_R(self->elements, self->rows, self->cols, (npy_byte *)data);
        break;
    }
    case NPY_UBYTE:
    {
        assign_from_np_R(self->elements, self->rows, self->cols, (npy_ubyte *)data);
        break;
    }
    case NPY_SHORT:
    {
        assign_from_np_R(self->elements, self->rows, self->cols, (npy_short *)data);
        break;
    }
    case NPY_USHORT:
    {
        assign_from_np_R(self->elements, self->rows, self->cols, (npy_ushort *)data);
        break;
    }
    case NPY_INT:
    {
        assign_from_np_R(self->elements, self->rows, self->cols, (npy_int *)data);
        break;
    }
    case NPY_UINT:
    {
        assign_from_np_R(self->elements, self->rows, self->cols, (npy_uint *)data);
        break;
    }
    case NPY_LONG:
    {
        assign_from_np_R(self->elements, self->rows, self->cols, (npy_long *)data);
        break;
    }
    case NPY_ULONG:
    {
        assign_from_np_R(self->elements, self->rows, self->cols, (npy_ulong *)data);
        break;
    }
    case NPY_LONGLONG:
    {
        assign_from_np_R(self->elements, self->rows, self->cols, (npy_longlong *)data);
        break;
    }
    case NPY_ULONGLONG:
    {
        assign_from_np_R(self->elements, self->rows, self->cols, (npy_ulonglong *)data);
        break;
    }
    case NPY_FLOAT:
    {
        assign_from_np_R(self->elements, self->rows, self->cols, (npy_float *)data);
        break;
    }
    case NPY_DOUBLE:
    {
        assign_from_np_R(self->elements, self->rows, self->cols, (npy_double *)data);
        break;
    }
    case NPY_LONGDOUBLE:
    {
        assign_from_np_R(self->elements, self->rows, self->cols, (npy_longdouble *)data);
        break;
    }
    case NPY_CFLOAT:
    {
        assign_from_np_C(self->elements, self->rows, self->cols, (npy_cfloat *)data);
        break;
    }
    case NPY_CDOUBLE:
    {
        assign_from_np_C(self->elements, self->rows, self->cols, (npy_cdouble *)data);
        break;
    }
    case NPY_CLONGDOUBLE:
    {
        assign_from_np_C(self->elements, self->rows, self->cols, (npy_clongdouble *)data);
        break;
    }
    case NPY_OBJECT:
    {
        return assign_from_obj(self->elements, self->rows, self->cols, (PyObject **)data);
    }
    case NPY_HALF:
    {
        assign_from_half(self->elements, self->rows, self->cols, (npy_half *)data);
        break;
    }
    default:
    {
        PyErr_SetString(PyExc_TypeError, "Unsupport dtype");
        return -1;
    }
    }
    return 0;
}

static int PyMatrix_init_from_matrix(PyMatrixObject *self, PyMatrixObject *other)
{
    return Matrix_copy(other, self);
}

static int PyMatrix_init(PyMatrixObject *self, PyObject *args, PyObject *kwds)
{
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
    if (PyArg_ParseTupleAndKeywords(args, kwds, "ii|$O", kwlist_0, &self->rows, &self->cols, &tmp_object))
    {
        return PyMatrix_init_from_rcf(self, tmp_object);
    }
    PyErr_Clear();
    tmp_object = nullptr;
    tmp_matrix = nullptr;
    if (PyArg_ParseTupleAndKeywords(args, kwds, "O|$O", kwlist_1, &tmp_matrix, &tmp_object))
    {
        if (PyList_CheckExact(tmp_matrix))
        {
            if (!PyList_Size(tmp_matrix))
            {
                PyErr_SetString(PyExc_ValueError, "Empty list");
                return -1;
            }
            PyObject *tmp = PyList_GetItem(tmp_matrix, 0);
            if (PyList_CheckExact(tmp))
            {
                return PyMatrix_init_from_Mf(self, tmp_matrix, tmp_object);
            }
            else if (PyTuple_CheckExact(tmp))
            {
                return PyMatrix_init_from_sMf(self, tmp_matrix, tmp_object);
            }
        }
    }
    PyErr_Clear();
    tmp_matrix = nullptr;
    if (PyArg_ParseTupleAndKeywords(args, kwds, "O", kwlist_2, &tmp_matrix))
    {
        if (PyArray_Check(tmp_matrix))
        {
            return PyMatrix_init_from_nparray(self, (PyArrayObject *)tmp_matrix);
        }
        if (PyMatrix_Check(tmp_matrix))
        {
            return PyMatrix_init_from_matrix(self, (PyMatrixObject *)tmp_matrix);
        }
    }
    PyErr_SetString(PyExc_TypeError, "Fail to match any overload");
    return -1;
}

static PyObject *PyMatrix_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyObject *self = type->tp_alloc(type, 0);
    if (!self)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    ((PyMatrixObject *)self)->elements = nullptr;
    return self;
}

// as number

static PyObject *PyMatrix_add(PyObject *self, PyObject *other)
{
    PyMatrixObject *result = nullptr;
    result = PyObject_New(PyMatrixObject, &PyMatrix_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->elements = nullptr;
    if (PyMatrix_Check(self) && PyMatrix_Check(other))
    {

        if (Matrix_add((PyMatrixObject *)self, (PyMatrixObject *)other, result))
        {
            Py_DECREF(result);
            return nullptr;
        }
        return (PyObject *)result;
    }
    Py_DECREF(result);
    Py_RETURN_NOTIMPLEMENTED;
}

static PyObject *PyMatrix_subtract(PyObject *self, PyObject *other)
{
    PyMatrixObject *result = nullptr;
    result = PyObject_New(PyMatrixObject, &PyMatrix_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->elements = nullptr;
    if (PyMatrix_Check(self) && PyMatrix_Check(other))
    {
        if (Matrix_sub((PyMatrixObject *)self, (PyMatrixObject *)other, result))
        {
            Py_DECREF(result);
            return nullptr;
        }
        return (PyObject *)result;
    }
    Py_DECREF(result);
    Py_RETURN_NOTIMPLEMENTED;
}

static PyObject *PyMatrix_multiply(PyObject *self, PyObject *other)
{
    ComplexVar tmp;
    PyMatrixObject *result = nullptr;
    result = PyObject_New(PyMatrixObject, &PyMatrix_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->elements = nullptr;
    if (PyMatrix_Check(self) && CanBeComplexVar(other))
    {
        assignComplexVar(other, tmp);
        if (Matrix_mul((PyMatrixObject *)self, tmp, result))
        {
            Py_DECREF(result);
            return nullptr;
        }
        return (PyObject *)result;
    }
    if (PyMatrix_Check(other) && CanBeComplexVar(self))
    {
        assignComplexVar(self, tmp);
        if (Matrix_mul((PyMatrixObject *)other, tmp, result))
        {
            Py_DECREF(result);
            return nullptr;
        }
        return (PyObject *)result;
    }
    Py_DECREF(result);
    Py_RETURN_NOTIMPLEMENTED;
}

static PyObject *PyMatrix_remainder(PyObject *self, PyObject *other)
{
    ComplexVar tmp;
    PyMatrixObject *result = nullptr;
    result = PyObject_New(PyMatrixObject, &PyMatrix_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->elements = nullptr;
    if (PyMatrix_Check(self) && CanBeComplexVar(other))
    {
        assignComplexVar(other, tmp);
        if (Matrix_mod((PyMatrixObject *)self, tmp, result))
        {
            Py_DECREF(result);
            return nullptr;
        }
        return (PyObject *)result;
    }
    if (PyMatrix_Check(other) && CanBeComplexVar(self))
    {
        assignComplexVar(self, tmp);
        if (Matrix_mod(tmp, (PyMatrixObject *)other, result))
        {
            Py_DECREF(result);
            return nullptr;
        }
        return (PyObject *)result;
    }
    Py_DECREF(result);
    Py_RETURN_NOTIMPLEMENTED;
}

static PyObject *PyMatrix_divmod(PyObject *self, PyObject *other)
{
    PyMatrixObject *D = nullptr;
    PyMatrixObject *M = nullptr;
    D = PyObject_New(PyMatrixObject, &PyMatrix_Type);
    if (!D)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    M = PyObject_New(PyMatrixObject, &PyMatrix_Type);
    if (!M)
    {
        Py_DECREF(D);
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    ComplexVar tmp;
    if (PyMatrix_Check(self) && CanBeComplexVar(other))
    {
        assignComplexVar(other, tmp);
        if (Matrix_fdv((PyMatrixObject *)self, tmp, D))
        {
            Py_DECREF(D);
            Py_DECREF(M);
            return nullptr;
        }
        if (Matrix_mul(D, tmp, M))
        {
            Py_DECREF(D);
            Py_DECREF(M);
            return nullptr;
        }
        for (Py_ssize_t i = 0; i < M->rows; i++)
        {
            for (Py_ssize_t j = 0; j < M->cols; j++)
            {
                PyMatrixAssign(M, i, j, ComplexVar_sub(PyMatrixGetitem((PyMatrixObject *)self, i, j), PyMatrixGetitem(M, i, j)));
            }
        }
        PyObject *returnvalue = Py_BuildValue("OO", D, M);
        Py_DECREF(D);
        Py_DECREF(M);
        return returnvalue;
    }
    if (PyMatrix_Check(other) && CanBeComplexVar(self))
    {
        assignComplexVar(self, tmp);
        if (Matrix_fdv(tmp, (PyMatrixObject *)other, D))
        {
            Py_DECREF(D);
            Py_DECREF(M);
            return nullptr;
        }
        if (Matrix_hadamard(D, (PyMatrixObject *)other, M))
        {
            Py_DECREF(D);
            Py_DECREF(M);
            return nullptr;
        }
        for (Py_ssize_t i = 0; i < M->rows; i++)
        {
            for (Py_ssize_t j = 0; j < M->cols; j++)
            {
                PyMatrixAssign(M, i, j, ComplexVar_sub(tmp, PyMatrixGetitem(M, i, j)));
            }
        }
        PyObject *returnvalue = Py_BuildValue("OO", D, M);
        Py_DECREF(D);
        Py_DECREF(M);
        return returnvalue;
    }
    Py_RETURN_NOTIMPLEMENTED;
}

static PyObject *PyMatrix_negative(PyObject *self)
{
    PyMatrixObject *result = nullptr;
    result = PyObject_New(PyMatrixObject, &PyMatrix_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->elements = nullptr;
    if (Matrix_neg((PyMatrixObject *)self, result))
    {
        Py_DECREF(result);
        return nullptr;
    }
    return (PyObject *)result;
}

static PyObject *PyMatrix_positive(PyObject *self)
{
    return PyMatrix_copy((PyMatrixObject *)self);
}

static PyObject *PyMatrix_inplace_add(PyMatrixObject *self, PyObject *other)
{
    if (PyMatrix_Check(other))
    {
        if (Matrix_iadd(self, (PyMatrixObject *)other))
        {
            return nullptr;
        }
        Py_INCREF(self);
        return (PyObject *)self;
    }
    Py_RETURN_NOTIMPLEMENTED;
}

static PyObject *PyMatrix_inplace_subtract(PyMatrixObject *self, PyObject *other)
{
    if (PyMatrix_Check(other))
    {
        if (Matrix_isub(self, (PyMatrixObject *)other))
        {
            return nullptr;
        }
        Py_INCREF(self);
        return (PyObject *)self;
    }
    Py_RETURN_NOTIMPLEMENTED;
}

static PyObject *PyMatrix_inplace_multiply(PyMatrixObject *self, PyObject *other)
{
    ComplexVar tmp;
    if (!assignComplexVar(other, tmp))
    {
        Matrix_imul(self, tmp);
        Py_INCREF(self);
        return (PyObject *)self;
    }
    Py_RETURN_NOTIMPLEMENTED;
}

static PyObject *PyMatrix_inplace_remainder(PyMatrixObject *self, PyObject *other)
{
    ComplexVar tmp;
    if (!assignComplexVar(other, tmp))
    {
        Matrix_imod(self, tmp);
        Py_INCREF(self);
        return (PyObject *)self;
    }
    Py_RETURN_NOTIMPLEMENTED;
}

static PyObject *PyMatrix_floor_divide(PyObject *self, PyObject *other)
{
    ComplexVar tmp;
    PyMatrixObject *result = nullptr;
    result = PyObject_New(PyMatrixObject, &PyMatrix_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->elements = nullptr;
    if (PyMatrix_Check(self) && CanBeComplexVar(other))
    {
        assignComplexVar(other, tmp);
        if (Matrix_fdv((PyMatrixObject *)self, tmp, result))
        {
            Py_DECREF(result);
            return nullptr;
        }
        return (PyObject *)result;
    }
    if (PyMatrix_Check(other) && CanBeComplexVar(self))
    {
        assignComplexVar(self, tmp);
        if (Matrix_fdv(tmp, (PyMatrixObject *)other, result))
        {
            Py_DECREF(result);
            return nullptr;
        }
        return (PyObject *)result;
    }
    Py_DECREF(result);
    Py_RETURN_NOTIMPLEMENTED;
}

static PyObject *PyMatrix_true_divide(PyObject *self, PyObject *other)
{
    ComplexVar tmp;
    PyMatrixObject *result = nullptr;
    result = PyObject_New(PyMatrixObject, &PyMatrix_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->elements = nullptr;
    if (PyMatrix_Check(self) && CanBeComplexVar(other))
    {
        assignComplexVar(other, tmp);
        if (Matrix_div((PyMatrixObject *)self, tmp, result))
        {
            Py_DECREF(result);
            return nullptr;
        }
        return (PyObject *)result;
    }
    if (PyMatrix_Check(other) && CanBeComplexVar(self))
    {
        assignComplexVar(self, tmp);
        if (Matrix_div(tmp, (PyMatrixObject *)other, result))
        {
            Py_DECREF(result);
            return nullptr;
        }
        return (PyObject *)result;
    }
    Py_DECREF(result);
    Py_RETURN_NOTIMPLEMENTED;
}

static PyObject *PyMatrix_inplace_floor_divide(PyMatrixObject *self, PyObject *other)
{
    ComplexVar tmp;
    if (!assignComplexVar(other, tmp))
    {
        Matrix_ifdv(self, tmp);
        Py_INCREF(self);
        return (PyObject *)self;
    }
    Py_RETURN_NOTIMPLEMENTED;
}

static PyObject *PyMatrix_inplace_true_divide(PyMatrixObject *self, PyObject *other)
{
    ComplexVar tmp;
    if (!assignComplexVar(other, tmp))
    {
        Matrix_idiv(self, tmp);
        Py_INCREF(self);
        return (PyObject *)self;
    }
    Py_RETURN_NOTIMPLEMENTED;
}

static PyObject *PyMatrix_matrix_multiply(PyObject *self, PyObject *other)
{
    PyMatrixObject *result = nullptr;
    result = PyObject_New(PyMatrixObject, &PyMatrix_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->elements = nullptr;
    if (PyMatrix_Check(self) && PyMatrix_Check(other))
    {
        if (Matrix_mul((PyMatrixObject *)self, (PyMatrixObject *)other, result))
        {
            Py_DECREF(result);
            return nullptr;
        }
        return (PyObject *)result;
    }
    Py_DECREF(result);
    Py_RETURN_NOTIMPLEMENTED;
}

// methods

static PyObject *PyMatrix_conj(PyMatrixObject *self, PyObject *args)
{
    PyMatrixObject *result = nullptr;
    result = PyObject_New(PyMatrixObject, &PyMatrix_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->elements = nullptr;
    if (Matrix_conj((PyMatrixObject *)self, result))
    {
        Py_DECREF(result);
        return nullptr;
    }
    return (PyObject *)result;
}

static PyObject *PyMatrix_iconj(PyMatrixObject *self, PyObject *args)
{
    Matrix_iconj(self);
    Py_INCREF(self);
    return (PyObject *)self;
}

static PyObject *PyMatrix_T(PyMatrixObject *self, PyObject *args)
{
    PyMatrixObject *result = nullptr;
    result = PyObject_New(PyMatrixObject, &PyMatrix_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->elements = nullptr;
    if (Matrix_transpose((PyMatrixObject *)self, result))
    {
        Py_DECREF(result);
        return nullptr;
    }
    return (PyObject *)result;
}

static PyObject *PyMatrix_iT(PyMatrixObject *self, PyObject *args)
{
    Matrix_itranspose(self);
    Py_INCREF(self);
    return (PyObject *)self;
}

static PyObject *PyMatrix_H(PyMatrixObject *self, PyObject *args)
{
    PyMatrixObject *result = nullptr;
    result = PyObject_New(PyMatrixObject, &PyMatrix_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->elements = nullptr;
    if (Matrix_hermite_transpose((PyMatrixObject *)self, result))
    {
        Py_DECREF(result);
        return nullptr;
    }
    return (PyObject *)result;
}

static PyObject *PyMatrix_iH(PyMatrixObject *self, PyObject *args)
{
    Matrix_ihermite_transpose(self);
    Py_INCREF(self);
    return (PyObject *)self;
}

static PyObject *PyMatrix_kronecker(const PyMatrixObject *self, PyObject *other)
{
    PyMatrixObject *result = nullptr;
    result = PyObject_New(PyMatrixObject, &PyMatrix_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->elements = nullptr;
    if (PyMatrix_Check(self) && PyMatrix_Check(other))
    {
        if (Matrix_kronecker((PyMatrixObject *)self, (PyMatrixObject *)other, result))
        {
            Py_DECREF(result);
            return nullptr;
        }
        return (PyObject *)result;
    }
    Py_DECREF(result);
    Py_RETURN_NOTIMPLEMENTED;
}

static PyObject *PyMatrix_hadamard(const PyMatrixObject *self, PyObject *other)
{
    PyMatrixObject *result = nullptr;
    result = PyObject_New(PyMatrixObject, &PyMatrix_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->elements = nullptr;
    if (PyMatrix_Check(self) && PyMatrix_Check(other))
    {
        if (Matrix_hadamard((PyMatrixObject *)self, (PyMatrixObject *)other, result))
        {
            Py_DECREF(result);
            return nullptr;
        }
        return (PyObject *)result;
    }
    Py_DECREF(result);
    Py_RETURN_NOTIMPLEMENTED;
}

static PyObject *PyMatrix_vstack(PyObject *self, PyObject *const *args, Py_ssize_t nargs)
{
    PyMatrixObject *result = nullptr;
    result = PyObject_New(PyMatrixObject, &PyMatrix_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->elements = nullptr;
    if (!PyMatrix_Check(args[0]))
    {
        PyErr_Format(PyExc_TypeError, "vstack get unsupport %s object on index 0", args[0]->ob_type->tp_name);
        Py_DECREF(result);
        return nullptr;
    }
    result->cols = ((PyMatrixObject *)(args[0]))->cols;
    result->rows = ((PyMatrixObject *)(args[0]))->rows;
    for (Py_ssize_t i = 1; i < nargs; i++)
    {
        if (!PyMatrix_Check(args[i]))
        {
            PyErr_Format(PyExc_TypeError, "vstack get unsupport %s object on index %ld", args[i]->ob_type->tp_name, i);
            Py_DECREF(result);
            return nullptr;
        }
        if (((PyMatrixObject *)(args[i]))->cols != result->cols)
        {
            PyErr_Format(PyExc_ShapeError, "suppose %ld cols, get %ld cols on index %ld", result->cols, ((PyMatrixObject *)(args[i]))->cols, i);
            Py_DECREF(result);
            return nullptr;
        }
        result->rows += ((PyMatrixObject *)(args[i]))->rows;
    }
    if (PyMatrixAlloc(result))
    {
        Py_DECREF(result);
        return nullptr;
    }
    int tmp = 0;
    for (Py_ssize_t i = 0; i < nargs; i++)
    {
        for (Py_ssize_t j = 0; j < ((PyMatrixObject *)(args[i]))->rows; j++)
        {
            for (Py_ssize_t k = 0; k < ((PyMatrixObject *)(args[i]))->cols; k++)
            {
                PyMatrixAssign(result, tmp + j, k, PyMatrixGetitem((PyMatrixObject *)(args[i]), j, k));
            }
        }
        tmp += ((PyMatrixObject *)(args[i]))->rows;
    }
    return (PyObject *)result;
}

static PyObject *PyMatrix_hstack(PyObject *self, PyObject *const *args, Py_ssize_t nargs)
{
    PyMatrixObject *result = nullptr;
    result = PyObject_New(PyMatrixObject, &PyMatrix_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->elements = nullptr;
    if (!PyMatrix_Check(args[0]))
    {
        PyErr_Format(PyExc_TypeError, "hstack get unsupport %s object on index 0", args[0]->ob_type->tp_name);
        Py_DECREF(result);
        return nullptr;
    }
    result->cols = ((PyMatrixObject *)(args[0]))->cols;
    result->rows = ((PyMatrixObject *)(args[0]))->rows;
    for (Py_ssize_t i = 1; i < nargs; i++)
    {
        if (!PyMatrix_Check(args[i]))
        {
            PyErr_Format(PyExc_TypeError, "hstack get unsupport %s object on index %ld", args[i]->ob_type->tp_name, i);
            Py_DECREF(result);
            return nullptr;
        }
        if (((PyMatrixObject *)(args[i]))->rows != result->rows)
        {
            PyErr_Format(PyExc_ShapeError, "suppose %ld rows, get %ld rows on index %ld", result->cols, ((PyMatrixObject *)(args[i]))->cols, i);
            Py_DECREF(result);
            return nullptr;
        }
        result->cols += ((PyMatrixObject *)(args[i]))->cols;
    }
    if (PyMatrixAlloc(result))
    {
        Py_DECREF(result);
        return nullptr;
    }
    int tmp = 0;
    for (Py_ssize_t i = 0; i < nargs; i++)
    {
        for (Py_ssize_t j = 0; j < ((PyMatrixObject *)(args[i]))->rows; j++)
        {
            for (Py_ssize_t k = 0; k < ((PyMatrixObject *)(args[i]))->cols; k++)
            {
                PyMatrixAssign(result, j, tmp + k, PyMatrixGetitem((PyMatrixObject *)(args[i]), j, k));
            }
        }
        tmp += ((PyMatrixObject *)(args[i]))->cols;
    }
    return (PyObject *)result;
}

// as map

static Py_ssize_t PyMatrix_length(PyMatrixObject *self)
{
    return self->total_elements;
}

static PyObject *PyMatrix_subscript_LL(PyMatrixObject *self, PyObject *a, PyObject *b)
{
    int r = PyLong_AS_LONG(a);
    int c = PyLong_AS_LONG(b);
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    if (PyMatrixGet_withcheck(self, r, c, result->num))
    {
        Py_DECREF(result);
        return nullptr;
    }
    return (PyObject *)result;
}

static PyObject *PyMatrix_subscript_LS(PyMatrixObject *self, PyObject *a, PyObject *b)
{
    int r = PyLong_AsLong(a);
    if (r < 0)
    {
        r = self->rows + r;
    }
    if ((r < 0) || (r >= self->rows))
    {
        PyErr_SetString(PyExc_IndexError, "Index out of range");
        return nullptr;
    }
    PyMatrixObject *result = nullptr;
    result = (PyMatrixObject *)PyMatrix_new(&PyMatrix_Type, nullptr, nullptr);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->rows = 1;
    Py_ssize_t b_start;
    Py_ssize_t b_stop;
    Py_ssize_t b_step;
    PySlice_Unpack(b, &b_start, &b_stop, &b_step);
    result->cols = PySlice_AdjustIndices(self->cols, &b_start, &b_stop, b_step);
    if (PyMatrixAlloc(result))
    {
        if (PyErr_ExceptionMatches(PyExc_ValueError))
        {
            PyErr_SetString(PyExc_IndexError, "Index out of range");
        }
        Py_DECREF(result);
        return nullptr;
    }
    Py_ssize_t c = b_start;
    for (Py_ssize_t j = 0; j < result->cols; j++)
    {
        PyMatrixAssign(result, 0, j, PyMatrixGetitem(self, r, c));
        c += b_step;
    }
    return (PyObject *)result;
}

static PyObject *PyMatrix_subscript_SL(PyMatrixObject *self, PyObject *a, PyObject *b)
{
    int c = PyLong_AsLong(b);
    if (c < 0)
    {
        c = self->rows + c;
    }
    if ((c < 0) || (c >= self->rows))
    {
        PyErr_SetString(PyExc_IndexError, "Index out of range");
        return nullptr;
    }
    PyMatrixObject *result = nullptr;
    result = (PyMatrixObject *)PyMatrix_new(&PyMatrix_Type, nullptr, nullptr);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->cols = 1;
    Py_ssize_t a_start;
    Py_ssize_t a_stop;
    Py_ssize_t a_step;
    PySlice_Unpack(a, &a_start, &a_stop, &a_step);
    result->rows = PySlice_AdjustIndices(self->cols, &a_start, &a_stop, a_step);
    if (PyMatrixAlloc(result))
    {
        if (PyErr_ExceptionMatches(PyExc_ValueError))
        {
            PyErr_SetString(PyExc_IndexError, "Index out of range");
        }
        Py_DECREF(result);
        return nullptr;
    }
    Py_ssize_t r = a_start;
    for (Py_ssize_t i = 0; i < result->rows; i++)
    {
        PyMatrixAssign(result, i, 0, PyMatrixGetitem(self, r, c));
        r += a_step;
    }
    return (PyObject *)result;
}

static PyObject *PyMatrix_subscript_SS(PyMatrixObject *self, PyObject *a, PyObject *b)
{
    PyMatrixObject *result = nullptr;
    result = (PyMatrixObject *)PyMatrix_new(&PyMatrix_Type, nullptr, nullptr);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    Py_ssize_t a_start;
    Py_ssize_t a_stop;
    Py_ssize_t a_step;
    PySlice_Unpack(a, &a_start, &a_stop, &a_step);
    result->rows = PySlice_AdjustIndices(self->cols, &a_start, &a_stop, a_step);
    Py_ssize_t b_start;
    Py_ssize_t b_stop;
    Py_ssize_t b_step;
    PySlice_Unpack(b, &b_start, &b_stop, &b_step);
    result->cols = PySlice_AdjustIndices(self->cols, &b_start, &b_stop, b_step);
    if (PyMatrixAlloc(result))
    {
        if (PyErr_ExceptionMatches(PyExc_ValueError))
        {
            PyErr_SetString(PyExc_IndexError, "Index out of range");
        }
        Py_DECREF(result);
        return nullptr;
    }
    Py_ssize_t r = a_start;
    Py_ssize_t c = b_start;
    for (Py_ssize_t i = 0; i < result->rows; i++)
    {
        for (Py_ssize_t j = 0; j < result->cols; j++)
        {
            PyMatrixAssign(result, i, j, PyMatrixGetitem(self, r, c));
            c += b_step;
        }
        c = b_start;
        r += a_step;
    }
    return (PyObject *)result;
}

static PyObject *PyMatrix_subscript(PyMatrixObject *self, PyObject *index)
{
    if (index == Py_Ellipsis)
    {
        return PyMatrix_copy(self);
    }
    if (!PyTuple_CheckExact(index))
    {
        PyErr_Format(PyExc_TypeError, "Unsupported index type:%s. Only take a ellipsis a tuple with 2 elements", index->ob_type->tp_name);
        return nullptr;
    }
    if (PyTuple_Size(index) != 2)
    {
        PyErr_Format(PyExc_IndexError, "Get %ld items. Only take a tuple with 2 elements", PyTuple_Size(index));
        return nullptr;
    }
    PyObject *a = PyTuple_GetItem(index, 0);
    PyObject *b = PyTuple_GetItem(index, 1);
    if (PyLong_CheckExact(a) && PyLong_CheckExact(b))
    {
        return PyMatrix_subscript_LL(self, a, b);
    }
    else if (PyLong_CheckExact(a) && PySlice_Check(b))
    {
        return PyMatrix_subscript_LS(self, a, b);
    }
    else if (PySlice_Check(a) && PyLong_CheckExact(b))
    {
        return PyMatrix_subscript_SL(self, a, b);
    }
    else if (PySlice_Check(a) && PySlice_Check(b))
    {
        return PyMatrix_subscript_SS(self, a, b);
    }
    PyErr_Format(PyExc_TypeError, "Indices must be integers or slices. Get %s and %s", a->ob_type->tp_name, b->ob_type->tp_name);
    return nullptr;
}

static int index_process(PyObject *x, Py_ssize_t Ml, Py_ssize_t &x_start, Py_ssize_t &x_stop, Py_ssize_t &x_step, Py_ssize_t &l, Py_ssize_t supposel)
{
    if (PySlice_Check(x))
    {
        PySlice_Unpack(x, &x_start, &x_stop, &x_step);
        l = PySlice_AdjustIndices(Ml, &x_start, &x_stop, x_step);
        if (!l)
        {
            PyErr_SetNone(PyExc_IndexError);
            return -1;
        }
    }
    else if (PyLong_CheckExact(x))
    {
        l = 1;
        x_start = PyLong_AsSsize_t(x);
        x_stop = 0;
        x_step = 0;
    }
    else
    {
        PyErr_Format(PyExc_TypeError, "Indices must be integers or slices. Get %s", x->ob_type->tp_name);
        return -1;
    }
    if (l != supposel)
    {
        PyErr_SetNone(PyExc_ShapeError);
        return -1;
    }
    return 0;
}

static int PyMatrix_ass_subscript_sM(PyMatrixObject *self, PyObject *value)
{
    if (!PyList_CheckExact(value))
    {
        PyErr_SetNone(PyExc_TypeError);
        return -1;
    }
    PyObject *tmp;
    int mrows = 0;
    int mcols = 0;
    int rtmp;
    int ctmp;
    PyObject *tmp_element;
    for (Py_ssize_t i = 0; i < PyList_Size(value); i++)
    {
        tmp = PyList_GetItem(value, i);
        if (!PyTuple_CheckExact(tmp))
        {
            PyErr_SetNone(PyExc_TypeError);
            return -1;
        }
        if (PyTuple_Size(tmp) != 3)
        {
            PyErr_Format(PyExc_ValueError, "Only take 3 items. On index:%ld", i);
            return -1;
        }
        if (!PyArg_ParseTuple(tmp, "iiO", &rtmp, &ctmp, &tmp_element))
        {
            PyErr_Format(PyExc_ValueError, "Fail to parse tuple. On index:%ld", i);
            return -1;
        }
        if (!CanBeComplexVar(tmp_element))
        {
            PyErr_Format(PyExc_TypeError, "Can turn %s into complexvar. On index:%ld", tmp_element->ob_type->tp_name, i);
            return -1;
        }
        if (rtmp > mrows)
        {
            mrows = rtmp;
        }
        if (ctmp > mcols)
        {
            mcols = ctmp;
        }
    }
    if ((mrows >= self->rows) || (mcols >= self->cols))
    {
        PyErr_SetNone(PyExc_IndexError);
        return -1;
    }
    ComplexVar tmp_element_value;
    for (Py_ssize_t i = 0; i < PyList_Size(value); i++)
    {
        tmp = PyList_GetItem(value, i);
        PyArg_ParseTuple(tmp, "iiO", &rtmp, &ctmp, &tmp_element);
        assignComplexVar(tmp_element, tmp_element_value);
        PyMatrixAssign(self, rtmp, ctmp, tmp_element_value);
    }
    return 0;
}

static int PyMatrix_ass_subscript_M(PyMatrixObject *self, PyObject *a, PyObject *b, PyMatrixObject *value)
{
    Py_ssize_t a_start;
    Py_ssize_t a_stop;
    Py_ssize_t a_step;
    Py_ssize_t rows;
    if (index_process(a, self->rows, a_start, a_stop, a_step, rows, value->rows))
    {
        return -1;
    }
    Py_ssize_t b_start;
    Py_ssize_t b_stop;
    Py_ssize_t b_step;
    Py_ssize_t cols;
    if (index_process(b, self->cols, b_start, b_stop, b_step, cols, value->cols))
    {
        return -1;
    }
    Py_ssize_t r = a_start;
    Py_ssize_t c = b_start;
    for (Py_ssize_t i = 0; i < rows; i++)
    {
        for (Py_ssize_t j = 0; j < cols; j++)
        {
            PyMatrixAssign(self, r, c, PyMatrixGetitem(value, i, j));
            c += b_step;
        }
        c = b_start;
        r += a_step;
    }
    return 0;
}

static int PyMatrix_ass_subscript_List(PyMatrixObject *self, PyObject *a, PyObject *b, PyObject *value)
{
    Py_ssize_t mrows = PyList_Size(value);
    Py_ssize_t mcols = 0;
    PyObject *tmp;
    for (Py_ssize_t i = 0; i < mrows; i++)
    {
        tmp = PyList_GetItem(value, i);
        if (!PyList_CheckExact(tmp))
        {
            PyErr_Format(PyExc_ValueError, "Inconsistency value format at %ld", i);
            return -1;
        }
        for (Py_ssize_t j = 0; j < PyList_Size(tmp); j++)
        {
            if (!CanBeComplexVar(PyList_GetItem(tmp, j)))
            {
                PyErr_Format(PyExc_ValueError, "Unsupported type at %ld %ld", i, j);
                return -1;
            }
        }
        if (mcols < PyList_Size(tmp))
        {
            mcols = PyList_Size(tmp);
        }
    }
    Py_ssize_t a_start;
    Py_ssize_t a_stop;
    Py_ssize_t a_step;
    Py_ssize_t rows;
    if (index_process(a, self->rows, a_start, a_stop, a_step, rows, mrows))
    {
        return -1;
    }
    Py_ssize_t b_start;
    Py_ssize_t b_stop;
    Py_ssize_t b_step;
    Py_ssize_t cols;
    if (index_process(b, self->cols, b_start, b_stop, b_step, cols, mcols))
    {
        return -1;
    }
    Py_ssize_t r = a_start;
    Py_ssize_t c = b_start;
    ComplexVar num;
    for (Py_ssize_t i = 0; i < mrows; i++)
    {
        tmp = PyList_GetItem(value, i);
        for (Py_ssize_t j = 0; j < PyList_Size(tmp); j++)
        {
            assignComplexVar(PyList_GetItem(tmp, j), self->elements[r * self->cols + c]);
            c += b_step;
        }
        c = b_start;
        r += a_step;
    }
    return 0;
}

static void PyMatrix_ass_subscript_nparray_r(PyMatrixObject *self, Py_ssize_t a_start, Py_ssize_t a_step, Py_ssize_t b_start, Py_ssize_t b_step, Py_ssize_t rows, Py_ssize_t cols, npy_real auto *data)
{
    Py_ssize_t r = a_start;
    Py_ssize_t c = b_start;
    int64_t pos;
    for (Py_ssize_t i = 0; i < rows; i++)
    {
        for (Py_ssize_t j = 0; j < cols; j++)
        {
            pos = r * self->cols + c;
            self->elements[pos].real = data[i * cols + j];
            self->elements[pos].imag = 0;
            self->elements[pos].isArbitrary = false;
            c += b_step;
        }
        c = b_start;
        r += a_step;
    }
}

static void PyMatrix_ass_subscript_nparray_c(PyMatrixObject *self, Py_ssize_t a_start, Py_ssize_t a_step, Py_ssize_t b_start, Py_ssize_t b_step, Py_ssize_t rows, Py_ssize_t cols, npy_complex auto *data)
{
    Py_ssize_t r = a_start;
    Py_ssize_t c = b_start;
    int64_t pos;
    int64_t dpos;
    for (Py_ssize_t i = 0; i < rows; i++)
    {
        for (Py_ssize_t j = 0; j < cols; j++)
        {
            pos = r * self->cols + c;
            dpos = i * cols + j;
            self->elements[pos].real = data[dpos].real;
            self->elements[pos].imag = data[dpos].imag;
            self->elements[pos].isArbitrary = false;
            c += b_step;
        }
        c = b_start;
        r += a_step;
    }
}

static void PyMatrix_ass_subscript_nparray_half(PyMatrixObject *self, Py_ssize_t a_start, Py_ssize_t a_step, Py_ssize_t b_start, Py_ssize_t b_step, Py_ssize_t rows, Py_ssize_t cols, npy_half *data)
{
    Py_ssize_t r = a_start;
    Py_ssize_t c = b_start;
    int64_t pos;
    for (Py_ssize_t i = 0; i < rows; i++)
    {
        for (Py_ssize_t j = 0; j < cols; j++)
        {
            pos = r * self->cols + c;
            self->elements[pos].real = casthalf_to_double(data[i * cols + j]);
            self->elements[pos].imag = 0;
            self->elements[pos].isArbitrary = false;
            c += b_step;
        }
        c = b_start;
        r += a_step;
    }
}

static int PyMatrix_ass_subscript_nparray_obj(PyMatrixObject *self, Py_ssize_t a_start, Py_ssize_t a_step, Py_ssize_t b_start, Py_ssize_t b_step, Py_ssize_t rows, Py_ssize_t cols, PyObject **data)
{
    Py_ssize_t r = a_start;
    Py_ssize_t c = b_start;
    for (Py_ssize_t i = 0; i < rows; i++)
    {
        for (Py_ssize_t j = 0; j < cols; j++)
        {
            if (assignComplexVar_withExc(data[i * cols + j], self->elements[r * self->cols + c]))
            {
                Addmessage("Trying to assign object at %ld %ld to %ld %ld", i, j, r, c);
                return -1;
            }
            c += b_step;
        }
        c = b_start;
        r += a_step;
    }
    return 0;
}

static int PyMatrix_ass_subscript_nparray(PyMatrixObject *self, PyObject *a, PyObject *b, PyArrayObject *value)
{
    if (PyArray_NDIM(value) != 2)
    {
        PyErr_SetString(PyExc_ValueError, "Only take 2 dimension array");
        return -1;
    }
    Py_ssize_t a_start;
    Py_ssize_t a_stop;
    Py_ssize_t a_step;
    Py_ssize_t rows;
    if (index_process(a, self->rows, a_start, a_stop, a_step, rows, PyArray_DIM(value, 0)))
    {
        return -1;
    }
    Py_ssize_t b_start;
    Py_ssize_t b_stop;
    Py_ssize_t b_step;
    Py_ssize_t cols;
    if (index_process(b, self->cols, b_start, b_stop, b_step, cols, PyArray_DIM(value, 1)))
    {
        return -1;
    }
    void *data = PyArray_DATA(value);
    switch (PyArray_DESCR(value)->type_num)
    {
    case NPY_BYTE:
    {
        npy_byte *A = (npy_byte *)data;
        PyMatrix_ass_subscript_nparray_r(self, a_start, a_step, b_start, b_step, rows, cols, A);
        break;
    }
    case NPY_UBYTE:
    {
        npy_ubyte *A = (npy_ubyte *)data;
        PyMatrix_ass_subscript_nparray_r(self, a_start, a_step, b_start, b_step, rows, cols, A);
        break;
    }
    case NPY_SHORT:
    {
        npy_short *A = (npy_short *)data;
        PyMatrix_ass_subscript_nparray_r(self, a_start, a_step, b_start, b_step, rows, cols, A);
        break;
    }
    case NPY_USHORT:
    {
        npy_ushort *A = (npy_ushort *)data;
        PyMatrix_ass_subscript_nparray_r(self, a_start, a_step, b_start, b_step, rows, cols, A);
        break;
    }
    case NPY_INT:
    {
        npy_int *A = (npy_int *)data;
        PyMatrix_ass_subscript_nparray_r(self, a_start, a_step, b_start, b_step, rows, cols, A);
        break;
    }
    case NPY_UINT:
    {
        npy_uint *A = (npy_uint *)data;
        PyMatrix_ass_subscript_nparray_r(self, a_start, a_step, b_start, b_step, rows, cols, A);
        break;
    }
    case NPY_LONG:
    {
        npy_long *A = (npy_long *)data;
        PyMatrix_ass_subscript_nparray_r(self, a_start, a_step, b_start, b_step, rows, cols, A);
        break;
    }
    case NPY_ULONG:
    {
        npy_ulong *A = (npy_ulong *)data;
        PyMatrix_ass_subscript_nparray_r(self, a_start, a_step, b_start, b_step, rows, cols, A);
        break;
    }
    case NPY_LONGLONG:
    {
        npy_longlong *A = (npy_longlong *)data;
        PyMatrix_ass_subscript_nparray_r(self, a_start, a_step, b_start, b_step, rows, cols, A);
        break;
    }
    case NPY_ULONGLONG:
    {
        npy_ulonglong *A = (npy_ulonglong *)data;
        PyMatrix_ass_subscript_nparray_r(self, a_start, a_step, b_start, b_step, rows, cols, A);
        break;
    }
    case NPY_FLOAT:
    {
        npy_float *A = (npy_float *)data;
        PyMatrix_ass_subscript_nparray_r(self, a_start, a_step, b_start, b_step, rows, cols, A);
        break;
    }
    case NPY_DOUBLE:
    {
        npy_double *A = (npy_double *)data;
        PyMatrix_ass_subscript_nparray_r(self, a_start, a_step, b_start, b_step, rows, cols, A);
        break;
    }
    case NPY_LONGDOUBLE:
    {
        npy_longdouble *A = (npy_longdouble *)data;
        PyMatrix_ass_subscript_nparray_r(self, a_start, a_step, b_start, b_step, rows, cols, A);
        break;
    }
    case NPY_CFLOAT:
    {
        npy_cfloat *A = (npy_cfloat *)data;
        PyMatrix_ass_subscript_nparray_c(self, a_start, a_step, b_start, b_step, rows, cols, A);
        break;
    }
    case NPY_CDOUBLE:
    {
        npy_cdouble *A = (npy_cdouble *)data;
        PyMatrix_ass_subscript_nparray_c(self, a_start, a_step, b_start, b_step, rows, cols, A);
        break;
    }
    case NPY_CLONGDOUBLE:
    {
        npy_clongdouble *A = (npy_clongdouble *)data;
        PyMatrix_ass_subscript_nparray_c(self, a_start, a_step, b_start, b_step, rows, cols, A);
        break;
    }
    case NPY_OBJECT:
    {
        PyObject **A = (PyObject **)data;
        if (PyMatrix_ass_subscript_nparray_obj(self, a_start, a_step, b_start, b_step, rows, cols, A))
        {
            return -1;
        }
        break;
    }
    case NPY_HALF:
    {
        npy_half *A = (npy_half *)data;
        PyMatrix_ass_subscript_nparray_half(self, a_start, a_step, b_start, b_step, rows, cols, A);
        break;
    }
    default:
    {
        PyErr_SetString(PyExc_TypeError, "Unsupport dtype");
        return -1;
    }
    }
    return 0;
}

static int PyMatrix_ass_subscript_LLV(PyMatrixObject *self, PyObject *a, PyObject *b, PyObject *value)
{
    int r = PyLong_AS_LONG(a);
    int c = PyLong_AS_LONG(b);
    ComplexVar tmp;
    if (assignComplexVar_withExc(value, tmp))
    {
        return -1;
    }
    if (PyMatrixAssign_withcheck(self, r, c, tmp))
    {
        return -1;
    }
    return 0;
}

static int PyMatrix_ass_subscript_LSV(PyMatrixObject *self, PyObject *a, PyObject *b, PyObject *value)
{
    int r = PyLong_AsLong(a);
    if (r < 0)
    {
        r = self->rows + r;
    }
    if ((r < 0) || (r >= self->rows))
    {
        PyErr_SetString(PyExc_IndexError, "Index out of range");
        return -1;
    }
    ComplexVar tmp;
    if (assignComplexVar_withExc(value, tmp))
    {
        return -1;
    }
    Py_ssize_t b_start;
    Py_ssize_t b_stop;
    Py_ssize_t b_step;
    PySlice_Unpack(b, &b_start, &b_stop, &b_step);
    Py_ssize_t cols = PySlice_AdjustIndices(self->cols, &b_start, &b_stop, b_step);
    Py_ssize_t c = b_start;
    for (Py_ssize_t j = 0; j < cols; j++)
    {
        PyMatrixAssign(self, r, c, tmp);
        c += b_step;
    }
    return 0;
}

static int PyMatrix_ass_subscript_SLV(PyMatrixObject *self, PyObject *a, PyObject *b, PyObject *value)
{
    int c = PyLong_AsLong(b);
    if (c < 0)
    {
        c = self->cols + c;
    }
    if ((c < 0) || (c >= self->cols))
    {
        PyErr_SetString(PyExc_IndexError, "Index out of range");
        return -1;
    }
    ComplexVar tmp;
    if (assignComplexVar_withExc(value, tmp))
    {
        return -1;
    }
    Py_ssize_t a_start;
    Py_ssize_t a_stop;
    Py_ssize_t a_step;
    PySlice_Unpack(a, &a_start, &a_stop, &a_step);
    Py_ssize_t rows = PySlice_AdjustIndices(self->rows, &a_start, &a_stop, a_step);
    Py_ssize_t r = a_start;
    for (Py_ssize_t i = 0; i < rows; i++)
    {
        PyMatrixAssign(self, r, c, tmp);
        r += a_step;
    }
    return 0;
}

static int PyMatrix_ass_subscript_SSV(PyMatrixObject *self, PyObject *a, PyObject *b, PyObject *value)
{
    ComplexVar tmp;
    if (assignComplexVar_withExc(value, tmp))
    {
        return -1;
    }
    Py_ssize_t a_start;
    Py_ssize_t a_stop;
    Py_ssize_t a_step;
    PySlice_Unpack(a, &a_start, &a_stop, &a_step);
    Py_ssize_t rows = PySlice_AdjustIndices(self->rows, &a_start, &a_stop, a_step);
    Py_ssize_t b_start;
    Py_ssize_t b_stop;
    Py_ssize_t b_step;
    PySlice_Unpack(b, &b_start, &b_stop, &b_step);
    Py_ssize_t cols = PySlice_AdjustIndices(self->cols, &b_start, &b_stop, b_step);
    Py_ssize_t r = a_start;
    Py_ssize_t c = b_start;
    for (Py_ssize_t i = 0; i < rows; i++)
    {
        for (Py_ssize_t j = 0; j < cols; j++)
        {
            PyMatrixAssign(self, r, c, tmp);
            c += b_step;
        }
        c = b_start;
        r += a_step;
    }
    return 0;
}

static int PyMatrix_ass_subscript(PyMatrixObject *self, PyObject *index, PyObject *value)
{
    if (index == Py_Ellipsis)
    {
        return PyMatrix_ass_subscript_sM(self, value);
    }
    if (!PyTuple_CheckExact(index))
    {
        PyErr_Format(PyExc_TypeError, "Unsupported index type:%s. Only take a tuple with 2 elements", index->ob_type->tp_name);
        return -1;
    }
    if (PyTuple_Size(index) != 2)
    {
        PyErr_Format(PyExc_IndexError, "Get %ld items. Only take a tuple with 2 elements", PyTuple_Size(index));
        return -1;
    }
    PyObject *a = PyTuple_GetItem(index, 0);
    PyObject *b = PyTuple_GetItem(index, 1);
    if (PyMatrix_Check(value))
    {
        return PyMatrix_ass_subscript_M(self, a, b, (PyMatrixObject *)value);
    }
    if (PyList_CheckExact(value))
    {
        return PyMatrix_ass_subscript_List(self, a, b, value);
    }
    if (PyArray_Check(value))
    {
        return PyMatrix_ass_subscript_nparray(self, a, b, (PyArrayObject *)value);
    }
    if (PyLong_CheckExact(a) && PyLong_CheckExact(b))
    {
        return PyMatrix_ass_subscript_LLV(self, a, b, value);
    }
    else if (PyLong_CheckExact(a) && PySlice_Check(b))
    {
        return PyMatrix_ass_subscript_LSV(self, a, b, value);
    }
    else if (PySlice_Check(a) && PyLong_CheckExact(b))
    {
        return PyMatrix_ass_subscript_SLV(self, a, b, value);
    }
    else if (PySlice_Check(a) && PySlice_Check(b))
    {
        return PyMatrix_ass_subscript_SSV(self, a, b, value);
    }
    return -1;
}

// get set

static PyObject *PyMatrix_get_shape(PyMatrixObject *self, void *closure)
{
    return Py_BuildValue("ii", self->rows, self->cols);
}

static PyObject *PyMatrix_get_rank(PyMatrixObject *self, void *closure)
{
    int r = Matrix_rank(self);
    if (r == -1)
    {
        Py_RETURN_UNSURE;
    }
    else if (r == -2)
    {
        return nullptr;
    }
    return PyLong_FromLong(r);
}

static PyNumberMethods PyMatrix_as_number = {
    .nb_add = (binaryfunc)PyMatrix_add,
    .nb_subtract = (binaryfunc)PyMatrix_subtract,
    .nb_multiply = (binaryfunc)PyMatrix_multiply,
    .nb_remainder = (binaryfunc)PyMatrix_remainder,
    .nb_divmod = (binaryfunc)PyMatrix_divmod,
    .nb_negative = (unaryfunc)PyMatrix_negative,
    .nb_positive = (unaryfunc)PyMatrix_positive,
    .nb_inplace_add = (binaryfunc)PyMatrix_inplace_add,
    .nb_inplace_subtract = (binaryfunc)PyMatrix_inplace_subtract,
    .nb_inplace_multiply = (binaryfunc)PyMatrix_inplace_multiply,
    .nb_inplace_remainder = (binaryfunc)PyMatrix_inplace_remainder,
    .nb_floor_divide = (binaryfunc)PyMatrix_floor_divide,
    .nb_true_divide = (binaryfunc)PyMatrix_true_divide,
    .nb_inplace_floor_divide = (binaryfunc)PyMatrix_inplace_floor_divide,
    .nb_inplace_true_divide = (binaryfunc)PyMatrix_inplace_true_divide,
    .nb_matrix_multiply = (binaryfunc)PyMatrix_matrix_multiply,
};

static PyMappingMethods PyMatrix_as_mapping = {
    .mp_length = (lenfunc)PyMatrix_length,
    .mp_subscript = (binaryfunc)PyMatrix_subscript,
    .mp_ass_subscript = (objobjargproc)PyMatrix_ass_subscript,
};

static PyMethodDef PyMatrix_methods[] = {
    {"__conj__", (PyCFunction)PyMatrix_conj, METH_NOARGS, nullptr},
    {"iconj", (PyCFunction)PyMatrix_iconj, METH_NOARGS, nullptr},
    {"T", (PyCFunction)PyMatrix_T, METH_NOARGS, nullptr},
    {"iT", (PyCFunction)PyMatrix_iT, METH_NOARGS, nullptr},
    {"H", (PyCFunction)PyMatrix_H, METH_NOARGS, nullptr},
    {"iH", (PyCFunction)PyMatrix_iH, METH_NOARGS, nullptr},
    {"__kronecker__", (PyCFunction)PyMatrix_kronecker, METH_O, nullptr},
    {"__hadamard__", (PyCFunction)PyMatrix_hadamard, METH_O, nullptr},
    {"vstack", (PyCFunction)PyMatrix_vstack, METH_FASTCALL | METH_STATIC, nullptr},
    {"hstack", (PyCFunction)PyMatrix_hstack, METH_FASTCALL | METH_STATIC, nullptr},
    nullptr,
};

static PyMemberDef PyMatrix_members[] = {
    {"rows", T_INT, offsetof(PyMatrixObject, rows), READONLY, nullptr},
    {"cols", T_INT, offsetof(PyMatrixObject, cols), READONLY, nullptr},
    {"total", T_LONGLONG, offsetof(PyMatrixObject, total_elements), READONLY, nullptr},
    nullptr,
};

static PyGetSetDef PyMatrix_getset[] = {
    {"shape", (getter)PyMatrix_get_shape, nullptr, nullptr, nullptr},
    {"rank", (getter)PyMatrix_get_rank, nullptr, nullptr, nullptr},
    nullptr,
};

PyTypeObject PyMatrix_Type = {
    .ob_base = PyVarObject_HEAD_INIT(&PyType_Type, 0).tp_name = "matrixcore.matrix",
    .tp_basicsize = sizeof(PyMatrixObject),
    .tp_dealloc = (destructor)PyMatrix_dealloc,
    .tp_repr = (reprfunc)PyMatrix_repr,
    .tp_as_number = &PyMatrix_as_number,
    .tp_as_mapping = &PyMatrix_as_mapping,
    .tp_str = (reprfunc)PyMatrix_str,
    .tp_flags = Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE,
    .tp_methods = PyMatrix_methods,
    .tp_members = PyMatrix_members,
    .tp_getset = PyMatrix_getset,
    .tp_init = (initproc)PyMatrix_init,
    .tp_new = (newfunc)PyMatrix_new,
};