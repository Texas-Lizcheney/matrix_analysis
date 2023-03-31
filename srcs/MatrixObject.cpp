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
    self->elements = nullptr;
    static char *kwlist[] = {
        (char *)"rows",
        (char *)"cols",
        (char *)"num",
        nullptr,
    };
    PyObject *tmp_object = nullptr;
    if (!PyArg_ParseTupleAndKeywords(args, kwds, "ii|O", kwlist, &self->rows, &self->cols, &tmp_object))
    {
        return -1;
    }
    if (self->rows <= 0 || self->cols <= 0)
    {
        PyErr_Format(PyExc_ValueError, "Invalid size. rows: %ld, cols: %ld", self->rows, self->cols);
        return -1;
    }
    self->total_elements = ((int64_t)self->rows) * ((int64_t)self->cols);
    try
    {
        self->elements = new ComplexVar[self->total_elements];
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        PyErr_Format(PyExc_MemoryError, "Fail to allocate %lld numbers.", self->total_elements);
        return -1;
    }
    ComplexVar tmp_value;
    if (assignComplexVar(tmp_object, tmp_value))
    {
        PyErr_Format(PyExc_ValueError, "Unsupported type: %s", tmp_object->ob_type->tp_name);
        return -1;
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
    .tp_init = (initproc)PyMatrix_init,
    .tp_new = (newfunc)PyMatrix_new,
};