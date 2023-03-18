import matrix_analysis

a = matrix_analysis.var.variable(1)
b = matrix_analysis.var.variable()
matrix_analysis.var.set_arg_format("deg")
a.arg = 90
print(a)
