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