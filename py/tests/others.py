from .Testcase import externed_Testcase
import matrix_analysis


class Test_other_methods(externed_Testcase):
    def test_reshape(self):
        x = matrix_analysis.matrix.matrix([[1, 2],
                                           [3, 4]])
        matrix_analysis.matrix.reshape(x, 1, 4)
        self.assertMatrixAlmostEqual(
            x, matrix_analysis.matrix.matrix([[1, 2, 3, 4]]))
        matrix_analysis.matrix.reshape(x, (4, 1))
        self.assertMatrixAlmostEqual(
            x, matrix_analysis.matrix.matrix([[1],
                                              [2],
                                              [3],
                                              [4]]))
        with self.assertRaises(matrix_analysis.ShapeError):
            matrix_analysis.matrix.reshape(x, 1, 3)
        with self.assertRaises(TypeError):
            matrix_analysis.matrix.reshape(
                matrix_analysis.matrix.vector([1, 2, 3]), 1, 3)
