#include <complexvar.h>

extern int doubleprecision;
extern bool isdeg;
extern PyObject *PyExc_UndefinedWarning;

int assignComplexVar(PyObject *value, ComplexVar &target)
{
    if (!value)
    {
        target.isArbitrary = true;
        return 0;
    }
    if (PyComplexVar_CheckExact(value))
    {
        target = ((PyComplexVarObject *)value)->num;
        return 0;
    }
    if (Py_IsUnsure(value))
    {
        target.isArbitrary = true;
        target.real = 0;
        target.imag = 0;
        return 0;
    }
    target.isArbitrary = false;
    if (PyComplex_CheckExact(value))
    {
        target.real = PyComplex_RealAsDouble(value);
        target.imag = PyComplex_ImagAsDouble(value);
        return 0;
    }
    try
    {
        target.real = error_double(value);
        target.imag = 0;
        target.isArbitrary = false;
    }
    catch (std::exception &e)
    {
        return -1;
    }
    return 0;
}

int assignComplexVar_withExc(PyObject *value, ComplexVar &target)
{
    if (assignComplexVar(value, target))
    {
        PyErr_Format(PyExc_TypeError, "Unsupported type: %s\n", value->ob_type->tp_name);
        return -1;
    }
    return 0;
}

static void PyComplexVar_dealloc(PyComplexVarObject *self)
{
    Py_TYPE(self)->tp_free((PyObject *)self);
    return;
}

static PyObject *PyComplexVar_repr(PyComplexVarObject *self)
{
    std::stringstream repr;
    repr << std::setprecision(doubleprecision) << (self->num);
    return PyUnicode_FromString(repr.str().c_str());
}

static PyObject *PyComplexVar_str(PyComplexVarObject *self)
{
    std::stringstream repr;
    repr << std::setprecision(doubleprecision) < (self->num);
    return PyUnicode_FromString(repr.str().c_str());
}

static PyObject *PyComplexVar_richcompare(PyComplexVarObject *self, PyObject *other, int opid)
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

static int PyComplexVar_init(PyComplexVarObject *self, PyObject *args, PyObject *kwds)
{
    static char *kwlist0[] = {
        (char *)"num",
        nullptr,
    };
    static char *kwlist1[] = {
        (char *)"real",
        (char *)"imag",
        nullptr,
    };
    static char *kwlist2[] = {
        (char *)"r",
        (char *)"arg",
        nullptr,
    };
    PyObject *tmp = nullptr;
    double x = 0;
    double y = 0;
    if (PyArg_ParseTupleAndKeywords(args, kwds, "|O", kwlist0, &tmp))
    {
        if (assignComplexVar_withExc(tmp, self->num))
        {
            return -1;
        }
        return 0;
    }
    PyErr_Clear();
    if (PyArg_ParseTupleAndKeywords(args, kwds, "dd|", kwlist1, &x, &y))
    {
        self->num.real = x;
        self->num.imag = y;
        return 0;
    }
    PyErr_Clear();
    if (PyArg_ParseTupleAndKeywords(args, kwds, "dd|", kwlist2, &x, &y))
    {
        if (isdeg)
        {
            y /= 180;
            y *= std::numbers::pi;
        }
        setvalue_frompolar(x, y, self->num);
        self->num.isArbitrary = false;
        return 0;
    }
    PyErr_SetString(PyExc_TypeError, "Fail to match any overload");
    return -1;
}

static PyObject *PyComplexVar_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
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

static PyObject *PyComplexVar_add(PyObject *self, PyObject *other)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
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

static PyObject *PyComplexVar_subtract(PyObject *self, PyObject *other)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
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

static PyObject *PyComplexVar_multiply(PyObject *self, PyObject *other)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
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

static PyObject *PyComplexVar_remainder(PyObject *self, PyObject *other)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
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

static PyObject *PyComplexVar_divmod(PyObject *self, PyObject *other)
{
    PyComplexVarObject *D = nullptr;
    D = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    PyComplexVarObject *M = nullptr;
    M = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
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

static PyObject *PyComplexVar_power(PyObject *self, PyObject *other, PyObject *mod)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
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

static PyObject *PyComplexVar_negative(PyObject *self)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_neg(((PyComplexVarObject *)self)->num);
    return (PyObject *)result;
}

static PyObject *PyComplexVar_positive(PyObject *self)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ((PyComplexVarObject *)self)->num;
    return (PyObject *)result;
}

static PyObject *PyComplexVar_absolute(PyObject *self)
{
    if (((PyComplexVarObject *)self)->num.isArbitrary)
    {
        return PyFloat_FromDouble(std::nan(""));
    }
    return PyFloat_FromDouble(ComplexVar_L2(((PyComplexVarObject *)self)->num).value);
}

static int PyComplexVar_bool(PyObject *self)
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

static PyObject *PyComplexVar_invert(PyObject *self)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    if (!result)
    {
        return nullptr;
    }
    result->num = ComplexVar_ivt(((PyComplexVarObject *)self)->num);
    return (PyObject *)result;
}

static PyObject *PyComplexVar_inplace_add(PyComplexVarObject *self, PyObject *other)
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

static PyObject *PyComplexVar_inplace_subtract(PyComplexVarObject *self, PyObject *other)
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

static PyObject *PyComplexVar_inplace_multiply(PyComplexVarObject *self, PyObject *other)
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

static PyObject *PyComplexVar_inplace_remainder(PyComplexVarObject *self, PyObject *other)
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

static PyObject *PyComplexVar_inplace_power(PyComplexVarObject *self, PyObject *other, PyObject *mod)
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

static PyObject *PyComplexVar_floor_divide(PyObject *self, PyObject *other)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
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

static PyObject *PyComplexVar_true_divide(PyObject *self, PyObject *other)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
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

static PyObject *PyComplexVar_inplace_floor_divide(PyComplexVarObject *self, PyObject *other)
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

static PyObject *PyComplexVar_inplace_true_divide(PyComplexVarObject *self, PyObject *other)
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

static PyObject *PyComplexVar_conj(PyComplexVarObject *self, PyObject *args)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_conj(self->num);
    return (PyObject *)result;
}

static PyObject *PyComplexVar_iconj(PyComplexVarObject *self, PyObject *args)
{
    ComplexVar_iconj(self->num);
    Py_INCREF(self);
    return (PyObject *)self;
}

static PyObject *PyComplexVar_exp(PyComplexVarObject *self, PyObject *args)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_exp(self->num);
    return (PyObject *)result;
}

static PyObject *PyComplexVar_ln(PyComplexVarObject *self, PyObject *args)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_ln(self->num);
    return (PyObject *)result;
}

static PyObject *PyComplexVar_sqrt(PyComplexVarObject *self)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_sqrt(self->num);
    return (PyObject *)result;
}

static PyObject *PyComplexVar_sin(PyComplexVarObject *self)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_sin(self->num);
    return (PyObject *)result;
}

static PyObject *PyComplexVar_cos(PyComplexVarObject *self)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_cos(self->num);
    return (PyObject *)result;
}

static PyObject *PyComplexVar_tan(PyComplexVarObject *self)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_tan(self->num);
    return (PyObject *)result;
}

static PyObject *PyComplexVar_cot(PyComplexVarObject *self)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_cot(self->num);
    return (PyObject *)result;
}

static PyObject *PyComplexVar_sec(PyComplexVarObject *self)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_sec(self->num);
    return (PyObject *)result;
}

static PyObject *PyComplexVar_csc(PyComplexVarObject *self)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_csc(self->num);
    return (PyObject *)result;
}

static PyObject *PyComplexVar_arcsin(PyComplexVarObject *self)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_arcsin(self->num);
    return (PyObject *)result;
}

static PyObject *PyComplexVar_arccos(PyComplexVarObject *self)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_arccos(self->num);
    return (PyObject *)result;
}

static PyObject *PyComplexVar_arctan(PyComplexVarObject *self)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_arctan(self->num);
    return (PyObject *)result;
}

static PyObject *PyComplexVar_arccot(PyComplexVarObject *self)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_arccot(self->num);
    return (PyObject *)result;
}

static PyObject *PyComplexVar_arcsec(PyComplexVarObject *self)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_arcsec(self->num);
    return (PyObject *)result;
}

static PyObject *PyComplexVar_arccsc(PyComplexVarObject *self)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_arccsc(self->num);
    return (PyObject *)result;
}

static PyObject *PyComplexVar_sinh(PyComplexVarObject *self)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_sinh(self->num);
    return (PyObject *)result;
}

static PyObject *PyComplexVar_cosh(PyComplexVarObject *self)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_cosh(self->num);
    return (PyObject *)result;
}

static PyObject *PyComplexVar_tanh(PyComplexVarObject *self)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_tanh(self->num);
    return (PyObject *)result;
}

static PyObject *PyComplexVar_coth(PyComplexVarObject *self)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_coth(self->num);
    return (PyObject *)result;
}

static PyObject *PyComplexVar_sech(PyComplexVarObject *self)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_sech(self->num);
    return (PyObject *)result;
}

static PyObject *PyComplexVar_csch(PyComplexVarObject *self)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_csch(self->num);
    return (PyObject *)result;
}

static PyObject *PyComplexVar_arcsinh(PyComplexVarObject *self)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_arcsinh(self->num);
    return (PyObject *)result;
}

static PyObject *PyComplexVar_arccosh(PyComplexVarObject *self)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_arccosh(self->num);
    return (PyObject *)result;
}

static PyObject *PyComplexVar_arctanh(PyComplexVarObject *self)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_arctanh(self->num);
    return (PyObject *)result;
}

static PyObject *PyComplexVar_arccoth(PyComplexVarObject *self)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_arccoth(self->num);
    return (PyObject *)result;
}

static PyObject *PyComplexVar_arcsech(PyComplexVarObject *self)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_arcsech(self->num);
    return (PyObject *)result;
}

static PyObject *PyComplexVar_arccsch(PyComplexVarObject *self)
{
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_arccsch(self->num);
    return (PyObject *)result;
}

static PyObject *PyComplexVar_log(PyComplexVarObject *self, PyObject *base)
{
    ComplexVar tmp;
    if (assignComplexVar(base, tmp))
    {
        PyErr_SetNone(PyExc_TypeError);
        return nullptr;
    }
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_log(self->num, tmp);
    return (PyObject *)result;
}

static PyObject *PyComplexVar_rlog(PyComplexVarObject *self, PyObject *neur)
{
    ComplexVar tmp;
    if (assignComplexVar(neur, tmp))
    {
        PyErr_SetNone(PyExc_TypeError);
        return nullptr;
    }
    PyComplexVarObject *result = nullptr;
    result = PyObject_New(PyComplexVarObject, &PyComplexVar_Type);
    if (!result)
    {
        PyErr_SetNone(PyExc_MemoryError);
        return nullptr;
    }
    result->num = ComplexVar_log(tmp, self->num);
    return (PyObject *)result;
}

// get set methods

static PyObject *PyComplexVar_get_real(PyComplexVarObject *self, void *closure)
{
    PyErrordoubleObject *real = nullptr;
    real = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    real->num = self->num.real;
    real->parent = &(self->num.real);
    return (PyObject *)real;
}

static int PyComplexVar_set_real(PyComplexVarObject *self, PyObject *value, void *closure)
{
    if (!value)
    {
        self->num.isArbitrary = true;
        self->num.real = 0;
        return 0;
    }
    try
    {
        self->num.real = error_double(value);
    }
    catch (std::exception &e)
    {
        return -1;
    }
    return 0;
}

static PyObject *PyComplexVar_get_imag(PyComplexVarObject *self, void *closure)
{
    PyErrordoubleObject *imag = nullptr;
    imag = PyObject_New(PyErrordoubleObject, &PyErrordouble_Type);
    imag->num = self->num.imag;
    imag->parent = &(self->num.imag);
    return (PyObject *)imag;
}

static int PyComplexVar_set_imag(PyComplexVarObject *self, PyObject *value, void *closure)
{
    if (!value)
    {
        self->num.isArbitrary = true;
        self->num.imag = 0;
        return 0;
    }
    try
    {
        self->num.imag = error_double(value);
    }
    catch (std::exception &e)
    {
        return -1;
    }
    return 0;
}

static PyObject *PyComplexVar_get_len(PyComplexVarObject *self, void *closure)
{
    if (self->num.isArbitrary)
    {
        PyErr_WarnEx(PyExc_UndefinedWarning, "This object is still undefined", 1);
    }
    return PyFloat_FromDouble(ComplexVar_L2(self->num).value);
}

static int PyComplexVar_set_len(PyComplexVarObject *self, PyObject *value, void *closure)
{
    if (!value)
    {
        self->num.isArbitrary = true;
        return 0;
    }
    if (self->num.isArbitrary)
    {
        PyErr_WarnEx(PyExc_UndefinedWarning, "This object is still undefined", 1);
    }
    try
    {
        setvalue_frompolar(error_double(value), ComplexVar_arg(self->num), self->num);
    }
    catch (std::exception &e)
    {
        return -1;
    }
    return 0;
}

static PyObject *PyComplexVar_get_arg(PyComplexVarObject *self, void *closure)
{
    if (self->num.isArbitrary)
    {
        PyErr_WarnEx(PyExc_UndefinedWarning, "This object is still undefined", 1);
    }
    double result = ComplexVar_arg(self->num).value;
    if (*((bool *)closure))
    {
        result *= 180;
        result /= std::numbers::pi;
    }
    return PyFloat_FromDouble(result);
}

static int PyComplexVar_set_arg(PyComplexVarObject *self, PyObject *value, void *closure)
{
    if (!value)
    {
        self->num.isArbitrary = true;
        return 0;
    }
    if (self->num.isArbitrary)
    {
        PyErr_WarnEx(PyExc_UndefinedWarning, "This object is still undefined", 1);
    }
    try
    {
        error_double arg{value};
        if (*((bool *)closure))
        {
            arg *= std::numbers::pi;
            arg /= 180;
        }
        setvalue_frompolar(ComplexVar_L2(self->num), arg, self->num);
    }
    catch (std::exception &e)
    {
        return -1;
    }
    return 0;
}

static PyNumberMethods PyComplexVar_as_number = {
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

static PyMethodDef PyComplexVar_methods[] = {
    {"__conj__", (PyCFunction)PyComplexVar_conj, METH_NOARGS, nullptr},
    {"iconj", (PyCFunction)PyComplexVar_iconj, METH_NOARGS, nullptr},
    {"__exp__", (PyCFunction)PyComplexVar_exp, METH_NOARGS, nullptr},
    {"__ln__", (PyCFunction)PyComplexVar_ln, METH_NOARGS, nullptr},
    {"__sqrt__", (PyCFunction)PyComplexVar_sqrt, METH_NOARGS, nullptr},
    {"__sin__", (PyCFunction)PyComplexVar_sin, METH_NOARGS, nullptr},
    {"__cos__", (PyCFunction)PyComplexVar_cos, METH_NOARGS, nullptr},
    {"__tan__", (PyCFunction)PyComplexVar_tan, METH_NOARGS, nullptr},
    {"__cot__", (PyCFunction)PyComplexVar_cot, METH_NOARGS, nullptr},
    {"__sec__", (PyCFunction)PyComplexVar_sec, METH_NOARGS, nullptr},
    {"__csc__", (PyCFunction)PyComplexVar_csc, METH_NOARGS, nullptr},
    {"__arcsin__", (PyCFunction)PyComplexVar_arcsin, METH_NOARGS, nullptr},
    {"__arccos__", (PyCFunction)PyComplexVar_arccos, METH_NOARGS, nullptr},
    {"__arctan__", (PyCFunction)PyComplexVar_arctan, METH_NOARGS, nullptr},
    {"__arccot__", (PyCFunction)PyComplexVar_arccot, METH_NOARGS, nullptr},
    {"__arcsec__", (PyCFunction)PyComplexVar_arcsec, METH_NOARGS, nullptr},
    {"__arccsc__", (PyCFunction)PyComplexVar_arccsc, METH_NOARGS, nullptr},
    {"__sinh__", (PyCFunction)PyComplexVar_sinh, METH_NOARGS, nullptr},
    {"__cosh__", (PyCFunction)PyComplexVar_cosh, METH_NOARGS, nullptr},
    {"__tanh__", (PyCFunction)PyComplexVar_tanh, METH_NOARGS, nullptr},
    {"__coth__", (PyCFunction)PyComplexVar_coth, METH_NOARGS, nullptr},
    {"__sech__", (PyCFunction)PyComplexVar_sech, METH_NOARGS, nullptr},
    {"__csch__", (PyCFunction)PyComplexVar_csch, METH_NOARGS, nullptr},
    {"__arcsinh__", (PyCFunction)PyComplexVar_arcsinh, METH_NOARGS, nullptr},
    {"__arccosh__", (PyCFunction)PyComplexVar_arccosh, METH_NOARGS, nullptr},
    {"__arctanh__", (PyCFunction)PyComplexVar_arctanh, METH_NOARGS, nullptr},
    {"__arccoth__", (PyCFunction)PyComplexVar_arccoth, METH_NOARGS, nullptr},
    {"__arcsech__", (PyCFunction)PyComplexVar_arcsech, METH_NOARGS, nullptr},
    {"__arccsch__", (PyCFunction)PyComplexVar_arccsch, METH_NOARGS, nullptr},
    {"__log__", (PyCFunction)PyComplexVar_log, METH_O, nullptr},
    {"__rlog__", (PyCFunction)PyComplexVar_rlog, METH_O, nullptr},
    nullptr,
};

static PyMemberDef PyComplexVar_members[] = {
    {"is_arbitrary", T_BOOL, offsetof(PyComplexVarObject, num.isArbitrary), 0, nullptr},
    nullptr,
};

static PyGetSetDef PyComplexVar_getset[] = {
    {"real", (getter)PyComplexVar_get_real, (setter)PyComplexVar_set_real, nullptr, nullptr},
    {"imag", (getter)PyComplexVar_get_imag, (setter)PyComplexVar_set_imag, nullptr, nullptr},
    {"r", (getter)PyComplexVar_get_len, (setter)PyComplexVar_set_len, nullptr, nullptr},
    {"arg", (getter)PyComplexVar_get_arg, (setter)PyComplexVar_set_arg, nullptr, &isdeg},
    nullptr,
};

PyTypeObject PyComplexVar_Type = {
    .ob_base = PyVarObject_HEAD_INIT(&PyType_Type, 0).tp_name = "varcore.complexvar",
    .tp_basicsize = sizeof(PyComplexVarObject),
    .tp_dealloc = (destructor)PyComplexVar_dealloc,
    .tp_repr = (reprfunc)PyComplexVar_repr,
    .tp_as_number = &PyComplexVar_as_number,
    .tp_str = (reprfunc)PyComplexVar_str,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_richcompare = (richcmpfunc)PyComplexVar_richcompare,
    .tp_methods = PyComplexVar_methods,
    .tp_members = PyComplexVar_members,
    .tp_getset = PyComplexVar_getset,
    .tp_init = (initproc)PyComplexVar_init,
    .tp_new = (newfunc)PyComplexVar_new,
};