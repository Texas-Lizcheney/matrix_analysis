#include <matrix.h>

PyTypeObject PyMatrixType={
    .ob_base=PyVarObject_HEAD_INIT(&PyType_Type,0).tp_name="matrixcore.matrix",
    .tp_basicsize=sizeof(PyMatrixObject),
};