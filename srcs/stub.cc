#define PY_SSIZE_T_CLEAN
#include <matrix.h>
#include <complexvar.h>
#include <Python.h>

extern PyTypeObject PyMatrixType;
extern PyTypeObject PyComplexVarType;
extern PyObject *PyExc_Undefined;

static PyModuleDef coreMoulde = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "core",
    .m_size = -1,
};

static PyMethodDef varModule_method[] = {
    {"set_print_precision", (PyCFunction)SetDoublePrecision, METH_O, nullptr},
    {"set_arg_format", (PyCFunction)SetArgFormat, METH_O, nullptr},
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
    PyExc_Undefined = PyErr_NewException("varcore.Undefined", nullptr, nullptr);
    if (!PyExc_Undefined)
    {
        Py_XDECREF(PyExc_Undefined);
        return nullptr;
    }
    PyObject *m = PyModule_Create(&varModule);
    if (!m)
    {
        Py_DECREF(PyExc_Undefined);
        return nullptr;
    }
    Py_INCREF(&PyComplexVarType);
    if ((PyModule_AddType(m, &PyComplexVarType) < 0) || (PyModule_AddObject(m, "Undefined", PyExc_Undefined) < 0))
    {
        Py_DECREF(&PyComplexVarType);
        Py_DECREF(PyExc_Undefined);
        Py_DECREF(m);
        return nullptr;
    }
    return m;
}

static PyModuleDef matrixModule = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "matrixcore",
    .m_size = -1,
};

PyObject *Init_matrixModule()
{
    if (PyType_Ready(&PyMatrixType) < 0)
    {
        return nullptr;
    }
    PyObject *m = PyModule_Create(&matrixModule);
    if (!m)
    {
        return nullptr;
    }
    Py_INCREF(&PyMatrixType);
    if ((PyModule_AddType(m, &PyMatrixType) < 0))
    {
        Py_DECREF(&PyMatrixType);
        Py_DECREF(m);
        return nullptr;
    }
    return m;
}

PyMODINIT_FUNC PyInit_core()
{
    PyObject *varm = Init_varModule();
    PyObject *matrixm = Init_matrixModule();
    if (!varm || !matrixm)
    {
        Py_XDECREF(varm);
        Py_XDECREF(matrixm);
        return nullptr;
    }
    PyObject *m = PyModule_Create(&coreMoulde);
    if (!m)
    {
        Py_DECREF(varm);
        return nullptr;
    }
    if ((PyModule_AddObject(m, "matrixcore", matrixm) < 0) || (PyModule_AddObject(m, "varcore", varm) < 0))
    {
        Py_DECREF(varm);
        Py_DECREF(matrixm);
        Py_DECREF(m);
        return nullptr;
    }
    return m;
}