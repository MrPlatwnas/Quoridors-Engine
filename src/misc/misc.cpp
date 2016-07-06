/*
File implementation : misc.cpp
Authors             : Platon-Nikolaos Kiorpelidis
Purpose             : mMscellaneous functions definitions
Date created        : 28-01-2015
*/

#include <iostream>

#include "../command/command.hpp"

//FIXME
void replace_string_chars(string my_string, char unwanted_char, char replacement_char)
{
  uint32_t string_length = strlen(string);
  uint32_t string_counter = 0;

  for(string_counter = 0; string_counter < string_length; string_counter++)
  {
    if(string[string_counter] == char_unwanted)
    {
      string[string_counter] = char_replacer;
    }
  }
  string[string_length] = '\0';
}

//FIXME
void remove_char(char* string, char garbageChar)
{
  char* src;
  char* dst;
  for(src = dst = string; *src != '\0'; src++)
  {
    *dst = *src;
    if(*dst != garbageChar)
    {
      dst++;  //so the garbageChar char gets overwriten.
    }
  }
  *dst = '\0';
}

//FIXME
void remove_extra_spaces(string my_string)
{
  char* src = string;
  char* dst = string;
  while (isspace(*src)) src++;
  for(; *src != '\0'; src++)
  {
    *dst = *src;
    if(*src != '\0')
    {
      if(*dst != *(src + 1) || *dst != ' ')
      {
        dst++;
      }
    }
  }
  if(dst[-1] == ' ')
  {
    dst[-1] = '\0';
  }
  else
  {
    *dst = '\0';
  }
}

string command_decode(string user_command)
{
  string command = "";
  uint32_t index = 0;
  while(user_command[index] != ' ' && user_command[index] != '\n')
  {
    command.append(input_command[index]);
    index++;
  }
  command.append('\0');
  return command;
}

string *arguments_decode()
{
  string *output_arguments = NULL;
  uint32_t index = 0;
  while(input_command[index] != ' ' && input_command[index] != '\n') index++;
  if(input_command[index] != '\n')
  {
    uint32_t num_arguments = 0;
    while(input_command[index] != '\n')
    {
      if(input_command[index] == ' ')
        num_arguments++;
      }
      index++;
    }

    if(num_arguments == 0)
      return NULL;

    output_arguements = new string[num_arguments];

    index = 0;
    uint32_t num_rows = 0;

    while(input_command[index] != ' ') index++;
    index++; //to go from ' ' to the next character.

    while(input_command[index] != '\n')
    {
      if(input_command[index] != ' ')
      {
        output_arguments[n_rows].append(input_command[index]);
      }
      else
      {
        output_arguments[num_rows].append('\0');
        n_rows++;
      }
      index++;
    }
    output_arguments[n_rows].append('\0');

    return output_arguments;
  }
  return NULL;
}

//FIXME
void remove_comments(char* string)
{
  unsigned index = 0;
  while(string[index] != '\0' && string[index] != '#')
  {
    index++;
  }
  if(string[index] == '#')
  {
    string[index] = '\0';
  }
}

//FIXME
void uncapitalize(char* string)
{
  unsigned index = 0;
  while(string[index] != '\0')
  {
    if(string[index] >= 65 && string[index] <= 90)
    {
      string[index] += 32;
    }
    index++;
  }
}

void available_path(int** grid, ArraySize grid_size, unsigned v_start, unsigned h_start, bool* available_path_exists, char player)
{
  if(player == 'w')
  {
    if(v_start == 1)
    {
      *available_path_exists = true;
      return;
    }
  }
  else if(player == 'b')
  {
    if(v_start == grid_size.v_size - 2)
    {
      *available_path_exists = true;
      return;
    }
  }

  printf("we are at %d %d\n", v_start, h_start);
  if(grid[v_start - 1][h_start] != '=')
    available_path(grid, grid_size, v_start - 2, h_start, available_path_exists, player);

  printf("we are at %d %d\n", v_start, h_start);
  if(*available_path_exists == false &&
  grid[v_start][h_start - 2] != 'H')
    available_path(grid, grid_size, v_start, h_start - 4, available_path_exists, player);

  printf("we are at %d %d\n", v_start, h_start);
  if(*available_path_exists == false &&
  grid[v_start][h_start + 2] != 'H')
    available_path(grid, grid_size, v_start, h_start + 4, available_path_exists, player);

  printf("we are at %d %d\n", v_start, h_start);
  if(*available_path_exists == false &&
  grid[v_start + 1][h_start] != '=')
    available_path(grid, grid_size, v_start + 2, h_start, available_path_exists, player);
}
