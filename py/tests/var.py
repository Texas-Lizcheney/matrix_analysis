from .Testcase import externed_Testcase
import matrix_analysis
import math
import sys
Unsure = matrix_analysis.Unsure


class Test_errordouble(externed_Testcase):
    def test_init(self):
        x = matrix_analysis.var.errordouble(value=1.0)
        y = matrix_analysis.var.errordouble(2, 1.5)
        with self.assertRaises(TypeError):
            z = matrix_analysis.var.errordouble("abc")

    def test_repr(self):
        x = matrix_analysis.var.errordouble(1)
        self.assertEqual(repr(x), "1")
        matrix_analysis.var.set_print_error(True)
        self.assertEqual(repr(x), "1(±2.220446e-16)")
        matrix_analysis.var.set_print_precision(20)
        self.assertEqual(repr(x), "1(±2.2204460492503130808e-16)")
        matrix_analysis.var.set_print_error(None)
        x = matrix_analysis.var.errordouble(1.23456789)
        matrix_analysis.var.set_print_precision(2)
        self.assertEqual(repr(x), "1.2")
        matrix_analysis.var.set_print_precision(None)

    def test_add(self):
        x = matrix_analysis.var.errordouble(0.1)
        y = matrix_analysis.var.errordouble(0.2)
        self.assertAlmostEqual(x+y, 0.3)
        self.assertAlmostEqual(x+1.1, 1.2)
        self.assertAlmostEqual(x+1, 1.1)
        self.assertAlmostEqual(1.1+x, 1.2)
        self.assertAlmostEqual(1+x, 1.1)
        with self.assertRaises(TypeError):
            x+"abc"
        with self.assertRaises(TypeError):
            "abc"+x
        k = id(x)
        x += y
        self.assertAlmostEqual(x, 0.3)
        self.assertEqual(k, id(x))
        self.assertEqual(sys.getrefcount(x), 2)
        with self.assertRaises(TypeError):
            x += "abc"

    def test_sub(self):
        x = matrix_analysis.var.errordouble(0.1)
        y = matrix_analysis.var.errordouble(0.2)
        self.assertAlmostEqual(x-y, -0.1)
        self.assertAlmostEqual(x-1.1, -1)
        self.assertAlmostEqual(x-1, -0.9)
        self.assertAlmostEqual(1.1-x, 1)
        self.assertAlmostEqual(1-x, 0.9)
        with self.assertRaises(TypeError):
            x-"abc"
        with self.assertRaises(TypeError):
            "abc"-x
        k = id(x)
        x -= y
        self.assertAlmostEqual(x, -0.1)
        self.assertEqual(k, id(x))
        self.assertEqual(sys.getrefcount(x), 2)
        with self.assertRaises(TypeError):
            x -= "abc"

    def test_mul(self):
        x = matrix_analysis.var.errordouble(0.1)
        y = matrix_analysis.var.errordouble(0.2)
        self.assertAlmostEqual(x*y, 0.02)
        self.assertAlmostEqual(x*1.1, 0.11)
        self.assertAlmostEqual(x*2, 0.2)
        self.assertAlmostEqual(1.1*x, 0.11)
        self.assertAlmostEqual(2*x, 0.2)
        with self.assertRaises(TypeError):
            x*"abc"
        with self.assertRaises(TypeError):
            "abc"*x
        k = id(x)
        x *= y
        self.assertAlmostEqual(x, 0.02)
        self.assertEqual(k, id(x))
        self.assertEqual(sys.getrefcount(x), 2)
        with self.assertRaises(TypeError):
            x *= "abc"

    def test_div(self):
        x = matrix_analysis.var.errordouble(0.1)
        y = matrix_analysis.var.errordouble(0.2)
        self.assertAlmostEqual(x/y, 0.5)
        self.assertAlmostEqual(x/2.0, 0.05)
        self.assertAlmostEqual(x/2, 0.05)
        self.assertAlmostEqual(1/x, 10)
        self.assertAlmostEqual(1.0/x, 10)
        with self.assertRaises(TypeError):
            x/"abc"
        with self.assertRaises(TypeError):
            "abc"/x
        k = id(x)
        x /= y
        self.assertAlmostEqual(x, 0.5)
        self.assertEqual(k, id(x))
        self.assertEqual(sys.getrefcount(x), 2)
        with self.assertRaises(TypeError):
            x *= "abc"

    def test_floor_div(self):
        x = matrix_analysis.var.errordouble(0.1)
        y = matrix_analysis.var.errordouble(0.2)
        self.assertAlmostEqual(x//y, 0)
        self.assertAlmostEqual(x//2.0, 0)
        self.assertAlmostEqual(x//2, 0)
        self.assertAlmostEqual(1.05//x, 10)
        self.assertAlmostEqual(1//x, 10)
        with self.assertRaises(TypeError):
            x//"abc"
        with self.assertRaises(TypeError):
            "abc"//x
        k = id(x)
        x //= y
        self.assertAlmostEqual(x, 0)
        self.assertEqual(k, id(x))
        self.assertEqual(sys.getrefcount(x), 2)
        with self.assertRaises(TypeError):
            x //= "abc"

    def test_mod(self):
        x = matrix_analysis.var.errordouble(0.1)
        y = matrix_analysis.var.errordouble(0.21)
        self.assertAlmostEqual(x % y, 0.1)
        self.assertAlmostEqual(x % 2.0, 0.1)
        self.assertAlmostEqual(x % 2, 0.1)
        self.assertAlmostEqual(1.05 % x, 0.05)
        self.assertAlmostEqual(1 % x, 0)
        with self.assertRaises(TypeError):
            x % "abc"
        with self.assertRaises(TypeError):
            "abc" % x
        k = id(x)
        x %= 0.03
        self.assertAlmostEqual(x, 0.01)
        self.assertEqual(k, id(x))
        self.assertEqual(sys.getrefcount(x), 2)
        with self.assertRaises(TypeError):
            x %= "abc"

    def test_divmod(self):
        x = matrix_analysis.var.errordouble(0.1)
        y = matrix_analysis.var.errordouble(0.21)
        z = divmod(y, x)
        self.assertAlmostEqual(z[0], 2)
        self.assertAlmostEqual(z[1], 0.01)
        with self.assertRaises(TypeError):
            divmod(y, "abc")
        with self.assertRaises(TypeError):
            divmod("abc", x)

    def test_pow(self):
        x = matrix_analysis.var.errordouble(0.1)
        y = matrix_analysis.var.errordouble(0.2)
        self.assertAlmostEqual(x**y, 0.1**0.2)
        self.assertAlmostEqual(x**2.0, 0.1**2.0)
        self.assertAlmostEqual(x**3, 0.1**3)
        self.assertAlmostEqual(1.05**x, 1.05**0.1)
        self.assertAlmostEqual(2**x, 2**0.1)
        with self.assertRaises(TypeError):
            x**"abc"
        with self.assertRaises(TypeError):
            "abc"**x
        k = id(x)
        x **= 0.03
        self.assertAlmostEqual(x, 0.1**0.03)
        self.assertEqual(k, id(x))
        self.assertEqual(sys.getrefcount(x), 2)
        with self.assertRaises(TypeError):
            x **= "abc"
        x = matrix_analysis.var.errordouble(4.4)
        y = matrix_analysis.var.errordouble(2.2)
        z = matrix_analysis.var.errordouble(3.3)
        self.assertAlmostEqual(pow(x, y, z), 2.9372964773)
        self.assertAlmostEqual(pow(x, 2.2, z), 2.9372964773)
        self.assertAlmostEqual(pow(x, y, 3.3), 2.9372964773)
        self.assertAlmostEqual(pow(x, 2.2, 3.3), 2.9372964773)
        with self.assertRaises(TypeError):
            pow(x, "abc", x)
        with self.assertRaises(TypeError):
            pow(x, x, "abc")

    def unary_test(self):
        x = matrix_analysis.var.errordouble(0.1)
        self.assertAlmostEqual(+x, 0.1)
        self.assertNotEqual(id(+x), id(x))
        self.assertAlmostEqual(-x, -0.1)
        self.assertAlmostEqual(abs(x), 0.1)
        self.assertAlmostEqual(abs(-x), 0.1)
        y = matrix_analysis.var.errordouble(0)
        z = matrix_analysis.var.errordouble(1)
        self.assertTrue(bool(z))
        self.assertFalse(bool(y))


class Test_var(externed_Testcase):
    def test_init(self):
        x = matrix_analysis.var.variable()
        y1 = matrix_analysis.var.variable(1)
        y2 = matrix_analysis.var.variable(num=1.2)
        y3 = matrix_analysis.var.variable(1+2j)
        z1 = matrix_analysis.var.variable(1, 3.2)
        z2 = matrix_analysis.var.variable(real=2, imag=1)
        z3 = matrix_analysis.var.variable(1, arg=1)
        with self.assertRaises(TypeError):
            y4 = matrix_analysis.var.variable("abc")
        with self.assertRaises(TypeError):
            z3 = matrix_analysis.var.variable(1, 2, 3)

    def test_repr(self):
        x = matrix_analysis.var.variable(-1.23456789, 2.34567891)
        self.assertEqual(repr(x), "-1.234568+2.345679j")
        self.assertEqual(str(x), "-1.234568+2.345679i")
        y = matrix_analysis.var.variable()
        self.assertEqual(repr(y), "Unsure")
        self.assertEqual(str(y), "undefined")

    def test_members(self):
        x = matrix_analysis.var.variable()
        self.assertTrue(x.is_arbitrary)
        with self.assertRaises(matrix_analysis.Undefined):
            print(x.r)
        with self.assertRaises(matrix_analysis.Undefined):
            print(x.arg)
        with self.assertRaises(matrix_analysis.Undefined):
            print(x.rec)
        with self.assertRaises(matrix_analysis.Undefined):
            print(x.pol)
        with self.assertRaises(matrix_analysis.Undefined):
            x.r = 1
        with self.assertRaises(matrix_analysis.Undefined):
            x.arg = 1
        with self.assertRaises(ValueError):
            x.rec = 1
        with self.assertRaises(ValueError):
            x.pol = "abc"
        y = matrix_analysis.var.variable(1+2j)
        self.assertFalse(y.is_arbitrary)
        self.assertAlmostEqual(y.real, 1)
        self.assertAlmostEqual(y.imag, 2)
        self.assertAlmostEqual(y.r, 5**0.5)
        self.assertAlmostEqual(y.arg, math.atan2(2, 1))
        self.assertTupleAlmostEqual(y.rec, (1, 2))
        self.assertTupleAlmostEqual(y.pol, (5**0.5, math.atan2(2, 1)))
        x.rec = (1, 1)
        self.assertFalse(x.is_arbitrary)
        self.assertAlmostEqual(x.r, 2**0.5)
        self.assertAlmostEqual(x.arg, math.pi/4)
        self.assertTupleAlmostEqual(x.pol, (2**0.5, math.pi/4))
        matrix_analysis.var.set_arg_format("deg")
        self.assertAlmostEqual(x.arg, 45)
        x.pol = (2, 60)
        self.assertTupleAlmostEqual(x.rec, (1, 3**0.5))
        matrix_analysis.var.set_arg_format("rad")
        x.pol = (4, math.pi/3*2)
        self.assertTupleAlmostEqual(x.rec, (-2, 2*3**0.5))
        x.real = matrix_analysis.var.errordouble(1)
        self.assertAlmostEqual(x.real, 1)
        x.real.value = 2
        self.assertAlmostEqual(x.real.value, 2)
        x.real.error = 3
        self.assertAlmostEqual(x.real.error, 3)
        x.imag = matrix_analysis.var.errordouble(2)
        self.assertAlmostEqual(x.imag, 2)
        x.imag.value = 3
        self.assertAlmostEqual(x.imag.value, 3)
        x.imag.error = 4
        self.assertAlmostEqual(x.imag.error, 4)

    def test_cmp(self):
        x = matrix_analysis.var.variable()
        x.real = 0
        x.imag = 0
        y = matrix_analysis.var.variable()
        y.real = 0
        y.imag = 0
        self.assertUnsure(x == y)
        self.assertUnsure(x != y)
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
        self.assertFalse(z.is_arbitrary)
        self.assertTupleAlmostEqual(z.rec, (3, 5))
        k = id(x)
        x += y
        self.assertEqual(id(x), k)
        self.assertVarAlmostEqual(z, x)
        f = matrix_analysis.var.variable(Unsure)
        z = x+f
        self.assertTrue(z.is_arbitrary)
        self.assertVarAlmostEqual(x+1, 1+x)
        x+1
        x+1.1
        x+(1+1j)
        x+matrix_analysis.Unsure
        x+matrix_analysis.var.errordouble(1.1)
        1+x
        1.1+x
        (1+1j)+x
        matrix_analysis.Unsure+x
        matrix_analysis.var.errordouble(1.1)+x
        x += 1
        x += 1.1
        x += (1+1j)
        x += matrix_analysis.Unsure
        x += matrix_analysis.var.errordouble(1.1)
        with self.assertRaises(TypeError):
            x+"abc"
        with self.assertRaises(TypeError):
            "abc"+x
        with self.assertRaises(TypeError):
            x += "abc"

    def test_sub(self):
        x = matrix_analysis.var.variable(1+2j)
        y = matrix_analysis.var.variable(2+3j)
        z = x-y
        self.assertFalse(z.is_arbitrary)
        self.assertTupleAlmostEqual(z.rec, (-1, -1))
        k = id(x)
        x -= y
        self.assertEqual(id(x), k)
        self.assertVarAlmostEqual(z, x)
        f = matrix_analysis.var.variable(Unsure)
        z = x-f
        self.assertTrue(z.is_arbitrary)
        self.assertVarAlmostEqual(x-1, -(1-x))
        x-1
        x-1.1
        x-(1+1j)
        x-matrix_analysis.Unsure
        x-matrix_analysis.var.errordouble(1.1)
        1-x
        1.1-x
        (1+1j)-x
        matrix_analysis.Unsure-x
        matrix_analysis.var.errordouble(1.1)-x
        x -= 1
        x -= 1.1
        x -= (1+1j)
        x -= matrix_analysis.Unsure
        x -= matrix_analysis.var.errordouble(1.1)
        with self.assertRaises(TypeError):
            x-"abc"
        with self.assertRaises(TypeError):
            "abc"-x
        with self.assertRaises(TypeError):
            x -= "abc"

    def test_mul(self):
        x = matrix_analysis.var.variable(1+2j)
        y = matrix_analysis.var.variable(2+3j)
        z = x*y
        self.assertFalse(z.is_arbitrary)
        self.assertTupleAlmostEqual(z.rec, (-4, 7))
        k = id(x)
        x *= y
        self.assertEqual(id(x), k)
        self.assertVarAlmostEqual(z, x)
        f = matrix_analysis.var.variable(Unsure)
        z = x*f
        self.assertTrue(z.is_arbitrary)
        g = matrix_analysis.var.variable(0)
        z = g*f
        self.assertFalse(z.is_arbitrary)
        self.assertTupleAlmostEqual(z.rec, (0, 0))
        self.assertVarAlmostEqual(x*1, 1*x)
        x*1
        x*1.1
        x*(1+1j)
        x*matrix_analysis.Unsure
        x*matrix_analysis.var.errordouble(1.1)
        1*x
        1.1*x
        (1+1j)*x
        matrix_analysis.Unsure*x
        matrix_analysis.var.errordouble(1.1)*x
        x *= 1
        x *= 1.1
        x *= (1+1j)
        x *= matrix_analysis.Unsure
        x *= matrix_analysis.var.errordouble(1.1)
        with self.assertRaises(TypeError):
            x*"abc"
        with self.assertRaises(TypeError):
            "abc"*x
        with self.assertRaises(TypeError):
            x *= "abc"

    def test_div(self):
        x = matrix_analysis.var.variable(1+2j)
        y = matrix_analysis.var.variable(2+3j)
        z = x/y
        self.assertFalse(z.is_arbitrary)
        self.assertTupleAlmostEqual(z.rec, (0.615384615385, 7.69230769231e-2))
        k = id(x)
        x /= y
        self.assertEqual(id(x), k)
        self.assertTupleAlmostEqual(x.rec, (0.615384615385, 7.69230769231e-2))
        f = matrix_analysis.var.variable(Unsure)
        z = x/f
        self.assertTrue(z.is_arbitrary)
        g = matrix_analysis.var.variable(0)
        z = g/f
        self.assertFalse(z.is_arbitrary)
        self.assertTupleAlmostEqual(z.rec, (0, 0))
        self.assertVarAlmostEqual(x/1, ~(1/x))
        x/1
        x/1.1
        x/(1+1j)
        x/matrix_analysis.Unsure
        x/matrix_analysis.var.errordouble(1.1)
        1/x
        1.1/x
        (1+1j)/x
        matrix_analysis.Unsure/x
        matrix_analysis.var.errordouble(1.1)/x
        x /= 1
        x /= 1.1
        x /= (1+1j)
        x /= matrix_analysis.Unsure
        x /= matrix_analysis.var.errordouble(1.1)
        with self.assertRaises(TypeError):
            x/"abc"
        with self.assertRaises(TypeError):
            "abc"/x
        with self.assertRaises(TypeError):
            x /= "abc"

    def test_floor_div(self):
        x = matrix_analysis.var.variable(1+2j)
        y = matrix_analysis.var.variable(2+3j)
        z = x//y
        self.assertFalse(z.is_arbitrary)
        self.assertTupleAlmostEqual(z.rec, (1, 0))
        k = id(x)
        x //= y
        self.assertEqual(id(x), k)
        self.assertTupleAlmostEqual(x.rec, (1, 0))
        f = matrix_analysis.var.variable(Unsure)
        z = x//f
        self.assertTrue(z.is_arbitrary)
        g = matrix_analysis.var.variable(0)
        z = g//f
        self.assertFalse(z.is_arbitrary)
        self.assertTupleAlmostEqual(z.rec, (0, 0))
        self.assertTupleAlmostEqual(((4+3j)//y).rec, (1, 0))
        x//1
        x//1.1
        x//(1+1j)
        x//matrix_analysis.Unsure
        x//matrix_analysis.var.errordouble(1.1)
        1//x
        1.1//x
        (1+1j)//x
        matrix_analysis.Unsure//x
        matrix_analysis.var.errordouble(1.1)//x
        x //= 1
        x //= 1.1
        x //= (1+1j)
        x //= matrix_analysis.Unsure
        x //= matrix_analysis.var.errordouble(1.1)
        with self.assertRaises(TypeError):
            x//"abc"
        with self.assertRaises(TypeError):
            "abc"//x
        with self.assertRaises(TypeError):
            x //= "abc"

    def test_mod(self):
        x = matrix_analysis.var.variable(1+2j)
        y = matrix_analysis.var.variable(2+3j)
        z = x % y
        self.assertFalse(z.is_arbitrary)
        self.assertTupleAlmostEqual(z.rec, (-1, -1))
        k = id(x)
        x %= y
        self.assertEqual(id(x), k)
        self.assertTupleAlmostEqual(x.rec, (-1, -1))
        f = matrix_analysis.var.variable(Unsure)
        z = x % f
        self.assertTrue(z.is_arbitrary)
        g = matrix_analysis.var.variable(0)
        z = g % f
        self.assertFalse(z.is_arbitrary)
        self.assertTupleAlmostEqual(z.rec, (0, 0))
        self.assertTupleAlmostEqual(((4+3j) % y).rec, (2, 0))
        x % 1
        x % 1.1
        x % (1+1j)
        x % matrix_analysis.Unsure
        x % matrix_analysis.var.errordouble(1.1)
        1 % x
        1.1 % x
        (1+1j) % x
        matrix_analysis.Unsure % x
        matrix_analysis.var.errordouble(1.1) % x
        x %= 1
        x %= 1.1
        x %= (1+1j)
        x %= matrix_analysis.Unsure
        x %= matrix_analysis.var.errordouble(1.1)
        with self.assertRaises(TypeError):
            x % "abc"
        with self.assertRaises(TypeError):
            "abc" % x
        with self.assertRaises(TypeError):
            x %= "abc"

    def test_divmod(self):
        x = matrix_analysis.var.variable(1+2j)
        y = matrix_analysis.var.variable(2+3j)
        z = divmod(y, x)
        self.assertEqual(sys.getrefcount(z[0]), 2)
        self.assertEqual(sys.getrefcount(z[1]), 2)
        self.assertTupleAlmostEqual(z[0].rec, (2, 0))
        self.assertTupleAlmostEqual(z[1].rec, (0, -1))
        with self.assertRaises(TypeError):
            divmod(y, "abc")
        with self.assertRaises(TypeError):
            divmod("abc", x)

    def test_pow(self):
        x = matrix_analysis.var.variable(1+2j)
        y = matrix_analysis.var.variable(2+3j)
        z = x**y
        self.assertFalse(z.is_arbitrary)
        self.assertTupleAlmostEqual(z.rec, (-0.0151326724227, -0.179867483913))
        a = matrix_analysis.var.variable(7, 9)
        b = matrix_analysis.var.variable(5, 3)
        c = matrix_analysis.var.variable(2, 3)
        d = pow(a, b, c)
        self.assertTupleAlmostEqual(d.rec, (0.59537457, -1.66951936))
        k = id(x)
        x **= y
        self.assertEqual(k, id(x))
        self.assertVarAlmostEqual(z, x)
        k = id(a)
        a.__ipow__(b, c)
        self.assertEqual(k, id(a))
        self.assertVarAlmostEqual(a, d)
        self.assertTupleAlmostEqual((2**y).rec, (-1.94797767186, 3.4936203271))
        x**1
        x**1.1
        x**(1+1j)
        x**matrix_analysis.Unsure
        x**matrix_analysis.var.errordouble(1.1)
        1**x
        1.1**x
        (1+1j)**x
        matrix_analysis.Unsure**x
        matrix_analysis.var.errordouble(1.1)**x
        x **= 1
        x **= 1.1
        x **= (1+1j)
        x **= matrix_analysis.Unsure
        x **= matrix_analysis.var.errordouble(1.1)
        with self.assertRaises(TypeError):
            x**"abc"
        with self.assertRaises(TypeError):
            "abc"**x
        with self.assertRaises(TypeError):
            x **= "abc"
        pow(x, 1, 1)
        pow(x, 1.1, 1)
        pow(x, 1+1j, 1)
        pow(x, matrix_analysis.Unsure, 1)
        pow(x, matrix_analysis.var.errordouble(1.1), 1)
        pow(x, 1, 1.1)
        pow(x, 1.1, 1.1)
        pow(x, 1+1j, 1.1)
        pow(x, matrix_analysis.Unsure, 1.1)
        pow(x, matrix_analysis.var.errordouble(1.1), 1.1)
        pow(x, 1, 1+1j)
        pow(x, 1.1, 1+1j)
        pow(x, 1+1j, 1+1j)
        pow(x, matrix_analysis.Unsure, 1+1j)
        pow(x, matrix_analysis.var.errordouble(1.1), 1+1j)
        pow(x, 1, matrix_analysis.Unsure)
        pow(x, 1.1, matrix_analysis.Unsure)
        pow(x, 1+1j, matrix_analysis.Unsure)
        pow(x, matrix_analysis.Unsure, matrix_analysis.Unsure)
        pow(x, matrix_analysis.var.errordouble(1.1), matrix_analysis.Unsure)
        pow(x, 1, matrix_analysis.var.errordouble(1.1))
        pow(x, 1.1, matrix_analysis.var.errordouble(1.1))
        pow(x, 1+1j, matrix_analysis.var.errordouble(1.1))
        pow(x, matrix_analysis.Unsure, matrix_analysis.var.errordouble(1.1))
        pow(x, matrix_analysis.var.errordouble(1.1),
            matrix_analysis.var.errordouble(1.1))
        pow(1, x, 1)
        pow(1+1j, x, 1)
        pow(matrix_analysis.Unsure, x, 1)
        pow(1, x, 1.1)
        pow(1+1j, x, 1.1)
        pow(matrix_analysis.Unsure, x, 1.1)
        pow(1, x, 1+1j)
        pow(1+1j, x, 1+1j)
        pow(matrix_analysis.Unsure, x, 1+1j)
        pow(1, x, matrix_analysis.Unsure)
        pow(1+1j, x, matrix_analysis.Unsure)
        pow(matrix_analysis.Unsure, x, matrix_analysis.Unsure)
        pow(1, x, matrix_analysis.var.errordouble(1.1))
        pow(1+1j, x, matrix_analysis.var.errordouble(1.1))
        pow(matrix_analysis.Unsure, x, matrix_analysis.var.errordouble(1.1))
        pow(1, 1, x)
        pow(matrix_analysis.Unsure, 1, x)
        pow(matrix_analysis.Unsure,  1+1j, x)
        pow(1, matrix_analysis.Unsure, x)
        pow(1+1j, matrix_analysis.Unsure, x)
        pow(matrix_analysis.Unsure,  matrix_analysis.Unsure, x)
        pow(1, matrix_analysis.var.errordouble(1.1), x)
        pow(1+1j, matrix_analysis.var.errordouble(1.1), x)
        pow(matrix_analysis.Unsure,  matrix_analysis.var.errordouble(1.1), x)
        with self.assertRaises(TypeError):
            pow(x, x, "abc")
        with self.assertRaises(TypeError):
            pow(x, "abc", x)

    def test_unaryfuncs(self):
        x = matrix_analysis.var.variable(1+2j)
        y = matrix_analysis.var.variable()
        a = -x
        A = -y
        self.assertTupleAlmostEqual(a.rec, (-1, -2))
        self.assertFalse(a.is_arbitrary)
        self.assertTrue(A.is_arbitrary)
        b = +x
        B = +y
        self.assertTupleAlmostEqual(b.rec, (1, 2))
        self.assertFalse(b.is_arbitrary)
        self.assertTrue(B.is_arbitrary)
        c = abs(x)
        C = abs(y)
        self.assertAlmostEqual(c, 5**0.5)
        self.assertTrue(math.isnan(C))
        d = ~x
        D = ~y
        self.assertTupleAlmostEqual(d.rec, (0.2, -0.4))
        self.assertFalse(d.is_arbitrary)
        self.assertTrue(D.is_arbitrary)

    def test_bool(self):
        x = matrix_analysis.var.variable()
        y = matrix_analysis.var.variable(0, 0)
        z = matrix_analysis.var.variable(1, 1)
        self.assertTrue(bool(x))
        self.assertFalse(bool(y))
        self.assertTrue(bool(z))

    def test_ufuncs(self):
        x = matrix_analysis.var.variable(2, 3)
        p = matrix_analysis.var.variable(Unsure)
        self.assertTupleAlmostEqual(
            (z := matrix_analysis.funcs.conj(x)).rec, (2, -3))
        self.assertFalse(z.is_arbitrary)
        self.assertTrue(matrix_analysis.funcs.conj(p).is_arbitrary)
        self.assertTupleAlmostEqual(
            (z := matrix_analysis.funcs.exp(x)).rec, (-7.31511009491, 1.04274365623))
        self.assertFalse(z.is_arbitrary)
        self.assertTrue(matrix_analysis.funcs.exp(p).is_arbitrary)
        self.assertTupleAlmostEqual(
            (z := matrix_analysis.funcs.ln(x)).rec, (1.28247467873, 0.982793723247))
        self.assertFalse(z.is_arbitrary)
        self.assertTrue(matrix_analysis.funcs.ln(p).is_arbitrary)
        self.assertTupleAlmostEqual(
            (z := matrix_analysis.funcs.sqrt(x)).rec, (1.67414922804, 0.89597747613))
        self.assertFalse(z.is_arbitrary)
        self.assertTrue(matrix_analysis.funcs.sqrt(p).is_arbitrary)
        self.assertTupleAlmostEqual(
            (z := matrix_analysis.funcs.sin(x)).rec, (9.15449914691, -4.16890695997))
        self.assertFalse(z.is_arbitrary)
        self.assertTrue(matrix_analysis.funcs.sin(p).is_arbitrary)
        self.assertTupleAlmostEqual(
            (z := matrix_analysis.funcs.cos(x)).rec, (-4.18962569097, -9.10922789376))
        self.assertFalse(z.is_arbitrary)
        self.assertTrue(matrix_analysis.funcs.cos(p).is_arbitrary)
        self.assertTupleAlmostEqual(
            (z := matrix_analysis.funcs.tan(x)).rec, (-0.0037640256415, 1.00323862735))
        self.assertFalse(z.is_arbitrary)
        self.assertTrue(matrix_analysis.funcs.tan(p).is_arbitrary)
        self.assertTupleAlmostEqual(
            (z := matrix_analysis.funcs.cot(x)).rec, (-0.00373971037636, -0.996757796573))
        self.assertFalse(z.is_arbitrary)
        self.assertTrue(matrix_analysis.funcs.cot(p).is_arbitrary)
        self.assertTupleAlmostEqual(
            (z := matrix_analysis.funcs.sec(x)).rec, (-0.0416749644111, 0.0906111371962))
        self.assertFalse(z.is_arbitrary)
        self.assertTrue(matrix_analysis.funcs.sec(p).is_arbitrary)
        self.assertTupleAlmostEqual(
            (z := matrix_analysis.funcs.csc(x)).rec, (0.0904732097532, 0.04120099862886))
        self.assertFalse(z.is_arbitrary)
        self.assertTrue(matrix_analysis.funcs.csc(p).is_arbitrary)
        self.assertTupleAlmostEqual(
            (z := matrix_analysis.funcs.arcsin(x)).rec, (0.570652784321, 1.98338702992))
        self.assertFalse(z.is_arbitrary)
        self.assertTrue(matrix_analysis.funcs.arcsin(p).is_arbitrary)
        self.assertTupleAlmostEqual(
            (z := matrix_analysis.funcs.arccos(x)).rec, (1.00014354247, -1.98338702992))
        self.assertFalse(z.is_arbitrary)
        self.assertTrue(matrix_analysis.funcs.arccos(p).is_arbitrary)
        self.assertTupleAlmostEqual(
            (z := matrix_analysis.funcs.arctan(x)).rec, (1.4099210496, 0.229072682969))
        self.assertFalse(z.is_arbitrary)
        self.assertTrue(matrix_analysis.funcs.arctan(p).is_arbitrary)
        self.assertTupleAlmostEqual(
            (z := matrix_analysis.funcs.arccot(x)).rec, (0.160875277198, -0.229072682968))
        self.assertFalse(z.is_arbitrary)
        self.assertTrue(matrix_analysis.funcs.arccot(p).is_arbitrary)
        self.assertTupleAlmostEqual(
            (z := matrix_analysis.funcs.arcsec(x)).rec, (1.42041072247, 0.231334698574))
        self.assertFalse(z.is_arbitrary)
        self.assertTrue(matrix_analysis.funcs.arcsec(p).is_arbitrary)
        self.assertTupleAlmostEqual(
            (z := matrix_analysis.funcs.arccsc(x)).rec, (0.150385604328, -0.231334698574))
        self.assertFalse(z.is_arbitrary)
        self.assertTrue(matrix_analysis.funcs.arccsc(p).is_arbitrary)
        self.assertTupleAlmostEqual(
            (z := matrix_analysis.funcs.sinh(x)).rec, (-3.59056458999, 0.530921086249))
        self.assertFalse(z.is_arbitrary)
        self.assertTrue(matrix_analysis.funcs.sinh(p).is_arbitrary)
        self.assertTupleAlmostEqual(
            (z := matrix_analysis.funcs.cosh(x)).rec, (-3.72454550492, 0.511822569937))
        self.assertFalse(z.is_arbitrary)
        self.assertTrue(matrix_analysis.funcs.cosh(p).is_arbitrary)
        self.assertTupleAlmostEqual(
            (z := matrix_analysis.funcs.tanh(x)).rec, (0.965385879022, -9.88437503832e-3))
        self.assertFalse(z.is_arbitrary)
        self.assertTrue(matrix_analysis.funcs.tanh(p).is_arbitrary)
        self.assertTupleAlmostEqual(
            (z := matrix_analysis.funcs.coth(x)).rec, (1.03574663777, 1.06047834703e-2))
        self.assertFalse(z.is_arbitrary)
        self.assertTrue(matrix_analysis.funcs.coth(p).is_arbitrary)
        self.assertTupleAlmostEqual(
            (z := matrix_analysis.funcs.sech(x)).rec, (-0.263512975158, -3.62116365587e-2))
        self.assertFalse(z.is_arbitrary)
        self.assertTrue(matrix_analysis.funcs.sech(p).is_arbitrary)
        self.assertTupleAlmostEqual(
            (z := matrix_analysis.funcs.csch(x)).rec, (-0.272548661463, -4.03005788568e-2))
        self.assertFalse(z.is_arbitrary)
        self.assertTrue(matrix_analysis.funcs.csch(p).is_arbitrary)
        self.assertTupleAlmostEqual(
            (z := matrix_analysis.funcs.arcsinh(x)).rec, (1.96863792579, 0.964658504408))
        self.assertFalse(z.is_arbitrary)
        self.assertTrue(matrix_analysis.funcs.arcsinh(p).is_arbitrary)
        self.assertTupleAlmostEqual(
            (z := matrix_analysis.funcs.arccosh(x)).rec, (1.98338702992, 1.00014354247))
        self.assertFalse(z.is_arbitrary)
        self.assertTrue(matrix_analysis.funcs.arccosh(p).is_arbitrary)
        self.assertTupleAlmostEqual(
            (z := matrix_analysis.funcs.arctanh(x)).rec, (0.146946666226, 1.33897252229))
        self.assertFalse(z.is_arbitrary)
        self.assertTrue(matrix_analysis.funcs.arctanh(p).is_arbitrary)
        self.assertTupleAlmostEqual(
            (z := matrix_analysis.funcs.arccoth(x)).rec, (0.146946666225, -0.2318238045))
        self.assertFalse(z.is_arbitrary)
        self.assertTrue(matrix_analysis.funcs.arccoth(p).is_arbitrary)
        self.assertTupleAlmostEqual(
            (z := matrix_analysis.funcs.arcsech(x)).rec, (0.231334698574, -1.42041072247))
        self.assertFalse(z.is_arbitrary)
        self.assertTrue(matrix_analysis.funcs.arcsech(p).is_arbitrary)
        self.assertTupleAlmostEqual(
            (z := matrix_analysis.funcs.arccsch(x)).rec, (0.157355498845, -0.229962902377))
        self.assertFalse(z.is_arbitrary)
        self.assertTrue(matrix_analysis.funcs.arccsch(p).is_arbitrary)

    def test_bfuncs(self):
        x = matrix_analysis.var.variable(2, 3)
        y = matrix_analysis.var.variable(1, 2)
        p = matrix_analysis.var.variable(Unsure)
        self.assertTupleAlmostEqual(
            (z := matrix_analysis.funcs.log(x, y)).rec, (1.13173165582, -0.335771297901))
        self.assertFalse(z.is_arbitrary)
        self.assertTupleAlmostEqual(
            (z := matrix_analysis.funcs.log(x, 1+2j)).rec, (1.13173165582, -0.335771297901))
        self.assertFalse(z.is_arbitrary)
        self.assertTupleAlmostEqual(
            (z := matrix_analysis.funcs.log(2+3j, y)).rec, (1.13173165582, -0.335771297901))
        self.assertFalse(z.is_arbitrary)
        self.assertTrue(matrix_analysis.funcs.log(x, p).is_arbitrary)
        self.assertTrue(matrix_analysis.funcs.log(p, y).is_arbitrary)
