import matrix_analysis
import numpy
import sys

d = numpy.__dict__
numpy_dtypes = [d[i] for i in d .keys() if isinstance(
    d[i], type) and i[-1].isnumeric() and i[0] not in "dt"]

for D in numpy_dtypes:
    print(D)
    x = matrix_analysis.matrix.matrix(4, 4, 0)
    x[0:4:2, 0:4:2] = numpy.ones((2, 2), dtype=D)
    print(x)
