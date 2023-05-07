from .Testcase import externed_Testcase
import matrix_analysis
import numpy
import sys

d = numpy.__dict__
numpy_dtypes = [d[i] for i in d .keys() if isinstance(
    d[i], type) and i[-1].isnumeric() and i[0] not in "dt"]
Unsure = matrix_analysis.Unsure


class Test_mat(externed_Testcase):
    def test_init(self):
        x = matrix_analysis.matrix.matrix(1, 1)
        self.assertEqual(str(x), "[undefined]")
        y = matrix_analysis.matrix.matrix(1, 1, fill=1)
        self.assertEqual(str(y), "[1+0i]")
        z = matrix_analysis.matrix.matrix(1, 1, fill=0.1)
        self.assertEqual(str(z), "[0.1+0i]")
        w = matrix_analysis.matrix.matrix(1, 1, fill=1+1j)
        self.assertEqual(str(w), "[1+1i]")
        u = matrix_analysis.matrix.matrix(1, 1, fill=Unsure)
        self.assertEqual(str(u), "[undefined]")
        v = matrix_analysis.matrix.matrix(
            1, 1, fill=matrix_analysis.var.variable(1+1j))
        self.assertEqual(str(v), "[1+1i]")
        with self.assertRaises(MemoryError):
            X = matrix_analysis.matrix.matrix(2100000000, 2100000000)
        with self.assertRaises(TypeError):
            Y = matrix_analysis.matrix.matrix(1, 1, fill="abc")  # type: ignore
        with self.assertRaises(ValueError):
            Z = matrix_analysis.matrix.matrix(0, 1)
        with self.assertRaises(ValueError):
            Z = matrix_analysis.matrix.matrix(1, 0)
        x = matrix_analysis.matrix.matrix(
            [[1, 2.0], [Unsure, 1+2j]])
        self.assertEqual(str(x), "[1+0i\t2+0i]\n[undefined\t1+2i]")
        x = matrix_analysis.matrix.matrix(
            [[1, 2.0], [Unsure, 1+2j, 0]], fill=Unsure)
        self.assertEqual(
            str(x), "[1+0i\t2+0i\tundefined]\n[undefined\t1+2i\t0+0i]")
        with self.assertRaises(ValueError):
            z = matrix_analysis.matrix.matrix([[1, 2], 0])  # type: ignore
        with self.assertRaises(TypeError):
            x = matrix_analysis.matrix.matrix(
                [[1, 2.0], [Unsure, 1+2j, 0]], fill="abc")  # type: ignore
        with self.assertRaises(TypeError):
            x = matrix_analysis.matrix.matrix(
                [[1, 2.0, "abc"], [Unsure, 1+2j, 0]])  # type: ignore
        y = matrix_analysis.matrix.matrix(
            [(0, 0, 1), (1, 1, 2.0), (2, 2, Unsure), (3, 3, 1+2j)])
        self.assertEqual(str(
            y), "[1+0i\tundefined\tundefined\tundefined]\n[undefined\t2+0i\tundefined\tundefined]\n[undefined\tundefined\tundefined\tundefined]\n[undefined\tundefined\tundefined\t1+2i]")
        y = matrix_analysis.matrix.matrix([(0, 1, 1)], fill=2)
        self.assertEqual(str(y), "[2+0i\t1+0i]")
        with self.assertRaises(MemoryError):
            z = matrix_analysis.matrix.matrix([(21000000, 21000000, 1)])
        with self.assertRaises(ValueError):
            z = matrix_analysis.matrix.matrix([(1, 2, 3, 4)])  # type: ignore
        with self.assertRaises(ValueError):
            z = matrix_analysis.matrix.matrix(
                [("abc", 12, 43)])  # type: ignore
        with self.assertRaises(TypeError):
            z = matrix_analysis.matrix.matrix(
                [(0, 0, 1), (1, 1, 2.0), (2, 2, Unsure), (3, 3, 1+2j)], fill="abc")  # type: ignore
        with self.assertRaises(TypeError):
            z = matrix_analysis.matrix.matrix([(1, 1, "Abc")])  # type: ignore
        with self.assertRaises(ValueError):
            z = matrix_analysis.matrix.matrix([])
        with self.assertRaises(ValueError):
            z = matrix_analysis.matrix.matrix([[]])
        with self.assertRaises(TypeError):
            z = matrix_analysis.matrix.matrix([[[]]])  # type: ignore
        for D in numpy_dtypes:
            x = matrix_analysis.matrix.matrix(numpy.array([[1]], dtype=D))
            self.assertEqual(str(x), "[1+0i]")
        with self.assertRaises(ValueError):
            x = matrix_analysis.matrix.matrix(
                numpy.array([[1]], dtype=numpy.timedelta64))
        x = matrix_analysis.matrix.matrix(
            numpy.array([[1, 2.0], [Unsure, 3+4j], [5, matrix_analysis.var.variable(1+1j)]], dtype=object))
        self.assertEqual(
            str(x), "[1+0i\t2+0i]\n[undefined\t3+4i]\n[5+0i\t1+1i]")
        with self.assertRaises(TypeError):
            x = matrix_analysis.matrix.matrix(
                numpy.array([["abc"]], dtype=object))
        with self.assertRaises(TypeError):
            x = matrix_analysis.matrix.matrix(
                "abd", 32, "fd", 4553.54)  # type: ignore

    def test_repr(self):
        x = matrix_analysis.matrix.matrix(
            [[1, 2.0, Unsure], [Unsure, 3+4j], [5, matrix_analysis.var.variable(1+1j)]])
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
        matrix_analysis.matrix.set_printarea()
        self.assertEqual(str(
            x), "[1+0i\t2+0i\tundefined]\n[undefined\t3+4i\tundefined]\n[5+0i\t1+1i\tundefined]")
        matrix_analysis.matrix.set_fastprint(False)

    def test_len(self):
        x = matrix_analysis.matrix.matrix(
            [[1, 2.0, Unsure], [Unsure, 3+4j], [5, matrix_analysis.var.variable(1+1j)]])
        self.assertEqual(len(x), 9)

    def test_getitem(self):
        x = matrix_analysis.matrix.matrix(
            [[1, 2.0, Unsure], [Unsure, 3+4j], [5, matrix_analysis.var.variable(1+1j)]])
        a = x[0, 0]
        self.assertAlmostEqual(a, 1)
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
            a = x["abc"]  # type: ignore
        with self.assertRaises(IndexError):
            a = x[1, 2, :]  # type: ignore
        with self.assertRaises(TypeError):
            a = x[1, "abc"]  # type: ignore
        y = x[...]
        self.assertEqual(str(x), str(y))
        self.assertNotEqual(id(x), id(y))

    def test_setitem(self):
        x = matrix_analysis.matrix.matrix(4, 4, fill=0)
        with self.assertRaises(TypeError):
            x["abc"] = matrix_analysis.matrix.matrix([[1]])  # type: ignore
        with self.assertRaises(IndexError):
            x[:, :, :] = matrix_analysis.matrix.matrix([[1]])  # type: ignore
        x[0:3, 0:4:2] = matrix_analysis.matrix.matrix([[1, 2], [3, 4], [5, 6]])
        self.assertEqual(str(x), str(
            matrix_analysis.matrix.matrix([[1, 0, 2, 0], [3, 0, 4, 0], [5, 0, 6, 0], [0, 0, 0, 0]])))
        with self.assertRaises(IndexError):
            x[4:5, 4:5] = matrix_analysis.matrix.matrix([[1]])
        with self.assertRaises(matrix_analysis.ShapeError):
            x[0:3, 0:4:2] = matrix_analysis.matrix.matrix([[1, 2], [3, 4]])
        x = matrix_analysis.matrix.matrix(4, 4, fill=0)
        x[0:3, 0:4:2] = [[1, 2], [3, Unsure], [5]]
        self.assertEqual(str(x), str(matrix_analysis.matrix.matrix([[1, 0, 2, 0], [
                         3, 0, Unsure, 0], [5, 0, 0, 0], [0, 0, 0, 0]])))
        with self.assertRaises(ValueError):
            x[0:1, 0] = [[1], (2)]
        with self.assertRaises(ValueError):
            x[0:1, 0] = [[1], ["abc"]]
        with self.assertRaises(matrix_analysis.ShapeError):
            x[1:2, 0] = [[1], [2, 3]]
        y = str(matrix_analysis.matrix.matrix(
            [[1, 0, 1, 0], [0, 0, 0, 0], [1, 0, 1, 0], [0, 0, 0, 0]]))
        for D in numpy_dtypes:
            x = matrix_analysis.matrix.matrix(4, 4, fill=0)
            x[0:4:2, 0:4:2] = numpy.ones((2, 2), dtype=D)
            self.assertEqual(str(x), y)
        x = matrix_analysis.matrix.matrix(4, 4, fill=0)
        with self.assertRaises(ValueError):
            x[0, 0] = matrix_analysis.matrix.matrix(
                numpy.array([[1]], dtype=numpy.timedelta64))
        x[1:4:2, 0:4:2] = numpy.array(
            [[Unsure, 1], [2.0, -1-1j]], dtype=object)
        self.assertEqual(str(x), str(matrix_analysis.matrix.matrix(
            [[0, 0, 0, 0], [Unsure, 0, 1, 0], [0, 0, 0, 0], [2.0, 0, -1-1j, 0]])))
        with self.assertRaises(TypeError):
            x[0, 0] = numpy.array([["abc"]], dtype=object)
        x = matrix_analysis.matrix.matrix(2, 2, fill=0)
        x[0, 0] = Unsure
        self.assertEqual(str(x), str(
            matrix_analysis.matrix.matrix([[Unsure, 0], [0, 0]])))
        x = matrix_analysis.matrix.matrix(2, 2, fill=0)
        x[:, 0] = Unsure
        self.assertEqual(str(x), str(
            matrix_analysis.matrix.matrix([[Unsure, 0], [Unsure, 0]])))
        x = matrix_analysis.matrix.matrix(2, 2, fill=0)
        x[0, :] = Unsure
        self.assertEqual(str(x), str(
            matrix_analysis.matrix.matrix([[Unsure, Unsure], [0, 0]])))
        x = matrix_analysis.matrix.matrix(2, 2, fill=0)
        x[:, :] = Unsure
        self.assertEqual(str(x), str(
            matrix_analysis.matrix.matrix([[Unsure, Unsure], [Unsure, Unsure]])))
        with self.assertRaises(IndexError):
            x[3, 3] = Unsure
        x = matrix_analysis.matrix.matrix(2, 2, fill=0)
        x[...] = [(0, 1, Unsure), (1, 0, 1+1j)]
        self.assertEqual(str(x), str(
            matrix_analysis.matrix.matrix([[0, Unsure], [1+1j, 0]])))
        with self.assertRaises(TypeError):
            x[...] = "abc"  # type: ignore
        with self.assertRaises(TypeError):
            x[...] = ["abc"]  # type: ignore
        with self.assertRaises(ValueError):
            x[...] = [(1, 2)]  # type: ignore
        with self.assertRaises(ValueError):
            x[...] = [("abc", 2, 3)]  # type: ignore
        with self.assertRaises(TypeError):
            x[...] = [(1, 2, "abc")]  # type: ignore

    def test_members(self):
        x = matrix_analysis.matrix.matrix(5, 10)
        self.assertEqual(x.rows, 5)
        with self.assertRaises(AttributeError):
            x.rows = 2
        self.assertEqual(x.cols, 10)
        with self.assertRaises(AttributeError):
            x.cols = 2
        self.assertEqual(x.total, 50)
        with self.assertRaises(AttributeError):
            x.total = 2

    def test_shape(self):
        x = matrix_analysis.matrix.matrix(3, 5)
        self.assertEqual(x.shape, (3, 5))

    def test_rank(self):
        x = matrix_analysis.matrix.matrix(
            [[1, -2, 2, -1], [2, -4, 8, 0], [-2, 4, -2, 3], [3, -6, 0, -6]])
        self.assertEqual(x.rank, 2)
        x = matrix_analysis.matrix.matrix(
            [[1, -2, 2, -1, 1], [2, -4, 8, 0, 2], [-2, 4, -2, 3, 3], [3, -6, 0, -6, 4]])
        self.assertEqual(x.rank, 3)
        x = matrix_analysis.matrix.matrix([[1, 1j], [1j, 1]])
        self.assertEqual(x.rank, 2)
        x = matrix_analysis.matrix.matrix([[1, Unsure], [0, 1]])
        self.assertEqual(x.rank, 2)
        x = matrix_analysis.matrix.matrix([[1, 0], [0, Unsure]])
        self.assertEqual(x.rank, Unsure)

    def test_add(self):
        x = matrix_analysis.matrix.matrix([[1, 2], [3, 4]])
        y = matrix_analysis.matrix.matrix(
            [[1, -1.0], [Unsure, 1j]])
        self.assertMatrixAlmostEqual(
            x+y, z := matrix_analysis.matrix.matrix([[2, 1], [Unsure, 4+1j]]))
        with self.assertRaises(matrix_analysis.ShapeError):
            x+matrix_analysis.matrix.matrix(1, 2)  # type: ignore
        with self.assertRaises(TypeError):
            x+1  # type: ignore
        with self.assertRaises(TypeError):
            1+x  # type: ignore
        a = id(x)
        x += y
        self.assertMatrixAlmostEqual(x, z)
        self.assertEqual(a, id(x))
        self.assertEqual(sys.getrefcount(x), 2)
        with self.assertRaises(matrix_analysis.ShapeError):
            x += matrix_analysis.matrix.matrix([[Unsure]])

    def test_sub(self):
        x = matrix_analysis.matrix.matrix([[1, 2], [3, 4]])
        y = matrix_analysis.matrix.matrix(
            [[1, -1.0], [Unsure, 1j]])
        self.assertMatrixAlmostEqual(
            x-y, z := matrix_analysis.matrix.matrix([[0, 3], [Unsure, 4-1j]]))
        with self.assertRaises(matrix_analysis.ShapeError):
            x-matrix_analysis.matrix.matrix(1, 2)  # type: ignore
        with self.assertRaises(TypeError):
            x-1  # type: ignore
        with self.assertRaises(TypeError):
            1-x  # type: ignore
        a = id(x)
        x -= y
        self.assertMatrixAlmostEqual(x, z)
        self.assertEqual(a, id(x))
        self.assertEqual(sys.getrefcount(x), 2)
        with self.assertRaises(matrix_analysis.ShapeError):
            x -= matrix_analysis.matrix.matrix([[Unsure]])

    def test_mul(self):
        x = matrix_analysis.matrix.matrix([[1, 2], [3, 4]])
        self.assertMatrixAlmostEqual(
            x*2, z := matrix_analysis.matrix.matrix([[2, 4], [6, 8]]))
        self.assertMatrixAlmostEqual(
            Unsure*x, matrix_analysis.matrix.matrix([[Unsure, Unsure], [Unsure, Unsure]]))
        with self.assertRaises(TypeError):
            x*"abc"  # type: ignore
        with self.assertRaises(TypeError):
            "abc"*x  # type: ignore
        a = id(x)
        x *= 2
        self.assertMatrixAlmostEqual(x, z)
        self.assertEqual(a, id(x))
        self.assertEqual(sys.getrefcount(x), 2)

    def test_div(self):
        x = matrix_analysis.matrix.matrix([[15, 18], [Unsure, 12+13j]])
        self.assertMatrixAlmostEqual(
            x/(1+1j), z1 := matrix_analysis.matrix.matrix([[7.5-7.5j, 9-9j], [Unsure, 12.5+0.5j]]))
        self.assertMatrixAlmostEqual(
            x//(1+1j), z2 := matrix_analysis.matrix.matrix([[8-8j, 9-9j], [Unsure, 13+1j]]))
        self.assertMatrixAlmostEqual(
            x % (1+1j), z3 := matrix_analysis.matrix.matrix([[-1, 0], [Unsure, -1j]]))
        y = divmod(x, 1+1j)
        self.assertMatrixAlmostEqual(y[0], z2)
        self.assertMatrixAlmostEqual(y[1], z3)
        self.assertMatrixAlmostEqual((100+99j)/x, matrix_analysis.matrix.matrix(
            [[6.6666666667+6.6j, 5.555555556+5.5j], [Unsure, 7.945686901-0.357827476j]]))
        self.assertMatrixAlmostEqual((100+99j)//x, a := matrix_analysis.matrix.matrix(
            [[7+7j, 6+6j], [Unsure, 8]]))
        self.assertMatrixAlmostEqual(
            (100+99j) % x, b := matrix_analysis.matrix.matrix([[-5-6j, -8-9j], [Unsure, 4-5j]]))
        y = divmod(100+99j, x)
        self.assertMatrixAlmostEqual(y[1], b)
        x1 = +x
        a = id(x1)
        x1 /= 1+1j
        self.assertMatrixAlmostEqual(x1, z1)
        self.assertEqual(a, id(x1))
        self.assertEqual(sys.getrefcount(x1), 2)
        x1 = +x
        a = id(x1)
        x1 //= 1+1j
        self.assertMatrixAlmostEqual(x1, z2)
        self.assertEqual(a, id(x1))
        self.assertEqual(sys.getrefcount(x1), 2)
        x1 = +x
        a = id(x1)
        x1 %= 1+1j
        self.assertMatrixAlmostEqual(x1, z3)
        self.assertEqual(a, id(x1))
        self.assertEqual(sys.getrefcount(x1), 2)

    def test_matmul(self):
        x = matrix_analysis.matrix.matrix([[1, 0], [3, 4]])
        y = matrix_analysis.matrix.matrix(
            [[1, -1.0], [matrix_analysis.Unsure, 1j]])
        self.assertMatrixAlmostEqual(
            x@y, matrix_analysis.matrix.matrix([[1, -1], [Unsure, -3+4j]])
        )
        with self.assertRaises(matrix_analysis.ShapeError):
            a = matrix_analysis.matrix.matrix(2, 3)
            b = matrix_analysis.matrix.matrix(4, 2)
            a@b  # type: ignore
        with self.assertRaises(TypeError):
            "abc"@x  # type: ignore
        with self.assertRaises(TypeError):
            x@"abc"  # type: ignore

    def test_unary(self):
        x = matrix_analysis.matrix.matrix([[1, 0], [3+1j, Unsure]])
        y = +x
        self.assertMatrixAlmostEqual(y, x)
        z = -x
        self.assertMatrixAlmostEqual(
            z, matrix_analysis.matrix.matrix([[-1, 0], [-3-1j, Unsure]]))

    def test_ufuncs(self):
        x = matrix_analysis.matrix.matrix([[1, 2], [Unsure, 1j], [2j, Unsure]])
        self.assertMatrixAlmostEqual(matrix_analysis.funcs.conj(
            x), matrix_analysis.matrix.matrix([[1, 2], [Unsure, -1j], [-2j, Unsure]]))

    def test_otherfuncs(self):
        x = matrix_analysis.matrix.matrix([[1, 2], [Unsure, 1j], [2j, Unsure]])
        self.assertMatrixAlmostEqual(x.T(), matrix_analysis.matrix.matrix([
                                     [1, Unsure, 2j], [2, 1j, Unsure]]))
        self.assertMatrixAlmostEqual(x.H(), matrix_analysis.matrix.matrix([
                                     [1, Unsure, -2j], [2, -1j, Unsure]]))
