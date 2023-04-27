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

PyTypeObject PyErrordouble_Type = {
    .ob_base = PyVarObject_HEAD_INIT(&PyType_Type, 0).tp_name = "varcore.errordouble",
    .tp_basicsize = sizeof(PyErrordoubleObject),
    .tp_dealloc = (destructor)PyErrordoubleObject_dealloc,
    .tp_repr = (reprfunc)PyErrordoubleObject_repr,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_init = (initproc)PyErrordoubleObject_init,
    .tp_new = (newfunc)PyErrordoubleObject_new,
};