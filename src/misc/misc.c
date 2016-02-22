/******************************************************
File implementation : main.c
Authors             : P. N. Kiorpelidis & G. Koryllos
Purpose             : miscellaneous functions definitions
Date                : 28-1-2015
*******************************************************/

#include <stdio.h>
#include <stdlib.h>

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

