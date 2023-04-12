import matrix_analysis
import numpy
matrix_analysis.matrix.set_fastprint(False)
matrix_analysis.matrix.set_printarea(left=2, up=1, down=2)

d = numpy.__dict__
numpy_dtypes = [d[i] for i in d .keys() if isinstance(
    d[i], type) and i[-1].isnumeric() and i[0] not in "dt"]

x = matrix_analysis.var.variable(None)
y = matrix_analysis.var.variable(0)
print(y/x)
print(x/y)