/******************************************************
File implementation : main.c
Authors             : P. N. Kiorpelidis & G. Koryllos
Purpose             : miscellaneous functions definitions
Date                : 28-1-2015
*******************************************************/

#include <stdio.h>
#include <stdlib.h>

int** initGrid(unsigned int n_rows, unsigned n_cols, int** grid)
{
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
      fprintf(stderr, "Error when malloc'ing grid's memory!");                  //In case the malloc failed for some reason.
    }
  }
  return grid;
}
