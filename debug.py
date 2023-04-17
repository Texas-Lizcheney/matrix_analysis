import matrix_analysis
import numpy
import sys

x = matrix_analysis.matrix.matrix(
    [[1, 2.0, None], [None, 3+4j], [5, matrix_analysis.var.variable(1+1j)]])

print(x[0,::-1])
