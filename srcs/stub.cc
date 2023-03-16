#define PY_SSIZE_T_CLEAN
#include <matrix.h>
#include <complexvar.h>
#include <Python.h>

extern PyTypeObject MatrixType;
extern PyTypeObject ComplexVarType;

static PyModuleDef coreMoulde = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "matrixcore",
    .m_size = -1,
};

static PyModuleDef varModule = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "varcore",
    .m_size = -1,
};

PyObject *Init_varModule()
{
    if (PyType_Ready(&ComplexVarType) < 0)
    {
        return nullptr;
    }
    PyObject *m = PyModule_Create(&varModule);
    if (!m)
    {
        return nullptr;
    }
    Py_INCREF(&ComplexVarType);
    if (((PyModule_AddType(m, &ComplexVarType) < 0)))
    {
        Py_DECREF(&ComplexVarType);
        Py_DECREF(m);
        return nullptr;
    }
    return m;
}

PyMODINIT_FUNC PyInit_matrixcore()
{
    PyObject *varm = Init_varModule();
    if (!varm)
    {
        Py_XDECREF(varm);
        return nullptr;
    }
    if (PyType_Ready(&MatrixType) < 0)
    {
        Py_DECREF(varm);
        return nullptr;
    }
    PyObject *m = PyModule_Create(&coreMoulde);
    if (!m)
    {
        Py_DECREF(varm);
        return nullptr;
    }
    Py_INCREF(&MatrixType);
    if ((PyModule_AddType(m, &MatrixType) < 0) ||
        (PyModule_AddObject(m, "varcore", varm) < 0))
    {
        Py_DECREF(&MatrixType);
        Py_DECREF(varm);
        Py_DECREF(m);
        return nullptr;
    }
    return m;
}