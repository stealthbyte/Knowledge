import random

def convert_opcode(opcode, filler):
    return convert_opcode_string(opcode, filler)

def convert_opcode_string(opcode, filler):
    '''
        PURPOSE - Convert a string of opcode numbers to little endian values
        INPUT
            opcode - String of opcode numbers (e.g., copied from object code)
            filler - Value to 'round out' the byte alignment
        OUTPUT - String of escaped opcode values byte-ordered for little endian
    '''
    # LOCAL VARIABLES
    retVal = ""  # String of escaped opcode values byte-ordered for little endian
    inStr = opcode  # 'Working' copy of opcode, in case the alignment needs to be rounded out
    wrdIndexStart = 0  # Index of the beginning of the current word
    wrdIndexStop = 0  # Index of the end of the current word
    tmpStr = ""  # Return value from reverse_endianness

    # INPUT VALIDATION
    if not isinstance(opcode, str):
        raise TypeError("Expected string: opcode")
    elif not isinstance(filler, str):
        raise TypeError("Expected string: filler")
    elif 0 == opcode.__len__():
        raise ValueError("Empty string: opcode")
    elif 0 == filler.__len__():
        raise ValueError("Empty string: filler")
    elif 2 < filler.__len__():
        raise ValueError("Too long: filler")
    elif 0 != (inStr.__len__() % 2):
        raise ValueError("Odd length: opcode")
    elif 0 != (inStr.__len__() % (2 * 4)):
        # print("\nBefore:\t{}\n".format(inStr))  # DEBUGGING
        # print("inStr.len() == {}\n".format(inStr.__len__()))
        # print("Missing:\t{}".format(8 - (inStr.__len__() % 8)))
        inStr = inStr + (filler * ((8 - (inStr.__len__() % (2 * 4))) / 2))
        # print(  "After: \t{}\n".format(inStr))  # DEBUGGING

    # REVERSE IT
    while (wrdIndexStop < inStr.__len__()):
        # Setup indices
        wrdIndexStart = wrdIndexStop
        wrdIndexStop += 8
        # Reverse
        tmpStr = reverse_endianness(inStr[wrdIndexStart:wrdIndexStop])

        retVal = retVal + tmpStr

    # print("\ntmpstr:\t{}".format(tmpStr))  # DEBUGGING
    # CONVERT IT
    # tmpStr = retVal
    retVal = escape_hex(retVal)

    # DONE
    return retVal


def convert_opcode_bytes(opcode, filler):
    '''
        PURPOSE - Convert a string of opcode bytes to little endian order
        INPUT
            opcode - String of opcode byte values (e.g., copied from \\xNN object code)
            filler - Value to 'round out' the byte alignment
        OUTPUT - String of escaped opcode values byte-ordered for little endian
    '''
    # LOCAL VARIABLES
    retVal = ""  # String of escaped opcode values byte-ordered for little endian
    inStr = opcode  # 'Working' copy of opcode, in case the alignment needs to be rounded out
    wrdIndexStart = 0  # Index of the beginning of the current word
    wrdIndexStop = 0  # Index of the end of the current word
    tmpStr = ""  # Return value from reverse_endianness

    # INPUT VALIDATION
    if not isinstance(opcode, str):
        raise TypeError("Expected string: opcode")
    elif not isinstance(filler, str):
        raise TypeError("Expected string: filler")
    elif 0 == opcode.__len__():
        raise ValueError("Empty string: opcode")
    elif 0 == filler.__len__():
        raise ValueError("Empty string: filler")
    elif 2 < filler.__len__():
        raise ValueError("Too long: filler")
    elif 0 != (inStr.__len__() % 2):
        raise ValueError("Odd length: opcode")
    elif 0 != (inStr.__len__() % (2 * 4)):
        inStr = inStr + (filler * ((8 - (inStr.__len__() % (2 * 4))) / 2))

    # REVERSE IT
    while (wrdIndexStop < inStr.__len__()):
        # Setup indices
        wrdIndexStart = wrdIndexStop
        wrdIndexStop += 8
        # Reverse
        tmpStr = reverse_endianness(inStr[wrdIndexStart:wrdIndexStop])

        retVal = retVal + tmpStr

    # DONE
    return retVal


def reverse_endianness(hexWord):
    '''
        PURPOSE - Flip the byte ordering of a string holding hex values as characters
        INPUT
            hexWord - A non-zero length string characters holding stringified values ("90" instead of "\x90")
        OUTPUT - A string holding the reverse stringified values 
    '''
    # LOCAL VARIABLES
    retVal = ""
    hexIndexStart = hexWord.__len__() - 2  # Index of the beginning of the current hex byte
    hexIndexStop = hexWord.__len__()  # Index of the end of the current hex byte

    # INPUT VALIDATION
    if not isinstance(hexWord, str):
        raise TypeError("Expected string: hexWord")
    elif 0 == hexWord.__len__():
        # print("\nhexword len:\t{}\n".format(hexWord.__len__()))  # DEBUGGING
        raise ValueError("Invalid length: hexWord")

    # FLIP IT
    while 0 < hexIndexStop:
        retVal += hexWord[hexIndexStart:hexIndexStop]
        # Decrement
        hexIndexStop = hexIndexStart
        hexIndexStart -= 2

    # DONE
    return retVal


def escape_hex(hexString):
    '''
        PURPOSE - Convert a string of hex values into a string of actual hex values
        INPUT
            hexString - A string of hex values represented by characters
        OUTPUT - A string of escaped hex values in a string
        NOTE
            This function will not modify order
    '''
    # LOCAL VARIABLES
    retVal = ""
    curIndex = 2  # End of the two-nibble slice being converted
    validChars = [ "0","1","2","3","4","5","6","7","8","9","a","b","c","d","e","f" ]

    # INPUT VALIDATION
    if not isinstance(hexString, str):
        raise TypeError("Expected string: hexString")
    elif 0 == hexString.__len__():
        raise ValueError("Empty string: hexString")
    elif 0 != hexString.__len__() % 2:
        raise ValueError("Invalid length: hexString")
    else:
        for char in hexString:
            if char.lower() not in validChars:
                raise ValueError("Invalid character: {}".format(char))

    # ESCAPE IT
    try:
        retVal = bytearray.fromhex(hexString)
    except Exception as err:
        print(repr(err))
        print(hexString)
        raise err

    # DONE
    return retVal


def generate_addrs(byte1, byte2, byte3, byte4):
    '''
        PURPOSE - Generate a psuedo-random hex memory address based on specified ranges/criteia
        INPUT
            byte? - A static string, list of low-to-high values, or None
                string - Will use that static string instead of randomizing values
                list - A list of two integers ranging from 0 - 255
                None - Randomize all available values
        OUTPUT
            string of 8 hex digits representing a random address
    '''
    # LOCAL VARIABLES
    retVal = ""

    # INPUT VALIDATION
    paramList = [ byte1, byte2, byte3, byte4 ]
    for param in paramList:
        if isinstance(param, str):
            if 2 != param.__len__():
                raise ValueError("Invalid string parameter length:\t{}".format(param))
        elif isinstance(param, list):
            if 2 != param.__len__():
                raise ValueError("Invalid list parameter length:\t{}".format(param))
            else:
                for entry in param:
                    if not isinstance(entry, int):
                        raise TypeError("Invalid list parameter entry:\t{}".format(entry))
                    if entry < 0 or entry > 255:
                        raise ValueError("List parameter entry out of range:\t{}".format(entry))
        elif param is None:
            pass
        else:
            raise TypeError("Unsupported data type:\t{}".format(param))

    # PARSE PARAMETERS
    try:
        for param in paramList:
            retVal += parse_byte_param(param)
    except Exception as err:
        print(repr(err))
        raise err

    # FINAL VALIDATION
    if not isinstance(retVal, str):
        raise TypeError("parse_byte_param() has failed!")
    elif 8 != retVal.__len__():
        # print(retVal.__len__())
        raise ValueError("parse_byte_param() has failed!!")

    # DONE
    return retVal


def parse_byte_param(byteX):
    '''
        PURPOSE - Randomize a value as specified by the byteX criteria and return that value
            represented as a two-digit hex value in a string
    '''
    # LOCAL VARIABLES
    retVal = ""
    tmpInt = 0

    # PARSE BYTEX
    if isinstance(byteX, str):
        retVal = byteX
    elif isinstance(byteX, list):
        if byteX[0] == byteX[1]:
            tmpInt = byteX[0]
        elif byteX[0] < byteX[1]:
            tmpInt = random.randint(byteX[0], byteX[1])
        elif byteX[0] > byteX[1]:
            tmpInt = random.randint(bytex[1], bytex[0])
        else:
            raise RuntimeError("How did we get here?!")
    elif byteX is None:
        tmpInt = random.randint(0,255)
    else:
        raise RuntimeError("How did we get HERE?!?!")

    # CONVERT
    if retVal.__len__() == 0:
    #     # Two digits
    #     if tmpInt > 15:
    #         retVal += hex((tmpInt & 240) >> 4).split('x')[-1]
    #     # Final digit
    #     retVal += hex(tmpInt & 15).split('x')[-1]
        retVal = convert_int_to_hex_bytes(tmpInt)

    # EXTEND
    if retVal.__len__() == 1:
        retVal = "0" + retVal

    # DONE
    # print("parse_byte_param({}) returned: {}".format(byteX, retVal))  # DEBUGGING
    return retVal


def convert_int_to_hex_bytes(hexValue):
    '''
        PURPOSE - Convert an integer of range(256) to a hex byte (00 through ff) as a string
        INPUT
            hexValue - Integer value within range(256)
        OUTPUT - String object containing the two-hex byte representation of hexValue
    '''
    # LOCAL VARIABLES
    retVal = ""

    # INPUT VALIDATION
    if not isinstance(hexValue, int):
        raise TypeError("Wrong data type")
    elif hexValue < 0:
        raise ValueError("Too small")
    elif hexValue > 255:
        raise ValueError("Too large")

    # CONVERT IT
    # Two digits
    if hexValue > 15:
        retVal += hex((hexValue & 240) >> 4).split('x')[-1]
    # Final digit
    retVal += hex(hexValue & 15).split('x')[-1]

    # EXTEND
    if retVal.__len__() == 1:
        retVal = "0" + retVal

    # CONVERT IT
    # try:
    #     retVal = hex(hexValue).split('x')[-1]
    #     if retVal.__len__() == 1:
    #         retVal = "0" + retVal
    #     retVal = chr(int(hex(hexValue).split('x')[-1], 16))
    #     print("c_i_t_h_b({}) returns {} as a {}".format(hexValue, retVal, type(retVal)))  # DEBUGGING
    # except Exception as err:
    #     print(repr(err))
    #     raise err

    # DONE
    return retVal


def convert_str_to_hex_bytes(hexStr):
    '''
        PURPOSE - Convert a string of byte values to their hex representations in a string
        INPUT
            hexStr - Non-zero length string of values within range(256)
        OUTPUT - A string of hex characters
    '''
    # LOCAL VARIABLES
    retVal = ""

    # INPUT VALIDATION
    if not isinstance(hexStr, str):
        raise TypeError("hexStr is not a string")
    elif 0 == hexStr.__len__():
        raise ValueError("hexStr is empty")
    else:
        for char in hexStr:
            if ord(char) > 255:
                ValueError("Value too large")
            elif ord(char) < 0:
                ValueError("Value too small")

    # CONVERT IT
    print("hexStr length: {}".format(hexStr.__len__()))  # DEBUGGING
    for char in hexStr:
        try:
            print(char)  # DEBUGGING
            retVal += convert_int_to_hex_bytes(ord(char))
        except Exception as err:
            print(repr(err))
            raise err

    # DONE
    return retVal










