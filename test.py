import unittest
import matrix_analysis
import math


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

    def test_add(self):
        x = matrix_analysis.var.variable(1+2j)
        y = matrix_analysis.var.variable(2+3j)
        z = x+y
        self.assertTupleAlmostEqual(z.rec, (3, 5))


if __name__ == "__main__":
    unittest.main(testLoader=seqTestLoader())
