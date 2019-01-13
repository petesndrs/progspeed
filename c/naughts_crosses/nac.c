#include <stdio.h>

#define FIRST_PLAYER 1
#define SECOND_PLAYER 2

#define NUMBER_SQUARES 9

int has_won(int* board) {
	if (board[0]==board[1] && board[1]==board[2]) {
	    return board[0];
	}
	else if (board[3]==board[4] && board[4]==board[5]) {
	    return board[3];
	}
	else if (board[6]==board[7] && board[7]==board[8]) {
	    return board[6];
	}
	else if (board[0]==board[3] && board[3]==board[6]) {
	    return board[0];
	}
	else if (board[1]==board[4] && board[4]==board[7]) {
	    return board[1];
	}
	else if (board[2]==board[5] && board[5]==board[8]) {
	    return board[2];
	}
	else if (board[0]==board[4] && board[4]==board[8]) {
	    return board[0];
	}
	else if (board[2]==board[4] && board[4]==board[6]) {
	    return board[2];
	}
	return 0;
}

void reset_square(int* board, int square) {
	board[square] = 100 + square;
}

void reset_board(int* board) {
	for (int i = 0; i<NUMBER_SQUARES; i++) {
		reset_square(board, i);
	}
}

int square_free(int* board, int square) {
	return (board[square] == 100 + square);
}

void next_move(int player, int* board, int* moves, int* result, int move){
	int i;
	for (i = 0; i < NUMBER_SQUARES; ++i){
		if ( square_free(board, i) ) {
			board[i] = player;
			int winner = has_won(board);
			if (winner){
				reset_square(board, i);
				moves[move-1]++;
				result[winner]++;
				return;
			}
			if (move == NUMBER_SQUARES){
				moves[move-1]++;
				result[0]++;
				reset_square(board, i);
				return;
			}
			int next_player = FIRST_PLAYER;
			if (player == FIRST_PLAYER) next_player = SECOND_PLAYER;
			next_move(next_player, board, moves, result, move+1);
			reset_square(board, i);
		}
	}
}

int main() {
	int board[NUMBER_SQUARES];
	int moves[NUMBER_SQUARES] = {0, 0, 0, 0, 0, 0, 0, 0, 0};
	int result[3] = {0, 0, 0};
	
	reset_board(board);
	
	next_move(FIRST_PLAYER, board, moves, result, 1);
	
	printf("No Result  : %d\n",result[0]);
	printf("First Wins : %d\n",result[FIRST_PLAYER]);
	printf("Second Wins: %d\n",result[SECOND_PLAYER]);
	
	int i;
	for (i = 0; i < NUMBER_SQUARES; ++i){
		printf("Moves %d = %d\n", i+1, moves[i]);
	}
}