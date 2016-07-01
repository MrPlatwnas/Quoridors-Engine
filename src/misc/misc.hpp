/******************************************************
File implementation : main.c
Authors             : Platwnas-Nikolaos Kiorpelidis
Purpose             : miscellaneous functions declarations
Date                : 2-1-2015
*******************************************************/

#ifndef MISC_H
#define MISC_H

#include "../command/command.hpp"

/**************************
ARRAY GENERAL FUNCTIONS
**************************/

//initializes an integer array of n_rows rows and n_cols columns and returns the malloc'ed array to function name.
int** initGridInt(unsigned int n_rows, unsigned n_cols);

//initializes an character array of n_rows rows and n_cols columns and returns the malloc'ed array to function name.
char** initGridChar(unsigned n_rows, unsigned n_cols);

//frees malloc'ed grid
void freeGrid(int** grid, unsigned n_rows);

//for a given 2D grid array with n_rows rows and n_cols columns, it's content is zero'ed.
void zerofyGrid(unsigned int n_rows, unsigned int n_cols, int** grid);

//for a given 2D array with n_rows rows and n_cols columns, it's content is printed as integers.
void print2DArrayAsInt(unsigned int n_rows, unsigned int n_cols, int** array);

//for a given 2D array with n_rows rows and n_cols columns, it's content is printed as characters.
void print2DArrayAsChar(unsigned n_rows, unsigned n_cols, int** array);

/**************************
ARRAY GENERAL FUNCTIONS
**************************/


/**************************
STRING GENERAL FUNCTIONS
**************************/

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
void replace_string_chars(char* string, char char_unwanted, char char_replacer);

//removes garbageChar from string.
void remove_char(char* string, char garbageChar);

//removes extra spaces.
void remove_extra_spaces(char* string);

//returns the user's command from stdin to getCommand's name.
char* get_line();

//uncapitalizes the string's characters.
void uncapitalize(char* string);

/**************************
STRING GENERAL FUNCTIONS
**************************/


/**************************
QUORIDOR MISC FUNCTIONS
**************************/

//builds int grid for quoridor.
int** build_grid(ArraySize grid_size);

//decodes the user's command to the command. Returns the command.
char* command_decode(char* input_command);

//decodes the user's command to it's arguments. Returns the arguments as a 2D array.
char** arguments_decode(char* input_command, unsigned* arguments_count);

//removes comments from a given string. (a comment is a text that starts with a # character ex. playmove w a5 #i want to play a move)
void remove_comments(char* string);

void available_path(int** grid, ArraySize grid_size, unsigned v_start, unsigned h_start, bool* available_path_exists, char player);

/**************************
QUORIDOR MISC FUNCTIONS
**************************/

#endif //#ifndef MISC_H
