#define PY_SSIZE_T_CLEAN
#include <Python.h>
#define NPY_NO_DEPRECATED_API NPY_1_24_API_VERSION
#define PY_ARRAY_UNIQUE_SYMBOL np_array_api
#include <arrayobject.h>
#include <complexvar.h>
#include <matrix.h>
#include <utilities.h>

extern PyTypeObject PyErrordouble_Type;
extern PyTypeObject PyComplexVar_Type;
extern PyTypeObject PyMatrix_Type;
extern PyTypeObject PyUnsure_Type;
extern PyObject *PyExc_UndefinedWarning;
extern PyObject *PyExc_ShapeError;
extern PyObject PyUnsure;

static PyMethodDef varModule_method[] = {
    {"set_print_precision", (PyCFunction)SetDoublePrecision, METH_O, nullptr},
    {"set_arg_format", (PyCFunction)SetArgFormat, METH_O, nullptr},
    {"set_print_error", (PyCFunction)SetPrintError, METH_O, nullptr},
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
    if (PyType_Ready(&PyComplexVar_Type) < 0)
    {
        return nullptr;
    }
    if (PyType_Ready(&PyErrordouble_Type) < 0)
    {
        return nullptr;
    }
    PyObject *m = PyModule_Create(&varModule);
    if (!m)
    {
        return nullptr;
    }
    Py_INCREF(&PyComplexVar_Type);
    Py_INCREF(&PyErrordouble_Type);
    if ((PyModule_AddType(m, &PyComplexVar_Type) < 0) ||
        (PyModule_AddType(m, &PyErrordouble_Type) < 0))
    {
        Py_DECREF(&PyComplexVar_Type);
        Py_DECREF(&PyErrordouble_Type);
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
    if (PyType_Ready(&PyMatrix_Type) < 0)
    {
        return nullptr;
    }
    PyObject *m = PyModule_Create(&matrixModule);
    if (!m)
    {
        return nullptr;
    }
    Py_INCREF(&PyMatrix_Type);
    if ((PyModule_AddType(m, &PyMatrix_Type) < 0))
    {
        Py_DECREF(&PyMatrix_Type);
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
    if (PyType_Ready(&PyUnsure_Type) < 0)
    {
        return nullptr;
    }
    import_array();
    PyObject *varm = Init_varModule();
    PyObject *matrixm = Init_matrixModule();
    PyObject *m = PyModule_Create(&coreMoulde);
    PyExc_UndefinedWarning = PyErr_NewException("core.UndefinedWarning", PyExc_Warning, nullptr);
    PyExc_ShapeError = PyErr_NewException("core.ShapeError", nullptr, nullptr);
    if (!varm ||
        !matrixm ||
        !PyExc_UndefinedWarning ||
        !PyExc_ShapeError ||
        !m)
    {
        Py_XDECREF(varm);
        Py_XDECREF(matrixm);
        Py_XDECREF(PyExc_UndefinedWarning);
        Py_XDECREF(PyExc_ShapeError);
        Py_XDECREF(m);
        return nullptr;
    }
    Py_INCREF(&PyUnsure);
    if ((PyModule_AddObject(m, "matrixcore", matrixm) < 0) ||
        (PyModule_AddObject(m, "varcore", varm) < 0) ||
        (PyModule_AddObject(m, "UndefinedWarning", PyExc_UndefinedWarning) < 0) ||
        (PyModule_AddObject(m, "ShapeError", PyExc_ShapeError) < 0) ||
        (PyModule_AddObject(m, "Unsure", &PyUnsure) < 0))
    {
        Py_DECREF(varm);
        Py_DECREF(matrixm);
        Py_DECREF(PyExc_UndefinedWarning);
        Py_DECREF(PyExc_ShapeError);
        Py_DECREF(&PyUnsure);
        Py_DECREF(m);
        return nullptr;
    }
    return m;
}