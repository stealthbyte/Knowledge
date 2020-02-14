import pickle
import random
import struct
import socket
import sys

PLAYER_X = "X"
PLAYER_O = "O"
CAT_GAME = "CAT"
EMPTY = " "


TTT_PORT = 8879

MESSAGE_MAKEMOVE = 1
MESSAGE_GAMEOVER = 2
MESSAGE_PRINTMESSAGE = 3


class Message(object):
    def __init__(self):
        self.type = 0
        self.len = 0
        self.data = 0

class Board(object):

    def __init__(self, initState=None):
        if initState:
            self.state = initState
        else:
            self.state = [[EMPTY, EMPTY, EMPTY],
                          [EMPTY, EMPTY, EMPTY],
                          [EMPTY, EMPTY, EMPTY]]
    def DrawBoard(self):

        for y in range(3):
            for x in range(3):
                print(self.state[x][y], end=' ')
                if y < 3 and x < 2:
                    print(" | ", end=' ')
            if y < 2:
                print("\n-------------")
        print("\n\n")

        

class TTTServer(Board):
    
    def __init__(self):
        super(TTTServer, self).__init__()
        self.listenSock = socket.socket()
        self.listenSock.bind(("", TTT_PORT))
        self.listenSock.listen(5)
        self.gameSock, opponent = self.listenSock.accept()
        self.listenSock.close()

        self.player = random.choice((PLAYER_X, PLAYER_O))

    def play(self):

        while (self.CheckGameOver() == None):

            

            if(self.player == PLAYER_X):    #Server is always X
                self.MakeMove()
                self.DrawBoard()
            else:
                self.GetMove()


        self.HandleGameOver()

    def HandleGameOver(self):
        winner = self.CheckGameOver()

        winnerMsg = "Game Over! The Winner is %s" %winner
        print(winnerMsg)
        
        msg = Message()
        msg.type = MESSAGE_PRINTMESSAGE
        msg.data = winnerMsg
        msg.len = len(winnerMsg)

        self.SendMsg(msg)

        msg = Message()
        msg.type = MESSAGE_GAMEOVER
        msg.data = pickle.dumps(self.state)
        msg.len = len(msg.data)

        self.SendMsg(msg)
        
        self.gameSock.close()

    def GetMove(self):

        msg = Message()
        msg.type = MESSAGE_MAKEMOVE
        msg.data = pickle.dumps(self.state)
        msg.len = len(msg.data)

        self.SendMsg(msg)

        print("Waiting for move from remote player...")
        move = self.gameSock.recv(100)

        if self.IsLegalMove(move):
            move = move.split(",")
            x = int(move[0])
            y = int(move[1])

            self.state[x][y] = PLAYER_O
            self.player = PLAYER_X
        else:
            msg = Message()
            msg.type = MESSAGE_PRINTMESSAGE
            msg.data = "INVALID MOVE!"
            msg.len = len(msg.data)
            self.SendMsg(msg)

    def SendMsg(self, msg):
        self.gameSock.send(struct.pack("B", msg.type))
        self.gameSock.send(struct.pack("B", msg.len))
        self.gameSock.send(msg.data)

    def CheckGameOver(self):

        #Check Rows
        for y in range(3):
            if self.state[0][y] == self.state[1][y] == self.state[2][y] and self.state[0][y] != EMPTY:
                return self.state[0][y]

        #Check Columns
        for x in range(3):
            if self.state[x][0] == self.state[x][1] == self.state[x][2] and self.state[x][0] != EMPTY:
                return self.state[x][0]

        #Check Diags
        if self.state[0][0] == self.state[1][1] == self.state[2][2] and self.state[0][0] != EMPTY:
            return self.state[0][0]

        if self.state[2][0] == self.state[1][1] == self.state[0][2] and self.state[2][0] != EMPTY:
            return self.state[2][0]

        #check for open spaces
        emptySpaceFound = False
        for x in range(3):
            for y in range(3):
                if self.state[x][y] == EMPTY:
                    return None

        return CAT_GAME

    def MakeMove(self):

        self.DrawBoard()

        move = input("Enter Move for Player X: ")


        if not self.IsLegalMove(move):
            print("INVALID MOVE!")
            return
        else:
            move = move.split(",")
            x = int(move[0])
            y = int(move[1])
            self.state[x][y] = PLAYER_X
            self.player = PLAYER_O
            return

    def IsLegalMove(self,move):
        
        try:
            move = move.split(",")
            x = int(move[0])
            y = int(move[1])
        except:
            return False
        
        if x >= 0 and x < 3 and y  >= 0 and y < 3 and self.state[x][y] == EMPTY:
            return True
        else:
            return False

        
    

class TTTClient(Board):
    def __init__(self):
        super(TTTClient, self).__init__()

       # host = raw_input("Enter IP to connect to: ")
        host = "127.0.0.1"
        self.sock = socket.socket()
        self.sock.connect((host, TTT_PORT))

    def RecvMsg(self):

        msg = Message()
        msg.type = struct.unpack("B", self.sock.recv(1))[0]
        msg.len = struct.unpack("B", self.sock.recv(1))[0]
        msg.data = self.sock.recv(msg.len)

        return msg

    def play(self):

        cont = True
        while cont:

            print("Waiting for data from server...")
            msg = self.RecvMsg()

            if msg.type == MESSAGE_MAKEMOVE:
                self.MakeMove(msg)
            elif msg.type == MESSAGE_GAMEOVER:
                self.GameOver(msg)
                cont = False
            elif msg.type == MESSAGE_PRINTMESSAGE:
                self.Print(msg)
            else:
                print("Invalid msg received! Aborting Game")
                cont = False
            
    def MakeMove(self, msg):

        self.state = pickle.loads(msg.data)
        self.DrawBoard()

        move = input("Enter your Move: ")
        
        self.sock.send(move)

    def GameOver(self, msg):

        self.state = pickle.loads(msg.data)
        self.DrawBoard()
        
        print("Game is over!")
        self.sock.close()

    def Print(self, msg):

        print(msg.data)


        
        


if __name__ == "__main__":

    side = ""
    side = sys.argv[1]
    
    while side != "S" and side != "C":
        side = input("Enter S for server or C for client: ")

    if side == "S":
        game = TTTServer()
    else:
        game = TTTClient()

    game.play()
    
    
