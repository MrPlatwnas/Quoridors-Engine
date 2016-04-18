/******************************************************
File implementation : command.h
Authors             : Platwnas-Nikolaos Kiorpelidis
Purpose             : command functions declarations
Date                : 28-1-2015
*******************************************************/

#ifndef COMMAND_H
#define COMMAND_H

#include <stdbool.h>

typedef struct gridsize {
  unsigned v_size;
  unsigned h_size;
  unsigned size;
}ArraySize;

typedef struct vertex {
  unsigned v_coordinate;
  unsigned h_coordinate;
}Vertex;

typedef struct walls {
  unsigned white_walls;
  unsigned black_walls;
}Walls;

typedef struct move_info {
  char player;
  unsigned n_row;
  unsigned n_col;
}Move_info;

typedef struct wall_info {
  char player;
  unsigned n_row;
  unsigned n_col;
  char orientation;
}Wall_info;

typedef struct players_location {
  Vertex white_location;
  Vertex black_location;
}Players_location;

typedef struct queue_elem {
  Players_location pawns_location;
  int** grid;
  bool isset;
}Queue_elem;

typedef struct queue {
  Queue_elem* elements;
  unsigned top;
  unsigned maxsize;
}Queue;

//reads the stdin of the program, analyzes it and calls the apropriate function.
void user_input_decode();

//prints at stdout the engine's name.
void name();

//prints true if command exits otherwise prints false.
void known_command(char** arguments, unsigned n_arguments);

//Prints at stdin all the commands. One per row.
void list_commands();

//the session is terminated and the connection is closed.
void quit(bool * quit_game);

//mallocs a 2D array with n_rows rows and n_cols columns. Changes the board size.
int** boardsize(ArraySize grid_size, Players_location* pawns_location);

//Clears the board. The two pawns return to their starting position.
void clear_board(int** grid, ArraySize grid_size, Players_location* pawns_location);

//Sets the number of walls each player has at the start of the game.
void walls(Walls* available_walls, unsigned int input_n_walls);

//the player of the requested color is played at the requested vertex.
void playmove(int** grid, ArraySize grid_size, Players_location* pawns_location,Move_info move_coordinates);

//a wall place at the requested vertex and orientation. Decrease the number of walls.
void playwall(int** grid, ArraySize grid_size, Walls* available_walls, Wall_info requested_wall_info);

//the engine makes a move or wall placement at the requested color.
void genmove(); //FIXME: Add support for the arguments.

//the game goes 'times' moves back.
void undo(int** grid, unsigned n_undo, Queue* my_queue, Players_location* pawns_location);

//outputs true and winner's color if the game ended, otherwise false.
void winner(ArraySize grid_size, Players_location* pawns_location);

//Prints the whole board as is.
void showboard(int** grid, Walls available_walls, ArraySize grid_size);

#endif  //ifndef COMMAND_H
