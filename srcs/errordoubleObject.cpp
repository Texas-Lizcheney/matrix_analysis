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
    return 0;
}

PyTypeObject PyErrordouble_Type = {
    .ob_base = PyVarObject_HEAD_INIT(&PyType_Type, 0).tp_name = "varcore.errordouble",
    .tp_basicsize = sizeof(PyErrordoubleObject),
    .tp_dealloc = (destructor)PyErrordoubleObject_dealloc,
    .tp_repr = (reprfunc)PyErrordoubleObject_repr,
};