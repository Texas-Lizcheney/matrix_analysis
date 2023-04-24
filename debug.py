import matrix_analysis
import numpy
import sys
x = matrix_analysis.matrix.matrix([[15, 18], [matrix_analysis.Unsure, 12+13j]])
z = id(x)
y = matrix_analysis.matrix.matrix([[1, 2], [3, 4]])
x += y
print(x)
print(sys.getrefcount(x))
print(z == id(x))
try:
    x += matrix_analysis.matrix.matrix([[1]])
except:
    print(sys.getrefcount(x))
