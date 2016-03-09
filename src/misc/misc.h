/******************************************************
File implementation : main.c
Authors             : P. N. Kiorpelidis & G. Koryllos
Purpose             : miscellaneous functions declarations
Date                : 2-1-2015
*******************************************************/

#ifndef _MISC_H_
#define _MISC_H_

#include "../command/command.h"

/**************************
ARRAY GENERAL FUNCTIONS
**************************/

//initializes an integer array of n_rows rows and n_cols columns and returns the malloc'ed array to function name.
int** initGridInt(unsigned int n_rows, unsigned n_cols);

//initializes an character array of n_rows rows and n_cols columns and returns the malloc'ed array to function name.
char** initGridChar(unsigned n_rows, unsigned n_cols);

//for a given 2D grid array with n_rows rows and n_cols columns, it's content is zero'ed.
void zerofyGrid(unsigned int n_rows, unsigned int n_cols, int** grid);

//for a given 2D array with n_rows rows and n_cols columns, it's content is printed as integers.
void print2DArrayAsInt(unsigned int n_rows, unsigned int n_cols, int** array);

//for a given 2D array with n_rows rows and n_cols columns, it's content is printed as characters.
void print2DArrayAsChar(unsigned int n_rows, unsigned int n_cols, int** array);

//copies sourceStr content to destinationStr until the character endChar is found, but not endChar. If character endChar does not exist inside sourceStr then everything is copied over.
void copyStringUntil(char* sourceStr, char* destinationStr, unsigned int destinationStrSize, char endChar);

//returns the amount of characters until character endChar. If endChar character does not exist in the string then -1 is returned.
unsigned int getStringlenUntil(char* string, char endChar);

//replaces string characters with odd indexed replaceChars characters when string characters are the same as even indexed replaceChars characters.
/*
string: the string that will be edited.
replaceChars: a char array with pairs of characters. the characters that are at
even indexes and exist inside string variable will be replaced with the characters at the next odd index.

Example:
char bla[21] = "Platwnas Kiorpelidis";
char bla2[4] = {'a', 'b', ' ', 'X'};
replaceStringChars(bla, bla2, 4);
printf("%s\n", bla);

This will print PlbtwnbsXKiorpelidis*/
void replaceStringChars(char* string, char* replaceChars, unsigned int n_rows);

//removes garbageChar from string.
void removeChar(char* string, char garbageChar);

//removes extra spaces.
void removeExtraSpaces(char* string);

//builds initial grid
void buildGrid(int** grid, unsigned int gridSize);

//frees malloc'ed grid
void freeGrid (int ** grid, unsigned int n_rows);

//returns the user's command from stdin to getCommand's name.
char* getLine();

//decodes the user's command to the command. Returns the command.
char* commandDecode(char* input_command, char* output_command);

//decodes the user's command to it's arguments. Returns the arguments as a 2D array.
char** argumentsDecode(char* input_command, unsigned int* arguments_count);

#endif //#ifndef _MISC_H_
