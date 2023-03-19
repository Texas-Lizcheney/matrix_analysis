import matrix_analysis
import sys

a = matrix_analysis.var.variable(7, 9)
b = matrix_analysis.var.variable(5, 3)
c = matrix_analysis.var.variable(2, 3)
d = pow(a, b, c)
k = id(a)
a.__ipow__(b, c)
print(a)
