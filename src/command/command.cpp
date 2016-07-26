/*
File implementation : command.cpp
Authors             : Platon-Nikolaos Kiorpelidis
Purpose             : Data structures definitions
Date created        : 28-01-2016
Language            : C++
*/

/*
Lorem ipsum dolor sit amet, consectetur adipisicing elit, sed do
eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut
enim ad minim veniam, quis nostrud exercitation ullamco laboris
nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor
in reprehenderit in voluptate velit esse cillum dolore eu fugiat
nulla pariatur. Excepteur sint occaecat cupidatat non proident,
sunt in culpa qui officia deserunt mollit anim id est laborum.
(not really, I did my best to provide exellent documentation)
*/

#include <iostream>
#include <string>
#include <cstdint>

using namespace std;

#include "command.hpp"

/*
@function: creates a board 9x9 and sets the variables to the default starting state.
The default starting state is explained in the documentation.
@date tested:
*/
Quoridors_game::Quoridors_game()
{
  //@commands: sets the white player's available walls and position in the board.
  white_player.num_walls = 10;
  white_player.location.x = 8;
  white_player.location.y = 5;

  //@commands: sets the black player's available walls and position in the board.
  black_player.num_walls = 10;
  black_player.location.x = 0;
  black_player.location.y = 5;

  //@command: the quit_game variable in control for when the game quits.
  //take a look at start_game function if you want to know more about this variable.
  //it's value can only change by the quit function.
  quit_game = false;

  board.board_config = NULL;
  board.board_size = 9;

  board.board_config = new Square*[9];
  for(size_t i = 0; i < 9; i++)
    board.board_config[i] = new Square[9];

  //@command: sets the pawns at their correct squares and clears the walls.
  set_board_config();

  //@commands: sets the user_commands' variables to NULL, empty or 0.
  user_commands.num_arguments = 0;
  user_commands.arguments = NULL;
  user_commands.command = "";
  user_commands.inputed_command = "";
}

/*
@function: prints the engine's name to stdout.
@date tested: yes
*/
void Quoridors_game::engine_name()
{
  cout << "= DeepOrange" << endl << endl;
}

/*
@function: takes as argument one command and returns true if the command exists otherwise returns false.
@date tested: yes
*/
void Quoridors_game::known_command()
{
  cout << user_commands.num_arguments << endl;
  //@command: checks if the user's command has one argument. if it is not one argument then the function returns.
  if(user_commands.num_arguments != 1)
  {
    cout << "? Error: you need to give one(1) argument (ex. known_command playwall)" << endl << endl;
    return;
  }

  //@command: stores the user's argument.
  string command = user_commands.arguments[0];

  //@command: stores the number of the available engine's commands.
  uint32_t num_commands = 13;
  string known_commands[] = {"name", "known_command", "list_commands", "quit", "boardsize", "clear_board", "walls", "playmove", "playwall", "genmove", "undo", "winner", "showboard"};

  for(size_t num_rows = 0; num_rows < num_commands; num_rows++)
  {
    //@command: checks if the user's argument matches with one of the available commands.
    //Prints true if it is, otherwise prints false.
    if(known_commands[num_rows].compare(user_commands.command) == 0)
    {
      cout << "= true" << endl << endl;
      return;
    }
  }
  cout << "= false" << endl << endl;
}

/*
@funtion: lists all the available commands.
@date tested: yes
*/
void Quoridors_game::list_commands()
{
  //@command: the number of available commands is 13.
  uint32_t num_commands = 13;
  string known_commands[] = {"name", "known_command", "list_commands", "quit", "boardsize", "clear_board", "walls", "playmove", "playwall", "genmove", "undo", "winner", "showboard"};

  cout << "=" << endl;
  for(size_t counter = 0; counter < num_commands; counter++)
    cout << known_commands[counter] << endl;
  cout << endl;
}

/*
@function: quits from the game.
@tested: yes
*/
bool Quoridors_game::quit()
{
  //@command: sets the quit_game flag to true so the start_game function will stop.
  quit_game = true;
  cout << "= quitting the game" << endl << endl;
}


/*
@funtion: creates a size*size board.
@tested: yes
*/
bool Quoridors_game::set_board_size()
{
  //@command: checks if the user's input is valid.
  //if it is not valid then the function returns.
  if(user_commands.num_arguments != 1)
  {
    cout << "? Error: you need to give one(1) argument (ex. boardsize 9)" << endl << endl;
    return false;
  }

  //@command: makes the string's content a number.
  //ex. "5" -> 5.
  uint32_t size = stoi(user_commands.arguments[0], nullptr);

  //@command: checks if the user's inputed size is valid.
  //if it is not valid then the function returns.
  if(size > 26 || size < 3 || size % 2 != 1)
  {
    cout << "? Error: you need to give an odd size between 3 and 26 (ex. boardsize 3 or boardsize 11)" << endl << endl;
    return false;
  }

  //@command: checks if there is no memory allocated for the board configuration.
  //It should always be not NULL since we allocated memory in the constructor.
  //Therefore it should never get inside the if body.
  //But to be safe I keep this check, just in case.
  if(board.board_config != NULL)
  {
    for(size_t i = 0; i < board.board_size; i++)
      delete[] board.board_config[i];
    delete [] board.board_config;
    board.board_config = NULL;
    cout << "free" << endl;
  }

  //@command: allocates memory for the board's configuration.
  board.board_config = new Square*[size];
  for(size_t i = 0; i < size; i++)
    board.board_config[i] = new Square[size];

  //@command: updates the board's size.
  board.board_size = size;

  cout << "= board size set to " << size << "x" << size << endl << endl;
  return true;
}


/*
@funtion: places/resets the pawns to the correct positions and removes any placed walls.
@tested:
*/
bool Quoridors_game::set_board_config()
{
  //@command: check if there is no memory allocated for the board configuration.
  //It should always be not NULL since we allocated memory in the constructor.
  //Therefore it should never get inside the if body.
  //But to be safe I keep this check, just in case.
  if(board.board_config == NULL)
  {
    cout << "? Error: run boardsize <size> first (ex. boardsize 9)" << endl << endl;
    return false;
  }

  //@commands: sets the white player's info.
  //when counting from 0.
  white_player.location.x = board.board_size - 1;
  white_player.location.y = board.board_size / 2;

  //@commands: sets the black player's info.
  //when counting from 0.
  black_player.location.x = 0;
  black_player.location.y = board.board_size / 2;


  //@commands: sets the board's limits for the upper left square.
  //@command: since it's the upper left square then the players can't move to the up or to the left.
  board.board_config[0][0].can_move_up = false;
  board.board_config[0][0].can_move_left = false;
  board.board_config[0][0].can_move_down = true;
  board.board_config[0][0].can_move_right = true;

  //@command: sets the left side of the board's limits.
  for(size_t i = 1; i < board.board_size - 1; i++)
  {
    //@command: since it's the left side of the board then players can't move to the left.
    board.board_config[i][0].can_move_left = false;
    board.board_config[i][0].can_move_right = true;
    board.board_config[i][0].can_move_up = true;
    board.board_config[i][0].can_move_down = true;
  }

  //@commands: sets the board's limits for the bottom left square.
  //@command: since it's the bottom left square then the players can't move to the down or to the left.
  board.board_config[board.board_size - 1][0].can_move_left = false;
  board.board_config[board.board_size - 1][0].can_move_down = false;
  board.board_config[board.board_size - 1][0].can_move_right = true;
  board.board_config[board.board_size - 1][0].can_move_up = true;

  //@command: sets the top side of the board's limits.
  for(size_t i = 1; i < board.board_size - 1; i++)
  {
    //@command: since it's the left side of the board then players can't move to the up.
    board.board_config[0][i].can_move_up = false;
    board.board_config[0][i].can_move_down = true;
    board.board_config[0][i].can_move_right = true;
    board.board_config[0][i].can_move_left = true;
  }

  //@commands: sets the board's limits for the top right square.
  //@command: since it's the top right square then the players can't move to the up or to the right.
  board.board_config[0][board.board_size - 1].can_move_up = false;
  board.board_config[0][board.board_size - 1].can_move_right = false;
  board.board_config[0][board.board_size - 1].can_move_down = true;
  board.board_config[0][board.board_size - 1].can_move_left = true;

  //@command: sets the right side of the board's limits.
  for(size_t i = 1; i < board.board_size - 1; i++)
  {
    //@command: since it's the right side of the board then players can't move to the right.
    board.board_config[i][board.board_size - 1].can_move_right = false;
    board.board_config[i][board.board_size - 1].can_move_left = true;
    board.board_config[i][board.board_size - 1].can_move_up = true;
    board.board_config[i][board.board_size - 1].can_move_down = true;
  }

  //@commands: sets the board's limits for the bottom right square.
  //@command: since it's the bottom right square then players can't move to the right or to the down.
  board.board_config[board.board_size - 1][board.board_size - 1].can_move_down = false;
  board.board_config[board.board_size - 1][board.board_size - 1].can_move_right = false;
  board.board_config[board.board_size - 1][board.board_size - 1].can_move_up = true;
  board.board_config[board.board_size - 1][board.board_size - 1].can_move_left = true;

  //@command: set's the bottom side of the board's limits.
  for(size_t i = 1; i < board.board_size - 1; i++)
  {
    //@command: since it's the bottom side of the board then players can't move to the down.
    board.board_config[board.board_size - 1][i].can_move_down = false;
    board.board_config[board.board_size - 1][i].can_move_up = true;
    board.board_config[board.board_size - 1][i].can_move_left = true;
    board.board_config[board.board_size - 1][i].can_move_right = true;
  }

  //@command: sets the middle squares to have no restrictions when moving players.
  for(size_t i = 1; i < board.board_size - 1; i++)
  {
    for(size_t j = 1; j < board.board_size - 1; j++)
    {
      board.board_config[i][j].can_move_up = true;
      board.board_config[i][j].can_move_down = true;
      board.board_config[i][j].can_move_right = true;
      board.board_config[i][j].can_move_left = true;
    }
  }

  return true;
}


/*
@function: updates each player's available walls to num_walls.
@tested:
*/
bool Quoridors_game::set_num_walls()
{
  //@command: checks if the user's input is valid.
  //if it is not then the function returns.
  if(user_commands.num_arguments != 1)
  {
    cout << "? Error: you need to give one(1) argument, the number of walls (ex. walls 10)" << endl << endl;
    return false;
  }

  //@command: makes the string's content a number.
  //ex. "5" -> 5.
  uint32_t num_walls = stoi(user_commands.arguments[0], nullptr);

  //@commands: changes the amount of the available walls for both players.
  white_player.num_walls = num_walls;
  black_player.num_walls = num_walls;

  return true;
}

/*
//@function: plays the user's requestes move.
//If it is an invalid move prints a message.
Quoridors_game::playmove(char player, uint32_t x, uint32_t y)
{

}

//@function:
Quoridors_game::playwall(char player, uint32_t x, uint32_t y, char orientation)
{

}

//@function:
Quoridors_game::genmove(char player)
{

}

//@function:
Quoridors_game::undo(uint32_t num_undo)
{

}

//@function:
Quoridors_game::winner()
{

}
*/

/*
@function: prints the current board configuration at stdout.
@tested:
*/
bool Quoridors_game::showboard()
{
  //@command: it prints ' ' characters so the letters are in the correct space when they are printed.
  cout << "  ";
  //@commands: prints each column's character name.
  char col_name = 'A';
  for(size_t i = 0; i < board.board_size; i++)
    cout << "   " << col_name++;

  //@command: goes to a new line to print the top row of the board's configuration.
  cout << endl;
  //@command: prints ' ' characters so the board configuration's row numbers have space to be printed.
  cout << "   ";

  //@commands: prints the top row of the board configuration.
  cout << "+";
  for(size_t i = 0; i < board.board_size; i++)
    cout << "---+";

  //@command: goes to a new line to print the second row of the board's configuration.
  cout << endl;

  //@command: stores the each row's number name.
  uint32_t row_num = board.board_size;
  for(size_t i = 0; i < board.board_size; i++)
  {
    if(row_num >= 10)
      cout << row_num << " ";
    else
      cout << " " << row_num << " ";
    row_num--;

    cout << "|";
    if(white_player.location.x == 0 && white_player.location.y == 0)
      cout << " W ";
    else
      cout << "   ";

    for(size_t j = 1; j < board.board_size; j++)
    {
      if(board.board_config[i][j].can_move_left == true)
        cout << "|";
      else
        cout << "H";

      if(white_player.location.x == 0 && white_player.location.y == 0)
        cout << " W ";
      else
        cout << "   ";

    }
    cout << "|";

    cout << endl;

    cout << "   ";
    cout << "+";

    for(size_t j = 0; j < board.board_size; j++)
    {
      if(board.board_config[i][j].can_move_down == true)
        cout << j << "--+";
      else
        cout << "===+";
    }

    cout << endl;
  }
  cout << "   ";
  cout << "+";
  for(size_t i = 0; i < board.board_size; i++)
    cout << "---+";

  cout << endl;
}

/*
@function: replaces the unwanted_char in my_string with the replacement_char.
@date tested:
*/
void Quoridors_game::User_command::replace_string_chars(string& my_string, char unwanted_char, char replacement_char)
{
  uint32_t string_length = my_string.length();
  uint32_t string_counter = 0;
  //TODO: change the for loop to stop when it finds a null character, instead when it reaches string_length.
  for(string_counter = 0; string_counter < string_length; string_counter++)
  {
    if(my_string[string_counter] == unwanted_char)
      my_string[string_counter] = replacement_char;
  }
  //@command: TODO: probably unnecessery. Invastigate if we can remove it.
  //if it is necessery then add comments explaining why it is necessery.
  my_string[string_length] = '\0';
}

/*
@funtion: removes the garbage_char from my_string.
@date tested:
*/
void Quoridors_game::User_command::remove_char(string& my_string, char garbage_char)
{
  //@command: where the edited string will be stored.
  string my_string_edited;

  size_t i;

  for(i = 0; my_string[i] != '\0'; i++)
  {
    //@command: copies the string character by character, exept garbage_char.
    if(my_string[i] != garbage_char)
      my_string_edited.append(1, my_string[i]);
  }
  my_string = my_string_edited;
}

/*
@function: removes leading, trailing and intercepting spaces.
@date tested:
*/
void Quoridors_game::User_command::remove_extra_spaces(string& my_string)
{
  //@command: where the string without spaces will be stored.
  char *my_string_edited = NULL;

  //@command: my_string counter.
  size_t i = 0;
  //@command: my_string_edited counter.
  size_t j = 0;

  //@command: stores the capasity of my_string_edited.
  uint32_t edited_string_size = 1;

  //@command: allocates space for the first character and indirectly for the '\0' character.
  my_string_edited = (char*)realloc(my_string_edited, sizeof(char) * edited_string_size);

  //@command: removes the leading spaces.
  while (isspace(my_string[i])) i++;

  //@commands: removes intercepting spaces.
  for(; my_string[i] != '\0'; i++)
  {
    //@command: copies one character at a time.
    my_string_edited[j] = my_string[i];
    //@command: to understand use DeMorgan's theorem.
    //if there aren't two spaces in a row then allocate space for the next character.
    if(my_string_edited[j] != my_string[i + 1] || my_string_edited[j] != ' ')
    {
      edited_string_size++;
      my_string_edited = (char*)realloc(my_string_edited, sizeof(char) * edited_string_size);
      j++;
    }
  }

  //@commands: remove trailing spaces.
  if(my_string_edited[j - 1] == ' ')
    my_string_edited[j - 1] = '\0';
  else
    my_string_edited[j] = '\0';

  my_string.assign(my_string_edited);
}

/*
@function: seperates the command from the user's input and stores it into command.
@date tested: yes
*/
string Quoridors_game::User_command::command_decode()
{
  string command = "";
  uint32_t index = 0;
  //@commands: copies the characters until it reaches a ' ' character of a LF.
  while(inputed_command[index] != ' ' && inputed_command[index] != '\0')
  {
    command.append(1, inputed_command[index]);
    index++;
  }
  command[index] = '\0';
  return command;
}

/*
@funtion: seperates the arguments from the user's input.
@date tested: yes
*/
string *Quoridors_game::User_command::arguments_decode()
{
  //@command: 2D string array, where the arguments will be saved.
  string *output_arguments = NULL;

  uint32_t index = 0;

  //@command: skips characters until we reach LF or a ' ' character.
  while(inputed_command[index] != ' ' && inputed_command[index] != '\0') index++;

  //@command: checks if the while loop before reached a ' ' character instead of a LF character.
  if(inputed_command[index] != '\n')
  {
    num_arguments = 0;

    //@command: starts counting the number of the arguments until it reaches a LF character.
    while(inputed_command[index] != '\0')
    {
      if(inputed_command[index] == ' ')
        num_arguments++;
      index++;
    }

    //@command: if there are no arguments then the array is empty, therefore we return NULL.
    if(num_arguments == 0)
      return NULL;

    //@command: creates the required memory to store the arguments.
    output_arguments = new string[num_arguments];

    index = 0;
    uint32_t num_rows = 0;

    while(inputed_command[index] != ' ') index++;

    //@command: to go from ' ' to the next character.
    index++;

    //@commands: stores one by one each character of each argument.
    //Then appends the NULL character.
    while(inputed_command[index] != '\0')
    {
      if(inputed_command[index] != ' ')
      {
        output_arguments[num_rows].append(1, inputed_command[index]);
      }
      else
      {
        output_arguments[num_rows].append(1, '\0');
        num_rows++;
      }
      index++;
    }
    output_arguments[num_rows].append(1, '\0');

    return output_arguments;
  }
  return NULL;
}

    /*print right side row numbers*/
    if(counter_rows % 2 == 1)
    {
      printf(" %d", (((n_rows - 1) / 2) - (counter_rows + 1) / 2) + 1);
    }
    /*end of print right side row numbers*/

    /*print available walls for each player on the right*/
    if(counter_rows == 1)
    {
      printf("  black walls: %d", available_walls.black_walls);
    }
    else if(counter_rows == 3)
    {
      printf("  white walls: %d", available_walls.white_walls);
    }
    /*end of print available walls for each player on the right*/

    printf("\n");
  }

  /*print letters at the bottom*/
  letters = 'A';
  counter_cols = 0;
  printf("  ");
  while(counter_cols < (n_cols - 1) / 4)
  {
    printf("   %c", letters++);
    counter_cols++;
  }
  printf("\n\n");
  /*end of print letters at the bottom*/
}
