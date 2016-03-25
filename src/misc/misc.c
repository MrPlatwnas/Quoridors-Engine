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

#include "../command/command.h"

//TODO: organize the functions to match the misc.h functions' order.
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

void print2DArrayAsChar(unsigned n_rows, unsigned n_cols, int** array)
{
  unsigned int rowsCounter = 0;
  unsigned int colsCounter = 0;
  for(rowsCounter = 0; rowsCounter < n_rows; rowsCounter++)
  {
    for(colsCounter = 0; colsCounter < n_cols; colsCounter++)
    {
      printf("%c", array[rowsCounter][colsCounter]);
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

void replace_string_chars(char* string, char char_unwanted, char char_replacer)
{
  unsigned string_length = strlen(string);
  unsigned string_counter = 0;

  for(string_counter = 0; string_counter < string_length; string_counter++)
  {
    if(string[string_counter] == char_unwanted)
    {
      string[string_counter] = char_replacer;
    }
  }
  string[string_length] = '\0';
}

void remove_char(char* string, char garbageChar)
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

void remove_extra_spaces(char* string)
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


void freeGrid(int** grid, unsigned n_rows)
{
  unsigned counter_rows = 0;
  for(counter_rows = 0; counter_rows < n_rows; counter_rows++)
  {
    free(grid[counter_rows]);
  }
  free(grid);
}

char* get_line()
{
  char c;
  unsigned n_chars = 1; //this way we will have one more character for the '\0' character.
  char* input_command = NULL;

  while((c = getchar()) == '\n'); //discard all the LF characters at the start of the command.
  while(c != '\n' && c != EOF)
  {
    n_chars++;
    input_command = realloc(input_command, n_chars * sizeof(char));
    input_command[n_chars - 2] = c;
    c = getchar();
  }
  input_command[n_chars - 1] = '\0';
  return input_command;
}

char* command_decode(char* input_command)
{
  char* output_command = NULL;
  unsigned no_chars = 1;  //this way we will have one more character for the '\0' character.
  unsigned index = 0;
  while(input_command[index] != ' ' && input_command[index] != '\0')
  {
    no_chars++;
    output_command = realloc(output_command, no_chars * sizeof(char));
    output_command[index] = input_command[index];
    index++;
  }
  output_command[index] = '\0';
  return output_command;
}

char** argumentsDecode(char* input_command, unsigned int* arguments_count)
{
  char** output_arguments = NULL;
  unsigned int index = 0;
  while(input_command[index] != ' ' && input_command[index] != '\n' && input_command[index] != '\0' && input_command[index] != EOF) index++;
  if(input_command[index] != '\n' && input_command[index] != EOF && input_command != '\0')
  {
    unsigned int current_chars = 0;
    unsigned int max_chars = 0;
    unsigned int no_arguments = 0;
    while(input_command[index] != '\n' && input_command[index] != '\0' && input_command[index] != EOF)
    {
      if(input_command[index] != ' ')
      {
        current_chars++;
      }
      else
      {
        if(current_chars > max_chars) max_chars = current_chars;
        current_chars = 0;
        no_arguments++;
      }
      index++;
    }
    if(current_chars > max_chars) max_chars = current_chars;

    output_arguments = initGridChar(no_arguments, max_chars + 1);

    index = 0;
    while(input_command[index] != ' ' && input_command[index] != '\n' && input_command[index] != '\0' && input_command[index] != EOF) index++;

    index++;
    unsigned int n_rows = 0;
    unsigned int n_cols = 0;
    while(input_command[index] != '\n' && input_command[index] != '\0' && input_command[index] != EOF)
    {
      if(input_command[index] != ' ')
      {
        output_arguments[n_rows][n_cols] = input_command[index];
        n_cols++;
        index++;
      }
      else
      {
        output_arguments[n_rows][n_cols] = '\0';
        n_rows++;
        n_cols = 0;
        index++;
      }
    }
    *arguments_count = no_arguments;
    return output_arguments;
  }
}

int** build_grid(ArraySize grid_size)
{
  int** grid = initGridInt(grid_size.v_size, grid_size.h_size + 1);

  unsigned n_rows = grid_size.v_size;
  unsigned n_cols = grid_size.h_size + 1; //+1 for the '\0' character.
  unsigned counter_rows = 0;
  unsigned counter_cols = 0;
  for(counter_rows = 0; counter_rows < n_rows; counter_rows++)
  {
    for(counter_cols = 0; counter_cols < n_cols; counter_cols++)
    {
      if(counter_rows % 2 == 0)
      {
        if(counter_cols % 4 == 0)
        {
          grid[counter_rows][counter_cols] = '+';
        }
        else if(counter_cols % 4 != 0)
        {
          grid[counter_rows][counter_cols] = '-';
        }
      }
      else if(counter_rows % 2 == 1)
      {
        if(counter_cols % 4 == 0)
        {
          grid[counter_rows][counter_cols] = '|';
        }
        else if(counter_cols % 4 != 0)
        {
          grid[counter_rows][counter_cols] = ' ';
        }
      }
    }
    grid[counter_rows][counter_cols - 1] = '\0';
  }
  return grid;
}

void remove_comments(char* string)
{
  unsigned index = 0;
  while(string[index] != '\0' && string[index] != '#')
  {
    index++;
  }
  if(string[index] == '#')
  {
    string[index] = '\0';
  }
}

void uncapitalize(char* string)
{
  unsigned index = 0;
  while(string[index] != '\0')
  {
    if(string[index] >= 65 && string[index] <= 90)
    {
      string[index] += 32;
    }
    index++;
  }
}
