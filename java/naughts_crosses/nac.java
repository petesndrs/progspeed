class nac 
{

	final static int FIRST_PLAYER = 1;
	final static int SECOND_PLAYER = 2;

	final static int NUMBER_SQUARES = 9;

	private static int has_won(int[] board)
	{
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

	private static void reset_square(int[] board, int square)
	{
		board[square] = 100 + square;
	}

	private static void reset_board(int[] board)
	{
		for (int i = 0; i<NUMBER_SQUARES; i++) {
			reset_square(board, i);
		}
	}

	private static boolean square_free(int[] board, int square)
	{
		return (board[square] == 100 + square);
	}

	private static void next_move(int player, int[] board, int[] moves, int[] result, int move)
	{
		int i;
		for (i = 0; i < NUMBER_SQUARES; ++i){
			if ( square_free(board, i) ) {
				board[i] = player;
				int winner = has_won(board);
				if (winner != 0){
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

	public static void main(String args[])
	{
		int[] board = new int[NUMBER_SQUARES];
		int[] moves = new int[NUMBER_SQUARES];
		int[] result = new int[3];
		
		reset_board(board);
		
		next_move(FIRST_PLAYER, board, moves, result, 1);
		
		System.out.printf("No Result  : %d\n",result[0]);
		System.out.printf("First Wins : %d\n",result[FIRST_PLAYER]);
		System.out.printf("Second Wins: %d\n",result[SECOND_PLAYER]);
		
		int i;
		for (i = 0; i < NUMBER_SQUARES; ++i){
			System.out.printf("Moves %d = %d\n", i+1, moves[i]);
		}
	}
}