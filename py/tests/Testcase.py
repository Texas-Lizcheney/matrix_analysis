import unittest
from matrix_analysis import Unsure
from matrix_analysis.var import variable
from matrix_analysis.matrix import matrix


class externed_Testcase(unittest.TestCase):
    def assertUnsure(self, x):
        if (x is not Unsure):
            raise AssertionError(f"{x} is not unsure")

    def assertVarAlmostEqual(self, first: variable, second: variable, places=None, msg=None, delta=None):
        if (first.is_arbitrary and second.is_arbitrary):
            return
        self.assertFalse(first.is_arbitrary)
        self.assertFalse(second.is_arbitrary)
        self.assertAlmostEqual(first.real, second.real, places, msg, delta)
        self.assertAlmostEqual(first.imag, second.imag, places, msg, delta)

    def assertMatrixAlmostEqual(self, first: matrix, second: matrix, places=None, msg=None, delta=None):
        self.assertEqual(p := first.shape, second.shape)
        for i in range(p[0]):
            for j in range(p[1]):
                self.assertVarAlmostEqual(first[i, j], second[i, j])


class seqTestLoader(unittest.TestLoader):
    def getTestCaseNames(self, testcase_class):
        test_names = super().getTestCaseNames(testcase_class)
        testcase_methods = list(testcase_class.__dict__.keys())
        test_names.sort(key=testcase_methods.index)  # type: ignore
        return test_names
