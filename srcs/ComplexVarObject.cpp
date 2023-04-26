#include <complexvar.h>

extern int doubleprecision;
bool isdeg = false;
extern PyObject *PyExc_Undefined;

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
    if (!value)
    {
        target.isArbitrary = true;
        goto set_both_zero;
    }
    if (PyComplexVar_CheckExact(value))
    {
        target = ((PyComplexVarObject *)value)->num;
        return 0;
    }
    if (Py_IsUnsure(value))
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
    return -1;
set_both_zero:
    target.real = 0;
set_imag_zero:
    target.imag = 0;
    return 0;
}

int assignComplexVar_withExc(PyObject *value, ComplexVar &target)
{
    if (assignComplexVar(value, target))
    {
        PyErr_Format(PyExc_TypeError, "Unsupported type: %s", value->ob_type->tp_name);
        return -1;
    }
    return 0;
}

void PyComplexVar_dealloc(PyComplexVarObject *self)
{
    Py_TYPE(self)->tp_free((PyObject *)self);
    return;
}

PyObject *PyComplexVar_repr(PyComplexVarObject *self)
{
    std::stringstream repr;
    repr << std::setprecision(doubleprecision) << (self->num);
    return PyUnicode_FromString(repr.str().c_str());
}

PyObject *PyComplexVar_str(PyComplexVarObject *self)
{
    std::stringstream repr;
    repr << std::setprecision(doubleprecision) < (self->num);
    return PyUnicode_FromString(repr.str().c_str());
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
            Py_RETURN_UNSURE;
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
            Py_RETURN_UNSURE;
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
        if (assignComplexVar_withExc(tmp, self->num))
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
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    return self;
}

// as number

PyObject *PyComplexVar_add(PyObject *self, PyObject *other)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    if (PyComplexVar_Check(self))
    {
        if (PyComplexVar_Check(other))
        {
            result->num = ComplexVar_add(((PyComplexVarObject *)self)->num, ((PyComplexVarObject *)other)->num);
        }
        else
        {
            ComplexVar tmp;
            if (assignComplexVar(other, tmp))
            {
                Py_DECREF(result);
                Py_RETURN_NOTIMPLEMENTED;
            }
            result->num = ComplexVar_add(((PyComplexVarObject *)self)->num, tmp);
        }
    }
    else
    {
        ComplexVar tmp;
        if (assignComplexVar(self, tmp))
        {
            Py_DECREF(result);
            Py_RETURN_NOTIMPLEMENTED;
        }
        result->num = ComplexVar_add(tmp, ((PyComplexVarObject *)other)->num);
    }
    return (PyObject *)result;
}

PyObject *PyComplexVar_subtract(PyObject *self, PyObject *other)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    if (PyComplexVar_Check(self))
    {
        if (PyComplexVar_Check(other))
        {
            result->num = ComplexVar_sub(((PyComplexVarObject *)self)->num, ((PyComplexVarObject *)other)->num);
        }
        else
        {
            ComplexVar tmp;
            if (assignComplexVar(other, tmp))
            {
                Py_DECREF(result);
                Py_RETURN_NOTIMPLEMENTED;
            }
            result->num = ComplexVar_sub(((PyComplexVarObject *)self)->num, tmp);
        }
    }
    else
    {
        ComplexVar tmp;
        if (assignComplexVar(self, tmp))
        {
            Py_DECREF(result);
            Py_RETURN_NOTIMPLEMENTED;
        }
        result->num = ComplexVar_sub(tmp, ((PyComplexVarObject *)other)->num);
    }
    return (PyObject *)result;
}

PyObject *PyComplexVar_multiply(PyObject *self, PyObject *other)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    if (PyComplexVar_Check(self))
    {
        if (PyComplexVar_Check(other))
        {
            result->num = ComplexVar_mul(((PyComplexVarObject *)self)->num, ((PyComplexVarObject *)other)->num);
        }
        else
        {
            ComplexVar tmp;
            if (assignComplexVar(other, tmp))
            {
                Py_DECREF(result);
                Py_RETURN_NOTIMPLEMENTED;
            }
            result->num = ComplexVar_mul(((PyComplexVarObject *)self)->num, tmp);
        }
    }
    else
    {
        ComplexVar tmp;
        if (assignComplexVar(self, tmp))
        {
            Py_DECREF(result);
            Py_RETURN_NOTIMPLEMENTED;
        }
        result->num = ComplexVar_mul(tmp, ((PyComplexVarObject *)other)->num);
    }
    return (PyObject *)result;
}

PyObject *PyComplexVar_remainder(PyObject *self, PyObject *other)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    if (PyComplexVar_Check(self))
    {
        if (PyComplexVar_Check(other))
        {
            result->num = ComplexVar_mod(((PyComplexVarObject *)self)->num, ((PyComplexVarObject *)other)->num);
        }
        else
        {
            ComplexVar tmp;
            if (assignComplexVar(other, tmp))
            {
                Py_DECREF(result);
                Py_RETURN_NOTIMPLEMENTED;
            }
            result->num = ComplexVar_mod(((PyComplexVarObject *)self)->num, tmp);
        }
    }
    else
    {
        ComplexVar tmp;
        if (assignComplexVar(self, tmp))
        {
            Py_DECREF(result);
            Py_RETURN_NOTIMPLEMENTED;
        }
        result->num = ComplexVar_mod(tmp, ((PyComplexVarObject *)other)->num);
    }
    return (PyObject *)result;
}

PyObject *PyComplexVar_divmod(PyObject *self, PyObject *other)
{
    PyComplexVarObject *D = nullptr;
    D = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    PyComplexVarObject *M = nullptr;
    M = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!D || !M)
    {
        Py_XDECREF(D);
        Py_XDECREF(M);
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    if (PyComplexVar_Check(self))
    {
        if (PyComplexVar_Check(other))
        {
            D->num = ComplexVar_fdv(((PyComplexVarObject *)self)->num, ((PyComplexVarObject *)other)->num);
            M->num = ComplexVar_sub(((PyComplexVarObject *)self)->num, ComplexVar_mul(D->num, ((PyComplexVarObject *)other)->num));
        }
        else
        {
            ComplexVar tmp;
            if (assignComplexVar(other, tmp))
            {
                Py_DECREF(D);
                Py_DECREF(M);
                Py_RETURN_NOTIMPLEMENTED;
            }
            D->num = ComplexVar_fdv(((PyComplexVarObject *)self)->num, tmp);
            M->num = ComplexVar_sub(((PyComplexVarObject *)self)->num, ComplexVar_mul(D->num, tmp));
        }
    }
    else
    {
        ComplexVar tmp;
        if (assignComplexVar(self, tmp))
        {
            Py_DECREF(D);
            Py_DECREF(M);
            Py_RETURN_NOTIMPLEMENTED;
        }
        D->num = ComplexVar_fdv(tmp, ((PyComplexVarObject *)other)->num);
        M->num = ComplexVar_sub(tmp, ComplexVar_mul(D->num, ((PyComplexVarObject *)other)->num));
    }
    PyObject *returnvalue = Py_BuildValue("OO", D, M);
    Py_DECREF(D);
    Py_DECREF(M);
    return returnvalue;
}

PyObject *PyComplexVar_power(PyObject *self, PyObject *other, PyObject *mod)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    ComplexVar powvalue;
    bool otherIsComplexVar = PyComplexVar_Check(other);
    if (PyComplexVar_Check(self))
    {
        if (otherIsComplexVar)
        {
            powvalue = ComplexVar_pow(((PyComplexVarObject *)self)->num, ((PyComplexVarObject *)other)->num);
        }
        else
        {
            ComplexVar tmp;
            if (assignComplexVar(other, tmp))
            {
                Py_DECREF(result);
                Py_RETURN_NOTIMPLEMENTED;
            }
            powvalue = ComplexVar_pow(((PyComplexVarObject *)self)->num, tmp);
        }
    }
    else
    {
        if (otherIsComplexVar)
        {
            ComplexVar tmp;
            if (assignComplexVar(self, tmp))
            {
                Py_DECREF(result);
                Py_RETURN_NOTIMPLEMENTED;
            }
            powvalue = ComplexVar_pow(tmp, ((PyComplexVarObject *)other)->num);
        }
        else
        {
            ComplexVar tmpx;
            ComplexVar tmpy;
            if (assignComplexVar(self, tmpx) || assignComplexVar(other, tmpy))
            {
                Py_DECREF(result);
                Py_RETURN_NOTIMPLEMENTED;
            }
            powvalue = ComplexVar_pow(tmpx, tmpy);
        }
    }
    if (Py_IsNone(mod))
    {
        result->num = powvalue;
    }
    else if (PyComplexVar_Check(mod))
    {
        result->num = ComplexVar_mod(powvalue, ((PyComplexVarObject *)mod)->num);
    }
    else
    {
        ComplexVar tmp;
        if (assignComplexVar(mod, tmp))
        {
            Py_DECREF(result);
            Py_RETURN_NOTIMPLEMENTED;
        }
        result->num = ComplexVar_mod(powvalue, tmp);
    }
    return (PyObject *)result;
}

PyObject *PyComplexVar_negative(PyObject *self)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_neg(((PyComplexVarObject *)self)->num);
    return (PyObject *)result;
}

PyObject *PyComplexVar_positive(PyObject *self)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ((PyComplexVarObject *)self)->num;
    return (PyObject *)result;
}

PyObject *PyComplexVar_absolute(PyObject *self)
{
    if (((PyComplexVarObject *)self)->num.isArbitrary)
    {
        return PyFloat_FromDouble(std::nan(""));
    }
    return PyFloat_FromDouble(ComplexVar_L2(((PyComplexVarObject *)self)->num).value);
}

int PyComplexVar_bool(PyObject *self)
{
    if (((PyComplexVarObject *)self)->num.isArbitrary)
    {
        return 1;
    }
    if (ComplexVar_iszero(((PyComplexVarObject *)self)->num))
    {
        return 0;
    }
    return 1;
}

PyObject *PyComplexVar_invert(PyObject *self)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        return nullptr;
    }
    result->num = ComplexVar_ivt(((PyComplexVarObject *)self)->num);
    return (PyObject *)result;
}

PyObject *PyComplexVar_inplace_add(PyComplexVarObject *self, PyObject *other)
{
    ComplexVar tmp;
    if (assignComplexVar(other, tmp))
    {
        Py_RETURN_NOTIMPLEMENTED;
    }
    self->num = ComplexVar_add(self->num, tmp);
    Py_INCREF(self);
    return (PyObject *)self;
}

PyObject *PyComplexVar_inplace_subtract(PyComplexVarObject *self, PyObject *other)
{
    ComplexVar tmp;
    if (assignComplexVar(other, tmp))
    {
        Py_RETURN_NOTIMPLEMENTED;
    }
    self->num = ComplexVar_sub(self->num, tmp);
    Py_INCREF(self);
    return (PyObject *)self;
}

PyObject *PyComplexVar_inplace_multiply(PyComplexVarObject *self, PyObject *other)
{
    ComplexVar tmp;
    if (assignComplexVar(other, tmp))
    {
        Py_RETURN_NOTIMPLEMENTED;
    }
    self->num = ComplexVar_mul(self->num, tmp);
    Py_INCREF(self);
    return (PyObject *)self;
}

PyObject *PyComplexVar_inplace_remainder(PyComplexVarObject *self, PyObject *other)
{
    ComplexVar tmp;
    if (assignComplexVar(other, tmp))
    {
        Py_RETURN_NOTIMPLEMENTED;
    }
    self->num = ComplexVar_mod(self->num, tmp);
    Py_INCREF(self);
    return (PyObject *)self;
}

PyObject *PyComplexVar_inplace_power(PyComplexVarObject *self, PyObject *other, PyObject *mod)
{
    ComplexVar tmp;
    ComplexVar powvalue;
    if (assignComplexVar(other, tmp))
    {
        Py_RETURN_NOTIMPLEMENTED;
    }
    powvalue = ComplexVar_pow(self->num, tmp);
    if (Py_IsNone(mod))
    {
        self->num = powvalue;
    }
    else
    {
        ComplexVar tmp2;
        if (assignComplexVar(mod, tmp2))
        {
            Py_RETURN_NOTIMPLEMENTED;
        }
        self->num = ComplexVar_mod(powvalue, tmp2);
    }
    Py_INCREF(self);
    return (PyObject *)self;
}

PyObject *PyComplexVar_floor_divide(PyObject *self, PyObject *other)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    if (PyComplexVar_Check(self))
    {
        if (PyComplexVar_Check(other))
        {
            result->num = ComplexVar_fdv(((PyComplexVarObject *)self)->num, ((PyComplexVarObject *)other)->num);
        }
        else
        {
            ComplexVar tmp;
            if (assignComplexVar(other, tmp))
            {
                Py_DECREF(result);
                Py_RETURN_NOTIMPLEMENTED;
            }
            result->num = ComplexVar_fdv(((PyComplexVarObject *)self)->num, tmp);
        }
    }
    else
    {
        ComplexVar tmp;
        if (assignComplexVar(self, tmp))
        {
            Py_DECREF(result);
            Py_RETURN_NOTIMPLEMENTED;
        }
        result->num = ComplexVar_fdv(tmp, ((PyComplexVarObject *)other)->num);
    }
    return (PyObject *)result;
}

PyObject *PyComplexVar_true_divide(PyObject *self, PyObject *other)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    if (PyComplexVar_Check(self))
    {
        if (PyComplexVar_Check(other))
        {
            result->num = ComplexVar_div(((PyComplexVarObject *)self)->num, ((PyComplexVarObject *)other)->num);
        }
        else
        {
            ComplexVar tmp;
            if (assignComplexVar(other, tmp))
            {
                Py_DECREF(result);
                Py_RETURN_NOTIMPLEMENTED;
            }
            result->num = ComplexVar_div(((PyComplexVarObject *)self)->num, tmp);
        }
    }
    else
    {
        ComplexVar tmp;
        if (assignComplexVar(self, tmp))
        {
            Py_DECREF(result);
            Py_RETURN_NOTIMPLEMENTED;
        }
        result->num = ComplexVar_div(tmp, ((PyComplexVarObject *)other)->num);
    }
    return (PyObject *)result;
}

PyObject *PyComplexVar_inplace_floor_divide(PyComplexVarObject *self, PyObject *other)
{
    ComplexVar tmp;
    if (assignComplexVar(other, tmp))
    {
        Py_RETURN_NOTIMPLEMENTED;
    }
    self->num = ComplexVar_fdv(self->num, tmp);
    Py_INCREF(self);
    return (PyObject *)self;
}

PyObject *PyComplexVar_inplace_true_divide(PyComplexVarObject *self, PyObject *other)
{
    ComplexVar tmp;
    if (assignComplexVar(other, tmp))
    {
        Py_RETURN_NOTIMPLEMENTED;
    }
    self->num = ComplexVar_div(self->num, tmp);
    Py_INCREF(self);
    return (PyObject *)self;
}

// methods

PyObject *PyComplexVar_conj(PyComplexVarObject *self)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_conj(self->num);
    return (PyObject *)result;
}

PyObject *PyComplexVar_exp(PyComplexVarObject *self, PyObject *args)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {

        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_exp(self->num);
    return (PyObject *)result;
}

PyObject *PyComplexVar_ln(PyComplexVarObject *self, PyObject *args)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_ln(self->num);
    return (PyObject *)result;
}

PyObject *PyComplexVar_log(PyComplexVarObject *self, PyObject *base)
{
    ComplexVar tmp;
    if (assignComplexVar(base, tmp))
    {
        Py_RETURN_NOTIMPLEMENTED;
    }
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_log(self->num, tmp);
    return (PyObject *)result;
}

PyObject *PyComplexVar_logasbase(PyComplexVarObject *self, PyObject *neur)
{
    ComplexVar tmp;
    if (assignComplexVar(neur, tmp))
    {
        Py_RETURN_NOTIMPLEMENTED;
    }
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {

        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_log(tmp, self->num);
    return (PyObject *)result;
}

PyObject *PyComplexVar_sqrt(PyComplexVarObject *self)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {

        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_sqrt(self->num);
    return (PyObject *)result;
}

PyObject *PyComplexVar_sin(PyComplexVarObject *self)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_sin(self->num);
    return (PyObject *)result;
}

PyObject *PyComplexVar_cos(PyComplexVarObject *self)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_cos(self->num);
    return (PyObject *)result;
}

PyObject *PyComplexVar_tan(PyComplexVarObject *self)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_tan(self->num);
    return (PyObject *)result;
}

PyObject *PyComplexVar_cot(PyComplexVarObject *self)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_cot(self->num);
    return (PyObject *)result;
}

PyObject *PyComplexVar_sec(PyComplexVarObject *self)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_sec(self->num);
    return (PyObject *)result;
}

PyObject *PyComplexVar_csc(PyComplexVarObject *self)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_csc(self->num);
    return (PyObject *)result;
}

PyObject *PyComplexVar_arcsin(PyComplexVarObject *self)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_arcsin(self->num);
    return (PyObject *)result;
}

PyObject *PyComplexVar_arccos(PyComplexVarObject *self)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_arccos(self->num);
    return (PyObject *)result;
}

PyObject *PyComplexVar_arctan(PyComplexVarObject *self)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_arctan(self->num);
    return (PyObject *)result;
}

PyObject *PyComplexVar_arccot(PyComplexVarObject *self)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_arccot(self->num);
    return (PyObject *)result;
}

PyObject *PyComplexVar_arcsec(PyComplexVarObject *self)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_arcsec(self->num);
    return (PyObject *)result;
}

PyObject *PyComplexVar_arccsc(PyComplexVarObject *self)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_arccsc(self->num);
    return (PyObject *)result;
}

PyObject *PyComplexVar_sinh(PyComplexVarObject *self)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_sinh(self->num);
    return (PyObject *)result;
}

PyObject *PyComplexVar_cosh(PyComplexVarObject *self)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_cosh(self->num);
    return (PyObject *)result;
}

PyObject *PyComplexVar_tanh(PyComplexVarObject *self)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_tanh(self->num);
    return (PyObject *)result;
}

PyObject *PyComplexVar_coth(PyComplexVarObject *self)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_coth(self->num);
    return (PyObject *)result;
}

PyObject *PyComplexVar_sech(PyComplexVarObject *self)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_sech(self->num);
    return (PyObject *)result;
}

PyObject *PyComplexVar_csch(PyComplexVarObject *self)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_csch(self->num);
    return (PyObject *)result;
}

PyObject *PyComplexVar_arcsinh(PyComplexVarObject *self)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_arcsinh(self->num);
    return (PyObject *)result;
}

PyObject *PyComplexVar_arccosh(PyComplexVarObject *self)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_arccosh(self->num);
    return (PyObject *)result;
}

PyObject *PyComplexVar_arctanh(PyComplexVarObject *self)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_arctanh(self->num);
    return (PyObject *)result;
}

PyObject *PyComplexVar_arccoth(PyComplexVarObject *self)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_arccoth(self->num);
    return (PyObject *)result;
}

PyObject *PyComplexVar_arcsech(PyComplexVarObject *self)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_arcsech(self->num);
    return (PyObject *)result;
}

PyObject *PyComplexVar_arccsch(PyComplexVarObject *self)
{
    PyComplexVarObject *result = PyObject_New(PyComplexVarObject, &PyComplexVarType);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_arccsch(self->num);
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
    return PyFloat_FromDouble(ComplexVar_L2(self->num).value);
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
    double result = ComplexVar_arg(self->num).value;
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
    error_double arg = {getdouble_fromPyObject(value)};
    if (*((bool *)closure))
    {
        arg *= std::numbers::pi;
        arg /= 180;
    }
    setvalue_frompolar(ComplexVar_L2(self->num), arg, self->num);
    return 0;
}

PyObject *PyComplexVar_get_recpair(PyComplexVarObject *self, void *closure)
{
    if (self->num.isArbitrary)
    {
        PyErr_SetString(PyExc_Undefined, "This object is still undefined");
        Py_RETURN_NONE;
    }
    return Py_BuildValue("dd", self->num.real.value, self->num.imag.value);
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
    double arg = ComplexVar_arg(self->num).value;
    if (*((bool *)closure))
    {
        arg *= 180;
        arg /= std::numbers::pi;
    }
    return Py_BuildValue("dd", ComplexVar_L2(self->num).value, arg);
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
    .nb_subtract = (binaryfunc)PyComplexVar_subtract,
    .nb_multiply = (binaryfunc)PyComplexVar_multiply,
    .nb_remainder = (binaryfunc)PyComplexVar_remainder,
    .nb_divmod = (binaryfunc)PyComplexVar_divmod,
    .nb_power = (ternaryfunc)PyComplexVar_power,
    .nb_negative = (unaryfunc)PyComplexVar_negative,
    .nb_positive = (unaryfunc)PyComplexVar_positive,
    .nb_absolute = (unaryfunc)PyComplexVar_absolute,
    .nb_bool = (inquiry)PyComplexVar_bool,
    .nb_invert = (unaryfunc)PyComplexVar_invert,
    .nb_inplace_add = (binaryfunc)PyComplexVar_inplace_add,
    .nb_inplace_subtract = (binaryfunc)PyComplexVar_inplace_subtract,
    .nb_inplace_multiply = (binaryfunc)PyComplexVar_inplace_multiply,
    .nb_inplace_remainder = (binaryfunc)PyComplexVar_inplace_remainder,
    .nb_inplace_power = (ternaryfunc)PyComplexVar_inplace_power,
    .nb_floor_divide = (binaryfunc)PyComplexVar_floor_divide,
    .nb_true_divide = (binaryfunc)PyComplexVar_true_divide,
    .nb_inplace_floor_divide = (binaryfunc)PyComplexVar_inplace_floor_divide,
    .nb_inplace_true_divide = (binaryfunc)PyComplexVar_inplace_true_divide,
};

static PyMethodDef PyComplexVarMethod[] = {
    {"conj", (PyCFunction)PyComplexVar_conj, METH_NOARGS, nullptr},
    {"exp", (PyCFunction)PyComplexVar_exp, METH_NOARGS, nullptr},
    {"ln", (PyCFunction)PyComplexVar_ln, METH_NOARGS, nullptr},
    {"log", (PyCFunction)PyComplexVar_log, METH_O, nullptr},
    {"log_asbase", (PyCFunction)PyComplexVar_logasbase, METH_O, nullptr},
    {"sqrt", (PyCFunction)PyComplexVar_sqrt, METH_NOARGS, nullptr},
    {"sin", (PyCFunction)PyComplexVar_sin, METH_NOARGS, nullptr},
    {"cos", (PyCFunction)PyComplexVar_cos, METH_NOARGS, nullptr},
    {"tan", (PyCFunction)PyComplexVar_tan, METH_NOARGS, nullptr},
    {"cot", (PyCFunction)PyComplexVar_cot, METH_NOARGS, nullptr},
    {"sec", (PyCFunction)PyComplexVar_sec, METH_NOARGS, nullptr},
    {"csc", (PyCFunction)PyComplexVar_csc, METH_NOARGS, nullptr},
    {"arcsin", (PyCFunction)PyComplexVar_arcsin, METH_NOARGS, nullptr},
    {"arccos", (PyCFunction)PyComplexVar_arccos, METH_NOARGS, nullptr},
    {"arctan", (PyCFunction)PyComplexVar_arctan, METH_NOARGS, nullptr},
    {"arccot", (PyCFunction)PyComplexVar_arccot, METH_NOARGS, nullptr},
    {"arcsec", (PyCFunction)PyComplexVar_arcsec, METH_NOARGS, nullptr},
    {"arccsc", (PyCFunction)PyComplexVar_arccsc, METH_NOARGS, nullptr},
    {"sinh", (PyCFunction)PyComplexVar_sinh, METH_NOARGS, nullptr},
    {"cosh", (PyCFunction)PyComplexVar_cosh, METH_NOARGS, nullptr},
    {"tanh", (PyCFunction)PyComplexVar_tanh, METH_NOARGS, nullptr},
    {"coth", (PyCFunction)PyComplexVar_coth, METH_NOARGS, nullptr},
    {"sech", (PyCFunction)PyComplexVar_sech, METH_NOARGS, nullptr},
    {"csch", (PyCFunction)PyComplexVar_csch, METH_NOARGS, nullptr},
    {"arcsinh", (PyCFunction)PyComplexVar_arcsinh, METH_NOARGS, nullptr},
    {"arccosh", (PyCFunction)PyComplexVar_arccosh, METH_NOARGS, nullptr},
    {"arctanh", (PyCFunction)PyComplexVar_arctanh, METH_NOARGS, nullptr},
    {"arccoth", (PyCFunction)PyComplexVar_arccoth, METH_NOARGS, nullptr},
    {"arcsech", (PyCFunction)PyComplexVar_arcsech, METH_NOARGS, nullptr},
    {"arccsch", (PyCFunction)PyComplexVar_arccsch, METH_NOARGS, nullptr},
    nullptr,
};

static PyMemberDef PyComplexVarMember[] = {
    {"real", T_DOUBLE, offsetof(PyComplexVarObject, num.real.value), 0, nullptr},
    {"real_error", T_DOUBLE, offsetof(PyComplexVarObject, num.real.error), 0, nullptr},
    {"imag", T_DOUBLE, offsetof(PyComplexVarObject, num.imag.value), 0, nullptr},
    {"imag_error", T_DOUBLE, offsetof(PyComplexVarObject, num.imag.error), 0, nullptr},
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
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_richcompare = (richcmpfunc)PyComplexVar_richcompare,
    .tp_methods = PyComplexVarMethod,
    .tp_members = PyComplexVarMember,
    .tp_getset = PyComplexVarGetSet,
    .tp_init = (initproc)PyComplexVar_init,
    .tp_new = (newfunc)PyComplexVar_new,
};