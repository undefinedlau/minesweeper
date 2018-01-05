#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "mine_sweeper.h"



int main (int argc, char* argv[]) {
	
	board* bd = (board*)malloc(sizeof(board));
	user_play* up = (user_play*) malloc(sizeof(user_play)); // struct to store user_play struct
	
	
	check_args(argc, argv, bd);
	
	bd->rows = atoi(argv[1]);
	bd->columns = atoi(argv[2]);
	bd->mines = atoi(argv[3]);
	
	
	tile** play_board = create_board(bd);
	create_game_board(bd);
	
	play_game(bd,play_board,up,argc); 
	//destroy_board(bd,up,play_board);

	
    return 0;
}

