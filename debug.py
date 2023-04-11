import matrix_analysis

matrix_analysis.matrix.set_fastprint(False)
matrix_analysis.matrix.set_printarea(left=2, up=1, down=2)


a = matrix_analysis.matrix.matrix(
    [(0, 0, 1), (0, 1, None), (1, 0, 2.0), (1, 1, 1+1j)])

print(str(a))