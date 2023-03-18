#define PY_SSIZE_T_CLEAN
#include <matrix.h>
#include <complexvar.h>
#include <Python.h>

extern PyTypeObject PyMatrixType;
extern PyTypeObject PyComplexVarType;

static PyModuleDef coreMoulde = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "matrixcore",
    .m_size = -1,
};

static PyMethodDef varModule_method[] = {
    {"set_print_precision", (PyCFunction)SetDoublePrecision, METH_O, nullptr},
    nullptr,
};

static PyModuleDef varModule = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "varcore",
    .m_size = -1,
    .m_methods = varModule_method,
};

PyObject *Init_varModule()
{
    if (PyType_Ready(&PyComplexVarType) < 0)
    {
        return nullptr;
    }
    PyObject *m = PyModule_Create(&varModule);
    if (!m)
    {
        return nullptr;
    }
    Py_INCREF(&PyComplexVarType);
    if (((PyModule_AddType(m, &PyComplexVarType) < 0)))
    {
        Py_DECREF(&PyComplexVarType);
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
    if (PyType_Ready(&PyMatrixType) < 0)
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
    Py_INCREF(&PyMatrixType);
    if ((PyModule_AddType(m, &PyMatrixType) < 0) ||
        (PyModule_AddObject(m, "varcore", varm) < 0))
    {
        Py_DECREF(&PyMatrixType);
        Py_DECREF(varm);
        Py_DECREF(m);
        return nullptr;
    }
    return m;
}