#pragma once
#include <Python.h>
#include <math.h>

double getdouble_fromPyObject(PyObject *);
double casthalf_to_double(const uint16_t &);
double cal_original_error(const double &);

void PyUnsure_dealloc(PyObject *);
PyObject *PyUnsure_repr(PyObject *);
PyObject *PyUnsure_new(PyTypeObject *, PyObject *, PyObject *);
extern PyObject PyUnsure;
#define Py_RETURN_UNSURE return Py_NewRef(&PyUnsure)
#define Py_IsUnsure(x) Py_Is((x), &PyUnsure)
