
PLAYER_X = "X"
PLAYER_O = "O"
CAT_GAME = "CAT"
EMPTY = " "


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

    def MakeMove(self, move, player):

        try:
            move = move.split(",")
            x = int(move[0])
            y = int(move[1])
        except:
            return False

        if not self.IsLegalMove(x,y):
            return False
        else:
            self.state[x][y] = player
            return True
        
    def IsLegalMove(self,x,y):
        if x >= 0 and x < 3 and y  >= 0 and y < 3 and self.state[x][y] == EMPTY:
            return True
        else:
            return False
        
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


if __name__ == "__main__":
    
    b = Board()
    b.DrawBoard()

    playerMove = PLAYER_X
    while b.CheckGameOver() == None:

        move = input("Enter Move(x,y) for Player %s: "%playerMove)

        while not b.MakeMove(move, playerMove):
            print("Invalid Move!")
            b.DrawBoard()
            move = input("\nEnter Move(x,y) for Player %s: "%playerMove)

        b.DrawBoard()

        if playerMove == PLAYER_X:
            playerMove = PLAYER_O
        else:
            playerMove = PLAYER_X

    winner = b.CheckGameOver()
    if winner == CAT_GAME:
        print("Cat's Game!")
    else:
        print("The Winner is %s!!!" % winner)
