/******************************************************
File implementation : command.h
Authors             : P. N. Kiorpelidis & G. Koryllos
Purpose             : command functions declarations
Date                : 28-1-2015
*******************************************************/

#ifndef _COMMAND_H_
#define _COMMAND_H_

//reads the stdin of the program, analyzes it and calls the apropriate function.
void inputCommand();

//prints at stdout the engine's name.
void name();

//prints true if command exits otherwise prints false.
void known_command(char* command);

//mallocs a 2D array with n_rows rows and n_cols columns. Changes the board size.
int** boardSize(int** grid, unsigned int newSize);

//Clears the board. The two pawns return to their starting position.
void clear_board(int** grid);
//Sets the number of walls each player has at the start of the game.
void walls(unsigned int n_walls);

//Prints at stdin all the commands. One per row.
void list_commands();

//Prints the whole board as is
void show_board(int** grid, int gridSize);

#endif  //ifndef _COMMAND_H_
