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