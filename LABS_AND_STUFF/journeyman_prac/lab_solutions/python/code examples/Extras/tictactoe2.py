
########################################################################################################################
# Imports
########################################################################################################################

import itertools

########################################################################################################################
# Classes
########################################################################################################################


class SpaceAlreadyTaken(Exception):
    pass


class InvalidCoordinates(Exception):
    pass


class Board:

    def __init__(self, init_state=None):
        if init_state:
            self.state = init_state
        else:
            self.state = [[' ' for i in range(3)] for j in range(3)]

    #-------------------------------------------------------------------------------------------------------------------

    def draw_board(self):
        for j in range(3):
            for i in range(3):
                print(f' {self.state[i][j]} ', end='')
                if j < 3 and i < 2:
                    print('|', end='')
            if j < 2:
                print('\n-----------')
        print(2*'\n')

    #-------------------------------------------------------------------------------------------------------------------

    def make_move(self, move, player):
        try:
            move = move.split(',')
            i = int(move[0])
            j = int(move[1])
        except:
            raise InvalidCoordinates('Coordinates should be two integers separated by a comma')

        if i not in range(3):
            raise InvalidCoordinates('x-coordinate should be 0, 1, or 2!')
        
        if j not in range(3):
            raise InvalidCoordinates('y-coordinate should be 0, 1, or 2!')

        if self.state[i][j] != ' ':
            raise SpaceAlreadyTaken(f'({i}, {j}) is already taken!')

        self.state[i][j] = player
    
    #-------------------------------------------------------------------------------------------------------------------
    
    def game_won(self):
        
        # check rows
        for j in range(3):
            if self.state[0][j] == self.state[1][j] == self.state[2][j] != ' ':
                return True

        # check columns
        for i in range(3):
            if self.state[i][0] == self.state[i][1] == self.state[i][2] != ' ':
                return True

        # check diagonals
        if self.state[0][0] == self.state[1][1] == self.state[2][2] != ' ':
            return True

        if self.state[2][0] == self.state[1][1] == self.state[0][2] != ' ':
            return True

        return False        

########################################################################################################################
# Main
########################################################################################################################

if __name__ == '__main__':
    
    b = Board()
    b.draw_board()

    for turn in itertools.cycle('XO'):
        while True:
            move = input(f'Enter Move(x,y) for Player {turn}: ')

            try:
                b.make_move(move, turn)
            except (InvalidCoordinates, SpaceAlreadyTaken) as e:
                print(e)
            else:
                b.draw_board()
                break

        if b.game_won():
            print(f'{turn} wins!!!')
            break
        elif all(' ' not in row for row in b.state):
            print('Draw!')
            break
