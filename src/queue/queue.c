/******************************************************
File implementation : queue.c
Authors             : Platwnas-Nikolaos Kiorpelidis
Purpose             : queue functions definitions
Date                : 2-1-2015
*******************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

Queue_elem stack_pop(Queue* my_queue)
{
  Queue_elem element;

  if(my_queue->top == 0)
  {
    //@command: setting the Queue_elem to invalid form.
    element.isset = false;
    return element;
  }

  //@command: go to the last pushed Queue_elem.
  my_queue->top--;

  //@command: copying the black position to the return Queue_elem.
  element.pawns_location.black_location.v_coordinate = my_queue->elements[my_queue->top].pawns_location.black_location.v_coordinate;
  element.pawns_location.black_location.h_coordinate = my_queue->elements[my_queue->top].pawns_location.black_location.h_coordinate;

  //@command: copying the white position to the return Queue_elem.
  element.pawns_location.white_location.v_coordinate = my_queue->elements[my_queue->top].pawns_location.white_location.v_coordinate;
  element.pawns_location.white_location.h_coordinate = my_queue->elements[my_queue->top].pawns_location.white_location.h_coordinate;

  //@command: copying the walls_grid to the return Queue_elem.
  element.grid = my_queue->elements[my_queue->top].grid;

  //@command: setting the Queue_elem to valid form.
  element.isset = true;

  return element;
}

void stack_push(Queue_elem element, Queue* my_queue, ArraySize grid_size)
{
  if(my_queue->top == my_queue->maxsize)
  {
    printf("? Error: stack overflow\n\n");
    return;
  }

  //@command: copying the black position to the queue.
  my_queue->elements[my_queue->top].pawns_location.black_location.v_coordinate = element.pawns_location.black_location.v_coordinate;
  my_queue->elements[my_queue->top].pawns_location.black_location.h_coordinate = element.pawns_location.black_location.h_coordinate;

  //@command: copying the white position to the queue.
  my_queue->elements[my_queue->top].pawns_location.white_location.v_coordinate = element.pawns_location.white_location.v_coordinate;
  my_queue->elements[my_queue->top].pawns_location.white_location.h_coordinate = element.pawns_location.white_location.h_coordinate;

  //@command: copying the grid configuration to the queue.
  my_queue->elements[my_queue->top].grid = malloc(grid_size.v_size * sizeof(int*));
  for(size_t counter = 0; counter < grid_size.v_size; counter++)
  {
    my_queue->elements[my_queue->top].grid[counter] = malloc(grid_size.h_size * sizeof(int));
    memcpy(my_queue->elements[my_queue->top].grid[counter], element.grid[counter], grid_size.h_size * sizeof(int));
  }
  // my_queue->elements[my_queue->top].grid = element.grid;

  my_queue->top++;

  return;
}

Queue* stack_construct()
{
  //@purpose: create and initialize the queue.
  Queue* my_queue = malloc(sizeof(Queue));
  my_queue->elements = malloc(500 * sizeof(Queue_elem));
  my_queue->top = 0;
  my_queue->maxsize = 500;

  return my_queue;
}

void stack_destruct(Queue* my_queue)
{
  free(my_queue->elements);
  free(my_queue);
}
