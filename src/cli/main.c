/******************************************************
File implementation : main.c
Authors             : P. N. Kiorpelidis & G. Koryllos
Purpose             : User Interface
Date                : 28-1-2015
*******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../misc/misc.h"
#include "../command/command.h"

int main(int argc, char* argv[])
{
  unsigned int quit_game = 0;
  while(quit_game == 0)
  {
    inputCommand(&quit_game);
  }
}
