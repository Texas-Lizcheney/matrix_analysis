#include <complexvar.h>

PyTypeObject ComplexVarType={
    .ob_base=PyVarObject_HEAD_INIT(&PyType_Type,0).tp_name="varcore.complexvar",
    .tp_basicsize=sizeof(PyComplexVarObject),
};