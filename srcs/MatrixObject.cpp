#include <matrix.h>

extern int doubleprecision;
bool fastprint = false;
int escape_rows_from = 3;
int escape_rows_to = 3;
int escape_cols_from = 3;
int escape_cols_to = 3;
extern PyObject *PyExc_ShapeError;
template <typename T>
concept npy_real = std::is_integral<T>::value || std::is_floating_point<T>::value;

template <typename T>
concept npy_complex = requires(T a) {
                          a.real;
                          a.imag;
                      };

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

ComplexVar PyMatrixGetitem(const PyMatrixObject *const self, int r, int c)
{
    return self->elements[r * self->cols + c];
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
        PyErr_SetString(PyExc_IndexError, "Index out of range.");
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
        PyErr_SetString(PyExc_IndexError, "Index out of range.");
        return -1;
    }
    value = self->elements[R * self->cols + C];
    return 0;
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
    if (assignComplexVar_withExc(fill, tmp_value))
    {
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
                if (assignComplexVar_withExc(PyList_GetItem(tmp, j), tmp_matrix_value))
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
    if (assignComplexVar_withExc(fill, tmp_value))
    {
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
        if (assignComplexVar_withExc(pair, tmp_matrix_value))
        {
            PyObject *type;
            PyObject *value;
            PyObject *traceback;
            PyErr_Fetch(&type, &value, &traceback);
            PyObject *newvalue = PyUnicode_FromFormat("%S\nOn index:%ld", value, i);
            PyErr_Restore(type, newvalue, traceback);
            Py_DECREF(value);
            return -1;
        }
        PyMatrixAssign(self, r, c, tmp_matrix_value);
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
            if (assignComplexVar_withExc(data[pos], target[pos]))
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
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    ((PyMatrixObject *)self)->elements = nullptr;
    return self;
}

// as number

PyObject *PyMatrix_add(PyObject *self, PyObject *other)
{
    if (PyMatrix_Check(self) && PyMatrix_Check(other))
    {
        return (PyObject *)MatrixAdd((PyMatrixObject *)self, (PyMatrixObject *)other);
    }
    Py_RETURN_NOTIMPLEMENTED;
}

PyObject *PyMatrix_subtract(PyObject *self, PyObject *other)
{
    if (PyMatrix_Check(self) && PyMatrix_Check(other))
    {
        return (PyObject *)MatrixSub((PyMatrixObject *)self, (PyMatrixObject *)other);
    }
    Py_RETURN_NOTIMPLEMENTED;
}

PyObject *PyMatrix_multiply(PyObject *self, PyObject *other)
{
    ComplexVar tmp;
    if (PyMatrix_Check(self) && CanBeComplexVar(other))
    {
        assignComplexVar(other, tmp);
        return (PyObject *)MatrixMulConstant((PyMatrixObject *)self, tmp);
    }
    if (PyMatrix_Check(other) && CanBeComplexVar(self))
    {
        assignComplexVar(self, tmp);
        return (PyObject *)MatrixMulConstant((PyMatrixObject *)other, tmp);
    }
    Py_RETURN_NOTIMPLEMENTED;
}

PyObject *PyMatrix_matrix_multiply(PyObject *self, PyObject *other)
{
    if (PyMatrix_Check(self) && PyMatrix_Check(other))
    {
        return (PyObject *)MatrixMatmul((PyMatrixObject *)self, (PyMatrixObject *)other);
    }
    Py_RETURN_NOTIMPLEMENTED;
}

// as map

Py_ssize_t PyMatrix_length(PyMatrixObject *self)
{
    return self->total_elements;
}

static PyObject *PyMatrix_subscript_LL(PyMatrixObject *self, PyObject *a, PyObject *b)
{
    int r = PyLong_AS_LONG(a);
    int c = PyLong_AS_LONG(b);
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
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
        PyErr_SetString(PyExc_IndexError, "Index out of range.");
        return nullptr;
    }
    PyMatrixObject *result = nullptr;
    result = (PyMatrixObject *)PyMatrix_new(&PyMatrixType, nullptr, nullptr);
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
            PyErr_SetString(PyExc_IndexError, "Index out of range.");
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
        PyErr_SetString(PyExc_IndexError, "Index out of range.");
        return nullptr;
    }
    PyMatrixObject *result = nullptr;
    result = (PyMatrixObject *)PyMatrix_new(&PyMatrixType, nullptr, nullptr);
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
            PyErr_SetString(PyExc_IndexError, "Index out of range.");
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
    result = (PyMatrixObject *)PyMatrix_new(&PyMatrixType, nullptr, nullptr);
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
            PyErr_SetString(PyExc_IndexError, "Index out of range.");
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

PyObject *PyMatrix_copy(PyMatrixObject *self)
{
    PyMatrixObject *result = nullptr;
    result = (PyMatrixObject *)PyMatrix_new(&PyMatrixType, nullptr, nullptr);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->rows = self->rows;
    result->cols = self->cols;
    if (PyMatrixAlloc(result))
    {
        Py_DECREF(result);
        return nullptr;
    }
    for (Py_ssize_t i = 0; i < result->rows; i++)
    {
        for (Py_ssize_t j = 0; j < result->cols; j++)
        {
            PyMatrixAssign(result, i, j, PyMatrixGetitem(self, i, j));
        }
    }
    return (PyObject *)result;
}

PyObject *PyMatrix_subscript(PyMatrixObject *self, PyObject *index)
{
    if (index == Py_Ellipsis)
    {
        return PyMatrix_copy(self);
    }
    if (!PyTuple_CheckExact(index))
    {
        PyErr_Format(PyExc_TypeError, "Unsupported index type:%s. Only take a ellipsis a tuple with 2 elements.", index->ob_type->tp_name);
        return nullptr;
    }
    if (PyTuple_Size(index) != 2)
    {
        PyErr_Format(PyExc_IndexError, "Get %ld items. Only take a tuple with 2 elements.", PyTuple_Size(index));
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
    PyErr_Format(PyExc_TypeError, "Indices must be integers or slices. Get %s and %s.", a->ob_type->tp_name, b->ob_type->tp_name);
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
        PyErr_Format(PyExc_TypeError, "Indices must be integers or slices. Get %s.", x->ob_type->tp_name);
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
            PyErr_Format(PyExc_ValueError, "Inconsistency value format. On index:%ld", i);
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
                PyObject *type;
                PyObject *value;
                PyObject *traceback;
                PyErr_Fetch(&type, &value, &traceback);
                PyObject *newvalue = PyUnicode_FromFormat("%S\nTrying to assign object at %ld %ld to %ld %ld.", value, i, j, r, c);
                PyErr_Restore(type, newvalue, traceback);
                Py_DECREF(value);
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
        PyErr_SetString(PyExc_ValueError, "Unsupport dtype.");
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
        PyErr_SetString(PyExc_IndexError, "Index out of range.");
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
        PyErr_SetString(PyExc_IndexError, "Index out of range.");
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

int PyMatrix_ass_subscript(PyMatrixObject *self, PyObject *index, PyObject *value)
{
    if (index == Py_Ellipsis)
    {
        return PyMatrix_ass_subscript_sM(self, value);
    }
    if (!PyTuple_CheckExact(index))
    {
        PyErr_Format(PyExc_TypeError, "Unsupported index type:%s. Only take a tuple with 2 elements.", index->ob_type->tp_name);
        return -1;
    }
    if (PyTuple_Size(index) != 2)
    {
        PyErr_Format(PyExc_IndexError, "Get %ld items. Only take a tuple with 2 elements.", PyTuple_Size(index));
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

PyObject *PyMatrix_get_shape(PyMatrixObject *self, void *closure)
{
    return Py_BuildValue("ii", self->rows, self->cols);
}

static PyNumberMethods PyMatrixNumber = {
    .nb_add = (binaryfunc)PyMatrix_add,
    .nb_subtract = (binaryfunc)PyMatrix_subtract,
    .nb_multiply = (binaryfunc)PyMatrix_multiply,
    .nb_matrix_multiply = (binaryfunc)PyMatrix_matrix_multiply,
};

static PyMappingMethods PyMatrixMap = {
    .mp_length = (lenfunc)PyMatrix_length,
    .mp_subscript = (binaryfunc)PyMatrix_subscript,
    .mp_ass_subscript = (objobjargproc)PyMatrix_ass_subscript,
};

static PyMemberDef PyMatrixMember[] = {
    {"rows", T_INT, offsetof(PyMatrixObject, rows), READONLY, nullptr},
    {"cols", T_INT, offsetof(PyMatrixObject, cols), READONLY, nullptr},
    {"total", T_LONGLONG, offsetof(PyMatrixObject, total_elements), READONLY, nullptr},
    nullptr,
};

static PyGetSetDef PyMatrixGetSet[] = {
    {"shape", (getter)PyMatrix_get_shape, nullptr, nullptr, nullptr},
    nullptr,
};

PyTypeObject PyMatrixType = {
    .ob_base = PyVarObject_HEAD_INIT(&PyType_Type, 0).tp_name = "matrixcore.matrix",
    .tp_basicsize = sizeof(PyMatrixObject),
    .tp_dealloc = (destructor)PyMatrix_dealloc,
    .tp_repr = (reprfunc)PyMatrix_repr,
    .tp_as_number = &PyMatrixNumber,
    .tp_as_mapping = &PyMatrixMap,
    .tp_str = (reprfunc)PyMatrix_str,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_members = PyMatrixMember,
    .tp_getset = PyMatrixGetSet,
    .tp_init = (initproc)PyMatrix_init,
    .tp_new = (newfunc)PyMatrix_new,
};