/******************************************************
File implementation : command.h
Authors             : P. N. Kiorpelidis & G. Koryllos
Purpose             : command functions definitions
Date                : 28-1-2015
*******************************************************/

#include <stdio.h>
#include <stdlib.h>

void name()
{
  char* engineName = "Deep Orange";
  printf("%s\n", engineName);
}

void knownCommand()
{

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

void boardsize(unsigned int newSize)
{
  if (newSize<=24)
  {
    if(gridMade == 0)
    {
      initGridInt(newSize, newSize);
    }
    else
    {
      reset(grid);
      gridMade = 1;
    }
  }
  else 
  {
    printf("unacceptable size");
  }
  
}

void clear_board(int** grid)
{

}

void walls(unsigned int n_walls)
{

}
