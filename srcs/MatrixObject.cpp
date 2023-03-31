#include <matrix.h>

void PyMatrix_dealloc(PyMatrixObject *self)
{
    if (self->elements)
    {
        delete[] self->elements;
    }
    Py_TYPE(self)->tp_free((PyObject *)self);
}

int PyMatrix_init(PyMatrixObject *self, PyObject *args, PyObject *kwds)
{
    PyMatrixObject *tmp = (PyMatrixObject *)self;
    tmp->elements = nullptr;
    static char *kwlist[] = {
        (char *)"rows",
        (char *)"cols",
        (char *)"num",
        nullptr,
    };
    PyObject *tmp_object = nullptr;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "ii|O", kwlist, tmp->rows, tmp->cols, &tmp_object))
    {
        Py_DECREF(self);
        return -1;
    }
    self->total_elements = tmp->rows * tmp->cols;
    try
    {
        tmp->elements = new ComplexVar[self->total_elements];
    }
    catch (const std::bad_alloc &e)
    {
        PyErr_Format(PyExc_MemoryError, "Fail to allocate %lld numbers.", self->total_elements);
        Py_DECREF(self);
        return -1;
    }
    ComplexVar tmp_value;
    if (assignComplexVar(tmp_object, tmp_value))
    {
        PyErr_Format(PyExc_ValueError, "Unsupported type: %s", tmp_object->ob_type->tp_name);
    }
    for (int64_t i = 0; i < self->total_elements; i++)
    {
        self->elements[i] = tmp_value;
    }
    return 0;
}

PyObject *PyMatrix_new(PyTypeObject *type, PyObject *args, PyObject *kwds)
{
    PyObject *self = type->tp_alloc(type, 0);
    if (!self)
    {
        Py_RETURN_NONE;
    }
    ((PyMatrixObject *)self)->elements = nullptr;
    return self;
}

PyTypeObject PyMatrixType = {
    .ob_base = PyVarObject_HEAD_INIT(&PyType_Type, 0).tp_name = "matrixcore.matrix",
    .tp_basicsize = sizeof(PyMatrixObject),
    .tp_dealloc = (destructor)PyMatrix_dealloc,
    .tp_new = (newfunc)PyMatrix_new,
};