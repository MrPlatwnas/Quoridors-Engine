/*
File implementation : main.cpp
Authors             : Platon-Nikolaos Kiorpelidis
Purpose             : User Interface
Date created        : 28-01-2016
Language:           : C++
*/

#include <iostream>

#include "../command/command.hpp"

int main(int argc, const char* argv[])
{
  Quoridors_game my_game;
  my_game.start_game();
}
