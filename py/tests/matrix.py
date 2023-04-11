from .Testcase import externed_Testcase
import matrix_analysis

class Test_mat(externed_Testcase):
    def test_init(self):
        x = matrix_analysis.matrix.matrix(10, 10)
        y = matrix_analysis.matrix.matrix(10, 10, 1)
        z = matrix_analysis.matrix.matrix(10, 10, 0.1)
        w = matrix_analysis.matrix.matrix(10, 10, 1+1j)
        u = matrix_analysis.matrix.matrix(10, 10, None)
        v = matrix_analysis.matrix.matrix(
            10, 10, matrix_analysis.var.variable(1+1j))
        with self.assertRaises(MemoryError):
            X = matrix_analysis.matrix.matrix(2100000000, 2100000000)
        with self.assertRaises(ValueError):
            Y = matrix_analysis.matrix.matrix(1, 1, "abc")
        with self.assertRaises(ValueError):
            Z = matrix_analysis.matrix.matrix(0, 1)
        with self.assertRaises(ValueError):
            Z = matrix_analysis.matrix.matrix(1, 0)

    def test_members(self):
        x = matrix_analysis.matrix.matrix(5, 10)
        self.assertEqual(x.rows, 5)
        self.assertEqual(x.cols, 10)
        self.assertEqual(x.total, 50)