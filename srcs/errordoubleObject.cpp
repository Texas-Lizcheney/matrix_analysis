#include <errordouble.h>

extern int doubleprecision;

static void PyErrordoubleObject_dealloc(PyErrordoubleObject *self)
{
    self->parent = nullptr;
    Py_TYPE(self)->tp_free((PyObject *)self);
    return;
}

static PyObject *PyErrordoubleObject_repr(PyErrordoubleObject *self)
{
    std::stringstream tmp;
    tmp << std::setprecision(doubleprecision) << self->num;
    return PyUnicode_FromString(tmp.str().c_str());
}

static PyObject *PyErrordoubleObject_richcompare(PyErrordoubleObject *self, PyObject *other, int opid)
{
    if (PyErrordouble_Check(other))
    {
        switch (Errordouble_compare(self->num, ((PyErrordoubleObject *)other)->num, opid))
        {
        case 2:
        {
            Py_RETURN_TRUE;
        }
        case 1:
        {
            Py_RETURN_UNSURE;
        }
        case 0:
        {
            Py_RETURN_FALSE;
        }
        default:
        {
            Py_RETURN_NOTIMPLEMENTED;
        }
        }
    }
    else if (PyFloat_Check(other) || PyLong_Check(other))
    {
        double othervalue = PyFloat_AsDouble(other);
        if (othervalue == -1 && PyErr_Occurred())
        {
            return nullptr;
        }
        switch (Errordouble_compare(self->num, othervalue, opid))
        {
        case 2:
        {
            Py_RETURN_TRUE;
        }
        case 1:
        {
            Py_RETURN_UNSURE;
        }
        case 0:
        {
            Py_RETURN_FALSE;
        }
        default:
        {
            Py_RETURN_NOTIMPLEMENTED;
        }
        }
    }
    Py_RETURN_NOTIMPLEMENTED;
}

static int PyErrordoubleObject_init(PyErrordoubleObject *self, PyObject *args, PyObject *kwds)
{
    static char *kwlist0[] = {
        (char *)"value",
        nullptr,
    };
    static char *kwlist1[] = {
        (char *)"value",
        (char *)"error",
        nullptr,
    };
    double tmp;
    if (PyArg_ParseTupleAndKeywords(args, kwds, "d|", kwlist0, &tmp))
    {
        self->num = tmp;
        return 0;
    }
    PyErr_Clear();
    if (PyArg_ParseTupleAndKeywords(args, kwds, "dd|", kwlist1, &self->num.value, &self->num.error))
    {
        return 0;
    }
    PyErr_SetString(PyExc_TypeError, "Fail to match any init arguments.");
    return -1;
}

static PyObject *PyErrordoubleObject_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyObject *self = type->tp_alloc(type, 0);
    ((PyErrordoubleObject *)self)->parent = nullptr;
    if (!self)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    return self;
}

// as number

static PyObject *PyErrordouble_add(PyObject *self, PyObject *other)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    if (PyErrordouble_Check(self))
    {
        if (PyErrordouble_Check(other))
        {
            result->num = ((PyErrordoubleObject *)self)->num + ((PyErrordoubleObject *)other)->num;
            return (PyObject *)result;
        }
        else if (PyFloat_Check(other))
        {
            result->num = ((PyErrordoubleObject *)self)->num + PyFloat_AsDouble(other);
            return (PyObject *)result;
        }
        else if (PyLong_Check(other))
        {
            double tmp = PyLong_AsDouble(other);
            if (tmp == -1 && PyErr_Occurred())
            {
                Py_DECREF(result);
                return nullptr;
            }
            result->num = ((PyErrordoubleObject *)self)->num + tmp;
            return (PyObject *)result;
        }
    }
    else if (PyFloat_Check(self))
    {
        result->num = PyFloat_AsDouble(self) + ((PyErrordoubleObject *)other)->num;
        return (PyObject *)result;
    }
    else if (PyLong_Check(self))
    {
        double tmp = PyLong_AsDouble(self);
        if (tmp == -1 && PyErr_Occurred())
        {
            Py_DECREF(result);
            return nullptr;
        }
        result->num = tmp + ((PyErrordoubleObject *)other)->num;
        return (PyObject *)result;
    }
    Py_DECREF(result);
    Py_RETURN_NOTIMPLEMENTED;
}

static PyObject *PyErrordouble_subtract(PyObject *self, PyObject *other)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    if (PyErrordouble_Check(self))
    {
        if (PyErrordouble_Check(other))
        {
            result->num = ((PyErrordoubleObject *)self)->num - ((PyErrordoubleObject *)other)->num;
            return (PyObject *)result;
        }
        else if (PyFloat_Check(other))
        {
            result->num = ((PyErrordoubleObject *)self)->num - PyFloat_AsDouble(other);
            return (PyObject *)result;
        }
        else if (PyLong_Check(other))
        {
            double tmp = PyLong_AsDouble(other);
            if (tmp == -1 && PyErr_Occurred())
            {
                Py_DECREF(result);
                return nullptr;
            }
            result->num = ((PyErrordoubleObject *)self)->num - tmp;
            return (PyObject *)result;
        }
    }
    else if (PyFloat_Check(self))
    {
        result->num = PyFloat_AsDouble(self) - ((PyErrordoubleObject *)other)->num;
        return (PyObject *)result;
    }
    else if (PyLong_Check(self))
    {
        double tmp = PyLong_AsDouble(self);
        if (tmp == -1 && PyErr_Occurred())
        {
            Py_DECREF(result);
            return nullptr;
        }
        result->num = tmp - ((PyErrordoubleObject *)other)->num;
        return (PyObject *)result;
    }
    Py_DECREF(result);
    Py_RETURN_NOTIMPLEMENTED;
}

static PyObject *PyErrordouble_multiply(PyObject *self, PyObject *other)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    if (PyErrordouble_Check(self))
    {
        if (PyErrordouble_Check(other))
        {
            result->num = ((PyErrordoubleObject *)self)->num * ((PyErrordoubleObject *)other)->num;
            return (PyObject *)result;
        }
        else if (PyFloat_Check(other))
        {
            result->num = ((PyErrordoubleObject *)self)->num * PyFloat_AsDouble(other);
            return (PyObject *)result;
        }
        else if (PyLong_Check(other))
        {
            double tmp = PyLong_AsDouble(other);
            if (tmp == -1 && PyErr_Occurred())
            {
                Py_DECREF(result);
                return nullptr;
            }
            result->num = ((PyErrordoubleObject *)self)->num * tmp;
            return (PyObject *)result;
        }
    }
    else if (PyFloat_Check(self))
    {
        result->num = PyFloat_AsDouble(self) * ((PyErrordoubleObject *)other)->num;
        return (PyObject *)result;
    }
    else if (PyLong_Check(self))
    {
        double tmp = PyLong_AsDouble(self);
        if (tmp == -1 && PyErr_Occurred())
        {
            Py_DECREF(result);
            return nullptr;
        }
        result->num = tmp * ((PyErrordoubleObject *)other)->num;
        return (PyObject *)result;
    }
    Py_DECREF(result);
    Py_RETURN_NOTIMPLEMENTED;
}

static PyObject *PyErrordouble_remainder(PyObject *self, PyObject *other)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    if (PyErrordouble_Check(self))
    {
        if (PyErrordouble_Check(other))
        {
            result->num = ((PyErrordoubleObject *)self)->num % ((PyErrordoubleObject *)other)->num;
            return (PyObject *)result;
        }
        else if (PyFloat_Check(other))
        {
            result->num = ((PyErrordoubleObject *)self)->num % PyFloat_AsDouble(other);
            return (PyObject *)result;
        }
        else if (PyLong_Check(other))
        {
            double tmp = PyLong_AsDouble(other);
            if (tmp == -1 && PyErr_Occurred())
            {
                Py_DECREF(result);
                return nullptr;
            }
            result->num = ((PyErrordoubleObject *)self)->num % tmp;
            return (PyObject *)result;
        }
    }
    else if (PyFloat_Check(self))
    {
        result->num = PyFloat_AsDouble(self) % ((PyErrordoubleObject *)other)->num;
        return (PyObject *)result;
    }
    else if (PyLong_Check(self))
    {
        double tmp = PyLong_AsDouble(self);
        if (tmp == -1 && PyErr_Occurred())
        {
            Py_DECREF(result);
            return nullptr;
        }
        result->num = tmp % ((PyErrordoubleObject *)other)->num;
        return (PyObject *)result;
    }
    Py_DECREF(result);
    Py_RETURN_NOTIMPLEMENTED;
}

static PyObject *PyErrordouble_divmod(PyObject *self, PyObject *other)
{
    PyErrordoubleObject *D = nullptr;
    D = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    PyErrordoubleObject *M = nullptr;
    M = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!D || !M)
    {
        Py_XDECREF(D);
        Py_XDECREF(M);
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    if (PyErrordouble_Check(self))
    {
        if (PyErrordouble_Check(other))
        {
            D->num = floor(((PyErrordoubleObject *)self)->num / ((PyErrordoubleObject *)other)->num);
            M->num = ((PyErrordoubleObject *)self)->num - D->num * ((PyErrordoubleObject *)other)->num;
        }
        else if (PyFloat_Check(other))
        {
            double tmp = PyFloat_AsDouble(other);
            D->num = floor(((PyErrordoubleObject *)self)->num / tmp);
            M->num = ((PyErrordoubleObject *)self)->num - D->num * tmp;
        }
        else if (PyLong_Check(other))
        {
            double tmp = PyLong_AsDouble(other);
            if (tmp == -1 && PyErr_Occurred())
            {
                Py_DECREF(D);
                Py_DECREF(M);
                return nullptr;
            }
            D->num = floor(((PyErrordoubleObject *)self)->num / tmp);
            M->num = ((PyErrordoubleObject *)self)->num - D->num * tmp;
        }
        else
        {
            Py_DECREF(D);
            Py_DECREF(M);
            Py_RETURN_NOTIMPLEMENTED;
        }
    }
    else if (PyFloat_Check(self))
    {
        double tmp = PyFloat_AsDouble(self);
        D->num = floor(tmp / ((PyErrordoubleObject *)other)->num);
        M->num = tmp - D->num * ((PyErrordoubleObject *)other)->num;
    }
    else if (PyLong_Check(self))
    {
        double tmp = PyLong_AsDouble(self);
        if (tmp == -1 && PyErr_Occurred())
        {
            Py_DECREF(D);
            Py_DECREF(M);
            return nullptr;
        }
        D->num = floor(tmp / ((PyErrordoubleObject *)other)->num);
        M->num = tmp - D->num * ((PyErrordoubleObject *)other)->num;
    }
    else
    {
        Py_DECREF(D);
        Py_DECREF(M);
        Py_RETURN_NOTIMPLEMENTED;
    }
    PyObject *returnvalue = Py_BuildValue("OO", D, M);
    Py_DECREF(D);
    Py_DECREF(M);
    return returnvalue;
}

static PyObject *PyErrordouble_power(PyObject *self, PyObject *other, PyObject *mod)
{
    error_double powvalue;
    if (PyErrordouble_Check(self))
    {
        if (PyErrordouble_Check(other))
        {
            powvalue = pow(((PyErrordoubleObject *)self)->num, ((PyErrordoubleObject *)other)->num);
        }
        else if (PyFloat_Check(other))
        {
            powvalue = pow(((PyErrordoubleObject *)self)->num, PyFloat_AsDouble(other));
        }
        else if (PyLong_Check(other))
        {
            double tmp = PyLong_AsDouble(other);
            if (tmp == -1 && PyErr_Occurred())
            {
                return nullptr;
            }
            powvalue = pow(((PyErrordoubleObject *)self)->num, tmp);
        }
        else
        {
            Py_RETURN_NOTIMPLEMENTED;
        }
    }
    else if (PyFloat_Check(self))
    {
        if (PyErrordouble_Check(other))
        {
            powvalue = pow(PyFloat_AsDouble(self), ((PyErrordoubleObject *)other)->num);
        }
        else if (PyFloat_Check(other))
        {
            powvalue = pow(PyFloat_AsDouble(self), PyFloat_AsDouble(other));
        }
        else if (PyLong_Check(other))
        {
            double tmp = PyLong_AsDouble(other);
            if (tmp == -1 && PyErr_Occurred())
            {
                return nullptr;
            }
            powvalue = pow(PyFloat_AsDouble(self), tmp);
        }
        else
        {
            Py_RETURN_NOTIMPLEMENTED;
        }
    }
    else if (PyLong_Check(self))
    {
        double tmp = PyLong_AsDouble(self);
        if (tmp == -1 && PyErr_Occurred())
        {
            return nullptr;
        }
        if (PyErrordouble_Check(other))
        {
            powvalue = pow(tmp, ((PyErrordoubleObject *)other)->num);
        }
        else if (PyFloat_Check(other))
        {
            powvalue = pow(tmp, PyFloat_AsDouble(other));
        }
        else if (PyLong_Check(other))
        {
            double tmpp = PyLong_AsDouble(other);
            if (tmpp == -1 && PyErr_Occurred())
            {
                return nullptr;
            }
            powvalue = pow(tmp, tmpp);
        }
        else
        {
            Py_RETURN_NOTIMPLEMENTED;
        }
    }
    else
    {
        Py_RETURN_NOTIMPLEMENTED;
    }
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    if (Py_IsNone(mod))
    {
        result->num = powvalue;
    }
    else if (PyErrordouble_Check(mod))
    {
        result->num = powvalue % ((PyErrordoubleObject *)mod)->num;
    }
    else if (PyFloat_Check(mod))
    {
        result->num = powvalue % PyFloat_AsDouble(mod);
    }
    else if (PyLong_Check(mod))
    {
        double tmp = PyLong_AsDouble(mod);
        if (tmp == -1 && PyErr_Occurred())
        {
            Py_DECREF(result);
            return nullptr;
        }
        result->num = powvalue % tmp;
    }
    else
    {
        Py_DECREF(result);
        Py_RETURN_NOTIMPLEMENTED;
    }
    return (PyObject *)result;
}

static PyObject *PyErrordouble_negative(PyObject *self)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = -((PyErrordoubleObject *)self)->num;
    return (PyObject *)result;
}

static PyObject *PyErrordouble_positive(PyObject *self)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = +((PyErrordoubleObject *)self)->num;
    return (PyObject *)result;
}

static PyObject *PyErrordouble_absolute(PyObject *self)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = abs(((PyErrordoubleObject *)self)->num);
    return (PyObject *)result;
}

static int PyErrordouble_bool(PyObject *self)
{
    if ((((PyErrordoubleObject *)self)->num.value == 0) && (((PyErrordoubleObject *)self)->num.error == 0))
    {
        return 0;
    }
    return 1;
}

static PyObject *PyErrordouble_int(PyObject *self)
{
    return PyLong_FromDouble(((PyErrordoubleObject *)self)->num.value);
}

static PyObject *PyErrordouble_float(PyObject *self)
{
    return PyFloat_FromDouble(((PyErrordoubleObject *)self)->num.value);
}

static PyObject *PyErrordouble_inplace_add(PyErrordoubleObject *self, PyObject *other)
{
    if (PyErrordouble_Check(other))
    {
        self->num += ((PyErrordoubleObject *)other)->num;
    }
    else if (PyFloat_Check(other))
    {
        self->num += PyFloat_AsDouble(other);
    }
    else if (PyLong_Check(other))
    {
        double tmp = PyLong_AsDouble(other);
        if (tmp == -1 && PyErr_Occurred())
        {
            return nullptr;
        }
        self->num += tmp;
    }
    else
    {
        Py_RETURN_NOTIMPLEMENTED;
    }
    Py_INCREF(self);
    return (PyObject *)self;
}

static PyObject *PyErrordouble_inplace_subtract(PyErrordoubleObject *self, PyObject *other)
{
    if (PyErrordouble_Check(other))
    {
        self->num -= ((PyErrordoubleObject *)other)->num;
    }
    else if (PyFloat_Check(other))
    {
        self->num -= PyFloat_AsDouble(other);
    }
    else if (PyLong_Check(other))
    {
        double tmp = PyLong_AsDouble(other);
        if (tmp == -1 && PyErr_Occurred())
        {
            return nullptr;
        }
        self->num -= tmp;
    }
    else
    {
        Py_RETURN_NOTIMPLEMENTED;
    }
    Py_INCREF(self);
    return (PyObject *)self;
}

static PyObject *PyErrordouble_inplace_multiply(PyErrordoubleObject *self, PyObject *other)
{
    if (PyErrordouble_Check(other))
    {
        self->num *= ((PyErrordoubleObject *)other)->num;
    }
    else if (PyFloat_Check(other))
    {
        self->num *= PyFloat_AsDouble(other);
    }
    else if (PyLong_Check(other))
    {
        double tmp = PyLong_AsDouble(other);
        if (tmp == -1 && PyErr_Occurred())
        {
            return nullptr;
        }
        self->num *= tmp;
    }
    else
    {
        Py_RETURN_NOTIMPLEMENTED;
    }
    Py_INCREF(self);
    return (PyObject *)self;
}

static PyObject *PyErrordouble_inplace_remainder(PyErrordoubleObject *self, PyObject *other)
{
    if (PyErrordouble_Check(other))
    {
        self->num %= ((PyErrordoubleObject *)other)->num;
    }
    else if (PyFloat_Check(other))
    {
        self->num %= PyFloat_AsDouble(other);
    }
    else if (PyLong_Check(other))
    {
        double tmp = PyLong_AsDouble(other);
        if (tmp == -1 && PyErr_Occurred())
        {
            return nullptr;
        }
        self->num %= tmp;
    }
    else
    {
        Py_RETURN_NOTIMPLEMENTED;
    }
    Py_INCREF(self);
    return (PyObject *)self;
}

static PyObject *PyErrordouble_inplace_power(PyErrordoubleObject *self, PyObject *other, PyObject *mod)
{
    if (PyErrordouble_Check(other))
    {
        self->num = pow(self->num, ((PyErrordoubleObject *)other)->num);
    }
    else if (PyFloat_Check(other))
    {
        self->num = pow(self->num, PyFloat_AsDouble(other));
    }
    else if (PyLong_Check(other))
    {
        double tmp = PyLong_AsDouble(other);
        if (tmp == -1 && PyErr_Occurred())
        {
            return nullptr;
        }
        self->num = pow(self->num, tmp);
    }
    else
    {
        Py_RETURN_NOTIMPLEMENTED;
    }
    if (Py_IsNone(mod))
    {
    }
    else if (PyErrordouble_Check(mod))
    {
        self->num %= ((PyErrordoubleObject *)mod)->num;
    }
    else if (PyFloat_Check(mod))
    {
        self->num %= PyFloat_AsDouble(mod);
    }
    else if (PyLong_Check(mod))
    {
        double tmp = PyLong_AsDouble(mod);
        if (tmp == -1 && PyErr_Occurred())
        {
            return nullptr;
        }
        self->num %= tmp;
    }
    else
    {
        Py_RETURN_NOTIMPLEMENTED;
    }
    Py_INCREF(self);
    return (PyObject *)self;
}

static PyObject *PyErrordouble_floor_divide(PyObject *self, PyObject *other)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    if (PyErrordouble_Check(self))
    {
        if (PyErrordouble_Check(other))
        {
            result->num = floor(((PyErrordoubleObject *)self)->num / ((PyErrordoubleObject *)other)->num);
            return (PyObject *)result;
        }
        else if (PyFloat_Check(other))
        {
            result->num = floor(((PyErrordoubleObject *)self)->num / PyFloat_AsDouble(other));
            return (PyObject *)result;
        }
        else if (PyLong_Check(other))
        {
            double tmp = PyLong_AsDouble(other);
            if (tmp == -1 && PyErr_Occurred())
            {
                Py_DECREF(result);
                return nullptr;
            }
            result->num = floor(((PyErrordoubleObject *)self)->num / tmp);
            return (PyObject *)result;
        }
    }
    else if (PyFloat_Check(self))
    {
        result->num = floor(PyFloat_AsDouble(self) / ((PyErrordoubleObject *)other)->num);
        return (PyObject *)result;
    }
    else if (PyLong_Check(self))
    {
        double tmp = PyLong_AsDouble(self);
        if (tmp == -1 && PyErr_Occurred())
        {
            Py_DECREF(result);
            return nullptr;
        }
        result->num = floor(tmp / ((PyErrordoubleObject *)other)->num);
        return (PyObject *)result;
    }
    Py_DECREF(result);
    Py_RETURN_NOTIMPLEMENTED;
}

static PyObject *PyErrordouble_true_divide(PyObject *self, PyObject *other)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    if (PyErrordouble_Check(self))
    {
        if (PyErrordouble_Check(other))
        {
            result->num = ((PyErrordoubleObject *)self)->num / ((PyErrordoubleObject *)other)->num;
            return (PyObject *)result;
        }
        else if (PyFloat_Check(other))
        {
            result->num = ((PyErrordoubleObject *)self)->num / PyFloat_AsDouble(other);
            return (PyObject *)result;
        }
        else if (PyLong_Check(other))
        {
            double tmp = PyLong_AsDouble(other);
            if (tmp == -1 && PyErr_Occurred())
            {
                Py_DECREF(result);
                return nullptr;
            }
            result->num = ((PyErrordoubleObject *)self)->num / tmp;
            return (PyObject *)result;
        }
    }
    else if (PyFloat_Check(self))
    {
        result->num = PyFloat_AsDouble(self) / ((PyErrordoubleObject *)other)->num;
        return (PyObject *)result;
    }
    else if (PyLong_Check(self))
    {
        double tmp = PyLong_AsDouble(self);
        if (tmp == -1 && PyErr_Occurred())
        {
            Py_DECREF(result);
            return nullptr;
        }
        result->num = tmp / ((PyErrordoubleObject *)other)->num;
        return (PyObject *)result;
    }
    Py_DECREF(result);
    Py_RETURN_NOTIMPLEMENTED;
}

static PyObject *PyErrordouble_inplace_floor_divide(PyErrordoubleObject *self, PyObject *other)
{
    if (PyErrordouble_Check(other))
    {
        self->num /= ((PyErrordoubleObject *)other)->num;
        self->num = floor(self->num);
    }
    else if (PyFloat_Check(other))
    {
        self->num /= PyFloat_AsDouble(other);
        self->num = floor(self->num);
    }
    else if (PyLong_Check(other))
    {
        double tmp = PyLong_AsDouble(other);
        if (tmp == -1 && PyErr_Occurred())
        {
            return nullptr;
        }
        self->num /= tmp;
        self->num = floor(self->num);
    }
    else
    {
        Py_RETURN_NOTIMPLEMENTED;
    }
    Py_INCREF(self);
    return (PyObject *)self;
}

static PyObject *PyErrordouble_inplace_true_divide(PyErrordoubleObject *self, PyObject *other)
{
    if (PyErrordouble_Check(other))
    {
        self->num /= ((PyErrordoubleObject *)other)->num;
    }
    else if (PyFloat_Check(other))
    {
        self->num /= PyFloat_AsDouble(other);
    }
    else if (PyLong_Check(other))
    {
        double tmp = PyLong_AsDouble(other);
        if (tmp == -1 && PyErr_Occurred())
        {
            return nullptr;
        }
        self->num /= tmp;
    }
    else
    {
        Py_RETURN_NOTIMPLEMENTED;
    }
    Py_INCREF(self);
    return (PyObject *)self;
}

// methods

static PyObject *PyErrordouble_round(PyErrordoubleObject *self, PyObject *const *args, Py_ssize_t nargs)
{
    if (!_PyArg_CheckPositional("__round__", nargs, 0, 1))
    {
        return nullptr;
    }
    if (nargs == 0)
    {
        return PyLong_FromDouble(round(self->num.value));
    }
    std::stringstream tmp;
    Py_ssize_t __ndigits = PyNumber_AsSsize_t(args[0], nullptr);
    if (__ndigits == -1 && PyErr_Occurred())
    {
        return nullptr;
    }
    tmp << std::fixed << std::setprecision(__ndigits) << self->num.value;
    double value;
    tmp >> value;
    return PyFloat_FromDouble(value);
}

static PyObject *PyErrordouble_exp(PyErrordoubleObject *self, PyObject *args)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = exp(self->num);
    return (PyObject *)result;
}

static PyObject *PyErrordouble_ln(PyErrordoubleObject *self, PyObject *args)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = log(self->num);
    return (PyObject *)result;
}

static PyObject *PyErrordouble_sqrt(PyErrordoubleObject *self, PyObject *args)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = sqrt(self->num);
    return (PyObject *)result;
}

static PyObject *PyErrordouble_sin(PyErrordoubleObject *self, PyObject *args)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = sin(self->num);
    return (PyObject *)result;
}

static PyObject *PyErrordouble_cos(PyErrordoubleObject *self, PyObject *args)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = cos(self->num);
    return (PyObject *)result;
}

static PyObject *PyErrordouble_tan(PyErrordoubleObject *self, PyObject *args)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = tan(self->num);
    return (PyObject *)result;
}

static PyObject *PyErrordouble_cot(PyErrordoubleObject *self, PyObject *args)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = cot(self->num);
    return (PyObject *)result;
}

static PyObject *PyErrordouble_sec(PyErrordoubleObject *self, PyObject *args)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = sec(self->num);
    return (PyObject *)result;
}

static PyObject *PyErrordouble_csc(PyErrordoubleObject *self, PyObject *args)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = csc(self->num);
    return (PyObject *)result;
}

static PyObject *PyErrordouble_arcsin(PyErrordoubleObject *self, PyObject *args)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = arcsin(self->num);
    return (PyObject *)result;
}

static PyObject *PyErrordouble_arccos(PyErrordoubleObject *self, PyObject *args)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = arccos(self->num);
    return (PyObject *)result;
}

static PyObject *PyErrordouble_arctan(PyErrordoubleObject *self, PyObject *args)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = arctan(self->num);
    return (PyObject *)result;
}

static PyObject *PyErrordouble_arccot(PyErrordoubleObject *self, PyObject *args)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = arccot(self->num);
    return (PyObject *)result;
}

static PyObject *PyErrordouble_arcsec(PyErrordoubleObject *self, PyObject *args)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = arcsec(self->num);
    return (PyObject *)result;
}

static PyObject *PyErrordouble_arccsc(PyErrordoubleObject *self, PyObject *args)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = arccsc(self->num);
    return (PyObject *)result;
}

static PyObject *PyErrordouble_sinh(PyErrordoubleObject *self, PyObject *args)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = sinh(self->num);
    return (PyObject *)result;
}

static PyObject *PyErrordouble_cosh(PyErrordoubleObject *self, PyObject *args)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = cosh(self->num);
    return (PyObject *)result;
}

static PyObject *PyErrordouble_tanh(PyErrordoubleObject *self, PyObject *args)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = tanh(self->num);
    return (PyObject *)result;
}

static PyObject *PyErrordouble_coth(PyErrordoubleObject *self, PyObject *args)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = coth(self->num);
    return (PyObject *)result;
}

static PyObject *PyErrordouble_sech(PyErrordoubleObject *self, PyObject *args)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = sech(self->num);
    return (PyObject *)result;
}

static PyObject *PyErrordouble_csch(PyErrordoubleObject *self, PyObject *args)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = csch(self->num);
    return (PyObject *)result;
}

static PyObject *PyErrordouble_arcsinh(PyErrordoubleObject *self, PyObject *args)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = arcsinh(self->num);
    return (PyObject *)result;
}

static PyObject *PyErrordouble_arccosh(PyErrordoubleObject *self, PyObject *args)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = arccosh(self->num);
    return (PyObject *)result;
}

static PyObject *PyErrordouble_arctanh(PyErrordoubleObject *self, PyObject *args)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = arctanh(self->num);
    return (PyObject *)result;
}

static PyObject *PyErrordouble_arccoth(PyErrordoubleObject *self, PyObject *args)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = arccoth(self->num);
    return (PyObject *)result;
}

static PyObject *PyErrordouble_arcsech(PyErrordoubleObject *self, PyObject *args)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = arcsech(self->num);
    return (PyObject *)result;
}

static PyObject *PyErrordouble_arccsch(PyErrordoubleObject *self, PyObject *args)
{
    PyErrordoubleObject *result = nullptr;
    result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = arccsch(self->num);
    return (PyObject *)result;
}

static PyObject *PyErrordouble_log(PyErrordoubleObject *self, PyObject *base)
{
    try
    {
        error_double tmp = {base};
        PyErrordoubleObject *result = nullptr;
        result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
        if (!result)
        {
            PyErr_SetNone(PyExc_MemoryError);
            return nullptr;
        }
        result->num = log(self->num) / log(tmp);
        return (PyObject *)result;
    }
    catch (const std::exception &e)
    {
        Py_RETURN_NOTIMPLEMENTED;
    }
    return nullptr;
}

static PyObject *PyErrordouble_rlog(PyErrordoubleObject *self, PyObject *base)
{
    try
    {
        error_double tmp = {base};
        PyErrordoubleObject *result = nullptr;
        result = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
        if (!result)
        {
            PyErr_SetNone(PyExc_MemoryError);
            return nullptr;
        }
        result->num = log(tmp) / log(self->num);
        return (PyObject *)result;
    }
    catch (const std::exception &e)
    {
        Py_RETURN_NOTIMPLEMENTED;
    }
    return nullptr;
}

// getset

static PyObject *PyErrordouble_get_value(PyErrordoubleObject *self, void *closure)
{
    return PyFloat_FromDouble(self->num.value);
}

static int PyErrordouble_set_value(PyErrordoubleObject *self, PyObject *value, void *closure)
{
    if (!value)
    {
        value = 0;
        return 0;
    }
    double tmp = PyFloat_AsDouble(value);
    if (tmp == -1 && PyErr_Occurred())
    {
        return -1;
    }
    self->num.value = tmp;
    if (self->parent)
    {
        self->parent->value = tmp;
    }
    return 0;
}

static PyObject *PyErrordouble_get_error(PyErrordoubleObject *self, void *closure)
{
    return PyFloat_FromDouble(self->num.error);
}

static int PyErrordouble_set_error(PyErrordoubleObject *self, PyObject *value, void *closure)
{
    if (!value)
    {
        value = 0;
        return 0;
    }
    double tmp = PyFloat_AsDouble(value);
    if (tmp == -1 && PyErr_Occurred())
    {
        return -1;
    }
    self->num.error = tmp;
    if (self->parent)
    {
        self->parent->error = tmp;
    }
    return 0;
}

static PyNumberMethods PyErrordouble_as_number = {
    .nb_add = (binaryfunc)PyErrordouble_add,
    .nb_subtract = (binaryfunc)PyErrordouble_subtract,
    .nb_multiply = (binaryfunc)PyErrordouble_multiply,
    .nb_remainder = (binaryfunc)PyErrordouble_remainder,
    .nb_divmod = (binaryfunc)PyErrordouble_divmod,
    .nb_power = (ternaryfunc)PyErrordouble_power,
    .nb_negative = (unaryfunc)PyErrordouble_negative,
    .nb_positive = (unaryfunc)PyErrordouble_positive,
    .nb_absolute = (unaryfunc)PyErrordouble_absolute,
    .nb_bool = (inquiry)PyErrordouble_bool,
    .nb_int = (unaryfunc)PyErrordouble_int,
    .nb_float = (unaryfunc)PyErrordouble_float,
    .nb_inplace_add = (binaryfunc)PyErrordouble_inplace_add,
    .nb_inplace_subtract = (binaryfunc)PyErrordouble_inplace_subtract,
    .nb_inplace_multiply = (binaryfunc)PyErrordouble_inplace_multiply,
    .nb_inplace_remainder = (binaryfunc)PyErrordouble_inplace_remainder,
    .nb_inplace_power = (ternaryfunc)PyErrordouble_inplace_power,
    .nb_floor_divide = (binaryfunc)PyErrordouble_floor_divide,
    .nb_true_divide = (binaryfunc)PyErrordouble_true_divide,
    .nb_inplace_floor_divide = (binaryfunc)PyErrordouble_inplace_floor_divide,
    .nb_inplace_true_divide = (binaryfunc)PyErrordouble_inplace_true_divide,
};

static PyMethodDef PyErrordouble_methods[] = {
    {"__round__", (PyCFunction)PyErrordouble_round, METH_FASTCALL, nullptr},
    {"__exp__", (PyCFunction)PyErrordouble_exp, METH_NOARGS, nullptr},
    {"__ln__", (PyCFunction)PyErrordouble_ln, METH_NOARGS, nullptr},
    {"__sqrt__", (PyCFunction)PyErrordouble_sqrt, METH_NOARGS, nullptr},
    {"__sin__", (PyCFunction)PyErrordouble_sin, METH_NOARGS, nullptr},
    {"__cos__", (PyCFunction)PyErrordouble_cos, METH_NOARGS, nullptr},
    {"__tan__", (PyCFunction)PyErrordouble_tan, METH_NOARGS, nullptr},
    {"__cot__", (PyCFunction)PyErrordouble_cot, METH_NOARGS, nullptr},
    {"__sec__", (PyCFunction)PyErrordouble_sec, METH_NOARGS, nullptr},
    {"__csc__", (PyCFunction)PyErrordouble_csc, METH_NOARGS, nullptr},
    {"__arcsin__", (PyCFunction)PyErrordouble_arcsin, METH_NOARGS, nullptr},
    {"__arccos__", (PyCFunction)PyErrordouble_arccos, METH_NOARGS, nullptr},
    {"__arctan__", (PyCFunction)PyErrordouble_arctan, METH_NOARGS, nullptr},
    {"__arccot__", (PyCFunction)PyErrordouble_arccot, METH_NOARGS, nullptr},
    {"__arcsec__", (PyCFunction)PyErrordouble_arcsec, METH_NOARGS, nullptr},
    {"__arccsc__", (PyCFunction)PyErrordouble_arccsc, METH_NOARGS, nullptr},
    {"__sinh__", (PyCFunction)PyErrordouble_sinh, METH_NOARGS, nullptr},
    {"__cosh__", (PyCFunction)PyErrordouble_cosh, METH_NOARGS, nullptr},
    {"__tanh__", (PyCFunction)PyErrordouble_tanh, METH_NOARGS, nullptr},
    {"__coth__", (PyCFunction)PyErrordouble_coth, METH_NOARGS, nullptr},
    {"__sech__", (PyCFunction)PyErrordouble_sech, METH_NOARGS, nullptr},
    {"__csch__", (PyCFunction)PyErrordouble_csch, METH_NOARGS, nullptr},
    {"__arcsinh__", (PyCFunction)PyErrordouble_arcsinh, METH_NOARGS, nullptr},
    {"__arccosh__", (PyCFunction)PyErrordouble_arccosh, METH_NOARGS, nullptr},
    {"__arctanh__", (PyCFunction)PyErrordouble_arctanh, METH_NOARGS, nullptr},
    {"__arccoth__", (PyCFunction)PyErrordouble_arccoth, METH_NOARGS, nullptr},
    {"__arcsech__", (PyCFunction)PyErrordouble_arcsech, METH_NOARGS, nullptr},
    {"__arccsch__", (PyCFunction)PyErrordouble_arccsch, METH_NOARGS, nullptr},
    {"__log__", (PyCFunction)PyErrordouble_log, METH_O, nullptr},
    {"__rlog__", (PyCFunction)PyErrordouble_rlog, METH_O, nullptr},
    nullptr,
};

static PyGetSetDef PyErrordouble_getset[] = {
    {"value", (getter)PyErrordouble_get_value, (setter)PyErrordouble_set_value, nullptr, nullptr},
    {"error", (getter)PyErrordouble_get_error, (setter)PyErrordouble_set_error, nullptr, nullptr},
    nullptr,
};

PyTypeObject PyErrordouble_Type = {
    .ob_base = PyVarObject_HEAD_INIT(&PyType_Type, 0).tp_name = "varcore.errordouble",
    .tp_basicsize = sizeof(PyErrordoubleObject),
    .tp_dealloc = (destructor)PyErrordoubleObject_dealloc,
    .tp_repr = (reprfunc)PyErrordoubleObject_repr,
    .tp_as_number = &PyErrordouble_as_number,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_richcompare = (richcmpfunc)PyErrordoubleObject_richcompare,
    .tp_methods = PyErrordouble_methods,
    .tp_getset = PyErrordouble_getset,
    .tp_init = (initproc)PyErrordoubleObject_init,
    .tp_new = (newfunc)PyErrordoubleObject_new,
};