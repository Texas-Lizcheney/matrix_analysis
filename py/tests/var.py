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
            z = matrix_analysis.var.errordouble("abc")  # type: ignore

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

    def test_value(self):
        x = matrix_analysis.var.errordouble(1)
        x.value = 2
        self.assertAlmostEqual(x.value, 2)

    def test_error(self):
        x = matrix_analysis.var.errordouble(1)
        x.error = 3
        self.assertAlmostEqual(x.error, 3)

    def test_relative_error(self):
        x = matrix_analysis.var.errordouble(1)
        y = x.error
        x.relative_error = 10
        self.assertAlmostEqual(10*y, x.error)

    def test_cmp(self):
        x = matrix_analysis.var.errordouble(2)
        self.assertTrue(x > 1)
        self.assertFalse(x > 3)
        self.assertTrue(x >= 2)
        self.assertFalse(x >= 3)
        self.assertTrue(x == 2)
        self.assertFalse(x == 3)
        self.assertTrue(x != 1)
        self.assertFalse(x != 2)
        self.assertTrue(x < 3)
        self.assertFalse(x < 1)
        self.assertTrue(x <= 2)
        self.assertFalse(x <= 1)
        x.error = 1
        self.assertUnsure(x > 2.5)
        self.assertUnsure(x >= 2.5)
        y = matrix_analysis.var.errordouble(3.1, 0.3)
        self.assertUnsure(x == y)
        self.assertUnsure(x != y)
        self.assertUnsure(x < 1.5)
        self.assertUnsure(x <= 1.5)

    def test_add(self):
        x = matrix_analysis.var.errordouble(0.1)
        y = matrix_analysis.var.errordouble(0.2)
        self.assertAlmostEqual(0.3, x+y)
        self.assertAlmostEqual(x+1.1, 1.2)
        self.assertAlmostEqual(x+1, 1.1)
        self.assertAlmostEqual(1.1+x, 1.2)
        self.assertAlmostEqual(1+x, 1.1)
        with self.assertRaises(TypeError):
            x+"abc"  # type: ignore
        with self.assertRaises(TypeError):
            "abc"+x  # type: ignore
        k = id(x)
        x += y
        self.assertAlmostEqual(x, 0.3)
        self.assertEqual(k, id(x))
        self.assertEqual(sys.getrefcount(x), 2)
        with self.assertRaises(TypeError):
            x += "abc"  # type: ignore

    def test_sub(self):
        x = matrix_analysis.var.errordouble(0.1)
        y = matrix_analysis.var.errordouble(0.2)
        self.assertAlmostEqual(x-y, -0.1)
        self.assertAlmostEqual(x-1.1, -1)
        self.assertAlmostEqual(x-1, -0.9)
        self.assertAlmostEqual(1.1-x, 1)
        self.assertAlmostEqual(1-x, 0.9)
        with self.assertRaises(TypeError):
            x-"abc"  # type: ignore
        with self.assertRaises(TypeError):
            "abc"-x  # type: ignore
        k = id(x)
        x -= y
        self.assertAlmostEqual(x, -0.1)
        self.assertEqual(k, id(x))
        self.assertEqual(sys.getrefcount(x), 2)
        with self.assertRaises(TypeError):
            x -= "abc"  # type: ignore

    def test_mul(self):
        x = matrix_analysis.var.errordouble(0.1)
        y = matrix_analysis.var.errordouble(0.2)
        self.assertAlmostEqual(x*y, 0.02)
        self.assertAlmostEqual(x*1.1, 0.11)
        self.assertAlmostEqual(x*2, 0.2)
        self.assertAlmostEqual(1.1*x, 0.11)
        self.assertAlmostEqual(2*x, 0.2)
        with self.assertRaises(TypeError):
            x*"abc"  # type: ignore
        with self.assertRaises(TypeError):
            "abc"*x  # type: ignore
        k = id(x)
        x *= y
        self.assertAlmostEqual(x, 0.02)
        self.assertEqual(k, id(x))
        self.assertEqual(sys.getrefcount(x), 2)
        with self.assertRaises(TypeError):
            x *= "abc"  # type: ignore

    def test_div(self):
        x = matrix_analysis.var.errordouble(0.1)
        y = matrix_analysis.var.errordouble(0.2)
        self.assertAlmostEqual(x/y, 0.5)
        self.assertAlmostEqual(x/2.0, 0.05)
        self.assertAlmostEqual(x/2, 0.05)
        self.assertAlmostEqual(1/x, 10)
        self.assertAlmostEqual(1.0/x, 10)
        with self.assertRaises(TypeError):
            x/"abc"  # type: ignore
        with self.assertRaises(TypeError):
            "abc"/x  # type: ignore
        k = id(x)
        x /= y
        self.assertAlmostEqual(x, 0.5)
        self.assertEqual(k, id(x))
        self.assertEqual(sys.getrefcount(x), 2)
        with self.assertRaises(TypeError):
            x *= "abc"  # type: ignore

    def test_floor_div(self):
        x = matrix_analysis.var.errordouble(0.1)
        y = matrix_analysis.var.errordouble(0.2)
        self.assertAlmostEqual(x//y, 0)
        self.assertAlmostEqual(x//2.0, 0)
        self.assertAlmostEqual(x//2, 0)
        self.assertAlmostEqual(1.05//x, 10)
        self.assertAlmostEqual(1//x, 10)
        with self.assertRaises(TypeError):
            x//"abc"  # type: ignore
        with self.assertRaises(TypeError):
            "abc"//x  # type: ignore
        k = id(x)
        x //= y
        self.assertAlmostEqual(x, 0)
        self.assertEqual(k, id(x))
        self.assertEqual(sys.getrefcount(x), 2)
        with self.assertRaises(TypeError):
            x //= "abc"  # type: ignore

    def test_mod(self):
        x = matrix_analysis.var.errordouble(0.1)
        y = matrix_analysis.var.errordouble(0.21)
        self.assertAlmostEqual(x % y, 0.1)
        self.assertAlmostEqual(x % 2.0, 0.1)
        self.assertAlmostEqual(x % 2, 0.1)
        self.assertAlmostEqual(1.05 % x, 0.05)
        self.assertAlmostEqual(1 % x, 0)
        with self.assertRaises(TypeError):
            x % "abc"  # type: ignore
        with self.assertRaises(TypeError):
            "abc" % x  # type: ignore
        k = id(x)
        x %= 0.03
        self.assertAlmostEqual(x, 0.01)
        self.assertEqual(k, id(x))
        self.assertEqual(sys.getrefcount(x), 2)
        with self.assertRaises(TypeError):
            x %= "abc"  # type: ignore

    def test_divmod(self):
        x = matrix_analysis.var.errordouble(0.1)
        y = matrix_analysis.var.errordouble(0.21)
        z = divmod(y, x)
        self.assertAlmostEqual(z[0], 2)
        self.assertAlmostEqual(z[1], 0.01)
        with self.assertRaises(TypeError):
            divmod(y, "abc")  # type: ignore
        with self.assertRaises(TypeError):
            divmod("abc", x)  # type: ignore

    def test_pow(self):
        x = matrix_analysis.var.errordouble(0.1)
        y = matrix_analysis.var.errordouble(0.2)
        self.assertAlmostEqual(x**y, 0.1**0.2)
        self.assertAlmostEqual(x**2.0, 0.1**2.0)
        self.assertAlmostEqual(x**3, 0.1**3)
        self.assertAlmostEqual(1.05**x, 1.05**0.1)
        self.assertAlmostEqual(2**x, 2**0.1)
        with self.assertRaises(TypeError):
            x**"abc"  # type: ignore
        with self.assertRaises(TypeError):
            "abc"**x  # type: ignore
        k = id(x)
        x **= 0.03
        self.assertAlmostEqual(x, 0.1**0.03)
        self.assertEqual(k, id(x))
        self.assertEqual(sys.getrefcount(x), 2)
        with self.assertRaises(TypeError):
            x **= "abc"  # type: ignore
        x = matrix_analysis.var.errordouble(4.4)
        y = matrix_analysis.var.errordouble(2.2)
        z = matrix_analysis.var.errordouble(3.3)
        self.assertAlmostEqual(pow(x, y, z), 2.9372964773)
        self.assertAlmostEqual(pow(x, 2.2, z), 2.9372964773)
        self.assertAlmostEqual(pow(x, y, 3.3), 2.9372964773)
        self.assertAlmostEqual(pow(x, 2.2, 3.3), 2.9372964773)
        with self.assertRaises(TypeError):
            pow(x, "abc", x)  # type: ignore
        with self.assertRaises(TypeError):
            pow(x, x, "abc")  # type: ignore

    def test_unaryfuncs(self):
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

    def test_round(self):
        x = matrix_analysis.var.errordouble(1.3)
        self.assertEqual(round(x), round(1.3))
        self.assertEqual(round(x, 4), round(1.3, 4))

    def test_exp(self):
        x = matrix_analysis.var.errordouble(1.3)
        self.assertEqual(matrix_analysis.funcs.exp(x), math.exp(x))

    def test_ln(self):
        x = matrix_analysis.var.errordouble(1.3)
        self.assertEqual(matrix_analysis.funcs.ln(x), math.log(x))
        with self.assertWarns(RuntimeWarning):
            matrix_analysis.funcs.ln(matrix_analysis.var.errordouble(-1))

    def test_sin(self):
        x = matrix_analysis.var.errordouble(1.3)
        self.assertEqual(matrix_analysis.funcs.sin(x), math.sin(x))

    def test_cos(self):
        x = matrix_analysis.var.errordouble(1.3)
        self.assertEqual(matrix_analysis.funcs.cos(x), math.cos(x))

    def test_tan(self):
        x = matrix_analysis.var.errordouble(1.3)
        self.assertEqual(matrix_analysis.funcs.tan(x), math.tan(x))

    def test_cot(self):
        x = matrix_analysis.var.errordouble(1.3)
        self.assertEqual(matrix_analysis.funcs.cot(x), 1 / math.tan(x))

    def test_sec(self):
        x = matrix_analysis.var.errordouble(1.3)
        self.assertEqual(matrix_analysis.funcs.sec(x), 1 / math.cos(x))

    def test_csc(self):
        x = matrix_analysis.var.errordouble(1.3)
        self.assertEqual(matrix_analysis.funcs.csc(x), 1 / math.sin(x))

    def test_arcsin(self):
        x = matrix_analysis.var.errordouble(0.9)
        self.assertEqual(matrix_analysis.funcs.arcsin(x), math.asin(x))
        with self.assertWarns(RuntimeWarning):
            matrix_analysis.funcs.arcsin(matrix_analysis.var.errordouble(1.3))

    def test_arccos(self):
        x = matrix_analysis.var.errordouble(0.9)
        self.assertEqual(matrix_analysis.funcs.arccos(x), math.acos(x))
        with self.assertWarns(RuntimeWarning):
            matrix_analysis.funcs.arccos(matrix_analysis.var.errordouble(1.3))

    def test_arctan(self):
        x = matrix_analysis.var.errordouble(1.3)
        self.assertEqual(matrix_analysis.funcs.arctan(x), math.atan(x))

    def test_arccot(self):
        x = matrix_analysis.var.errordouble(1.3)
        self.assertEqual(matrix_analysis.funcs.arccot(x),
                         math.pi/2-math.atan(x))

    def test_arcsec(self):
        x = matrix_analysis.var.errordouble(1.3)
        self.assertEqual(matrix_analysis.funcs.arcsec(x), math.acos(1/x))
        with self.assertWarns(RuntimeWarning):
            matrix_analysis.funcs.arcsec(matrix_analysis.var.errordouble(0.9))

    def test_arccsc(self):
        x = matrix_analysis.var.errordouble(1.3)
        self.assertEqual(matrix_analysis.funcs.arccsc(x), math.asin(1/x))
        with self.assertWarns(RuntimeWarning):
            matrix_analysis.funcs.arccsc(matrix_analysis.var.errordouble(0.9))

    def test_sinh(self):
        x = matrix_analysis.var.errordouble(1.3)
        self.assertEqual(matrix_analysis.funcs.sinh(x), math.sinh(x))

    def test_cosh(self):
        x = matrix_analysis.var.errordouble(1.3)
        self.assertEqual(matrix_analysis.funcs.cosh(x), math.cosh(x))

    def test_tanh(self):
        x = matrix_analysis.var.errordouble(1.3)
        self.assertEqual(matrix_analysis.funcs.tanh(x), math.tanh(x))

    def test_coth(self):
        x = matrix_analysis.var.errordouble(1.3)
        self.assertEqual(matrix_analysis.funcs.coth(x), 1 / math.tanh(x))

    def test_sech(self):
        x = matrix_analysis.var.errordouble(1.3)
        self.assertEqual(matrix_analysis.funcs.sech(x), 1 / math.cosh(x))

    def test_csch(self):
        x = matrix_analysis.var.errordouble(1.3)
        self.assertEqual(matrix_analysis.funcs.csch(x), 1 / math.sinh(x))

    def test_arcsinh(self):
        x = matrix_analysis.var.errordouble(1.3)
        self.assertEqual(matrix_analysis.funcs.arcsinh(x), math.asinh(x))

    def test_arccosh(self):
        x = matrix_analysis.var.errordouble(1.3)
        self.assertEqual(matrix_analysis.funcs.arccosh(x), math.acosh(x))
        with self.assertWarns(RuntimeWarning):
            matrix_analysis.funcs.arccosh(matrix_analysis.var.errordouble(-1))

    def test_arctanh(self):
        x = matrix_analysis.var.errordouble(0.9)
        self.assertEqual(matrix_analysis.funcs.arctanh(x), math.atanh(x))
        with self.assertWarns(RuntimeWarning):
            matrix_analysis.funcs.arctanh(matrix_analysis.var.errordouble(1.3))

    def test_arccoth(self):
        x = matrix_analysis.var.errordouble(1.3)
        self.assertEqual(matrix_analysis.funcs.arccoth(x), math.atanh(1/x))
        with self.assertWarns(RuntimeWarning):
            matrix_analysis.funcs.arccoth(matrix_analysis.var.errordouble(0.9))

    def test_arcsech(self):
        x = matrix_analysis.var.errordouble(0.9)
        self.assertEqual(matrix_analysis.funcs.arcsech(x), math.acosh(1/x))
        with self.assertWarns(RuntimeWarning):
            matrix_analysis.funcs.arcsech(matrix_analysis.var.errordouble(1.3))
        with self.assertWarns(RuntimeWarning):
            matrix_analysis.funcs.arcsech(matrix_analysis.var.errordouble(-1))

    def test_arccsch(self):
        x = matrix_analysis.var.errordouble(1.3)
        self.assertEqual(matrix_analysis.funcs.arccsch(x), math.asinh(1/x))
        with self.assertWarns(RuntimeWarning):
            matrix_analysis.funcs.arccsch(matrix_analysis.var.errordouble(0))

    def test_sqrt(self):
        x = matrix_analysis.var.errordouble(1.3)
        self.assertEqual(matrix_analysis.funcs.sqrt(x), math.sqrt(x))
        with self.assertWarns(RuntimeWarning):
            matrix_analysis.funcs.sqrt(matrix_analysis.var.errordouble(-1))

    def test_log(self):
        x = matrix_analysis.var.errordouble(3)
        self.assertAlmostEqual(matrix_analysis.funcs.log(x, 2), math.log(3, 2))
        self.assertAlmostEqual(matrix_analysis.funcs.log(2, x), math.log(2, 3))
        with self.assertWarns(RuntimeWarning):
            matrix_analysis.funcs.log(x, -1)
        with self.assertWarns(RuntimeWarning):
            matrix_analysis.funcs.log(-1, x)
        with self.assertRaises(TypeError):
            matrix_analysis.funcs.log(x, "abc")  # type: ignore
        with self.assertRaises(TypeError):
            matrix_analysis.funcs.log("abc", x)  # type: ignore


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
            y4 = matrix_analysis.var.variable("abc")  # type: ignore
        with self.assertRaises(TypeError):
            z3 = matrix_analysis.var.variable(1, 2, 3)  # type: ignore

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
        with self.assertWarns(matrix_analysis.UndefinedWarning):
            x.r
        with self.assertWarns(matrix_analysis.UndefinedWarning):
            x.arg
        with self.assertWarns(matrix_analysis.UndefinedWarning):
            x.r = 1
        with self.assertWarns(matrix_analysis.UndefinedWarning):
            x.arg = 1
        y = matrix_analysis.var.variable(1+2j)
        self.assertFalse(y.is_arbitrary)
        self.assertAlmostEqual(y.real, 1)
        self.assertAlmostEqual(y.imag, 2)
        self.assertAlmostEqual(y.r, 5**0.5)
        self.assertAlmostEqual(y.arg, math.atan2(2, 1))
        self.assertAlmostEqual(y, 1+2j)
        x = matrix_analysis.var.variable(1+1j)
        matrix_analysis.var.set_arg_format("deg")
        self.assertAlmostEqual(x.arg, 45)
        x.arg = 60
        x.r = 2
        self.assertAlmostEqual(x, 1+(3**0.5)*1j)
        matrix_analysis.var.set_arg_format("rad")
        x.real = 1
        self.assertAlmostEqual(x.real, 1)
        x.real.value = 2  # type: ignore
        self.assertAlmostEqual(x.real.value, 2)  # type: ignore
        x.real.error = 3  # type: ignore
        self.assertAlmostEqual(x.real.error, 3)  # type: ignore
        x.imag = 2
        self.assertAlmostEqual(x.imag, 2)
        x.imag.value = 3  # type: ignore
        self.assertAlmostEqual(x.imag.value, 3)  # type: ignore
        x.imag.error = 4  # type: ignore
        self.assertAlmostEqual(x.imag.error, 4)  # type: ignore

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
            x < y  # type: ignore
        with self.assertRaises(TypeError):
            x < "abc"  # type: ignore
        self.assertEqual(z, 1)
        self.assertEqual(z, 1.0)
        w = matrix_analysis.var.variable(1-1j)
        self.assertEqual(w, 1-1j)

    def test_add(self):
        x = matrix_analysis.var.variable(1+2j)
        y = matrix_analysis.var.variable(2+3j)
        z = x+y
        self.assertFalse(z.is_arbitrary)
        self.assertAlmostEqual(z, 3+5j)
        k = id(x)
        x += y
        self.assertEqual(id(x), k)
        self.assertVarAlmostEqual(z, x)
        f = matrix_analysis.var.variable(Unsure)
        z = x+f
        self.assertTrue(z.is_arbitrary)
        self.assertVarAlmostEqual(x+1, 1+x)
        x+1  # type: ignore
        x+1.1  # type: ignore
        x+(1+1j)  # type: ignore
        x+matrix_analysis.Unsure  # type: ignore
        x+matrix_analysis.var.errordouble(1.1)  # type: ignore
        1+x  # type: ignore
        1.1+x  # type: ignore
        (1+1j)+x  # type: ignore
        matrix_analysis.Unsure+x  # type: ignore
        matrix_analysis.var.errordouble(1.1)+x  # type: ignore
        x += 1
        x += 1.1
        x += (1+1j)
        x += matrix_analysis.Unsure
        x += matrix_analysis.var.errordouble(1.1)
        with self.assertRaises(TypeError):
            x+"abc"  # type: ignore
        with self.assertRaises(TypeError):
            "abc"+x  # type: ignore
        with self.assertRaises(TypeError):
            x += "abc"  # type: ignore

    def test_sub(self):
        x = matrix_analysis.var.variable(1+2j)
        y = matrix_analysis.var.variable(2+3j)
        z = x-y
        self.assertFalse(z.is_arbitrary)
        self.assertAlmostEqual(z, -1-1j)
        k = id(x)
        x -= y
        self.assertEqual(id(x), k)
        self.assertVarAlmostEqual(z, x)
        f = matrix_analysis.var.variable(Unsure)
        z = x-f
        self.assertTrue(z.is_arbitrary)
        self.assertVarAlmostEqual(x-1, -(1-x))
        x-1  # type: ignore
        x-1.1  # type: ignore
        x-(1+1j)  # type: ignore
        x-matrix_analysis.Unsure  # type: ignore
        x-matrix_analysis.var.errordouble(1.1)  # type: ignore
        1-x  # type: ignore
        1.1-x  # type: ignore
        (1+1j)-x  # type: ignore
        matrix_analysis.Unsure-x  # type: ignore
        matrix_analysis.var.errordouble(1.1)-x  # type: ignore
        x -= 1
        x -= 1.1
        x -= (1+1j)
        x -= matrix_analysis.Unsure
        x -= matrix_analysis.var.errordouble(1.1)
        with self.assertRaises(TypeError):
            x-"abc"  # type: ignore
        with self.assertRaises(TypeError):
            "abc"-x  # type: ignore
        with self.assertRaises(TypeError):
            x -= "abc"  # type: ignore

    def test_mul(self):
        x = matrix_analysis.var.variable(1+2j)
        y = matrix_analysis.var.variable(2+3j)
        z = x*y
        self.assertFalse(z.is_arbitrary)
        self.assertAlmostEqual(z, -4+7j)
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
        self.assertAlmostEqual(z, 0)
        self.assertVarAlmostEqual(x*1, 1*x)
        x*1  # type: ignore
        x*1.1  # type: ignore
        x*(1+1j)  # type: ignore
        x*matrix_analysis.Unsure  # type: ignore
        x*matrix_analysis.var.errordouble(1.1)  # type: ignore
        1*x  # type: ignore
        1.1*x  # type: ignore
        (1+1j)*x  # type: ignore
        matrix_analysis.Unsure*x  # type: ignore
        matrix_analysis.var.errordouble(1.1)*x  # type: ignore
        x *= 1
        x *= 1.1
        x *= (1+1j)
        x *= matrix_analysis.Unsure
        x *= matrix_analysis.var.errordouble(1.1)
        with self.assertRaises(TypeError):
            x*"abc"  # type: ignore
        with self.assertRaises(TypeError):
            "abc"*x  # type: ignore
        with self.assertRaises(TypeError):
            x *= "abc"  # type: ignore

    def test_div(self):
        x = matrix_analysis.var.variable(1+2j)
        y = matrix_analysis.var.variable(2+3j)
        z = x/y
        self.assertFalse(z.is_arbitrary)
        self.assertAlmostEqual(z, 0.615384615385+7.69230769231e-2j)
        k = id(x)
        x /= y
        self.assertEqual(id(x), k)
        self.assertAlmostEqual(x, 0.615384615385+7.69230769231e-2j)
        f = matrix_analysis.var.variable(Unsure)
        z = x/f
        self.assertTrue(z.is_arbitrary)
        g = matrix_analysis.var.variable(0)
        z = g/f
        self.assertFalse(z.is_arbitrary)
        self.assertAlmostEqual(z, 0)
        self.assertVarAlmostEqual(x/1, ~(1/x))
        x/1  # type: ignore
        x/1.1  # type: ignore
        x/(1+1j)  # type: ignore
        x/matrix_analysis.Unsure  # type: ignore
        x/matrix_analysis.var.errordouble(1.1)  # type: ignore
        1/x  # type: ignore
        1.1/x  # type: ignore
        (1+1j)/x  # type: ignore
        matrix_analysis.Unsure/x  # type: ignore
        matrix_analysis.var.errordouble(1.1)/x  # type: ignore
        x /= 1
        x /= 1.1
        x /= (1+1j)
        x /= matrix_analysis.Unsure
        x /= matrix_analysis.var.errordouble(1.1)
        with self.assertRaises(TypeError):
            x/"abc"  # type: ignore
        with self.assertRaises(TypeError):
            "abc"/x  # type: ignore
        with self.assertRaises(TypeError):
            x /= "abc"  # type: ignore

    def test_floor_div(self):
        x = matrix_analysis.var.variable(1+2j)
        y = matrix_analysis.var.variable(2+3j)
        z = x//y
        self.assertFalse(z.is_arbitrary)
        self.assertAlmostEqual(z, 1)
        k = id(x)
        x //= y
        self.assertEqual(id(x), k)
        self.assertAlmostEqual(x, 1)
        f = matrix_analysis.var.variable(Unsure)
        z = x//f
        self.assertTrue(z.is_arbitrary)
        g = matrix_analysis.var.variable(0)
        z = g//f
        self.assertFalse(z.is_arbitrary)
        self.assertAlmostEqual(z, 0)
        self.assertAlmostEqual((4+3j)//y, 1)
        x//1  # type: ignore
        x//1.1  # type: ignore
        x//(1+1j)  # type: ignore
        x//matrix_analysis.Unsure  # type: ignore
        x//matrix_analysis.var.errordouble(1.1)  # type: ignore
        1//x  # type: ignore
        1.1//x  # type: ignore
        (1+1j)//x  # type: ignore
        matrix_analysis.Unsure//x  # type: ignore
        matrix_analysis.var.errordouble(1.1)//x  # type: ignore
        x //= 1
        x //= 1.1
        x //= (1+1j)
        x //= matrix_analysis.Unsure
        x //= matrix_analysis.var.errordouble(1.1)
        with self.assertRaises(TypeError):
            x//"abc"  # type: ignore
        with self.assertRaises(TypeError):
            "abc"//x  # type: ignore
        with self.assertRaises(TypeError):
            x //= "abc"  # type: ignore

    def test_mod(self):
        x = matrix_analysis.var.variable(1+2j)
        y = matrix_analysis.var.variable(2+3j)
        z = x % y
        self.assertFalse(z.is_arbitrary)
        self.assertAlmostEqual(z, -1-1j)
        k = id(x)
        x %= y
        self.assertEqual(id(x), k)
        self.assertAlmostEqual(x, -1-1j)
        f = matrix_analysis.var.variable(Unsure)
        z = x % f
        self.assertTrue(z.is_arbitrary)
        g = matrix_analysis.var.variable(0)
        z = g % f
        self.assertFalse(z.is_arbitrary)
        self.assertAlmostEqual(z, 0)
        self.assertAlmostEqual((4+3j) % y, 2)
        x % 1  # type: ignore
        x % 1.1  # type: ignore
        x % (1+1j)  # type: ignore
        x % matrix_analysis.Unsure  # type: ignore
        x % matrix_analysis.var.errordouble(1.1)  # type: ignore
        1 % x  # type: ignore
        1.1 % x  # type: ignore
        (1+1j) % x  # type: ignore
        matrix_analysis.Unsure % x  # type: ignore
        matrix_analysis.var.errordouble(1.1) % x  # type: ignore
        x %= 1
        x %= 1.1
        x %= (1+1j)
        x %= matrix_analysis.Unsure
        x %= matrix_analysis.var.errordouble(1.1)
        with self.assertRaises(TypeError):
            x % "abc"  # type: ignore
        with self.assertRaises(TypeError):
            "abc" % x  # type: ignore
        with self.assertRaises(TypeError):
            x %= "abc"  # type: ignore

    def test_divmod(self):
        x = matrix_analysis.var.variable(1+2j)
        y = matrix_analysis.var.variable(2+3j)
        z = divmod(y, x)
        self.assertEqual(sys.getrefcount(z[0]), 2)
        self.assertEqual(sys.getrefcount(z[1]), 2)
        self.assertAlmostEqual(z[0], 2)
        self.assertAlmostEqual(z[1], -1j)
        with self.assertRaises(TypeError):
            divmod(y, "abc")  # type: ignore
        with self.assertRaises(TypeError):
            divmod("abc", x)  # type: ignore

    def test_pow(self):
        x = matrix_analysis.var.variable(1+2j)
        y = matrix_analysis.var.variable(2+3j)
        z = x**y
        self.assertFalse(z.is_arbitrary)
        self.assertAlmostEqual(z, -0.0151326724227-0.179867483913j)
        a = matrix_analysis.var.variable(7, 9)
        b = matrix_analysis.var.variable(5, 3)
        c = matrix_analysis.var.variable(2, 3)
        d = pow(a, b, c)
        self.assertAlmostEqual(d, 0.59537457-1.66951936j)
        k = id(x)
        x **= y
        self.assertEqual(k, id(x))
        self.assertVarAlmostEqual(z, x)
        k = id(a)
        a.__ipow__(b, c)
        self.assertEqual(k, id(a))
        self.assertVarAlmostEqual(a, d)
        self.assertAlmostEqual(2**y, -1.94797767186+3.4936203271j)
        x**1  # type: ignore
        x**1.1  # type: ignore
        x**(1+1j)  # type: ignore
        x**matrix_analysis.Unsure  # type: ignore
        x**matrix_analysis.var.errordouble(1.1)  # type: ignore
        1**x  # type: ignore
        1.1**x  # type: ignore
        (1+1j)**x  # type: ignore
        matrix_analysis.Unsure**x  # type: ignore
        matrix_analysis.var.errordouble(1.1)**x  # type: ignore
        x **= 1
        x **= 1.1
        x **= (1+1j)
        x **= matrix_analysis.Unsure
        x **= matrix_analysis.var.errordouble(1.1)
        with self.assertRaises(TypeError):
            x**"abc"  # type: ignore
        with self.assertRaises(TypeError):
            "abc"**x  # type: ignore
        with self.assertRaises(TypeError):
            x **= "abc"  # type: ignore
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
        with self.assertRaises(TypeError):
            pow(x, x, "abc")  # type: ignore
        with self.assertRaises(TypeError):
            pow(x, "abc", x)  # type: ignore

    def test_unaryfuncs(self):
        x = matrix_analysis.var.variable(1+2j)
        y = matrix_analysis.var.variable()
        a = -x
        A = -y
        self.assertAlmostEqual(a, -1-2j)
        self.assertFalse(a.is_arbitrary)
        self.assertTrue(A.is_arbitrary)
        b = +x
        B = +y
        self.assertAlmostEqual(b, 1+2j)
        self.assertFalse(b.is_arbitrary)
        self.assertTrue(B.is_arbitrary)
        c = abs(x)
        C = abs(y)
        self.assertAlmostEqual(c, 5**0.5)
        self.assertTrue(math.isnan(C))
        d = ~x
        D = ~y
        self.assertAlmostEqual(d, 0.2-0.4j)
        self.assertFalse(d.is_arbitrary)
        self.assertTrue(D.is_arbitrary)

    def test_bool(self):
        x = matrix_analysis.var.variable()
        y = matrix_analysis.var.variable(0, 0)
        z = matrix_analysis.var.variable(1, 1)
        self.assertTrue(bool(x))
        self.assertFalse(bool(y))
        self.assertTrue(bool(z))

    def test_conj(self):
        x = matrix_analysis.var.variable(2, 3)
        p = matrix_analysis.var.variable(Unsure)
        self.assertAlmostEqual(z := matrix_analysis.funcs.conj(x), 2-3j)
        self.assertFalse(z.is_arbitrary)
        self.assertTrue(matrix_analysis.funcs.conj(p).is_arbitrary)
        self.assertAlmostEqual(z := matrix_analysis.funcs.exp(
            x), -7.31511009491+1.04274365623j)
        self.assertFalse(z.is_arbitrary)

    def test_exp(self):
        x = matrix_analysis.var.variable(2, 3)
        p = matrix_analysis.var.variable(Unsure)
        self.assertTrue(matrix_analysis.funcs.exp(p).is_arbitrary)
        self.assertAlmostEqual(z := matrix_analysis.funcs.ln(
            x), 1.28247467873+0.982793723247j)
        self.assertFalse(z.is_arbitrary)

    def test_ln(self):
        x = matrix_analysis.var.variable(2, 3)
        p = matrix_analysis.var.variable(Unsure)
        self.assertTrue(matrix_analysis.funcs.ln(p).is_arbitrary)
        self.assertAlmostEqual(z := matrix_analysis.funcs.sqrt(
            x), 1.67414922804+0.89597747613j)
        self.assertFalse(z.is_arbitrary)

    def test_sqrt(self):
        x = matrix_analysis.var.variable(2, 3)
        p = matrix_analysis.var.variable(Unsure)
        self.assertTrue(matrix_analysis.funcs.sqrt(p).is_arbitrary)
        self.assertAlmostEqual(z := matrix_analysis.funcs.sin(
            x), 9.15449914691-4.16890695997j)
        self.assertFalse(z.is_arbitrary)

    def test_sin(self):
        x = matrix_analysis.var.variable(2, 3)
        p = matrix_analysis.var.variable(Unsure)
        self.assertTrue(matrix_analysis.funcs.sin(p).is_arbitrary)
        self.assertAlmostEqual(z := matrix_analysis.funcs.cos(
            x), -4.18962569097-9.10922789376j)
        self.assertFalse(z.is_arbitrary)

    def test_cos(self):
        x = matrix_analysis.var.variable(2, 3)
        p = matrix_analysis.var.variable(Unsure)
        self.assertTrue(matrix_analysis.funcs.cos(p).is_arbitrary)
        self.assertAlmostEqual(z := matrix_analysis.funcs.tan(
            x), -0.0037640256415+1.00323862735j)
        self.assertFalse(z.is_arbitrary)

    def test_tan(self):
        x = matrix_analysis.var.variable(2, 3)
        p = matrix_analysis.var.variable(Unsure)
        self.assertTrue(matrix_analysis.funcs.tan(p).is_arbitrary)
        self.assertAlmostEqual(z := matrix_analysis.funcs.cot(
            x), -0.00373971037636-0.996757796573j)
        self.assertFalse(z.is_arbitrary)

    def test_cot(self):
        x = matrix_analysis.var.variable(2, 3)
        p = matrix_analysis.var.variable(Unsure)
        self.assertTrue(matrix_analysis.funcs.cot(p).is_arbitrary)
        self.assertAlmostEqual(z := matrix_analysis.funcs.sec(
            x), -0.0416749644111+0.0906111371962j)
        self.assertFalse(z.is_arbitrary)

    def test_sec(self):
        x = matrix_analysis.var.variable(2, 3)
        p = matrix_analysis.var.variable(Unsure)
        self.assertTrue(matrix_analysis.funcs.sec(p).is_arbitrary)
        self.assertAlmostEqual(z := matrix_analysis.funcs.csc(
            x), 0.0904732097532+0.04120099862886j)
        self.assertFalse(z.is_arbitrary)

    def test_csc(self):
        x = matrix_analysis.var.variable(2, 3)
        p = matrix_analysis.var.variable(Unsure)
        self.assertTrue(matrix_analysis.funcs.csc(p).is_arbitrary)
        self.assertAlmostEqual(z := matrix_analysis.funcs.arcsin(
            x), 0.570652784321+1.98338702992j)
        self.assertFalse(z.is_arbitrary)

    def test_arcsin(self):
        x = matrix_analysis.var.variable(2, 3)
        p = matrix_analysis.var.variable(Unsure)
        self.assertTrue(matrix_analysis.funcs.arcsin(p).is_arbitrary)
        self.assertAlmostEqual(z := matrix_analysis.funcs.arccos(
            x), 1.00014354247-1.98338702992j)
        self.assertFalse(z.is_arbitrary)

    def test_arccos(self):
        x = matrix_analysis.var.variable(2, 3)
        p = matrix_analysis.var.variable(Unsure)
        self.assertTrue(matrix_analysis.funcs.arccos(p).is_arbitrary)
        self.assertAlmostEqual(z := matrix_analysis.funcs.arctan(
            x), 1.4099210496+0.229072682969j)
        self.assertFalse(z.is_arbitrary)

    def test_arctan(self):
        x = matrix_analysis.var.variable(2, 3)
        p = matrix_analysis.var.variable(Unsure)
        self.assertTrue(matrix_analysis.funcs.arctan(p).is_arbitrary)
        self.assertAlmostEqual(z := matrix_analysis.funcs.arccot(
            x), 0.160875277198-0.229072682968j)
        self.assertFalse(z.is_arbitrary)

    def test_arccot(self):
        x = matrix_analysis.var.variable(2, 3)
        p = matrix_analysis.var.variable(Unsure)
        self.assertTrue(matrix_analysis.funcs.arccot(p).is_arbitrary)
        self.assertAlmostEqual(z := matrix_analysis.funcs.arcsec(
            x), 1.42041072247+0.231334698574j)
        self.assertFalse(z.is_arbitrary)

    def test_arcsec(self):
        x = matrix_analysis.var.variable(2, 3)
        p = matrix_analysis.var.variable(Unsure)
        self.assertTrue(matrix_analysis.funcs.arcsec(p).is_arbitrary)
        self.assertAlmostEqual(z := matrix_analysis.funcs.arccsc(
            x), 0.150385604328-0.231334698574j)
        self.assertFalse(z.is_arbitrary)

    def test_sinh(self):
        x = matrix_analysis.var.variable(2, 3)
        p = matrix_analysis.var.variable(Unsure)
        self.assertTrue(matrix_analysis.funcs.arccsc(p).is_arbitrary)
        self.assertAlmostEqual(z := matrix_analysis.funcs.sinh(
            x), -3.59056458999+0.530921086249j)
        self.assertFalse(z.is_arbitrary)

    def test_cosh(self):
        x = matrix_analysis.var.variable(2, 3)
        p = matrix_analysis.var.variable(Unsure)
        self.assertTrue(matrix_analysis.funcs.sinh(p).is_arbitrary)
        self.assertAlmostEqual(z := matrix_analysis.funcs.cosh(
            x), -3.72454550492+0.511822569937j)
        self.assertFalse(z.is_arbitrary)

    def test_tanh(self):
        x = matrix_analysis.var.variable(2, 3)
        p = matrix_analysis.var.variable(Unsure)
        self.assertTrue(matrix_analysis.funcs.cosh(p).is_arbitrary)
        self.assertAlmostEqual(z := matrix_analysis.funcs.tanh(
            x), 0.965385879022-9.88437503832e-3j)
        self.assertFalse(z.is_arbitrary)

    def test_coth(self):
        x = matrix_analysis.var.variable(2, 3)
        p = matrix_analysis.var.variable(Unsure)
        self.assertTrue(matrix_analysis.funcs.tanh(p).is_arbitrary)
        self.assertAlmostEqual(z := matrix_analysis.funcs.coth(
            x), 1.03574663777+1.06047834703e-2j)
        self.assertFalse(z.is_arbitrary)

    def test_sech(self):
        x = matrix_analysis.var.variable(2, 3)
        p = matrix_analysis.var.variable(Unsure)
        self.assertTrue(matrix_analysis.funcs.coth(p).is_arbitrary)
        self.assertAlmostEqual(z := matrix_analysis.funcs.sech(
            x), -0.263512975158-3.62116365587e-2j)
        self.assertFalse(z.is_arbitrary)

    def test_csch(self):
        x = matrix_analysis.var.variable(2, 3)
        p = matrix_analysis.var.variable(Unsure)
        self.assertTrue(matrix_analysis.funcs.sech(p).is_arbitrary)
        self.assertAlmostEqual(z := matrix_analysis.funcs.csch(
            x), -0.272548661463-4.03005788568e-2j)
        self.assertFalse(z.is_arbitrary)

    def test_arcsinh(self):
        x = matrix_analysis.var.variable(2, 3)
        p = matrix_analysis.var.variable(Unsure)
        self.assertTrue(matrix_analysis.funcs.csch(p).is_arbitrary)
        self.assertAlmostEqual(z := matrix_analysis.funcs.arcsinh(
            x), 1.96863792579+0.964658504408j)
        self.assertFalse(z.is_arbitrary)

    def test_arccosh(self):
        x = matrix_analysis.var.variable(2, 3)
        p = matrix_analysis.var.variable(Unsure)
        self.assertTrue(matrix_analysis.funcs.arcsinh(p).is_arbitrary)
        self.assertAlmostEqual(z := matrix_analysis.funcs.arccosh(
            x), 1.98338702992+1.00014354247j)
        self.assertFalse(z.is_arbitrary)

    def test_arctanh(self):
        x = matrix_analysis.var.variable(2, 3)
        p = matrix_analysis.var.variable(Unsure)
        self.assertTrue(matrix_analysis.funcs.arccosh(p).is_arbitrary)
        self.assertAlmostEqual(z := matrix_analysis.funcs.arctanh(
            x), 0.146946666226+1.33897252229j)
        self.assertFalse(z.is_arbitrary)

    def test_arccoth(self):
        x = matrix_analysis.var.variable(2, 3)
        p = matrix_analysis.var.variable(Unsure)
        self.assertTrue(matrix_analysis.funcs.arctanh(p).is_arbitrary)
        self.assertAlmostEqual(z := matrix_analysis.funcs.arccoth(
            x), 0.146946666225-0.2318238045j)
        self.assertFalse(z.is_arbitrary)

    def test_arcsech(self):
        x = matrix_analysis.var.variable(2, 3)
        p = matrix_analysis.var.variable(Unsure)
        self.assertTrue(matrix_analysis.funcs.arcsech(p).is_arbitrary)
        self.assertAlmostEqual(z := matrix_analysis.funcs.arcsech(
            x), 0.231334698574-1.42041072247j)
        self.assertFalse(z.is_arbitrary)

    def test_arccsch(self):
        x = matrix_analysis.var.variable(2, 3)
        p = matrix_analysis.var.variable(Unsure)
        self.assertTrue(matrix_analysis.funcs.arcsech(p).is_arbitrary)
        self.assertAlmostEqual(z := matrix_analysis.funcs.arccsch(
            x), 0.157355498845-0.229962902377j)
        self.assertFalse(z.is_arbitrary)
        self.assertTrue(matrix_analysis.funcs.arccsch(p).is_arbitrary)

    def test_log(self):
        x = matrix_analysis.var.variable(2, 3)
        y = matrix_analysis.var.variable(1, 2)
        p = matrix_analysis.var.variable(Unsure)
        self.assertAlmostEqual(z := matrix_analysis.funcs.log(
            x, y), 1.13173165582-0.335771297901j)
        self.assertFalse(z.is_arbitrary)
        self.assertAlmostEqual(z := matrix_analysis.funcs.log(
            x, 1+2j), 1.13173165582-0.335771297901j)
        self.assertFalse(z.is_arbitrary)
        self.assertAlmostEqual(z := matrix_analysis.funcs.log(
            2+3j, y), 1.13173165582-0.335771297901j)
        self.assertFalse(z.is_arbitrary)
        self.assertTrue(matrix_analysis.funcs.log(x, p).is_arbitrary)
        self.assertTrue(matrix_analysis.funcs.log(p, y).is_arbitrary)
        with self.assertWarns(RuntimeWarning):
            matrix_analysis.funcs.log(x, 0)
        with self.assertRaises(TypeError):
            matrix_analysis.funcs.log(x, "abc")  # type: ignore
        with self.assertRaises(TypeError):
            matrix_analysis.funcs.log("abc", x)  # type: ignore
