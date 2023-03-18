#include <complexvar.h>

int doubleprecision = 7;
bool isdeg = false;
PyObject *PyExc_Undefined = nullptr;

PyObject *SetDoublePrecision(PyObject *self, PyObject *value)
{
    if (!PyLong_CheckExact(value))
    {
        PyErr_SetString(PyExc_ValueError, "Fail to set precision! Only accept int.");
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

PyObject *SetArgFormat(PyObject *self, PyObject *value)
{
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
    PyErr_SetString(PyExc_ValueError, "Fail to set format! Only accept \"deg\" and \"rad\".");
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
        target.real = PyFloat_AsDouble(value);
        goto set_imag_zero;
    }
    if (PyComplex_CheckExact(value))
    {
        target.real = PyComplex_RealAsDouble(value);
        target.imag = PyComplex_ImagAsDouble(value);
        return 0;
    }
    PyErr_Format(PyExc_ValueError, "Unsupported type: %s", value->ob_type->tp_name);
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

PyObject *PyComplexVar_richcompare(PyComplexVarObject *self, PyObject *other, int opid)
{
    ComplexVar tmp;
    if (assignComplexVar(other, tmp))
    {
        Py_RETURN_NOTIMPLEMENTED;
    }
    switch (opid)
    {
    case Py_EQ:
    {
        if (self->num.isArbitrary || tmp.isArbitrary)
        {
            Py_RETURN_FALSE;
        }
        if (self->num.real == tmp.real && self->num.imag == tmp.imag)
        {
            Py_RETURN_TRUE;
        }
        Py_RETURN_FALSE;
    }
    case Py_NE:
    {
        if (self->num.isArbitrary || tmp.isArbitrary)
        {
            Py_RETURN_FALSE;
        }
        if (self->num.real != tmp.real || self->num.imag != tmp.imag)
        {
            Py_RETURN_TRUE;
        }
        Py_RETURN_FALSE;
    }
    default:
        break;
    }
    Py_RETURN_NOTIMPLEMENTED;
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
        if (!PyArg_ParseTupleAndKeywords(args, kwds, "O|", kwlist, &tmp))
        {
            return -1;
        }
        if (assignComplexVar(tmp, self->num))
        {
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
        if (!PyArg_ParseTupleAndKeywords(args, kwds, "dd|", kwlist, &self->num.real, &self->num.imag))
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

// as number

PyObject *PyComplexVar_add(PyComplexVarObject *self, PyObject *other)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    ComplexVar tmp;
    if (assignComplexVar(other, tmp))
    {
        Py_RETURN_NONE;
    }
    result->num = ComplexVar_add(self->num, tmp);
    return (PyObject *)result;
}

// get set methods

PyObject *PyComplexVar_get_len(PyComplexVarObject *self, void *closure)
{
    if (self->num.isArbitrary)
    {
        PyErr_SetString(PyExc_Undefined, "This object is still undefined");
        return PyFloat_FromDouble(nan(""));
    }
    return PyFloat_FromDouble(ComplexVar_length(self->num));
}

int PyComplexVar_set_len(PyComplexVarObject *self, PyObject *value, void *closure)
{
    if (!value)
    {
        self->num.isArbitrary = true;
        return 0;
    }
    if (self->num.isArbitrary)
    {
        PyErr_SetString(PyExc_Undefined, "This object is still undefined");
        return -1;
    }
    double length = getdouble_fromPyObject(value);
    setvalue_frompolar(length, ComplexVar_arg(self->num), self->num);
    return 0;
}

PyObject *PyComplexVar_get_arg(PyComplexVarObject *self, void *closure)
{
    if (self->num.isArbitrary)
    {
        PyErr_SetString(PyExc_Undefined, "This object is still undefined");
        return PyFloat_FromDouble(nan(""));
    }
    double result = ComplexVar_arg(self->num);
    if (*((bool *)closure))
    {
        result *= 180;
        result /= std::numbers::pi;
    }
    return PyFloat_FromDouble(result);
}

int PyComplexVar_set_arg(PyComplexVarObject *self, PyObject *value, void *closure)
{
    if (!value)
    {
        self->num.isArbitrary = true;
        return 0;
    }
    if (self->num.isArbitrary)
    {
        PyErr_SetString(PyExc_Undefined, "This object is still undefined");
        return -1;
    }
    double arg = getdouble_fromPyObject(value);
    if (*((bool *)closure))
    {
        arg *= std::numbers::pi;
        arg /= 180;
    }
    setvalue_frompolar(ComplexVar_length(self->num), arg, self->num);
    return 0;
}

PyObject *PyComplexVar_get_recpair(PyComplexVarObject *self, void *closure)
{
    if (self->num.isArbitrary)
    {
        PyErr_SetString(PyExc_Undefined, "This object is still undefined");
        Py_RETURN_NONE;
    }
    return Py_BuildValue("dd", self->num.real, self->num.imag);
}

int PyComplexVar_set_recpair(PyComplexVarObject *self, PyObject *value, void *closure)
{
    if (!value)
    {
        self->num.isArbitrary = true;
        return 0;
    }
    if (PyTuple_CheckExact(value))
    {
        if (PyArg_ParseTuple(value, "dd|", &self->num.real, &self->num.imag))
        {
            self->num.isArbitrary = false;
            return 0;
        }
    }
    PyErr_SetString(PyExc_ValueError, "Only take tuple with two float elements.");
    return -1;
}

PyObject *PyComplexVar_get_polarpair(PyComplexVarObject *self, void *closure)
{
    if (self->num.isArbitrary)
    {
        PyErr_SetString(PyExc_Undefined, "This object is still undefined");
        Py_RETURN_NONE;
    }
    double arg = ComplexVar_arg(self->num);
    if (*((bool *)closure))
    {
        arg *= 180;
        arg /= std::numbers::pi;
    }
    return Py_BuildValue("dd", ComplexVar_length(self->num), arg);
}

int PyComplexVar_set_polarpair(PyComplexVarObject *self, PyObject *value, void *closure)
{
    if (!value)
    {
        self->num.isArbitrary = true;
        return 0;
    }
    if (PyTuple_CheckExact(value))
    {
        double r;
        double a;
        if (PyArg_ParseTuple(value, "dd|", &r, &a))
        {
            if (*((bool *)closure))
            {
                a *= std::numbers::pi;
                a /= 180;
            }
            setvalue_frompolar(r, a, self->num);
            return 0;
        }
    }
    PyErr_SetString(PyExc_ValueError, "Only take tuple with two float elements.");
    return -1;
}

static PyNumberMethods PyComplexVarNumber = {
    .nb_add = (binaryfunc)PyComplexVar_add,
};

static PyMemberDef PyComplexVarMember[] = {
    {"real", T_DOUBLE, offsetof(PyComplexVarObject, num.real), 0, nullptr},
    {"imag", T_DOUBLE, offsetof(PyComplexVarObject, num.imag), 0, nullptr},
    {"is_arbitrary", T_BOOL, offsetof(PyComplexVarObject, num.isArbitrary), 0, nullptr},
    nullptr,
};

static PyGetSetDef PyComplexVarGetSet[] = {
    {"length", (getter)PyComplexVar_get_len, (setter)PyComplexVar_set_len, nullptr, nullptr},
    {"arg", (getter)PyComplexVar_get_arg, (setter)PyComplexVar_set_arg, nullptr, &isdeg},
    {"rec", (getter)PyComplexVar_get_recpair, (setter)PyComplexVar_set_recpair, nullptr, nullptr},
    {"pol", (getter)PyComplexVar_get_polarpair, (setter)PyComplexVar_set_polarpair, nullptr, &isdeg},
    nullptr,
};

PyTypeObject PyComplexVarType = {
    .ob_base = PyVarObject_HEAD_INIT(&PyType_Type, 0).tp_name = "varcore.complexvar",
    .tp_basicsize = sizeof(PyComplexVarObject),
    .tp_dealloc = (destructor)PyComplexVar_dealloc,
    .tp_repr = (reprfunc)PyComplexVar_repr,
    .tp_as_number = &PyComplexVarNumber,
    .tp_str = (reprfunc)PyComplexVar_str,
    .tp_richcompare = (richcmpfunc)PyComplexVar_richcompare,
    .tp_members = PyComplexVarMember,
    .tp_getset = PyComplexVarGetSet,
    .tp_init = (initproc)PyComplexVar_init,
    .tp_new = (newfunc)PyComplexVar_new,
};