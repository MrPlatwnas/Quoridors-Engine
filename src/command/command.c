/******************************************************
File implementation : command.c
Authors             : P. N. Kiorpelidis & G. Koryllos
Purpose             : command functions definitions
Date                : 28-1-2015
*******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../misc/misc.h"
#include "../command/command.h"

void inputCommand()
{
  char* inputCommand = getLine();

  char replaceChars[2] = {9, ' '};
  replaceStringChars(inputCommand, replaceChars, 2);
  removeChar(inputCommand, 13);
  removeExtraSpaces(inputCommand);

  char* command = NULL;
  command = commandDecode(inputCommand, command);

  unsigned int no_arguments;
  char** arguments = NULL;
  arguments = argumentsDecode(inputCommand, &no_arguments);

  if(strcmp(command, "name") == 0)
  {
    name();
  }
  else if(strcmp(command, "known_command") == 0)
  {
    if(no_arguments == 1) known_command(arguments[0]);
    else printf("= false\n\n");
  }
  else if(strcmp(command, "list_commands") == 0)
  {
    list_commands();
  }
  else
  {
    // Add all the commands.
  }
}

void name()
{
  char* engineName = "Deep Orange";
  printf("%s\n", engineName);
}

void known_command(char* command)
{
  static unsigned int n_elems = 13;
  static char* allCommands[] = {"name", "known_command", "list_commands", "quit", "boardsize", "clear_board", "walls", "playmove", "playwall", "genmove", "undo", "winner", "showboard"};

  unsigned int n_rows = 0;
  for(n_rows = 0; n_rows < n_elems; n_rows++)
  {
    if(strcmp(command, allCommands[n_rows]) == 0)
    {
      printf("= true\n\n");
      return;
    }
  }
  printf("= false\n\n");
}

void list_commands()
{
  static unsigned int n_elems = 13;
  static char* allCommands[] = {"name", "known_command", "list_commands", "quit", "boardsize", "clear_board", "walls", "playmove", "playwall", "genmove", "undo", "winner", "showboard"};
  unsigned int counter;
  for(counter = 0; counter < n_elems; counter++)
  {
    printf("%s\n", allCommands[counter]);
  }
}

int** boardSize(int** grid, unsigned int newSize)
{
  static int gridMade=0, gridSize;

  if (newSize<=24)
  {
    gridSize = (newSize*2)-1;
    if(gridMade == 0)
    {
      gridMade = 1;
    }
    else
    {
      freeGrid(grid,gridSize);
    }
    grid = initGridInt(gridSize, gridSize);
    printf("made %d\n",gridSize);
  }
  else
  {
    printf("unacceptable size");
  }
  return grid;
}

void show_board(int** grid, int gridSize)
{
  int i,j,realSize;
  char c='A';
  realSize=(gridSize-1)/2;
  for (i=0 ; i<gridSize ; i++)
  {
    if (i==0)
    {
      printf("     ");
      for (j=0 ; j<gridSize ; j++)
      {
        if ((j/2)==0)
        printf("%c " , c+j);
        else
        printf("   ");
      }
    }
    printf(" %d |", i);
    if (i>0)
    {
      for (j=0 ; j<gridSize ; j++)
      {
        printf(" %c ", grid[i-1][j]);
      }
      printf("| %d", i);
    }
    printf("\n");
  }
  for (j=0 ; j<=gridSize ; j++)
    printf(" %c" , c+j);
}


void clear_board(int** grid)
{

}

void walls(unsigned int n_walls)
{
  //FIXME: Add function to this function.
}
