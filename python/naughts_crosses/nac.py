
FIRST_PLAYER=1
SECOND_PLAYER=2

NUMBER_SQUARES=9

def has_won(board):
    if (board[0]==board[1] and board[1]==board[2]):
        return board[0]
    elif (board[3]==board[4] and board[4]==board[5]):
        return board[3]
    elif (board[6]==board[7] and board[7]==board[8]):
        return board[6]
    elif (board[0]==board[3] and board[3]==board[6]):
        return board[0]
    elif (board[1]==board[4] and board[4]==board[7]):
        return board[1]
    elif (board[2]==board[5] and board[5]==board[8]):
        return board[2]
    elif (board[0]==board[4] and board[4]==board[8]):
        return board[0]
    elif (board[2]==board[4] and board[4]==board[6]):
        return board[2]
    return 0;

def reset_square(board, square):
    board[square] = 100 + square

def reset_board(board):
    for i in range (0, NUMBER_SQUARES):
        reset_square(board, i)

def square_free(board, square):
    return (board[square] == 100 + square)


def next_move(player, board, moves, result, move):
    for i in range(0, NUMBER_SQUARES):
        if ( square_free(board, i) ):
            board[i] = player
            winner = has_won(board)
            if (winner):
                reset_square(board, i)
                moves[move-1] += 1
                result[winner] += 1
                return

            if (move == NUMBER_SQUARES):
                moves[move-1] +=1
                result[0] +=1
                reset_square(board, i)
                return

            next_player = FIRST_PLAYER
            if (player == FIRST_PLAYER):
                next_player = SECOND_PLAYER
            next_move(next_player, board, moves, result, move+1)
            reset_square(board, i)


def main():
    board= [0, 0, 0, 0, 0, 0, 0, 0, 0]
    moves = [0, 0, 0, 0, 0, 0, 0, 0, 0]
    result = [0, 0, 0]

    reset_board(board)

    next_move(FIRST_PLAYER, board, moves, result, 1)
    
    print("No Result  : {}".format(result[0]))
    print("First Wins : {}".format(result[FIRST_PLAYER]))
    print("Second Wins: {}".format(result[SECOND_PLAYER]))
    
    for i in range(0, NUMBER_SQUARES):
        print("Moves {} = {}".format(i+1, moves[i]))


if __name__ == "__main__":
    main()