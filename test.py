import unittest
import matrix_analysis
import math
import sys


class externed_Testcase(unittest.TestCase):
    def assertTupleAlmostEqual(self, first, second, places=None, msg=None, delta=None):
        self.assertEqual(len(first), len(second))
        for _ in range(len(first)):
            self.assertAlmostEqual(first[_], second[_], places, msg, delta)


class seqTestLoader(unittest.TestLoader):
    def getTestCaseNames(self, testcase_class):
        test_names = super().getTestCaseNames(testcase_class)
        testcase_methods = list(testcase_class.__dict__.keys())
        test_names.sort(key=testcase_methods.index)
        return test_names


class Test_var(externed_Testcase):
    def test_init(self):
        x = matrix_analysis.var.variable()
        y1 = matrix_analysis.var.variable(1)
        y2 = matrix_analysis.var.variable(num=1.2)
        y3 = matrix_analysis.var.variable(1+2j)
        z1 = matrix_analysis.var.variable(1, 3.2)
        z2 = matrix_analysis.var.variable(real=2, imag=1)
        with self.assertRaises(ValueError):
            y4 = matrix_analysis.var.variable("abc")
        with self.assertRaises(ValueError):
            z3 = matrix_analysis.var.variable(1, 2, 3)

    def test_repr(self):
        x = matrix_analysis.var.variable(-1.23456789, 2.34567891)
        self.assertEqual(repr(x), "-1.234568+2.345679j")
        matrix_analysis.var.set_print_precision(-10)
        self.assertEqual(repr(x), "-1+2j")
        matrix_analysis.var.set_print_precision(3)
        self.assertEqual(repr(x), "-1.23+2.35j")
        matrix_analysis.var.set_print_precision(20)
        self.assertEqual(
            repr(x), "-1.2345678899999998901+2.3456789100000001724j")
        self.assertEqual(
            str(x), "-1.2345678899999998901+2.3456789100000001724i")
        matrix_analysis.var.set_print_precision(7)
        y = matrix_analysis.var.variable()
        self.assertEqual(repr(y), "None")
        self.assertEqual(str(y), "undefined")

    def test_members(self):
        x = matrix_analysis.var.variable()
        self.assertTrue(x.is_arbitrary)
        with self.assertRaises(matrix_analysis.var.Undefined):
            print(x.length)
        with self.assertRaises(matrix_analysis.var.Undefined):
            print(x.arg)
        with self.assertRaises(matrix_analysis.var.Undefined):
            print(x.rec)
        with self.assertRaises(matrix_analysis.var.Undefined):
            print(x.pol)
        with self.assertRaises(matrix_analysis.var.Undefined):
            x.length = 1
        with self.assertRaises(matrix_analysis.var.Undefined):
            x.arg = 1
        with self.assertRaises(ValueError):
            x.rec = 1
        with self.assertRaises(ValueError):
            x.pol = "abc"
        y = matrix_analysis.var.variable(1+2j)
        self.assertFalse(y.is_arbitrary)
        self.assertAlmostEqual(y.real, 1)
        self.assertAlmostEqual(y.imag, 2)
        self.assertAlmostEqual(y.length, 5**0.5)
        self.assertAlmostEqual(y.arg, math.atan2(2, 1))
        self.assertTupleAlmostEqual(y.rec, (1, 2))
        self.assertTupleAlmostEqual(y.pol, (5**0.5, math.atan2(2, 1)))
        x.rec = (1, 1)
        self.assertFalse(x.is_arbitrary)
        self.assertAlmostEqual(x.length, 2**0.5)
        self.assertAlmostEqual(x.arg, math.pi/4)
        self.assertTupleAlmostEqual(x.pol, (2**0.5, math.pi/4))
        matrix_analysis.var.set_arg_format("deg")
        self.assertAlmostEqual(x.arg, 45)
        x.pol = (2, 60)
        self.assertTupleAlmostEqual(x.rec, (1, 3**0.5))
        matrix_analysis.var.set_arg_format("rad")
        x.pol = (4, math.pi/3*2)
        self.assertTupleAlmostEqual(x.rec, (-2, 2*3**0.5))

    def test_cmp(self):
        x = matrix_analysis.var.variable()
        x.real = 0
        x.imag = 0
        y = matrix_analysis.var.variable()
        y.real = 0
        y.imag = 0
        self.assertFalse(x == y)
        self.assertFalse(x != y)
        x.is_arbitrary = False
        y.is_arbitrary = False
        self.assertTrue(x == y)
        self.assertFalse(x != y)
        z = matrix_analysis.var.variable(1)
        self.assertFalse(x == z)
        self.assertTrue(x != z)
        with self.assertRaises(TypeError):
            x < y
        with self.assertRaises(TypeError):
            x < "abc"
        self.assertEqual(z, 1)
        self.assertEqual(z, 1.0)
        w = matrix_analysis.var.variable(1-1j)
        self.assertEqual(w, 1-1j)

    def test_add(self):
        x = matrix_analysis.var.variable(1+2j)
        y = matrix_analysis.var.variable(2+3j)
        z = x+y
        self.assertTupleAlmostEqual(z.rec, (3, 5))
        k = id(x)
        x += y
        self.assertEqual(id(x), k)
        self.assertTupleAlmostEqual(z.rec, x.rec)

    def test_sub(self):
        x = matrix_analysis.var.variable(1+2j)
        y = matrix_analysis.var.variable(2+3j)
        z = x-y
        self.assertTupleAlmostEqual(z.rec, (-1, -1))
        k = id(x)
        x -= y
        self.assertEqual(id(x), k)
        self.assertTupleAlmostEqual(z.rec, x.rec)

    def test_sub(self):
        x = matrix_analysis.var.variable(1+2j)
        y = matrix_analysis.var.variable(2+3j)
        z = x*y
        self.assertTupleAlmostEqual(z.rec, (-4, 7))
        k = id(x)
        x *= y
        self.assertEqual(id(x), k)
        self.assertTupleAlmostEqual(z.rec, x.rec)

    def test_div(self):
        x = matrix_analysis.var.variable(1+2j)
        y = matrix_analysis.var.variable(2+3j)
        z1 = x/y
        self.assertTupleAlmostEqual(z1.rec, (0.6153846154, 0.07692307692))
        z2 = y//x
        self.assertTupleAlmostEqual(z2.rec, (1, -1))
        z3 = y % x
        self.assertTupleAlmostEqual(z3.rec, (-1, 2))
        z4 = divmod(y, x)
        self.assertEqual(sys.getrefcount(z4[0]), 2)
        self.assertEqual(sys.getrefcount(z4[1]), 2)
        self.assertTupleAlmostEqual(z4[0].rec, (1, -1))
        self.assertTupleAlmostEqual(z4[1].rec, (-1, 2))
        x1 = matrix_analysis.var.variable(1+2j)
        k1 = id(x1)
        x1 /= y
        self.assertEqual(id(x1), k1)
        self.assertTupleAlmostEqual(z1.rec, x1.rec)
        x2 = matrix_analysis.var.variable(2+3j)
        k2 = id(x2)
        x2 //= x
        self.assertEqual(id(x2), k2)
        x3 = matrix_analysis.var.variable(2+3j)
        k3 = id(x3)
        x3 %= x
        self.assertEqual(id(x3), k3)
        self.assertTupleAlmostEqual(z3.rec, x3.rec)

    def test_pow(self):
        x = matrix_analysis.var.variable(1+2j)
        y = matrix_analysis.var.variable(2+3j)
        z = x**y
        self.assertTupleAlmostEqual(z.rec, (-0.0151326724227, -0.179867483913))
        a = matrix_analysis.var.variable(7, 9)
        b = matrix_analysis.var.variable(5, 3)
        c = matrix_analysis.var.variable(2, 3)
        d = pow(a, b, c)
        self.assertTupleAlmostEqual(d.rec, (-0.40462543, 3.33048064))
        k = id(x)
        x **= y
        self.assertEqual(k, id(x))
        self.assertTupleAlmostEqual(z.rec, x.rec)
        k = id(a)
        a.__ipow__(b, c)
        self.assertEqual(k, id(a))
        self.assertTupleAlmostEqual(a.rec, d.rec)

    def test_unaryfuncs(self):
        x = matrix_analysis.var.variable(1+2j)
        y = matrix_analysis.var.variable()
        a = -x
        A = -y
        self.assertTupleAlmostEqual(a.rec, (-1, -2))
        self.assertTrue(A.is_arbitrary)
        b = +x
        B = +y
        self.assertTupleAlmostEqual(b.rec, (1, 2))
        self.assertTrue(B.is_arbitrary)
        c = abs(x)
        C = abs(y)
        self.assertAlmostEqual(c, 5**0.5)
        self.assertTrue(math.isnan(C))

    def test_bool(self):
        x = matrix_analysis.var.variable()
        y = matrix_analysis.var.variable(0, 0)
        z = matrix_analysis.var.variable(1, 1)
        self.assertTrue(bool(x))
        self.assertFalse(bool(y))
        self.assertTrue(bool(z))


if __name__ == "__main__":
    unittest.main(testLoader=seqTestLoader())
