#define PY_SSIZE_T_CLEAN
#include <matrix.h>
#include <Python.h>

extern PyTypeObject MatrixType;

static PyModuleDef MatrixMoulde = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "matrixcore",
    .m_size = -1,
};

PyMODINIT_FUNC PyInit_matrixcore()
{
    if (PyType_Ready(&MatrixType) < 0)
    {
        return NULL;
    }
    PyObject *m = PyModule_Create(&MatrixMoulde);
    if (!m)
    {
        return NULL;
    }
    Py_INCREF(&MatrixType);
    if ((PyModule_AddType(m, &MatrixType) < 0))
    {
        Py_DECREF(&MatrixType);
        Py_DECREF(m);
        return NULL;
    }
    return m;
}