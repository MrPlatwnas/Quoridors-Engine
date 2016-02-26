/******************************************************
File implementation : main.c
Authors             : P. N. Kiorpelidis & G. Koryllos
Purpose             : miscellaneous functions declarations
Date                : 2-1-2015
*******************************************************/

#ifndef _MISC_H_
#define _MISC_H_

//initializes an integer array of n_rows rows and n_cols columns and returns the malloc'ed array to function name.
int** initGridInt(unsigned int n_rows, unsigned n_cols);

//for a given 2D grid array with n_rows rows and n_cols columns, it's content is zero'ed.
void zerofyGrid(unsigned int n_rows, unsigned int n_cols, int** grid);

//for a given 2D array with n_rows rows and n_cols columns, it's content is printed as integers.
void print2DArrayAsInt(unsigned int n_rows, unsigned int n_cols, int** array);

//for a given 2D array with n_rows rows and n_cols columns, it's content is printed as characters.
void print2DArrayAsChar(unsigned int n_rows, unsigned int n_cols, int** array);

//copies sourceStr content to destinationStr until the character endChar is found. If character endChar does not exist inside sourceStr then everything is copied over.
void copyStringUntil(char* sourceStr, char* destinationStr, unsigned int destinationStrSize, char endChar);

//returns the amount of characters until character endChar. If endChar character does not exist in the string then -1 is returned.
unsigned int getStringlenUntil(char* string, char endChar);

#endif //#ifndef _MISC_H_
