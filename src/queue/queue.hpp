/******************************************************
File implementation : queue.c
Authors             : Platwnas-Nikolaos Kiorpelidis
Purpose             : queue function declarations
Date                : 2-1-2015
*******************************************************/

#ifndef QUEUE_H
#define QUEUE_H

#include "../command/command.hpp"

Queue* stack_construct();

void stack_destruct(Queue* my_queue);

void stack_push(Queue_elem element, Queue* my_queue, ArraySize grid_size);

Queue_elem stack_pop(Queue* my_queue);

#endif /* #ifndef QUEUE_H */
