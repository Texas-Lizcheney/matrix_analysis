#pragma once
#include <Python.h>
#include <math.h>
#include <stdarg.h>

// global settings
double casthalf_to_double(const uint16_t &);
double cal_original_error(const double &);
void Addmessage(const char *, ...);
PyObject *SetDoublePrecision(PyObject *, PyObject *);
PyObject *SetArgFormat(PyObject *, PyObject *);
PyObject *SetFastPrint(PyObject *, PyObject *);
PyObject *SetPrintArea(PyObject *, PyObject *, PyObject *);
PyObject *SetPrintError(PyObject *, PyObject *);

// PyUnsure
void PyUnsure_dealloc(PyObject *);
PyObject *PyUnsure_repr(PyObject *);
PyObject *PyUnsure_new(PyTypeObject *, PyObject *, PyObject *);
extern PyObject PyUnsure;
#define Py_RETURN_UNSURE return Py_NewRef(&PyUnsure)
#define Py_IsUnsure(x) Py_Is((x), &PyUnsure)
template <typename T>
concept npy_real = std::is_integral_v<T> || std::is_floating_point_v<T>;
template <typename T>
concept npy_complex = requires(T a) {
    a.real;
    a.imag;
};
template <typename T>
concept is_double = std::is_same_v<T, double>;