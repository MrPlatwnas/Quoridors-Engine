/******************************************************
File implementation : main.c
Authors             : P. N. Kiorpelidis & G. Koryllos
Purpose             : miscellaneous functions declarations
Date                : 2-1-2015
*******************************************************/

#ifndef _MISC_H_
#define _MISC_H_

int** initGridInt(unsigned int n_rows, unsigned n_cols);
void zerofyGrid(unsigned int n_rows, unsigned int n_cols, int** grid);
void print2DArrayAsInt(unsigned int n_rows, unsigned int n_cols, int** array);
void print2DArrayAsChar(unsigned int n_rows, unsigned int n_cols, int** array);
unsigned int getStringlenUntil(char* string, char endChar);

#endif
