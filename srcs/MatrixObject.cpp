#include <matrix.h>

extern void **np_array_api;
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
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        PyErr_Format(PyExc_MemoryError, "Fail to allocate %lld numbers.", self->total_elements);
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
                        repr += ",\t";
                    }
                    else if (!colskipped)
                    {
                        repr += "...,\t";
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
                    repr += "...,\t";
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
                repr += ",\t";
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
                        repr += "\t";
                    }
                    else if (!colskipped)
                    {
                        repr += "...\t";
                        colskipped = true;
                    }
                }
                repr += "]\n";
            }
            else if (!rowskipped)
            {
                repr += "[";
                for (int c = 0; c < std::min(self->cols, escape_cols_from + escape_cols_to + 1); c++)
                {
                    repr += "...\t";
                }
                repr += "]\n";
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
                repr += "\t";
            }
            repr += "]\n";
        }
    }
    return PyUnicode_FromString(repr.c_str());
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
    PyObject *tmp_object = nullptr;
    PyObject *tmp_matrix = nullptr;
    if (PyArg_ParseTupleAndKeywords(args, kwds, "ii|O", kwlist_0, &self->rows, &self->cols, &tmp_object))
    {
        if (self->rows <= 0 || self->cols <= 0)
        {
            PyErr_Format(PyExc_ValueError, "Invalid size. rows: %ld, cols: %ld", self->rows, self->cols);
            return -1;
        }
        ComplexVar tmp_value;
        if (assignComplexVar(tmp_object, tmp_value))
        {
            PyErr_Format(PyExc_ValueError, "Unsupported type: %s", tmp_object->ob_type->tp_name);
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
    }
    else if (PyArg_ParseTupleAndKeywords(args, kwds, "O|O", kwlist_1, &tmp_matrix, &tmp_object))
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
                self->rows = PyList_Size(tmp_matrix);
                self->cols = PyList_Size(tmp);
                for (Py_ssize_t i = 1; i < self->rows; i++)
                {
                    tmp = PyList_GetItem(tmp_matrix, i);
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
                if (assignComplexVar(tmp_object, tmp_value))
                {
                    PyErr_Format(PyExc_ValueError, "Unsupported type: %s", tmp_object->ob_type->tp_name);
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
                    tmp = PyList_GetItem(tmp_matrix, i);
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
            }
            else if (PyTuple_CheckExact(tmp))
            {
                self->rows = 0;
                self->cols = 0;
                PyObject *tmp;
                Py_ssize_t total = PyList_Size(tmp_matrix);
                PyObject *pair;
                int r;
                int c;
                for (Py_ssize_t i = 0; i < total; i++)
                {
                    tmp = PyList_GetItem(tmp_matrix, i);
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
                if (assignComplexVar(tmp_object, tmp_value))
                {
                    PyErr_Format(PyExc_ValueError, "Unsupported type: %s", tmp_object->ob_type->tp_name);
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
                    PyArg_ParseTuple(PyList_GetItem(tmp_matrix, i), "iiO", &r, &c, &pair);
                    {
                        PyErr_Format(PyExc_ValueError, "Unsupport type: %s On index:%ld", pair->ob_type->tp_name, i);
                        return -1;
                    }
                    PyMatrixAssign(self, r, c, tmp_matrix_value);
                }
            }
            else
            {
                goto init_error;
            }
        }
        else if (PyArray_CheckExact(tmp_matrix))
        {
        }
        else
        {
            goto init_error;
        }
    }
    else
    {
        goto init_error;
    }
    PyErr_Clear();
    return 0;
init_error:
    PyErr_SetString(PyExc_ValueError, "Fail to match init arguments.");
    return -1;
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