/******************************************************
File implementation : command.h
Authors             : P. N. Kiorpelidis & G. Koryllos
Purpose             : command functions declarations
Date                : 28-1-2015
*******************************************************/

#ifndef _COMMAND_H_
#define _COMMAND_H_

typedef struct gridsize {
  unsigned v_size;
  unsigned h_size;
  unsigned size;
}ArraySize;

typedef struct vertex {
  unsigned x_coordinate;
  unsigned y_coordinate;
}Vertex;

typedef struct walls {
  unsigned white_walls;
  unsigned black_walls;
}Walls;

typedef struct move_info {
  char player;
  unsigned n_row;
  unsigned n_col;
}move_info;

//reads the stdin of the program, analyzes it and calls the apropriate function.
void inputCommand(unsigned int* quit_game);

//prints at stdout the engine's name.
void name();

//prints true if command exits otherwise prints false.
void known_command(char* command);

//Prints at stdin all the commands. One per row.
void list_commands();

//the session is terminated and the connection is closed.
void quit(unsigned int* quit_game);

//mallocs a 2D array with n_rows rows and n_cols columns. Changes the board size.
int** boardsize(int** grid, ArraySize grid_size);

//Clears the board. The two pawns return to their starting position.
void clear_board(int** grid, ArraySize grid_size);

//Sets the number of walls each player has at the start of the game.
void walls(unsigned int* n_walls, unsigned int input_n_walls);

//the player of the requested color is played at the requested vertex.
void playmove(int** grid, ArraySize grid_size, char player, Vertex move_coordinates);

//a wall place at the requested vertex and orientation. Decrease the number of walls.
void playwall(int** grid, ArraySize grid_size, unsigned* n_walls, char player, Vertex wall_coordinates, unsigned orientation);

//the engine makes a move or wall placement at the requested color.
void genmove(); //FIXME: Add support for the arguments.

//the game goes 'times' moves back.
void undo(int** grid);  //FIXME: Add support for the arguments.

//outputs true and winner's color if the game ended, otherwise false.
void winner(int** grid, ArraySize grid_size);

//Prints the whole board as is.
void showboard(int** grid, Walls available_walls, ArraySize grid_size);

#endif  //ifndef _COMMAND_H_
