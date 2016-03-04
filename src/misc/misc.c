/******************************************************
File implementation : main.c
Authors             : P. N. Kiorpelidis & G. Koryllos
Purpose             : miscellaneous functions definitions
Date                : 28-1-2015
*******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int** initGridInt(unsigned int n_rows, unsigned n_cols)
{
  int** grid;
  grid = malloc(n_rows * sizeof(int*));
  if(grid == NULL)
  {
    fprintf(stderr, "Error when malloc'ing grid's memory!");
  }

  unsigned int counter = 0;
  for(counter = 0; counter < n_rows; counter++)
  {
    grid[counter] = malloc(n_cols * sizeof(int));
    if(grid[counter] == NULL)
    {
      fprintf(stderr, "Error when malloc'ing grid's memory!");  //In case the malloc failed for some reason.
    }
  }
  return grid;
}

char** initGridChar(unsigned int n_rows, unsigned int n_cols)
{
  char** grid;
  grid = malloc(n_rows * sizeof(char*));
  if(grid == NULL)
  {
    fprintf(stderr, "Error when malloc'ing grid's memory!");
  }

  unsigned int counter = 0;
  for(counter = 0; counter < n_rows; counter++)
  {
    grid[counter] = malloc(n_cols * sizeof(char));
    if(grid[counter] == NULL)
    {
      fprintf(stderr, "Error when malloc'ing grid's memory!");  //In case the malloc failed for some reason.
    }
  }
  return grid;
}

void zerofyGrid(unsigned int n_rows, unsigned int n_cols, int** grid)
{
  unsigned int counterRows;
  unsigned int counterCols;

  for(counterRows = 0; counterRows < n_rows; counterRows++)
  {
    for(counterCols = 0; counterCols < n_cols; counterCols++)
    {
      grid[counterRows][counterCols] = 0;
    }
  }
}

void print2DArrayAsInt(unsigned int n_rows, unsigned int n_cols, int** array)
{
  unsigned int rowsCounter = 0;
  unsigned int colsCounter = 0;
  for(rowsCounter = 0; rowsCounter < n_rows; rowsCounter++)
  {
    for(colsCounter = 0; colsCounter < n_cols; colsCounter++)
    {
      printf("%d ", array[rowsCounter][colsCounter]);
    }
    printf("\n");
  }
}

void print2DArrayAsChar(unsigned int n_rows, unsigned int n_cols, int** array)
{
  unsigned int rowsCounter = 0;
  unsigned int colsCounter = 0;
  for(rowsCounter = 0; rowsCounter < n_rows; rowsCounter++)
  {
    for(colsCounter = 0; colsCounter < n_cols; colsCounter++)
    {
      printf("%c ", array[rowsCounter][colsCounter]);
    }
    printf("\n");
  }
}

void copyStringUntil(char* sourceStr, char* destinationStr, unsigned int destinationStrSize, int endChar)
{
  unsigned int counter = 0;
  for(counter = 0; counter < destinationStrSize; counter++)
  {
    if(sourceStr[counter] == endChar)
    {
      break;
    }
    destinationStr[counter] = sourceStr[counter];
  }
  destinationStr[counter] = '\0'; //To remove garbage from destinationStr in case destinationStr is bigger than sourceStr.
}

int getStringlenUntil(char* string, char endChar)
{
  unsigned int stringLength = strlen(string); //so we wont go out of bountaries.
  unsigned int counter = 0;
  unsigned int n_chars = 0; //number of characters until endChar.
  for(counter = 0; counter < stringLength; counter++)
  {
    n_chars++;
    if(string[counter] == endChar)
    {
      return n_chars;
    }
  }
  return -1;  //returns -1 if endChar does not exist inside string.
}

void replaceStringChars(char* string, char* replaceChars, unsigned int n_rows)
{
  unsigned int stringLength = strlen(string);
  unsigned int counterString = 0;
  unsigned int counterReplaceChar = 0;
  for(counterString = 0; counterString < stringLength; counterString++)
  {
    for(counterReplaceChar = 0; counterReplaceChar < n_rows - 1; counterReplaceChar += 2) //n_rows - 1 because counterReplaceChar is increased by 2 each time(segmentation fault).
    {
      if(string[counterString] == replaceChars[counterReplaceChar])
      {
        string[counterString] = replaceChars[counterReplaceChar + 1];
      }
    }
  }
  string[stringLength] = '\0';
}

void removeChar(char* string, char garbageChar)
{
  char* src;
  char* dst;
  for(src = dst = string; *src != '\0'; src++)
  {
    *dst = *src;
    if(*dst != garbageChar)
    {
      dst++;  //so the garbageChar char gets overwriten.
    }
  }
  *dst = '\0';
}

void removeExtraSpaces(char* string)
{
  char* src = string;
  char* dst = string;
  while (isspace(*src)) src++;
  for(; *src != '\0'; src++)
  {
    *dst = *src;
    if(*src != '\0')
    {
      if(*dst != *(src + 1) || *dst != ' ')
      {
        dst++;
      }
    }
  }
  if(dst[-1] == ' ')
  {
    dst[-1] = '\0';
  }
  else
  {
    *dst = '\0';
  }
}
