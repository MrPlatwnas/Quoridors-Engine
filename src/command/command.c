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

void inputCommand(unsigned int* quit_game)
{
  char* inputCommand = get_line();

  char replaceChars[2] = {9, ' '};
  replaceStringChars(inputCommand, replaceChars, 2);
  removeChar(inputCommand, 13);
  removeExtraSpaces(inputCommand);

  char* command = NULL;
  command = commandDecode(inputCommand, command);

  unsigned int n_arguments;
  char** arguments = NULL;
  arguments = argumentsDecode(inputCommand, &n_arguments);

  int** grid = NULL;
  ArraySize grid_size;
  unsigned int n_walls = 0;

  if(strcmp(command, "name") == 0)
  {
    name();
  }
  else if(strcmp(command, "known_command") == 0)
  {
    if(n_arguments == 1)
    {
      known_command(arguments[0]);
    }
    else
    {
      printf("= false\n\n");
    }
  }
  else if(strcmp(command, "list_commands") == 0)
  {
    list_commands();
  }
  else if(strcmp(command, "quit") == 0)
  {
    quit(quit_game);
  }
  else if(strcmp(command, "boardsize") == 0)
  {
    if(n_arguments == 1)
    {
      grid_size.size = atoi(arguments[0]);
      grid = boardsize(grid, grid_size);
    }
    else
    {
      printf("? Error: you need to give one(1) argument (ex. boardsize 9)\n\n");
    }
  }
  else if(strcmp(command, "clear_board") == 0)
  {
    //clear_board(grid);
  }
  else if(strcmp(command, "walls") == 0)
  {
    if(n_arguments == 1)
    {
      walls(&n_walls, atoi(arguments[0]));
    }
    else
    {
      printf("? Error: you need to give one(1) argument (ex. walls 10)\n\n");
    }
  }
  else if(strcmp(command, "playmove") == 0)
  {
    if(grid != NULL && n_arguments == 2)
    {
      //playmove(grid, arguments[0], arguments[1]);
    }
    else if(grid == NULL)
    {
      printf("? Error: you need to create a board first\n\n");
    }
    else if(n_arguments != 2)
    {
      printf("? Error: you need to give two(2) arguments (ex. playmove w a1)\n\n");
    }
  }
  else if(strcmp(command, "playwall") == 0)
  {
    if(n_arguments == 3)
    {
      //playwall(grid, &n_walls, arguments[0], arguments[1], arguments[2]);
    }
    else
    {
      printf("? Error: you need to give 3 arguments (ex. playwall w a5 v)\n\n");
    }
  }
  else if(strcmp(command, "genmove") == 0)
  {
    if(n_arguments == 1)
    {
      //genmove(grid, grid_size, arguments[0]);
    }
    else
    {
      printf("? Error: you need to give one(1) argument (ex. genmove w)\n\n");
    }
  }
  else if(strcmp(command, "undo") == 0)
  {
    if(n_arguments == 1)
    {
      //undo(grid, arguments[0]);
    }
    else
    {
      printf("? Error: you need to give one(1) argument (ex. undo 4)\n\n");
    }
  }
  else if(strcmp(command, "winner") == 0)
  {
    //winner(grid, grid_size);
  }
  else if(strcmp(command, "showboard") == 0)
  {
    //showboard(grid, gridSize_v, gridSize_h);
  }
  else
  {
    printf("? Error: unknown command (run: list_commands)\n\n");
  }
}

/*********************************************************
-------------------------COMMANDS-------------------------
*********************************************************/

void name()
{
  char* engineName = "Deep Orange";
  printf("= %s\n\n", engineName);
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

  printf("=\n");
  unsigned int counter;
  for(counter = 0; counter < n_elems; counter++)
  {
    printf("%s\n", allCommands[counter]);
  }
  printf("\n");
}

void quit(unsigned int* quit_game)
{
  *quit_game = 1;
  printf("= quitting game\n");
}

int** boardsize(int** grid, ArraySize grid_size)
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

void clear_board(int** grid)
{

}

void walls(unsigned int* n_walls)
{
  //FIXME: Add functionality to this function.
}

void playmove(int** grid)
{
  //FIXME: Add functionality to this function.
}

void playwall(int ** grid)
{
  //FIXME: Add functionality to this function.
}

void genmove()
{
  //FIXME: Add functionality to this function.
}

void undo(int** grid)
{
  //FIXME: Add functionality to this function.
}

void winner()
{
  //FIXME: Add functionality to this function.
}

void showboard(int** grid, int gridSize)
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
