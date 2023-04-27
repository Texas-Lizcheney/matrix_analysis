#include <utilities.h>

double getdouble_fromPyObject(PyObject *value)
{
    if (PyLong_CheckExact(value))
    {
        return PyLong_AsDouble(value);
    }
    if (PyFloat_Check(value))
    {
        return PyFloat_AsDouble(value);
    }
    PyErr_Format(PyExc_ValueError, "Fail to convert into double, type:%s", value->ob_type->tp_name);
    return nan("");
}

double casthalf_to_double(const uint16_t &x)
{
    uint64_t result = 0;
    result |= (((uint64_t)x) & (uint64_t)0x03ff) << 42;
    result |= ((uint64_t)(((x >> 10) & 0x1f) - 0xf + 0x3ff)) << 52;
    if (x & 0x8000)
    {
        result |= 0x8000000000000000;
    }
    return (double &)result;
}

double cal_original_error(const double &x)
{
    int64_t exp = ((uint64_t &)x >> 52) & 0x7ff;
    exp -= 54;
    if (std::signbit(exp))
    {
        return 0.0;
    }
    exp <<= 52;
    return (double &)(exp);
}

int doubleprecision = 7;
PyObject *SetDoublePrecision(PyObject *self, PyObject *value)
{
    if (Py_IsNone(value))
    {
        doubleprecision = 7;
        Py_RETURN_NONE;
    }
    if (!PyLong_CheckExact(value))
    {
        PyErr_SetString(PyExc_ValueError, "Fail to set precision! Only accept int or none.");
        Py_RETURN_NONE;
    }
    int pre = PyLong_AsLong(value);
    if (pre < 0)
    {
        doubleprecision = 0;
        Py_RETURN_NONE;
    }
    doubleprecision = pre;
    Py_RETURN_NONE;
}

bool isdeg = false;
PyObject *SetArgFormat(PyObject *self, PyObject *value)
{
    if (Py_IsNone(value))
    {
        isdeg = false;
        Py_RETURN_NONE;
    }
    if (PyUnicode_CheckExact(value))
    {
        const char *format = PyUnicode_AsUTF8(value);
        if (!strcmp(format, "deg"))
        {
            isdeg = true;
            Py_RETURN_NONE;
        }
        if (!strcmp(format, "rad"))
        {
            isdeg = false;
            Py_RETURN_NONE;
        }
    }
    PyErr_SetString(PyExc_ValueError, "Fail to set format! Only accept \"deg\" and \"rad\" or none.");
    return nullptr;
}

bool fastprint = false;
PyObject *SetFastPrint(PyObject *self, PyObject *value)
{
    if (Py_IsNone(value))
    {
        fastprint = false;
    }
    if (Py_IsTrue(value))
    {
        fastprint = true;
    }
    else if (Py_IsFalse(value))
    {
        fastprint = false;
    }
    else
    {
        PyErr_SetString(PyExc_ValueError, "Fail to set value! Only accept bool or none.");
        return nullptr;
    }
    Py_RETURN_NONE;
}

int escape_rows_from = 3;
int escape_rows_to = 3;
int escape_cols_from = 3;
int escape_cols_to = 3;
PyObject *SetPrintArea(PyObject *self, PyObject *args, PyObject *kwds)
{
    static char *kwlist[] = {
        (char *)"left",
        (char *)"right",
        (char *)"up",
        (char *)"down",
        nullptr,
    };
    if ((PyTuple_Size(args) == 0) && !kwds)
    {
        escape_rows_from = 3;
        escape_rows_to = 3;
        escape_cols_from = 3;
        escape_cols_to = 3;
        Py_RETURN_NONE;
    }
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "|$iiii", kwlist, &escape_cols_from, &escape_cols_to, &escape_rows_from, &escape_rows_to))
    {
        PyErr_SetString(PyExc_ValueError, "Fail to set value! Only accept ints.");
        return nullptr;
    }
    Py_RETURN_NONE;
}

bool print_error = false;
PyObject *SetPrintError(PyObject *self, PyObject *value)
{
    if (Py_IsNone(value))
    {
        print_error = false;
    }
    else if (Py_IsTrue(value))
    {
        print_error = true;
    }
    else if (Py_IsFalse(value))
    {
        print_error = false;
    }
    else
    {
        PyErr_SetString(PyExc_ValueError, "Fail to set value! Only accept bool or none.");
        return nullptr;
    }
    Py_RETURN_NONE;
}