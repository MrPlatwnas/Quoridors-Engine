/******************************************************
File implementation : command.h
Authors             : P. N. Kiorpelidis & G. Koryllos
Purpose             : command functions declarations
Date                : 28-1-2015
*******************************************************/

#ifndef _COMMAND_H_
#define _COMMAND_H_

void name();
void boardsize(unsigned int n_rows, unsigned int n_cols);
void clear_board(int** grid);
void walls(unsigned int n_walls);

#endif
