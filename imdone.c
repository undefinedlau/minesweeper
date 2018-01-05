#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "mine_sweeper.h"



// check args and input for the randSeed
void check_args (int argc, char* argv[], board* bd) {	

    if (argc > 5) {
        printf("Too many arguments. Usage:\n");
        printf("./mine_sweeper.out num_rows num_cols num_mines [seed])\n");
        exit(0);
    }
    
    else if (argc < 4 || atoi(argv[1]) * atoi(argv[2]) < atoi(argv[3])) {
        printf("Not enough arguments. Usage:\n");
        printf("./mine_sweeper.out num_rows num_cols num_mines [seed])\n");
        exit(0);
    }
	
    
    else if (argc == 5) {
		bd->randSeed = atoi(argv[4]);
    }
	
}

// allocate memory for an array of structs, each struct represents a tile of the board
// sketchy on the function
tile** create_board (board* bd) {
	
	int i;
	int j;
	tile** play_board;
	
	play_board = (tile**) malloc(bd->rows * sizeof(tile*));
	
	for (i = 0; i < bd->rows; i++) {
		play_board[i] = (tile*) malloc(bd->columns * sizeof(tile));
	}
	
	// now set each tile's attributes
	
	for (i = 0; i < bd->rows; i++) {
		for (j = 0; j < bd->columns; j++) {
			play_board[i][j].visible = false;
			play_board[i][j].marked = false;
			play_board[i][j].questioned = false;
			play_board[i][j].mined = false;
			play_board[i][j].nearmine = '0';
		}
	}
	
	return play_board;
	
}




void set_mines (int argc, board* bd, tile** play_board) {
	
	int i;
	int random_row;
	int random_column;
	bool valid_loc = false;
	
	if (argc != 5) {
		srand(time(NULL));
	}
	else{
		srand(bd->randSeed);
	}
	
	// start generating random numbers
	
	for (i = 0; i < bd->mines; i++) {
		valid_loc = false;
		while (valid_loc == false) {
			random_row = rand() % bd->rows;
			random_column = rand() % bd->columns;
			if (play_board[random_row][random_column].mined == false) {
				play_board[random_row][random_column].mined = true;
				printf("Placing mine at %d, %d\n", random_row, random_column);
				valid_loc = true;
			}
			else {
				continue;
			}
			
		}
	}
	
	return;
	
}


void create_game_board(board* bd) {
	
    int i, j;
    bd->game_board = (char**)malloc(bd->rows * sizeof(char*));
	
	
    for(i = 0; i < bd->rows; i++) {
        bd->game_board[i] = (char*)malloc(bd->columns * sizeof(char));
    }
	
    for(i = 0; i < bd->rows; i++) {
        for(j = 0; j < bd->columns; j++) {
			bd->game_board[i][j] = '#';
        }
		
    }
	bd->num_marked = 0;
	bd->correct_marked = 0;
    return;
}

void print_board(board* bd){
    int i, j;
	
    for(i = bd->rows - 1; i >= 0; i--) {
        printf("%d ", i);
        for(j = 0; j < bd->columns; j++){
            printf("%c ", bd->game_board[i][j]);
			
        }
        printf("\n");
    }
    printf("  ");
    for(j = 0; j < bd->columns; j++){
		printf("%d ", j);
    }
    printf("\n");
    return;
}





// grab a coordinate pair from user. tested to work
void get_play(board* bd, user_play* up) {
	
	
	int row_play;
	int col_play;
	
	printf("Enter row a row between 0-%d and a column between 0-%d: ", (bd->rows) - 1, (bd->columns) - 1);
	scanf("%d %d", &row_play, &col_play);
	
	up->row = row_play;
	up->column = col_play;
	//printf("%d", up->row);
	
}

// check if play is within bounds of board. tested to work
bool check_bounds(user_play* up, board* bd) {
	
	if ((up->row >= 0 && up->row < bd->rows) && (up->column >= 0 && up->column < bd->columns)) {
		//printf("true\n");
		return true;
	}
	else {
		//printf("false\n");
		return false;
	}
	
}

//recursive function

void reveal(board* bd, tile** play_board, int play_row, int play_column) {
	
	int i;
	int j;

	if(play_board[play_row][play_column].nearmine != '0') {
		if (play_board[play_row][play_column].questioned == true || play_board[play_row][play_column].marked == true) {
			return;
		}
		else {
		
			play_board[play_row][play_column].visible = true;
			bd->game_board[play_row][play_column] = play_board[play_row][play_column].nearmine;	
			return;
		}
	}
	else if (play_board[play_row][play_column].mined == true) {
		return;
	}
	else if (play_board[play_row][play_column].visible == true) {
		return;
		
	}
	else if (play_board[play_row][play_column].questioned == true || play_board[play_row][play_column].marked == true) {
		return;
	}
	

	
	else {	
		for (i = -1; i < 2; i++) {
			for (j = -1; j < 2; j++) {
				if ((0 <= play_row + i && play_row + i < bd->rows) && (0 <= play_column + j && play_column + j < bd->columns)) {
					if (i == 0 && j == 0) {
						continue;
					}
					else {
						play_board[play_row][play_column].visible = true;
						bd->game_board[play_row][play_column] = play_board[play_row][play_column].nearmine;
						reveal(bd, play_board,play_row + i, play_column + j);
					}
				
				}
			}
		}
	}
}	 
	

int check_win(board* bd, tile** play_board){

	int counter = 0;
	int i, j;
	

	for(i = 0; i < bd->rows; i++){
		for(j = 0; j < bd->columns; j++){
			if(play_board[i][j].visible == true){
				counter++;
			}
		}
	}
	if (counter == bd->rows * bd->columns - bd->mines && (bd->mines == bd->correct_marked)){
		return 1;
	}
	else {
		return 0;
	}
}
				
				
	

// execute something after provided with coordinate. 
enum GameStatus execute_action(user_play* up, board* bd, tile** play_board) {
	
	int action;
	
	
	if (play_board[up->row][up->column].visible == true) { 
		printf("This tile is already revealed.\n");
		return GAME_ON; //break out of function, call another play
	}
	else if (play_board[up->row][up->column].questioned == true) {
		printf("Enter Action\n");
		printf("0. UnQuestion\n1. Cancel\nAction: ");
		scanf("%d", &action);
		if (action == 0) {
			play_board[up->row][up->column].questioned = false;
			bd->game_board[up->row][up->column] = '#';
			printf("There are %d mines left\n", bd->mines - bd->num_marked);
			print_board(bd);
			return GAME_ON;
		}
		
		else {
			return GAME_ON;
		}
		
	}
	else if (play_board[up->row][up->column].marked == true) {
		printf("Enter Action\n");
		printf("0. UnMark\n1. Cancel\nAction: ");
		scanf("%d", &action);
		if (action == 0) {
			play_board[up->row][up->column].marked = false;
			bd->game_board[up->row][up->column] = '#';
			bd->num_marked--;
			printf("There are %d mines left\n", bd->mines - bd->num_marked);
			
			if (play_board[up->row][up->column].mined == true) {
				bd->correct_marked--;
	
			}
			else if (play_board[up->row][up->column].mined == false){
				if(check_win(bd, play_board) == 1){
					return GAME_WIN;
				}
			}
			
			print_board(bd);
			
			return GAME_ON;
		}
		
		else {
			return GAME_ON;
		}
		
	}
	else {
		printf("Enter Action\n0. Reveal\n1. Question\n2. Mark\n3. Cancel\nAction: ");
		scanf("%d", &action);
	
		if (action == 0) {
			
			if (play_board[up->row][up->column].mined == false) {
				reveal(bd,play_board,up->row, up->column);
				if(check_win(bd, play_board) == 1){
					return GAME_WIN;
				}
				printf("There are %d mines left\n", bd->mines - bd->num_marked);
				print_board(bd);
				return GAME_ON;
			}
			else if (play_board[up->row][up->column].mined == true) {
				return GAME_OVER; //game lost, don't call another turn
			}
		}
		else if (action == 1) {
			play_board[up->row][up->column].questioned = true;
			bd->game_board[up->row][up->column] = '?';
			printf("There are %d mines left\n", bd->mines - bd->num_marked);
			print_board(bd);
			return GAME_ON;
			
		}
		else if (action == 2) {
			play_board[up->row][up->column].marked = true;
			bd->game_board[up->row][up->column] = '!';
			bd->num_marked++;
			
			// check win
			if (play_board[up->row][up->column].mined == true) {
				bd->correct_marked++;
			}
			if(check_win(bd, play_board) == 1){
				return GAME_WIN;
			}	
			printf("There are %d mines left\n", bd->mines - bd->num_marked);
			print_board(bd);
			return GAME_ON;
		}
		
		else if (action == 3) {
			return GAME_ON;
		}
		
	}
	
	return 0;
	
}

void count_mines(tile** play_board, board* bd) {
	
	int i;
	int j;
	int m;
	int n;
	char counter;
	
	for (i = 0; i < bd->rows; i++) {
		for (j = 0; j < bd->columns; j++) {
			
			if (play_board[i][j].mined == false) {
				
				counter = '0';
				
				for (m = i - 1; m <= i + 1; m++) {
					
						for (n = j - 1; n <= j + 1; n++)
						{
							if (0 <= m && m < bd->rows && 0 <= n && n < bd->columns)
							{
								if (play_board[m][n].mined == true){
									counter++;
							}
						}
					}
					
					play_board[i][j].nearmine = counter;
				}
			}
			
		}
		
	}
}
					





void destroy_board(board* bd, user_play* up, tile** play_board) {
	
	int i;
	
	for (i = 0; i < bd->rows; i++) {
		free(bd->game_board[i]);
	}
	
	free(bd->game_board);
	free(bd);
	free(up);
	
	for (i = 0; i < bd->rows; i++) {
		free(play_board[i]);
	}
	
	free(play_board);
	
	return;
}

void play_game(board* bd, tile** play_board, user_play* up, int argc) {
	
	int i;
	int j;
	enum GameStatus s = GAME_ON; // the enum
	
	
	set_mines(argc, bd, play_board);
	count_mines(play_board, bd);	
	
	
	printf("There are %d mines left\n", bd->mines);
	print_board(bd);
	

	while (s == GAME_ON) {
		get_play(bd, up);
		while(check_bounds(up, bd) == false){
	  		get_play(bd, up);
		}
		s = execute_action(up, bd, play_board);

	}
	
	for(i = 0; i < bd->rows; i++){
		for(j = 0; j < bd->columns; j++){
			if(play_board[i][j].mined == true){
				bd->game_board[i][j] = '*';
			}
			else{
				bd->game_board[i][j] = play_board[i][j].nearmine;
			}
		}
	}
	//printf("\n");
		
    for(i = bd->rows - 1; i >= 0; i--) {
        printf("%d ", i);
	//printf("\n");
        for(j = 0; j < bd->columns; j++){
            printf("%c ", bd->game_board[i][j]);
			
        }
        printf("\n");
    }
    printf("  ");
    for(j = 0; j < bd->columns; j++){
		printf("%d ", j);
    }
    printf("\n");



	if (s == GAME_WIN){
		printf("You Won!!\n");
	}
	
	if (s == GAME_OVER){
		printf("You Lost :(\n");
	}
	return;
}
