from .Testcase import externed_Testcase
import matrix_analysis
import numpy

d = numpy.__dict__
numpy_dtypes = [d[i] for i in d .keys() if isinstance(
    d[i], type) and i[-1].isnumeric() and i[0] not in "dt"]


class Test_mat(externed_Testcase):
    def test_init(self):
        x = matrix_analysis.matrix.matrix(1, 1)
        self.assertEqual(str(x), "[undefined]")
        y = matrix_analysis.matrix.matrix(1, 1, 1)
        self.assertEqual(str(y), "[1+0i]")
        z = matrix_analysis.matrix.matrix(1, 1, 0.1)
        self.assertEqual(str(z), "[0.1+0i]")
        w = matrix_analysis.matrix.matrix(1, 1, 1+1j)
        self.assertEqual(str(w), "[1+1i]")
        u = matrix_analysis.matrix.matrix(1, 1, None)
        self.assertEqual(str(u), "[undefined]")
        v = matrix_analysis.matrix.matrix(
            1, 1, matrix_analysis.var.variable(1+1j))
        self.assertEqual(str(v), "[1+1i]")
        with self.assertRaises(MemoryError):
            X = matrix_analysis.matrix.matrix(2100000000, 2100000000)
        with self.assertRaises(ValueError):
            Y = matrix_analysis.matrix.matrix(1, 1, "abc")
        with self.assertRaises(ValueError):
            Z = matrix_analysis.matrix.matrix(0, 1)
        with self.assertRaises(ValueError):
            Z = matrix_analysis.matrix.matrix(1, 0)
        x = matrix_analysis.matrix.matrix([[1, 2.0], [None, 1+2j]])
        self.assertEqual(str(x), "[1+0i\t2+0i]\n[undefined\t1+2i]")
        x = matrix_analysis.matrix.matrix(
            [[1, 2.0], [None, 1+2j, 0]], fill=None)
        self.assertEqual(
            str(x), "[1+0i\t2+0i\tundefined]\n[undefined\t1+2i\t0+0i]")
        with self.assertRaises(ValueError):
            z = matrix_analysis.matrix.matrix([[1, 2], 0])
        with self.assertRaises(ValueError):
            x = matrix_analysis.matrix.matrix(
                [[1, 2.0], [None, 1+2j, 0]], fill="abc")
        with self.assertRaises(ValueError):
            x = matrix_analysis.matrix.matrix(
                [[1, 2.0, "abc"], [None, 1+2j, 0]])
        y = matrix_analysis.matrix.matrix(
            [(0, 0, 1), (1, 1, 2.0), (2, 2, None), (3, 3, 1+2j)])
        self.assertEqual(str(
            y), "[1+0i\tundefined\tundefined\tundefined]\n[undefined\t2+0i\tundefined\tundefined]\n[undefined\tundefined\tundefined\tundefined]\n[undefined\tundefined\tundefined\t1+2i]")
        y = matrix_analysis.matrix.matrix([(0, 1, 1)], fill=2)
        self.assertEqual(str(y), "[2+0i\t1+0i]")
        with self.assertRaises(MemoryError):
            z = matrix_analysis.matrix.matrix([(21000000, 21000000, 1)])
        with self.assertRaises(ValueError):
            z = matrix_analysis.matrix.matrix([(1, 2, 3, 4)])
        with self.assertRaises(ValueError):
            z = matrix_analysis.matrix.matrix([("abc", 12, 43)])
        with self.assertRaises(ValueError):
            z = matrix_analysis.matrix.matrix(
                [(0, 0, 1), (1, 1, 2.0), (2, 2, None), (3, 3, 1+2j)], fill="abc")
        with self.assertRaises(ValueError):
            z = matrix_analysis.matrix.matrix([(1, 1, "Abc")])
        for D in numpy_dtypes:
            x = matrix_analysis.matrix.matrix(numpy.array([[1]], dtype=D))
            self.assertEqual(str(x), "[1+0i]")
        with self.assertRaises(ValueError):
            x = matrix_analysis.matrix.matrix(
                numpy.array([[1]], dtype=numpy.timedelta64))
        x = matrix_analysis.matrix.matrix(
            numpy.array([[1, 2.0], [None, 3+4j], [5, matrix_analysis.var.variable(1+1j)]], dtype=object))
        self.assertEqual(
            str(x), "[1+0i\t2+0i]\n[undefined\t3+4i]\n[5+0i\t1+1i]")
        with self.assertRaises(ValueError):
            x = matrix_analysis.matrix.matrix(
                numpy.array([["abc"]], dtype=object))

    def test_members(self):
        x = matrix_analysis.matrix.matrix(5, 10)
        self.assertEqual(x.rows, 5)
        self.assertEqual(x.cols, 10)
        self.assertEqual(x.total, 50)
