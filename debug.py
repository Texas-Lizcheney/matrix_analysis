import matrix_analysis
import numpy
import sys
x = matrix_analysis.matrix.matrix([[15, 18], [matrix_analysis.Unsure, 12+13j]])
x1 = +x
print(id(x1))
x1 *= (1+1j)
print(id(x1))
