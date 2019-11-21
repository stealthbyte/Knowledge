#!/usr/bin/python

from subprocess import Popen
from subprocess import PIPE
import os                                                                                        # os.path.join(), os.getcwd()
from struct import pack

BINARY_NAME = "format0"


def main():
    # LOCAL VARIABLES
    currEnv = os.environ.copy()
    absBinFilename = os.path.join(os.getcwd(), BINARY_NAME)
    magicNum = pack("I", 0xdeadbeef)  # 0xdeadbeef
    payload = "%64d" + magicNum
    commandList = []

    # VERIFY FILE    
    if not os.path.isfile(absBinFilename):
        raise IOError("{} not found".format(absBinFilename))

    # RUN IT
    commandList.append(absBinFilename)
    commandList.append(payload)
    binary = Popen(commandList, env = currEnv, stdin = PIPE, stdout = PIPE)
    if binary is not None:
        tmpRet = binary.communicate()
        if tmpRet[0]:
            print(tmpRet[0])
        elif tmpRet[1]:
            print("Error:\t{}".format(tmpRet[1]))
        else:
            print("Nothing")

    # DONE
    return


if __name__ == "__main__":
    try:
        main()
    except Exception as err:
        print(repr(err))
