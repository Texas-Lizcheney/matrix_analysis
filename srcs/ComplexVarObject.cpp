#include <complexvar.h>

int doubleprecision = 7;

PyObject *SetDoublePrecision(PyObject *self, PyObject *p)
{
    if (!PyLong_CheckExact(p))
    {
        PyErr_SetString(PyExc_ValueError, "Fail to set precision!");
        Py_RETURN_NONE;
    }
    int pre = PyLong_AsLong(p);
    if (pre < 0)
    {
        doubleprecision = 0;
        Py_RETURN_NONE;
    }
    if (pre > 15)
    {
        doubleprecision = 15;
        Py_RETURN_NONE;
    }
    doubleprecision = pre;
    Py_RETURN_NONE;
}

int assignComplexVar(PyObject *value, ComplexVar &target)
{
    if (PyComplexVar_CheckExact(value))
    {
        target = ((PyComplexVarObject *)value)->num;
        return 0;
    }
    if Py_IsNone (value)
    {
        target.isArbitrary = true;
        goto set_both_zero;
    }
    target.isArbitrary = false;
    if (PyLong_CheckExact(value))
    {
        target.real = PyLong_AsDouble(value);
        goto set_imag_zero;
    }
    if (PyFloat_CheckExact(value))
    {
        target.real = PyLong_AsDouble(value);
        goto set_imag_zero;
    }
    if (PyComplex_CheckExact(value))
    {
        target.real = PyComplex_RealAsDouble(value);
        target.imag = PyComplex_ImagAsDouble(value);
        return 0;
    }
    return -1;
set_both_zero:
    target.real = 0;
set_imag_zero:
    target.imag = 0;
    return 0;
}

void PyComplexVar_dealloc(PyComplexVarObject *self)
{
    Py_TYPE(self)->tp_free((PyObject *)self);
}

PyObject *PyComplexVar_repr(PyComplexVarObject *self)
{
    std::stringstream tmp;
    if (self->num.isArbitrary)
    {
        tmp << "None";
    }
    else
    {
        tmp << std::setprecision(doubleprecision) << self->num.real;
        if (self->num.imag >= 0)
        {
            tmp << '+';
        }
        tmp << self->num.imag << 'j';
    }
    return PyUnicode_FromString(tmp.str().c_str());
}

PyObject *PyComplexVar_str(PyComplexVarObject *self)
{
    std::stringstream tmp;
    if (self->num.isArbitrary)
    {
        tmp << "undefined";
    }
    else
    {
        tmp << std::setprecision(doubleprecision) << self->num.real;
        if (self->num.imag >= 0)
        {
            tmp << '+';
        }
        tmp << self->num.imag << 'i';
    }
    return PyUnicode_FromString(tmp.str().c_str());
}

int PyComplexVar_init(PyComplexVarObject *self, PyObject *args, PyObject *kwds)
{
    int argcount = 0;
    if (args)
    {
        argcount += PyTuple_GET_SIZE(args);
    }
    if (kwds)
    {
        argcount += PyDict_GET_SIZE(kwds);
    }
    switch (argcount)
    {
    case 0:
    {
        self->num.isArbitrary = true;
        self->num.real = 0;
        self->num.imag = 0;
        break;
    }
    case 1:
    {
        static char *kwlist[] = {
            (char *)"num",
            nullptr,
        };
        PyObject *tmp;
        if (!PyArg_ParseTupleAndKeywords(args, kwds, "O|", kwlist,
                                         &tmp))
        {
            return -1;
        }
        if (assignComplexVar(tmp, self->num))
        {
            PyErr_Format(PyExc_ValueError, "Unsupported type: %s", tmp->ob_type->tp_name);
            return -1;
        }
        break;
    }
    case 2:
    {
        static char *kwlist[] = {
            (char *)"real",
            (char *)"imag",
            nullptr,
        };
        if (!PyArg_ParseTupleAndKeywords(args, kwds, "dd|", kwlist,
                                         &self->num.real,
                                         &self->num.imag))
        {
            return -1;
        }
        break;
    }
    default:
    {
        PyErr_Format(PyExc_ValueError, "Too many arguments, take %d in total.", argcount);
        return -1;
    }
    }
    return 0;
}

PyObject *PyComplexVar_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyObject *self = type->tp_alloc(type, 0);
    if (!self)
    {
        Py_RETURN_NONE;
    }
    return self;
}

PyTypeObject PyComplexVarType = {
    .ob_base = PyVarObject_HEAD_INIT(&PyType_Type, 0).tp_name = "varcore.complexvar",
    .tp_basicsize = sizeof(PyComplexVarObject),
    .tp_dealloc = (destructor)PyComplexVar_dealloc,
    .tp_repr = (reprfunc)PyComplexVar_repr,
    .tp_str = (reprfunc)PyComplexVar_str,
    .tp_init = (initproc)PyComplexVar_init,
    .tp_new = (newfunc)PyComplexVar_new,
};