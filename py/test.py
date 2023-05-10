import sys
import unittest
sys.path.append("..")

if __name__ == "__main__":
    from tests.Testcase import seqTestLoader
    from tests.matrix import Test_mat, Test_vector
    from tests.var import Test_var, Test_errordouble
    from tests.others import Test_other_methods
    unittest.main(testLoader=seqTestLoader())
