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
#include <cmath>

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
  //@command: checks if the user's command has one argument.
  //if it is not one argument then the function returns.
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

  set_board_config();

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
@function: plays the user's requester move.
@tested:
*/
bool Quoridors_game::playmove()
{
  //@command: checks if the user inputed the correct amount of arguments.
  if(user_commands.num_arguments != 2)
  {
    cout << "? Error: you need to give two(2) arguments, the player and the square position (ex. playmove w A5)" << endl << endl;
    return false;
  }

  char player = user_commands.arguments[0][0];
  int32_t y = user_commands.arguments[1][0] - 'a';
  int32_t x = board.board_size - (user_commands.arguments[1][1] - '0');

  //@command: checks if the move is out of bounds.
  if(x >= board.board_size || x <= 0 || y <= 0 || y >= board.board_size)
  {
    cout << "? Error: you issued a move that is out of board's bounds" << endl << endl;
    return false;
  }

  cout << "inputed player:" << player << endl; //Debug
  cout << "inputed x:" << x << endl; //Debug
  cout << "inputed y:" << y << endl; //Debug

  cout << "abs white_player.location.x - x:" << abs(white_player.location.x - x) << endl; //Debug
  cout << "abs white_player.location.y - y:" << abs(white_player.location.y - y) << endl; //Debug
  cout << "white_player.location.y - y:" << white_player.location.y - y << endl; //Debug

  //TODO: check if it's a valid move using the player objects (white_player and black_player) and board object (for the walls). if it is not valid then return.
  //TODO: implement the diagonal move when there is a player and a wall in from of the moving player.

  if(player == 'w')
  {
    //@command: checks if it is a horizontal move.
    if(abs(white_player.location.x - x) == 0 && abs(white_player.location.y - y) == 1)
    {
      //@command: checks if there is a wall blocking the player from moving.
      //if there is a wall then the function returns.
      if( (white_player.location.y - y == 1 && board.board_config[white_player.location.x][white_player.location.y].can_move_left == false) ||
      (white_player.location.y - y == -1 && board.board_config[white_player.location.x][white_player.location.y].can_move_right == false) )
      {
        cout << "? Error: there is a wall blocking the white player from moving to the " << y + 'a' << x + '0' << " square" << endl << endl;
        return false;
      }
      cout << "Valid move (horizontal) (debug)" << endl; //Debug
      //@commands: plays the actual move.
      white_player.location.x = x;
      white_player.location.y = y;
      cout << "= moved white player to " << static_cast<char>(y + 'A') << board.board_size - x  << endl << endl;
    }
    //@command: checks if it is a vertical move.
    else if(abs(white_player.location.x - x) == 1 && abs(white_player.location.y - y) == 0)
    {
      //@command: checks if there is a wall blocking the player from moving.
      if( (white_player.location.x - x == 1 && board.board_config[white_player.location.x][white_player.location.y].can_move_up == false) ||
      (white_player.location.x - x == -1 && board.board_config[white_player.location.x][white_player.location.y].can_move_down == false) )
      {
        cout << "? Error: There is a wall blocking the white player from moving to the " << y + 'a' << x + '0' << " square" << endl << endl;
        return false;
      }
      cout << "Valid move (vertical) (debug)" << endl; //Debug
      white_player.location.x = x;
      white_player.location.y = y;
    }
    else
    {
      cout << "Invalid move (debug)" << endl; //Debug
      cout << "? Error: Invalid move" << endl << endl;
      return false;
    }
  }
  else if(player == 'b')
  {
    //@command: checks if it is a horizontal move.
    if(abs(black_player.location.x - x) == 0 && abs(black_player.location.y - y) == 1)
    {
      //@command: checks if there is a wall blocking the player from moving.
      //if there is a wall then the function returns.
      if( (black_player.location.y - y == 1 && board.board_config[black_player.location.x][black_player.location.y].can_move_left == false) ||
      (black_player.location.y - y == -1 && board.board_config[black_player.location.x][black_player.location.y].can_move_right == false) )
      {
        cout << "? Error: there is a wall blocking the black player from moving to the " << y + 'a' << x + '0' << " square" << endl << endl;
        return false;
      }
      cout << "Valid move (horizontal) (debug)" << endl; //Debug
      //@commands: plays the actual move.
      black_player.location.x = x;
      black_player.location.y = y;
    }
    //@command: checks if it is a vertical move.
    else if(abs(black_player.location.x - x) == 1 && abs(black_player.location.y - y) == 0)
    {
      //@command: checks if there is a wall blocking the player from moving.
      if( (black_player.location.x - x == 1 && board.board_config[black_player.location.x][black_player.location.y].can_move_up == false) ||
      (black_player.location.x - x == -1 && board.board_config[black_player.location.x][black_player.location.y].can_move_down == false) )
      {
        cout << "? Error: There is a wall blocking the white player from moving to the " << y + 'a' << x + '0' << " square" << endl << endl;
        return false;
      }
      cout << "Valid move (vertical) (debug)" << endl; //Debug
      black_player.location.x = x;
      black_player.location.y = y;
    }
    else
    {
      cout << "Invalid move (debug)" << endl; //Debug
      cout << "? Error: Invalid move" << endl << endl;
      return false;
    }
  }
  return true;
}

/*
@function: places a wall at the user defined board's location.
@tested:
*/
bool Quoridors_game::playwall()
{
  //@command: checks if the user inputed the correct amount of arguments.
  if(user_commands.num_arguments != 3)
  {
    cout << "? Error: you need to give three(3) arguments, the player, the location and the direction (ex. playwall w d3 h)" << endl << endl;
    return false;
  }

  //@commands: convert and store the move's information (player, coordinates, direction).
  char player = user_commands.arguments[0][0];
  int32_t x = board.board_size - (user_commands.arguments[1][1] - '0');
  int32_t y = user_commands.arguments[1][0] - 'a';
  char direction = user_commands.arguments[2][0];

  cout << "inputed player:" << player << endl; //Debug
  cout << "inputed x:" << x << endl; //Debug
  cout << "inputed y:" << y << endl; //Debug
  cout << "inputed direction:" << direction << endl; //Debug

  //@command: places the actual wall and checks if there is already one there.
  if(direction == 'h')
  {
    //@command: checks if the move is out of the board's bounds.
    if(x <= 0 || x >= board.board_size || y <= -1 || y >= board.board_size - 1)
    {
      cout << "? Error: you issued a wall placement that is out of the board's boundaries" << endl << endl;
      return false;
    }
    //@command: checks if there is already a wall there.
    if(board.board_config[x][y].can_move_up == false)
    {
      cout << "? Error: there is already a wall there" << endl << endl;
      return false;
    }
    //@commands: place the actual wall.
    board.board_config[x][y].can_move_up = false;
    board.board_config[x][y + 1].can_move_up = false;
    board.board_config[x - 1][y].can_move_down = false;
    board.board_config[x - 1][y + 1].can_move_down = false;
    cout << "= placed the wall" << endl << endl;
  }
  else if(direction == 'v')
  {
    //@command: checks if the move is out of the board's bounds.
    if(x < 0 || x >= board.board_size - 1 || y < 0 || y >= board.board_size - 1)
    {
      cout << "? Error: you issued a wall placement that is out of board's boundaries" << endl << endl;
      return false;
    }
    //@command: checks if there is already a wall there.
    if(board.board_config[x][y].can_move_right == false)
    {
      cout << "? Error: there is already a wall there" << endl << endl;
      return false;
    }
    //@commands: place the actual wall.
    board.board_config[x][y].can_move_right = false;
    board.board_config[x + 1][y].can_move_right = false;
    board.board_config[x][y + 1].can_move_left = false;
    board.board_config[x + 1][y + 1].can_move_left = false;
    cout << "= placed the wall" << endl << endl;
  }

  //@command: removes one available wall from the player that just placed the wall.
  if(player == 'w')
  {
    white_player.num_walls--;
  }
  else if(player == 'b')
  {
    black_player.num_walls--;
  }

  return true;
}

/*
//@function:
Quoridors_game::genmove(char player)
{

}

//@function:
Quoridors_game::undo(uint32_t num_undo)
{

}
*/

/*
@function: checks if there is a winner.
@tested:
*/
bool Quoridors_game::winner()
{
  if(white_player.location.x == 0)
  {
    cout << "= white" << endl << endl;
    return true;
  }
  else if(black_player.location.x == board.board_size - 1)
  {
    cout << "= black" << endl << endl;
    return true;
  }
}

/*
@function: prints the current board configuration at stdout.
@tested:
*/
bool Quoridors_game::showboard()
{
  //@command: qtp compatibility.
  cout << "=" << endl;
  //@command: it prints ' ' characters so the letters(column names) are in the correct distance when printed.
  cout << "  ";
  //@commands: prints each column's name.
  char col_name = 'A';
  for(size_t i = 0; i < board.board_size; i++)
    cout << "   " << col_name++;

  //@command: goes to a new line to print the top row of the board's configuration.
  cout << endl;
  //@command: prints ' ' characters so the board configuration's row numbers have space to be printed afterwards.
  cout << "   ";

  //@commands: prints the top row of the board configuration.
  cout << "+";
  for(size_t i = 0; i < board.board_size; i++)
    cout << "---+";

  //@command: goes to a new line to print the second row of the board's configuration.
  cout << endl;

  //@command: stores the each row's number name.
  uint32_t row_num = board.board_size;
  //@command: starts printing the rows two at a time.
  //one with the players and the vertical walls and one with the horizontal walls.
  for(size_t i = 0; i < board.board_size; i++)
  {
    //@commands: print the row's number.
    if(row_num >= 10)
      cout << row_num << " ";
    else
      cout << " " << row_num << " ";

    cout << "|";

    //@commands: checks if there is a player at the current square and prints him.
    if(white_player.location.x == 0 && white_player.location.y == 0)
      cout << " W ";
    else if(black_player.location.x == 0 && black_player.location.y == 0)
      cout << " B ";
    else
      cout << "   ";

    //@command: prints the row with the players and the vertical walls.
    for(size_t j = 1; j < board.board_size; j++)
    {
      //@commands: prints the vertical walls.
      if(board.board_config[i][j].can_move_left == true)
        cout << "|";
      else
        cout << "H";

      //@commands: prints the players.
      if(white_player.location.x == i && white_player.location.y == j)
        cout << " W ";
      else if(black_player.location.x == i && black_player.location.y == j)
        cout << " B ";
      else
        cout << "   ";

    }
    cout << "|";

    //@command: prints the row names at the right side of the board's configuration.
    cout << " " << row_num;
    row_num--;

    //commands: prints the amount of available walls of each player.
    if(i == 0)
      cout << "  black walls: " << black_player.num_walls;
    else if(i == 1)
      cout << "  white walls: " << white_player.num_walls;

    //@command: if it is the last row then don't print the horizontal walls.
    //because it will be wrong because of set_board_config function.
    if(i == board.board_size - 1)
      break;

    cout << endl;

    cout << "   ";
    cout << "+";

    //@command: prints the horizontal walls.
    for(size_t j = 0; j < board.board_size; j++)
    {
      if(board.board_config[i][j].can_move_down == true)
      {
        if(board.board_config[i][j].can_move_right == true)
        {
          cout << "---+";
        }
        else if(i != board.board_size - 1 && board.board_config[i][j].can_move_right == false)
        {
          if(j != board.board_size - 1 && board.board_config[i + 1][j].can_move_right == false)
          {
            cout << "---H";
          }
          else
          {
            cout << "---+";
          }
        }
      }
      else
      {
        if(j != board.board_size - 1)
        {
          if(board.board_config[i][j + 1].can_move_down == false)
          {
            cout << "====";
            j++;
          }
        }
        cout << "===+";
      }
    }

    cout << endl;
  }
  //@commands: print the last row of the board.
  cout << endl;
  cout << "   ";
  cout << "+";
  for(size_t i = 0; i < board.board_size; i++)
    cout << "---+";

  //@command: LF so the column names are correctly printed.
  cout << endl;
  //@command: it prints ' ' characters so the letters(column names) are in the correct distance when printed.
  cout << "  ";
  //@commands: prints each column's name.
  col_name = 'A';
  for(size_t i = 0; i < board.board_size; i++)
    cout << "   " << col_name++;

  //@command: qtp compatibility.
  cout << endl << endl;
  return true;
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

/*
@function: removes the character after character #.
@tested: yes
*/
void Quoridors_game::User_command::remove_comments()
{
  string inputed_command_edited;

  size_t index = 0;
  //@command: finds the # character if it exists.
  //If it does not then it will reach '\0' and it will stop.
  while(inputed_command[index] != '\0' && inputed_command[index] != '#')
  {
    inputed_command_edited.append(1, inputed_command[index]);
    index++;
  }
  //@command: replaces character '#' with character '\0'.
  if(inputed_command[index] == '#')
  {
    //@command: this way the string will be printed until it encounters the '\0' character.
    inputed_command[index] = '\0';
  }
  inputed_command = inputed_command_edited;
}

/*
@function: makes capital letters to lower-case.
@tested: yes
*/
void Quoridors_game::User_command::uncapitalize(string& my_string)
{
  size_t index = 0;
  while(my_string[index] != '\0')
  {
    //@command: checks if the character is a capital one.
    //If it is then it makes it a lowercase one.
    if(my_string[index] >= 65 && my_string[index] <= 90)
      my_string[index] += 32;
    index++;
  }
}

/*
@function: This function is the controller of the game. It runs until quit function is called.
It calls other functions to seperate the command from the arguments and the arguments from the command.
As well as to edit the user's input(remove comments, trailing and leading spaces etc.).
At the end it calls the correct funtion to execute the user's request.
@date tested:
*/
void Quoridors_game::start_game()
{
  //@command: runs until the quit function is called.
  //This while loop analizes line by line the user's input.
  while(quit_game == false)
  {
    //@command: reads the next user's input line.
    getline(cin, user_commands.inputed_command);

    //@commands: these functions edit the command so the rest of the program can easily analize the user's input.
    user_commands.replace_string_chars(user_commands.inputed_command, 9, ' ');
    user_commands.remove_char(user_commands.inputed_command, 13);
    user_commands.remove_comments();
    user_commands.remove_extra_spaces(user_commands.inputed_command);
    user_commands.uncapitalize(user_commands.inputed_command);


    //@command: the command gets seperated from the user's input ex. playmove w c3, here the command is playmove.
    user_commands.command = user_commands.command_decode();
    //@command: the arguments are seperated from the user's input in an 2D array
    //ex. playmove w c3, here the arguments are w and c3.
    user_commands.arguments = user_commands.arguments_decode();

    //@command: the user's requested command is executed by matching the user's command
    //with the engine's available commands.
    //if the user's command is not one of the engine's known commands then a descriptive error is printed.
    if(user_commands.command.compare("name") == 0)
      engine_name();
    else if(user_commands.command.compare("known_command") == 0)
      known_command();
    else if(user_commands.command.compare("list_commands") == 0)
      list_commands();
    else if(user_commands.command.compare("quit") == 0)
      Quoridors_game::quit();
    else if(user_commands.command.compare("boardsize") == 0)
      Quoridors_game::set_board_size();
    else if(user_commands.command.compare("clear_board") == 0)
      Quoridors_game::set_board_config();
    else if(user_commands.command.compare("walls") == 0)
      Quoridors_game::set_num_walls();
    else if(user_commands.command.compare("playmove") == 0)
      Quoridors_game::playmove();
    else if(user_commands.command.compare("playwall") == 0)
      Quoridors_game::playwall();
    //else if(command.compare("genmove") == 0)
    //  genmove();
    //else if(command.compare("undo") == 0)
    //  undo();
    else if(user_commands.command.compare("winner") == 0)
      winner();
    else if(user_commands.command.compare("showboard") == 0)
      showboard();
    else
      cout << "? Error: unknown command (run: list_commands)" << endl << endl;
  }
}
