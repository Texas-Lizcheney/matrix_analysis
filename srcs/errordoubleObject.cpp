#include <errordouble.h>

extern int doubleprecision;

void PyErrordoubleObject_dealloc(PyErrordoubleObject *self)
{
    Py_TYPE(self)->tp_free((PyObject *)self);
    return;
}

PyObject *PyErrordoubleObject_repr(PyErrordoubleObject *self)
{
    std::stringstream tmp;
    tmp << std::setprecision(doubleprecision) << self->num;
    return PyUnicode_FromString(tmp.str().c_str());
}

int PyErrordoubleObject_init(PyErrordoubleObject *self, PyObject *args, PyObject *kwds)
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
        goto ErrordoubleObject_init_done;
    }
    if (PyArg_ParseTupleAndKeywords(args, kwds, "dd|", kwlist1, &self->num.value, &self->num.error))
    {
        goto ErrordoubleObject_init_done;
    }
    PyErr_SetString(PyExc_TypeError, "Fail to match any init arguments.");
    return -1;
ErrordoubleObject_init_done:
    PyErr_Clear();
    return 0;
}

PyObject *PyErrordoubleObject_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyObject *self = type->tp_alloc(type, 0);
    if (!self)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    return self;
}

// as number

PyObject *PyErrordouble_add(PyObject *self, PyObject *other)
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
            result->num = ((PyErrordoubleObject *)self)->num + PyLong_AsDouble(other);
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
        result->num = PyLong_AsDouble(self) + ((PyErrordoubleObject *)other)->num;
        return (PyObject *)result;
    }
    Py_DECREF(result);
    Py_RETURN_NOTIMPLEMENTED;
}

PyObject *PyErrordouble_subtract(PyObject *self, PyObject *other)
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
            result->num = ((PyErrordoubleObject *)self)->num - PyLong_AsDouble(other);
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
        result->num = PyLong_AsDouble(self) - ((PyErrordoubleObject *)other)->num;
        return (PyObject *)result;
    }
    Py_DECREF(result);
    Py_RETURN_NOTIMPLEMENTED;
}

PyObject *PyErrordouble_multiply(PyObject *self, PyObject *other)
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
            result->num = ((PyErrordoubleObject *)self)->num * PyLong_AsDouble(other);
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
        result->num = PyLong_AsDouble(self) * ((PyErrordoubleObject *)other)->num;
        return (PyObject *)result;
    }
    Py_DECREF(result);
    Py_RETURN_NOTIMPLEMENTED;
}

PyObject *PyErrordouble_remainder(PyObject *self, PyObject *other)
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
            result->num = ((PyErrordoubleObject *)self)->num % PyLong_AsDouble(other);
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
        result->num = PyLong_AsDouble(self) % ((PyErrordoubleObject *)other)->num;
        return (PyObject *)result;
    }
    Py_DECREF(result);
    Py_RETURN_NOTIMPLEMENTED;
}

PyObject *PyErrordouble_divmod(PyObject *self, PyObject *other)
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

PyObject *PyErrordouble_power(PyObject *self, PyObject *other, PyObject *mod)
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
            powvalue = pow(((PyErrordoubleObject *)self)->num, PyLong_AsDouble(other));
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
            powvalue = pow(PyFloat_AsDouble(self), PyLong_AsDouble(other));
        }
        else
        {
            Py_RETURN_NOTIMPLEMENTED;
        }
    }
    else if (PyLong_Check(self))
    {
        if (PyErrordouble_Check(other))
        {
            powvalue = pow(PyLong_AsDouble(self), ((PyErrordoubleObject *)other)->num);
        }
        else if (PyFloat_Check(other))
        {
            powvalue = pow(PyLong_AsDouble(self), PyFloat_AsDouble(other));
        }
        else if (PyLong_Check(other))
        {
            powvalue = pow(PyLong_AsDouble(self), PyLong_AsDouble(other));
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
        result->num = powvalue % PyLong_AsDouble(mod);
    }
    else
    {
        Py_DECREF(result);
        Py_RETURN_NOTIMPLEMENTED;
    }
    return (PyObject *)result;
}

PyObject *PyErrordouble_negative(PyObject *self)
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

PyObject *PyErrordouble_positive(PyObject *self)
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

PyObject *PyErrordouble_absolute(PyObject *self)
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

int PyErrordouble_bool(PyObject *self)
{
    if ((((PyErrordoubleObject *)self)->num.value == 0)&&(((PyErrordoubleObject *)self)->num.error == 0))
    {
        return 0;
    }
    return 1;
}

PyObject *PyErrordouble_int(PyObject *self)
{
    return PyLong_FromDouble(((PyErrordoubleObject *)self)->num.value);
}

PyObject *PyErrordouble_float(PyObject *self)
{
    return PyFloat_FromDouble(((PyErrordoubleObject *)self)->num.value);
}

PyObject *PyErrordouble_inplace_add(PyErrordoubleObject *self, PyObject *other)
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
        self->num += PyLong_AsDouble(other);
    }
    else
    {
        Py_RETURN_NOTIMPLEMENTED;
    }
    Py_INCREF(self);
    return (PyObject *)self;
}

PyObject *PyErrordouble_inplace_subtract(PyErrordoubleObject *self, PyObject *other)
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
        self->num -= PyLong_AsDouble(other);
    }
    else
    {
        Py_RETURN_NOTIMPLEMENTED;
    }
    Py_INCREF(self);
    return (PyObject *)self;
}

PyObject *PyErrordouble_inplace_multiply(PyErrordoubleObject *self, PyObject *other)
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
        self->num *= PyLong_AsDouble(other);
    }
    else
    {
        Py_RETURN_NOTIMPLEMENTED;
    }
    Py_INCREF(self);
    return (PyObject *)self;
}

PyObject *PyErrordouble_inplace_remainder(PyErrordoubleObject *self, PyObject *other)
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
        self->num %= PyLong_AsDouble(other);
    }
    else
    {
        Py_RETURN_NOTIMPLEMENTED;
    }
    Py_INCREF(self);
    return (PyObject *)self;
}

PyObject *PyErrordouble_inplace_power(PyErrordoubleObject *self, PyObject *other, PyObject *mod)
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
        self->num = pow(self->num, PyLong_AsDouble(other));
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
        self->num %= PyLong_AsDouble(mod);
    }
    else
    {
        Py_RETURN_NOTIMPLEMENTED;
    }
    Py_INCREF(self);
    return (PyObject *)self;
}

PyObject *PyErrordouble_floor_divide(PyObject *self, PyObject *other)
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
            result->num = floor(((PyErrordoubleObject *)self)->num / PyLong_AsDouble(other));
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
        result->num = floor(PyLong_AsDouble(self) / ((PyErrordoubleObject *)other)->num);
        return (PyObject *)result;
    }
    Py_DECREF(result);
    Py_RETURN_NOTIMPLEMENTED;
}

PyObject *PyErrordouble_true_divide(PyObject *self, PyObject *other)
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
            result->num = ((PyErrordoubleObject *)self)->num / PyLong_AsDouble(other);
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
        result->num = PyLong_AsDouble(self) / ((PyErrordoubleObject *)other)->num;
        return (PyObject *)result;
    }
    Py_DECREF(result);
    Py_RETURN_NOTIMPLEMENTED;
}

PyObject *PyErrordouble_inplace_floor_divide(PyErrordoubleObject *self, PyObject *other)
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
        self->num /= PyLong_AsDouble(other);
        self->num = floor(self->num);
    }
    else
    {
        Py_RETURN_NOTIMPLEMENTED;
    }
    Py_INCREF(self);
    return (PyObject *)self;
}

PyObject *PyErrordouble_inplace_true_divide(PyErrordoubleObject *self, PyObject *other)
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
        self->num /= PyLong_AsDouble(other);
    }
    else
    {
        Py_RETURN_NOTIMPLEMENTED;
    }
    Py_INCREF(self);
    return (PyObject *)self;
}

// methods

PyObject *PyErrordouble_round(PyErrordoubleObject *self, PyObject *const *args, Py_ssize_t nargs)
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
    nullptr,
};

static PyMemberDef PyErrordouble_members[] = {
    {"value", T_DOUBLE, offsetof(PyErrordoubleObject, num.value), 0, nullptr},
    {"error", T_DOUBLE, offsetof(PyErrordoubleObject, num.error), 0, nullptr},
    nullptr,
};

PyTypeObject PyErrordouble_Type = {
    .ob_base = PyVarObject_HEAD_INIT(&PyType_Type, 0).tp_name = "varcore.errordouble",
    .tp_basicsize = sizeof(PyErrordoubleObject),
    .tp_dealloc = (destructor)PyErrordoubleObject_dealloc,
    .tp_repr = (reprfunc)PyErrordoubleObject_repr,
    .tp_as_number = &PyErrordouble_as_number,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_methods = PyErrordouble_methods,
    .tp_members = PyErrordouble_members,
    .tp_init = (initproc)PyErrordoubleObject_init,
    .tp_new = (newfunc)PyErrordoubleObject_new,
};