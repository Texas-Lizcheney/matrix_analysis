#define PY_SSIZE_T_CLEAN
#include <Python.h>
#define NPY_NO_DEPRECATED_API NPY_1_24_API_VERSION
#define PY_ARRAY_UNIQUE_SYMBOL np_array_api
#include <arrayobject.h>
#include <complexvar.h>
#include <matrix.h>
#include <utilities.h>

extern PyTypeObject PyMatrixType;
extern PyTypeObject PyComplexVarType;
extern PyTypeObject PyUnsureType;
extern PyObject *PyExc_Undefined;
extern PyObject *PyExc_ShapeError;
extern PyObject PyUnsure;

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
    PyObject *m = PyModule_Create(&varModule);
    if (!m)
    {
        Py_DECREF(PyExc_Undefined);
        return nullptr;
    }
    Py_INCREF(&PyComplexVarType);
    if ((PyModule_AddType(m, &PyComplexVarType) < 0))
    {
        Py_DECREF(&PyComplexVarType);
        Py_DECREF(PyExc_Undefined);
        Py_DECREF(m);
        return nullptr;
    }
    return m;
}

static PyMethodDef matrixModule_method[] = {
    {"set_fastprint", (PyCFunction)SetFastPrint, METH_O, nullptr},
    {"set_printarea", (PyCFunction)SetPrintArea, METH_VARARGS | METH_KEYWORDS, nullptr},
    nullptr,
};

static PyModuleDef matrixModule = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "matrixcore",
    .m_size = -1,
    .m_methods = matrixModule_method,
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

static PyModuleDef coreMoulde = {
    .m_base = PyModuleDef_HEAD_INIT,
    .m_name = "core",
    .m_size = -1,
};

PyMODINIT_FUNC PyInit_core()
{
    if (PyType_Ready(&PyUnsureType) < 0)
    {
        return nullptr;
    }
    import_array();
    PyObject *varm = Init_varModule();
    PyObject *matrixm = Init_matrixModule();
    PyObject *m = PyModule_Create(&coreMoulde);
    PyExc_Undefined = PyErr_NewException("core.Undefined", nullptr, nullptr);
    PyExc_ShapeError = PyErr_NewException("core.ShapeError", nullptr, nullptr);
    if (!varm ||
        !matrixm ||
        !PyExc_Undefined ||
        !PyExc_ShapeError ||
        !m)
    {
        Py_XDECREF(varm);
        Py_XDECREF(matrixm);
        Py_XDECREF(PyExc_Undefined);
        Py_XDECREF(PyExc_ShapeError);
        Py_XDECREF(m);
        return nullptr;
    }
    Py_INCREF(&PyUnsure);
    if ((PyModule_AddObject(m, "matrixcore", matrixm) < 0) ||
        (PyModule_AddObject(m, "varcore", varm) < 0) ||
        (PyModule_AddObject(m, "Undefined", PyExc_Undefined) < 0) ||
        (PyModule_AddObject(m, "ShapeError", PyExc_ShapeError) < 0) ||
        (PyModule_AddObject(m, "Unsure", &PyUnsure) < 0))
    {
        Py_DECREF(varm);
        Py_DECREF(matrixm);
        Py_DECREF(PyExc_Undefined);
        Py_DECREF(PyExc_ShapeError);
        Py_DECREF(&PyUnsure);
        Py_DECREF(m);
        return nullptr;
    }
    return m;
}