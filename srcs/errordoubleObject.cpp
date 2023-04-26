#include <errordouble.h>

int doubleprecision = 7;
bool print_error = false;

void PyerrordoubleObject_dealloc(PyerrordoubleObject *self)
{
    Py_TYPE(self)->tp_free((PyObject *)self);
    return;
}

PyTypeObject PyerrordoubleType = {
    .ob_base = PyVarObject_HEAD_INIT(&PyType_Type, 0).tp_name = "varcore.errordouble",
    .tp_basicsize = sizeof(PyerrordoubleObject),
    .tp_dealloc = (destructor)PyerrordoubleObject_dealloc,
};