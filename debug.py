import matplotlib.pyplot as plt
import matrix_analysis
import sys
import matplotlib
matplotlib.use('TkAgg')

i = 0.1

x = [_/10 for _ in range(-20, 21)]
yr = [matrix_analysis.var.variable(
    _/10, i).arccosh().real for _ in range(-20, 21)]
yi = [matrix_analysis.var.variable(
    _/10, i).arccosh().imag for _ in range(-20, 21)]
plt.plot(x, yr, c="r")
plt.plot(x, yi, c='b')
print(matrix_analysis.var.variable(0.1,-0.1).arccosh())