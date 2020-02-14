import unittest

class TestClass(unittest.TestCase):

    def setUp(self):
        print('In setUp!')


    def tearDown(self):
        print('In tearDown!')


    def test_adder_success(self):
        print('In %s' % self._testMethodName)
        self.assertEqual(4 + 4, 8)


    @unittest.expectedFailure
    def test_adder_fail(self):
        print('In %s' % self._testMethodName)
        self.assertEqual(4 + 3, 8)


if __name__ == '__main__':
    unittest.main()