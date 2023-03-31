import matplotlib.pyplot as plt
import matrix_analysis
import sys
import matplotlib
matplotlib.use('TkAgg')
print(sys.getrefcount(tuple))
print(sys.getrefcount(matrix_analysis.var.variable))
print(sys.getrefcount(matrix_analysis.matrix.matrix))