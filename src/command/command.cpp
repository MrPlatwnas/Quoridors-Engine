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

      printf("= black player moved to %c%d\n\n", requested_move_info.n_col + 'A', requested_move_info.n_row);
    }
  }
}

void playwall(int** grid, ArraySize grid_size, Walls* available_walls, Wall_info requested_wall_info, Players_location pawns_location)
{
  //@purpose: checks if there are available walls to be placed.
  if(requested_wall_info.player == 'w')
  {
    if(available_walls->white_walls == 0)
    {
      printf("? Error: the white player has not any walls left\n");
      return;
    }
  }
  else if(requested_wall_info.player == 'b')
//@function: removes leading, trailing and intercepting spaces.
void Quoridos_game::User_command::remove_extra_spaces(string my_string)
{
  //@command: stores the original string into my_string.
  string src = my_string;
  //@command: where the string without spaces will be stored.
  string dst = my_string;
  //@command: removes the leading spaces.
  while (isspace(*src)) src++;
  //@command: removes intercepting spaces.
  for(; *src != '\n'; src++)
  {
    *dst = *src;
    //@command: this is unnecessary, for loop checks already for this.
    if(*src != '\n')
    {
      //@command: to understand use DeMorgan's theorem.
      if(*dst != *(src + 1) || *dst != ' ')
        dst++;
    }
  }
  //@command: removes trailing spaces.
  if(dst[-1] == ' ')
  {
    dst[-1] = '\0';
  }
  else
  {
    *dst = '\0';
  }
}

void genmove()
{
  //TODO: Add functionality to this function.
}

void undo(int*** grid, unsigned n_undo, Queue* my_queue, Players_location* pawns_location, ArraySize grid_size)
{
  Queue_elem element;
  //@command: removing the unrequired Queue_elems.
  while(n_undo > 1)
  {
    element = stack_pop(my_queue);
    if(element.isset == false)
    {
      printf("? Error: you can not undo anymore\n\n");
      return;
    }
  }

  //@command: fetching the required Queue_elem.
  element = stack_pop(my_queue);

  if(element.isset == false)
  {
    printf("? Error: you can not undo anymore\n\n");
    return;
  }

  //@purpose: copying the information to the current grid conf.
  //@command: copying the black information to the current grid conf.
  pawns_location->black_location.h_coordinate = element.pawns_location.black_location.h_coordinate;
  pawns_location->black_location.v_coordinate = element.pawns_location.black_location.v_coordinate;
  //@command: copying the white information to the current grid conf.
  pawns_location->white_location.h_coordinate = element.pawns_location.white_location.h_coordinate;
  pawns_location->white_location.v_coordinate = element.pawns_location.white_location.v_coordinate;

  freeGrid((*grid), grid_size.v_size);
  (*grid) = NULL;

  (*grid) = initGridInt(grid_size.v_size, grid_size.h_size);

  //@command: copying the grid configuration to the grid.
  for(size_t counter = 0; counter < grid_size.v_size; counter++)
  {
    memcpy((*grid)[counter], element.grid[counter], grid_size.h_size * sizeof(int));
  }

  //@command: frees the stored grid configuration.
  freeGrid(element.grid, grid_size.v_size);

  return;
}

void winner(ArraySize grid_size, Players_location* pawns_location)
{
  //@purpose: to check if white or black pawn is at the top or last row respectively.
  if(pawns_location->white_location.v_coordinate == 1)
  {
    printf("= true white\n\n");
    return;
  }
  else if(pawns_location->black_location.v_coordinate == grid_size.v_size - 2)
  {
    printf("= true black\n\n");
    return;
  }
  //@command: since neither white or black is at the top or last row then we have no winner yet, therefore = false is printed.
  printf("= false\n\n");
  return;
}

void showboard(int** grid, Walls available_walls, ArraySize grid_size)
{
  unsigned n_rows = grid_size.v_size;
  unsigned n_cols = grid_size.h_size;
  unsigned counter_rows = 0;
  unsigned counter_cols = 0;

  printf("=\n");

  /*print letters at the bottom*/
  char letters = 'A';
  counter_cols = 0;
  printf("  ");
  while(counter_cols < (n_cols - 1) / 4)
  {
    printf("   %c", letters++);
    counter_cols++;
  }
  printf("\n");
  /*end of print letters at the bottom*/

  for(counter_rows = 0; counter_rows < n_rows; counter_rows++)
  {
    /*print left side row numbers*/
    if(counter_rows % 2 == 1)
    {
      if((((n_rows - 1) / 2) - (counter_rows + 1) / 2) + 1 >= 10)
      {
        printf("%d ", (((n_rows - 1) / 2) - (counter_rows + 1) / 2) + 1);
      }
      else if((((n_rows - 1) / 2) - (counter_rows + 1) / 2) < 10)
      {
        printf(" %d ", (((n_rows - 1) / 2) - (counter_rows + 1) / 2) + 1);
      }
    }
    else if(counter_rows % 2 == 0)
    {
      printf("   ");
    }
    /*end of print left side row numbers*/

    /*print array content*/
    for(counter_cols = 0; counter_cols < n_cols; counter_cols++)
    {
      printf("%c", grid[counter_rows][counter_cols]);
    }
    /*print array content*/

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
