#!/usr/bin/python2

from Opcoad import convert_opcode
import unittest


### NORMAL ###
class NormalTests(unittest.TestCase):


    def test_01(self):
        # LOCAL VARIABLES
        testInput = "00112233"
        fillerInput = "90"
        expOutput = "\x33\x22\x11\x00"
        actOutput = ""

        # FUNCTION CALL
        try:
            actOutput = convert_opcode(testInput, fillerInput)
        except TypeError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except ValueError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except Exception as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        else:
            self.assertEqual(expOutput, actOutput)


    def test_02(self):
        # LOCAL VARIABLES
        testInput = "0011223344556677"
        fillerInput = "90"
        expOutput = "\x33\x22\x11\x00\x77\x66\x55\x44"
        actOutput = ""

        # FUNCTION CALL
        try:
            actOutput = convert_opcode(testInput, fillerInput)
        except TypeError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except ValueError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except Exception as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        else:
            self.assertEqual(expOutput, actOutput)


    def test_03(self):
        # LOCAL VARIABLES
        testInput = "00112233445566778899AABBCCDDEEFF"
        fillerInput = "90"
        expOutput = "\x33\x22\x11\x00\x77\x66\x55\x44\xBB\xAA\x99\x88\xFF\xEE\xDD\xCC"
        actOutput = ""

        # FUNCTION CALL
        try:
            actOutput = convert_opcode(testInput, fillerInput)
        except TypeError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except ValueError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except Exception as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        else:
            self.assertEqual(expOutput, actOutput)


### ERROR ###
class ErrorTests(unittest.TestCase):


    def test_01(self):
        # LOCAL VARIABLES
        testInput = [90, 90, 90, 90]
        fillerInput = "90"
        expOutput = ""
        actOutput = ""

        # FUNCTION CALL
        try:
            actOutput = convert_opcode(testInput, fillerInput)
        except TypeError as err:
            self.assertEqual(err.message, "Expected string: opcode")
        except ValueError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except Exception as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        else:
            self.assertEqual(expOutput, actOutput)


    def test_02(self):
        # LOCAL VARIABLES
        testInput = ""
        fillerInput = "90"
        expOutput = ""
        actOutput = ""

        # FUNCTION CALL
        try:
            actOutput = convert_opcode(testInput, fillerInput)
        except TypeError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except ValueError as err:            
            self.assertEqual(err.message, "Empty string: opcode")
        except Exception as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        else:
            self.assertEqual(expOutput, actOutput)


    def test_03(self):
        # LOCAL VARIABLES
        testInput = "9090909090909090"
        fillerInput = [90, 90]
        expOutput = ""
        actOutput = ""

        # FUNCTION CALL
        try:
            actOutput = convert_opcode(testInput, fillerInput)
        except TypeError as err:
            self.assertEqual(err.message, "Expected string: filler")
        except ValueError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except Exception as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        else:
            self.assertEqual(expOutput, actOutput)


    def test_04(self):
        # LOCAL VARIABLES
        testInput = "9090909090909090"
        fillerInput = ""
        expOutput = ""
        actOutput = ""

        # FUNCTION CALL
        try:
            actOutput = convert_opcode(testInput, fillerInput)
        except TypeError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except ValueError as err:            
            self.assertEqual(err.message, "Empty string: filler")
        except Exception as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        else:
            self.assertEqual(expOutput, actOutput)


    def test_05(self):
        # LOCAL VARIABLES
        testInput = "9090909090909090"
        fillerInput = "909"
        expOutput = ""
        actOutput = ""

        # FUNCTION CALL
        try:
            actOutput = convert_opcode(testInput, fillerInput)
        except TypeError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except ValueError as err:            
            self.assertEqual(err.message, "Too long: filler")
        except Exception as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        else:
            self.assertEqual(expOutput, actOutput)


### BOUNDARY ###
class BoundaryTests(unittest.TestCase):


    def test_01(self):
        # LOCAL VARIABLES
        testInput = "00"
        fillerInput = "90"
        expOutput = "\x90\x90\x90\x00"
        actOutput = ""

        # FUNCTION CALL
        try:
            actOutput = convert_opcode(testInput, fillerInput)
        except TypeError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except ValueError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except Exception as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        else:
            self.assertEqual(expOutput, actOutput)


    def test_02(self):
        # LOCAL VARIABLES
        testInput = "0011"
        fillerInput = "90"
        expOutput = "\x90\x90\x11\x00"
        actOutput = ""

        # FUNCTION CALL
        try:
            actOutput = convert_opcode(testInput, fillerInput)
        except TypeError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except ValueError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except Exception as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        else:
            self.assertEqual(expOutput, actOutput)


    def test_03(self):
        # LOCAL VARIABLES
        testInput = "001122"
        fillerInput = "90"
        expOutput = "\x90\x22\x11\x00"
        actOutput = ""

        # FUNCTION CALL
        try:
            actOutput = convert_opcode(testInput, fillerInput)
        except TypeError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except ValueError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except Exception as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        else:
            self.assertEqual(expOutput, actOutput)

   
    def test_04(self):
        # LOCAL VARIABLES
        testInput = "0011223344"
        fillerInput = "90"
        expOutput = "\x33\x22\x11\x00\x90\x90\x90\x44"
        actOutput = ""

        # FUNCTION CALL
        try:
            actOutput = convert_opcode(testInput, fillerInput)
        except TypeError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except ValueError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except Exception as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        else:
            self.assertEqual(expOutput, actOutput)


    def test_05(self):
        # LOCAL VARIABLES
        testInput = "001122334455"
        fillerInput = "90"
        expOutput = "\x33\x22\x11\x00\x90\x90\x55\x44"
        actOutput = ""

        # FUNCTION CALL
        try:
            actOutput = convert_opcode(testInput, fillerInput)
        except TypeError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except ValueError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except Exception as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        else:
            self.assertEqual(expOutput, actOutput)


    def test_06(self):
        # LOCAL VARIABLES
        testInput = "00112233445566"
        fillerInput = "90"
        expOutput = "\x33\x22\x11\x00\x90\x66\x55\x44"
        actOutput = ""

        # FUNCTION CALL
        try:
            actOutput = convert_opcode(testInput, fillerInput)
        except TypeError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except ValueError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except Exception as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        else:
            self.assertEqual(expOutput, actOutput)


    def test_07(self):
        # LOCAL VARIABLES
        testInput = "00"
        fillerInput = "CC"
        expOutput = "\xCC\xCC\xCC\x00"
        actOutput = ""

        # FUNCTION CALL
        try:
            actOutput = convert_opcode(testInput, fillerInput)
        except TypeError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except ValueError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except Exception as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        else:
            self.assertEqual(expOutput, actOutput)


    def test_08(self):
        # LOCAL VARIABLES
        testInput = "0011"
        fillerInput = "CC"
        expOutput = "\xCC\xCC\x11\x00"
        actOutput = ""

        # FUNCTION CALL
        try:
            actOutput = convert_opcode(testInput, fillerInput)
        except TypeError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except ValueError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except Exception as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        else:
            self.assertEqual(expOutput, actOutput)


    def test_09(self):
        # LOCAL VARIABLES
        testInput = "001122"
        fillerInput = "CC"
        expOutput = "\xCC\x22\x11\x00"
        actOutput = ""

        # FUNCTION CALL
        try:
            actOutput = convert_opcode(testInput, fillerInput)
        except TypeError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except ValueError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except Exception as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        else:
            self.assertEqual(expOutput, actOutput)

   
    def test_10(self):
        # LOCAL VARIABLES
        testInput = "0011223344"
        fillerInput = "CC"
        expOutput = "\x33\x22\x11\x00\xCC\xCC\xCC\x44"
        actOutput = ""

        # FUNCTION CALL
        try:
            actOutput = convert_opcode(testInput, fillerInput)
        except TypeError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except ValueError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except Exception as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        else:
            self.assertEqual(expOutput, actOutput)


    def test_11(self):
        # LOCAL VARIABLES
        testInput = "001122334455"
        fillerInput = "CC"
        expOutput = "\x33\x22\x11\x00\xCC\xCC\x55\x44"
        actOutput = ""

        # FUNCTION CALL
        try:
            actOutput = convert_opcode(testInput, fillerInput)
        except TypeError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except ValueError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except Exception as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        else:
            self.assertEqual(expOutput, actOutput)


    def test_12(self):
        # LOCAL VARIABLES
        testInput = "00112233445566"
        fillerInput = "CC"
        expOutput = "\x33\x22\x11\x00\xCC\x66\x55\x44"
        actOutput = ""

        # FUNCTION CALL
        try:
            actOutput = convert_opcode(testInput, fillerInput)
        except TypeError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except ValueError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except Exception as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        else:
            self.assertEqual(expOutput, actOutput)


### SPECIAL ###
class SpecialTests(unittest.TestCase):


    def test_01(self):
        # LOCAL VARIABLES
        testInput = "eb134831c0b001bf010000005e4831d2b2290f05cce8e8ffffff5468617420776561766520" + \
                    "676f74746120676f2e2050696e6b20736c69702e202d436172646920420a00"
        fillerInput = "90"
        expOutput = "\x31\x48\x13\xeb\xbf\x01\xb0\xc0\x00\x00\x00\x01\xd2\x31\x48\x5e\x05\x0f" + \
                    "\x29\xb2\xff\xe8\xe8\xcc\x68\x54\xff\xff\x77\x20\x74\x61\x65\x76\x61\x65" + \
                    "\x74\x6f\x67\x20\x67\x20\x61\x74\x50\x20\x2e\x6f\x20\x6b\x6e\x69\x70\x69" + \
                    "\x6c\x73\x43\x2d\x20\x2e\x69\x64\x72\x61\x00\x0a\x42\x20"
        actOutput = ""

        # FUNCTION CALL
        try:
            actOutput = convert_opcode(testInput, fillerInput)
        except TypeError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except ValueError as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        except Exception as err:
            self.fail("Raised exception:\t{}".format(repr(err)))
        else:
            self.assertEqual(expOutput, actOutput)


if __name__ == '__main__':
    unittest.main(verbosity = 2)