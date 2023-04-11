import unittest


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
