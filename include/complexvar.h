#pragma once
#include <sstream>
#include <string>
#include <iomanip>
#include <numbers>
#include <Python.h>
#include <structmember.h>
#include <utilities.h>
#include <sstream>
#include <errordouble.h>

struct ComplexVar
{
    error_double real;
    error_double imag;
    bool isArbitrary;
    ComplexVar(const double & = 0, const double & = 0, bool = false);
    ComplexVar(const error_double &, const error_double &, bool);
    ComplexVar(const ComplexVar &) noexcept;
    void operator=(const ComplexVar &);
};

#define ComplexVar_iszero(x) (x.real == 0 && x.imag == 0 && !x.isArbitrary)
error_double ComplexVar_L1(const ComplexVar &);
error_double ComplexVar_squaredL2(const ComplexVar &);
error_double ComplexVar_L2(const ComplexVar &);
error_double ComplexVar_arg(const ComplexVar &);
void setvalue_frompolar(const error_double &r, const error_double &a, ComplexVar &);
std::ostream &operator<<(std::ostream &, const ComplexVar &);
std::ostream &operator<(std::ostream &, const ComplexVar &);
ComplexVar ComplexVar_conj(const ComplexVar &);
void ComplexVar_iconj(ComplexVar &);
ComplexVar ComplexVar_add(const ComplexVar &, const ComplexVar &);
ComplexVar ComplexVar_sub(const ComplexVar &, const ComplexVar &);
ComplexVar ComplexVar_mul(const ComplexVar &, const ComplexVar &);
ComplexVar ComplexVar_div(const ComplexVar &, const ComplexVar &);
ComplexVar ComplexVar_fdv(const ComplexVar &, const ComplexVar &);
ComplexVar ComplexVar_mod(const ComplexVar &, const ComplexVar &);
ComplexVar ComplexVar_exp(const ComplexVar &);
ComplexVar ComplexVar_ln(const ComplexVar &);
ComplexVar ComplexVar_log(const ComplexVar &, const ComplexVar &);
ComplexVar ComplexVar_pow(const ComplexVar &, const ComplexVar &);
ComplexVar ComplexVar_neg(const ComplexVar &);
ComplexVar ComplexVar_ivt(const ComplexVar &);
ComplexVar ComplexVar_sqrt(const ComplexVar &);
ComplexVar ComplexVar_sin(const ComplexVar &);
ComplexVar ComplexVar_cos(const ComplexVar &);
ComplexVar ComplexVar_tan(const ComplexVar &);
ComplexVar ComplexVar_cot(const ComplexVar &);
ComplexVar ComplexVar_sec(const ComplexVar &);
ComplexVar ComplexVar_csc(const ComplexVar &);
ComplexVar ComplexVar_arcsin(const ComplexVar &);
ComplexVar ComplexVar_arccos(const ComplexVar &);
ComplexVar ComplexVar_arctan(const ComplexVar &);
ComplexVar ComplexVar_arccot(const ComplexVar &);
ComplexVar ComplexVar_arcsec(const ComplexVar &);
ComplexVar ComplexVar_arccsc(const ComplexVar &);
ComplexVar ComplexVar_sinh(const ComplexVar &);
ComplexVar ComplexVar_cosh(const ComplexVar &);
ComplexVar ComplexVar_tanh(const ComplexVar &);
ComplexVar ComplexVar_coth(const ComplexVar &);
ComplexVar ComplexVar_sech(const ComplexVar &);
ComplexVar ComplexVar_csch(const ComplexVar &);
ComplexVar ComplexVar_arcsinh(const ComplexVar &);
ComplexVar ComplexVar_arccosh(const ComplexVar &);
ComplexVar ComplexVar_arctanh(const ComplexVar &);
ComplexVar ComplexVar_arccoth(const ComplexVar &);
ComplexVar ComplexVar_arcsech(const ComplexVar &);
ComplexVar ComplexVar_arccsch(const ComplexVar &);

struct PyComplexVarObject
{
    PyObject_HEAD;
    ComplexVar num;
};

int assignComplexVar(PyObject *, ComplexVar &);
int assignComplexVar_withExc(PyObject *, ComplexVar &);
extern PyTypeObject PyComplexVar_Type;
#define PyComplexVar_Check(op) PyObject_TypeCheck(op, &PyComplexVar_Type)
#define PyComplexVar_CheckExact(op) Py_IS_TYPE(op, &PyComplexVar_Type)
#define CanBeComplexVar(op) (PyComplexVar_CheckExact(op) || Py_IsUnsure(op) || PyLong_CheckExact(op) || PyFloat_CheckExact(op) || PyComplex_CheckExact(op))