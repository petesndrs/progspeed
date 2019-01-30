package main

import "fmt"

const FIRST_PLAYER int = 1
const SECOND_PLAYER int = 2

const NUMBER_SQUARES int = 9

func has_won(board []int) int {
	if board[0]==board[1] && board[1]==board[2] {
	    return board[0]
	} else if board[3]==board[4] && board[4]==board[5] {
	    return board[3]
	} else if board[6]==board[7] && board[7]==board[8] {
	    return board[6]
	} else if board[0]==board[3] && board[3]==board[6] {
	    return board[0]
	} else if board[1]==board[4] && board[4]==board[7] {
	    return board[1]
	} else if board[2]==board[5] && board[5]==board[8] {
	    return board[2]
	} else if board[0]==board[4] && board[4]==board[8] {
	    return board[0]
	} else if board[2]==board[4] && board[4]==board[6] {
	    return board[2]
	}
	return 0
}

func reset_square(board []int, square int) {
	board[square] = 100 + square
}

func reset_board(board []int) {
	var i int
	for i = 0; i<NUMBER_SQUARES; i+=1 {
		reset_square(board, i)
	}
}

func square_free(board []int, square int) bool {
	return (board[square] == 100 + square)
}

func next_move(player int, board []int, moves []int, result []int, move int){
	var i int
	for i = 0; i < NUMBER_SQUARES; i+=1 {
		if square_free(board, i) {
			board[i] = player
			var winner int = has_won(board);
			if winner > 0 {
				reset_square(board, i)
				moves[move-1] += 1
				result[winner] += 1
				return
			}
			if move == NUMBER_SQUARES {
				moves[move-1] += 1
				result[0] += 1
				reset_square(board, i)
				return
			}
			var next_player int = FIRST_PLAYER;
			if player == FIRST_PLAYER  { next_player = SECOND_PLAYER }
			next_move(next_player, board, moves, result, move+1)
			reset_square(board, i)
		}
	}
}

func main() {
	var board[NUMBER_SQUARES] int
	moves := [NUMBER_SQUARES] int {0, 0, 0, 0, 0, 0, 0, 0, 0}
	result := [3] int {0, 0, 0}
	
	reset_board(board[:])
	
	next_move(FIRST_PLAYER, board[:], moves[:], result[:], 1)
	
	fmt.Printf("No Result  : %d\n",result[0])
	fmt.Printf("First Wins : %d\n",result[FIRST_PLAYER])
	fmt.Printf("Second Wins: %d\n",result[SECOND_PLAYER])
	
	var i int
	for i = 0; i < NUMBER_SQUARES; i+=1 {
		fmt.Printf("Moves %d = %d\n", i+1, moves[i])
	}
}