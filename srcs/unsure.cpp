#include <utilities.h>

void PyUnsure_dealloc(PyObject *self)
{
    Py_FatalError("deallocating Unsure");
}

PyObject *PyUnsure_repr(PyObject *self)
{
    return PyUnicode_FromString("Unsure");
}

PyObject *PyUnsure_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    if ((args && PyTuple_GET_SIZE(args)) || (kwds && PyDict_GET_SIZE(kwds)))
    {
        PyErr_SetString(PyExc_TypeError, "Unsure takes no arguments");
        return nullptr;
    }
    Py_RETURN_UNSURE;
}

PyTypeObject PyUnsureType = {
    .ob_base = PyVarObject_HEAD_INIT(&PyType_Type, 0).tp_name = "varcore.unsure",
    .tp_basicsize = 0,
    .tp_dealloc = (destructor)PyUnsure_dealloc,
    .tp_repr = (reprfunc)PyUnsure_repr,
    .tp_flags = Py_TPFLAGS_DEFAULT,
    .tp_new = (newfunc)PyUnsure_new,
};

PyObject PyUnsure = {
    _PyObject_HEAD_EXTRA.ob_refcnt = 1,
    .ob_type = &PyUnsureType,
};