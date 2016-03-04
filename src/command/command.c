/******************************************************
File implementation : command.h
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

  if(strcmp(command, "name"))
  {
    name();
  }
  else if(strcmp(command, "known_command"))
  {
    known_command(command);
  }
  else if(strcmp(command, "list_commands"))
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

void boardsize(unsigned int n_rows, unsigned int n_cols)
{

}

void clear_board(int** grid)
{

}

void walls(unsigned int n_walls)
{

}
