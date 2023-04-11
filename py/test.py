import sys
import unittest
sys.path.append("..")

if __name__ == "__main__":
    from tests.matrix import Test_mat
    from tests.var import Test_var
    from tests.Testcase import seqTestLoader
    unittest.main(testLoader=seqTestLoader())
