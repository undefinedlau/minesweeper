#include <stdbool.h>

#ifndef MINE_SWEEPER_H
	#define MINE_SWEEPER_H

	typedef struct tile_struct { 
	
		int row; 
		int column;
		bool visible; 
		bool marked;
		bool questioned;
		bool mined;
		int nearmine;
	
	} tile;

	typedef struct board_struct { 
	
		int rows;
		int columns;
		int mines;
		int randSeed;
		int num_marked;
		char** game_board;
		int correct_marked;
	
	} board;

	typedef struct user_play_struct {
	
		int row;
		int column;
	
	} user_play;

	int main(int argc, char* argv[]);
	enum GameStatus{GAME_ON, GAME_WIN, GAME_OVER};
	void check_args (int argc, char* argv[], board* bd);
	tile** create_board (board* bd);
	void set_mines (int argc, board* bd, tile** play_board);
	void create_game_board(board* bd);
	void print_board(board* bd);
	void get_play(board* bd, user_play* up);
	bool check_bounds(user_play* up, board* bd);
	enum GameStatus execute_action(user_play* up, board* bd, tile** play_board);
	void count_mines(tile** play_board, board* bd);
	void reveal(board* bd, tile** play_board, int play_row, int play_column);
	//void destroy_board(board* bd, user_play* up, tile** play_board);
	int check_win(board* bd, tile** play_board);
	void play_game(board* bd, tile** play_board, user_play* up, int argc);

#endif
	

