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
        u = matrix_analysis.matrix.matrix(1, 1, matrix_analysis.Unsure)
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
        x = matrix_analysis.matrix.matrix(
            [[1, 2.0], [matrix_analysis.Unsure, 1+2j]])
        self.assertEqual(str(x), "[1+0i\t2+0i]\n[undefined\t1+2i]")
        x = matrix_analysis.matrix.matrix(
            [[1, 2.0], [matrix_analysis.Unsure, 1+2j, 0]], fill=matrix_analysis.Unsure)
        self.assertEqual(
            str(x), "[1+0i\t2+0i\tundefined]\n[undefined\t1+2i\t0+0i]")
        with self.assertRaises(ValueError):
            z = matrix_analysis.matrix.matrix([[1, 2], 0])
        with self.assertRaises(ValueError):
            x = matrix_analysis.matrix.matrix(
                [[1, 2.0], [matrix_analysis.Unsure, 1+2j, 0]], fill="abc")
        with self.assertRaises(ValueError):
            x = matrix_analysis.matrix.matrix(
                [[1, 2.0, "abc"], [matrix_analysis.Unsure, 1+2j, 0]])
        y = matrix_analysis.matrix.matrix(
            [(0, 0, 1), (1, 1, 2.0), (2, 2, matrix_analysis.Unsure), (3, 3, 1+2j)])
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
                [(0, 0, 1), (1, 1, 2.0), (2, 2, matrix_analysis.Unsure), (3, 3, 1+2j)], fill="abc")
        with self.assertRaises(ValueError):
            z = matrix_analysis.matrix.matrix([(1, 1, "Abc")])
        with self.assertRaises(ValueError):
            z = matrix_analysis.matrix.matrix([])
        with self.assertRaises(ValueError):
            z = matrix_analysis.matrix.matrix([[]])
        with self.assertRaises(ValueError):
            z = matrix_analysis.matrix.matrix([[[]]])
        for D in numpy_dtypes:
            x = matrix_analysis.matrix.matrix(numpy.array([[1]], dtype=D))
            self.assertEqual(str(x), "[1+0i]")
        with self.assertRaises(ValueError):
            x = matrix_analysis.matrix.matrix(
                numpy.array([[1]], dtype=numpy.timedelta64))
        x = matrix_analysis.matrix.matrix(
            numpy.array([[1, 2.0], [matrix_analysis.Unsure, 3+4j], [5, matrix_analysis.var.variable(1+1j)]], dtype=object))
        self.assertEqual(
            str(x), "[1+0i\t2+0i]\n[undefined\t3+4i]\n[5+0i\t1+1i]")
        with self.assertRaises(TypeError):
            x = matrix_analysis.matrix.matrix(
                numpy.array([["abc"]], dtype=object))

    def test_repr(self):
        x = matrix_analysis.matrix.matrix(
            [[1, 2.0, matrix_analysis.Unsure], [matrix_analysis.Unsure, 3+4j], [5, matrix_analysis.var.variable(1+1j)]])
        self.assertEqual(str(
            x), "[1+0i\t2+0i\tundefined]\n[undefined\t3+4i\tundefined]\n[5+0i\t1+1i\tundefined]")
        self.assertEqual(
            repr(x), "[[1+0j,2+0j,Unsure,],[Unsure,3+4j,Unsure,],[5+0j,1+1j,Unsure,],]")
        matrix_analysis.matrix.set_fastprint(True)
        matrix_analysis.matrix.set_printarea(left=1, right=1, up=1, down=1)
        self.assertEqual(
            repr(x), "[[1+0j,2+0j,Unsure,],[Unsure,3+4j,Unsure,],[5+0j,1+1j,Unsure,],]")
        self.assertEqual(str(
            x), "[1+0i\t...\tundefined]\n[...\t...\t...]\n[5+0i\t...\tundefined]")
        matrix_analysis.matrix.set_fastprint(False)

    def test_len(self):
        x = matrix_analysis.matrix.matrix(
            [[1, 2.0, matrix_analysis.Unsure], [matrix_analysis.Unsure, 3+4j], [5, matrix_analysis.var.variable(1+1j)]])
        self.assertEqual(len(x), 9)

    def test_getitem(self):
        x = matrix_analysis.matrix.matrix(
            [[1, 2.0, matrix_analysis.Unsure], [matrix_analysis.Unsure, 3+4j], [5, matrix_analysis.var.variable(1+1j)]])
        a = x[0, 0]
        self.assertAlmostEqual(a.rec, (1, 0))
        with self.assertRaises(IndexError):
            a = x[3, 3]
        self.assertEqual(str(x[0, :]), "[1+0i\t2+0i\tundefined]")
        self.assertEqual(str(x[0, 0:3:2]), "[1+0i\tundefined]")
        self.assertEqual(str(x[0, ::-1]), "[undefined\t2+0i\t1+0i]")
        self.assertEqual(str(x[0, ::-2]), "[undefined\t1+0i]")
        with self.assertRaises(IndexError):
            a = x[3, :]
        with self.assertRaises(IndexError):
            a = x[-4, :]
        self.assertEqual(str(x[:, 0]), "[1+0i]\n[undefined]\n[5+0i]")
        self.assertEqual(str(x[0:3:2, 0]), "[1+0i]\n[5+0i]")
        self.assertEqual(str(x[::-1, 0]), "[5+0i]\n[undefined]\n[1+0i]")
        self.assertEqual(str(x[::-2, 0]), "[5+0i]\n[1+0i]")
        with self.assertRaises(IndexError):
            a = x[:, 3]
        with self.assertRaises(IndexError):
            a = x[:, -4]
        self.assertEqual(str(x[:, :]), str(x))
        self.assertEqual(str(x[1:3, 1:3]),
                         "[3+4i\tundefined]\n[1+1i\tundefined]")
        self.assertEqual(str(x[-1:-3:-1, -1:-3:-2]),
                         "[undefined]\n[undefined]")
        with self.assertRaises(IndexError):
            a = x[4:10, 4:10]
        with self.assertRaises(TypeError):
            a = x["abc"]
        with self.assertRaises(IndexError):
            a = x[1, 2, :]
        with self.assertRaises(TypeError):
            a = x[1, "abc"]
        y = x[...]
        self.assertEqual(str(x), str(y))
        self.assertNotEqual(id(x), id(y))

    def test_setitem(self):
        x = matrix_analysis.matrix.matrix(4, 4, 0)
        with self.assertRaises(TypeError):
            x["abc"] = matrix_analysis.matrix.matrix([[1]])
        with self.assertRaises(IndexError):
            x[:, :, :] = matrix_analysis.matrix.matrix([[1]])
        x[0:3, 0:4:2] = matrix_analysis.matrix.matrix([[1, 2], [3, 4], [5, 6]])
        self.assertEqual(str(x), str(
            matrix_analysis.matrix.matrix([[1, 0, 2, 0], [3, 0, 4, 0], [5, 0, 6, 0], [0, 0, 0, 0]])))
        with self.assertRaises(IndexError):
            x[4:5, 4:5] = matrix_analysis.matrix.matrix([[1]])
        with self.assertRaises(matrix_analysis.ShapeError):
            x[0:3, 0:4:2] = matrix_analysis.matrix.matrix([[1, 2], [3, 4]])
        x = matrix_analysis.matrix.matrix(4, 4, 0)
        x[0:3, 0:4:2] = [[1, 2], [3, matrix_analysis.Unsure], [5]]
        self.assertEqual(str(x), str(matrix_analysis.matrix.matrix([[1, 0, 2, 0], [
                         3, 0, matrix_analysis.Unsure, 0], [5, 0, 0, 0], [0, 0, 0, 0]])))
        with self.assertRaises(ValueError):
            x[0:1, 0] = [[1], (2)]
        with self.assertRaises(ValueError):
            x[0:1, 0] = [[1], ["abc"]]
        with self.assertRaises(matrix_analysis.ShapeError):
            x[1:2, 0] = [[1], [2, 3]]
        y = str(matrix_analysis.matrix.matrix(
            [[1, 0, 1, 0], [0, 0, 0, 0], [1, 0, 1, 0], [0, 0, 0, 0]]))
        for D in numpy_dtypes:
            x = matrix_analysis.matrix.matrix(4, 4, 0)
            x[0:4:2, 0:4:2] = numpy.ones((2, 2), dtype=D)
            self.assertEqual(str(x), y)
        x = matrix_analysis.matrix.matrix(4, 4, 0)
        with self.assertRaises(ValueError):
            x[0, 0] = matrix_analysis.matrix.matrix(
                numpy.array([[1]], dtype=numpy.timedelta64))
        x[1:4:2, 0:4:2] = numpy.array(
            [[matrix_analysis.Unsure, 1], [2.0, -1-1j]], dtype=object)
        self.assertEqual(str(x), str(matrix_analysis.matrix.matrix(
            [[0, 0, 0, 0], [matrix_analysis.Unsure, 0, 1, 0], [0, 0, 0, 0], [2.0, 0, -1-1j, 0]])))
        with self.assertRaises(TypeError):
            x[0, 0] = numpy.array([["abc"]], dtype=object)
        x = matrix_analysis.matrix.matrix(2, 2, 0)
        x[0, 0] = matrix_analysis.Unsure
        self.assertEqual(str(x), str(
            matrix_analysis.matrix.matrix([[matrix_analysis.Unsure, 0], [0, 0]])))
        x = matrix_analysis.matrix.matrix(2, 2, 0)
        x[:, 0] = matrix_analysis.Unsure
        self.assertEqual(str(x), str(
            matrix_analysis.matrix.matrix([[matrix_analysis.Unsure, 0], [matrix_analysis.Unsure, 0]])))
        x = matrix_analysis.matrix.matrix(2, 2, 0)
        x[0, :] = matrix_analysis.Unsure
        self.assertEqual(str(x), str(
            matrix_analysis.matrix.matrix([[matrix_analysis.Unsure, matrix_analysis.Unsure], [0, 0]])))
        x = matrix_analysis.matrix.matrix(2, 2, 0)
        x[:, :] = matrix_analysis.Unsure
        self.assertEqual(str(x), str(
            matrix_analysis.matrix.matrix([[matrix_analysis.Unsure, matrix_analysis.Unsure], [matrix_analysis.Unsure, matrix_analysis.Unsure]])))
        with self.assertRaises(IndexError):
            x[3, 3] = matrix_analysis.Unsure
        x = matrix_analysis.matrix.matrix(2, 2, 0)
        x[...] = [(0, 1, matrix_analysis.Unsure), (1, 0, 1+1j)]
        self.assertEqual(str(x), str(
            matrix_analysis.matrix.matrix([[0, matrix_analysis.Unsure], [1+1j, 0]])))
        with self.assertRaises(TypeError):
            x[...] = "abc"
        with self.assertRaises(TypeError):
            x[...] = ["abc"]
        with self.assertRaises(ValueError):
            x[...] = [(1, 2)]
        with self.assertRaises(ValueError):
            x[...] = [("abc", 2, 3)]
        with self.assertRaises(TypeError):
            x[...] = [(1, 2, "abc")]

    def test_members(self):
        x = matrix_analysis.matrix.matrix(5, 10)
        self.assertEqual(x.rows, 5)
        self.assertEqual(x.cols, 10)
        self.assertEqual(x.total, 50)

    def test_property(self):
        x = matrix_analysis.matrix.matrix(3, 5)
        self.assertEqual(x.shape, (3, 5))
