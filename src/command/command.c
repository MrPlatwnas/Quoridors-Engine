/******************************************************
File implementation : command.c
Authors             : Platwnas-Nikolaos Kiorpelidis
Purpose             : command functions definitions
Date                : 28-1-2015
*******************************************************/

/* C89 std */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* C99 std */
#include <stdint.h>
#include <stdbool.h>

/* custom headers */
#include "../misc/misc.h"
#include "../command/command.h"
#include "../queue/queue.h"

//this function runs while quit_game is false and prossecess the commands that the user enters and then calls the right function to execute the requested command.
void user_input_decode()
{
  char *inputed_command = NULL; //get_lines the user's input from the stdin.

  char *command = NULL; //here actual command gets stored ex. playmove w c3, the command here is playmove.
  char **arguments = NULL; //the actual arguments get stored ex playmove w c3, the arguments here are w and c3.
  unsigned n_arguments; //the number of arguments the user inputed. this variable is used to catch errors ex. playmove w, this command is missing one argument, the coordinates.

  //struct to store the board's visual size, grid_size.size, the board's actual v_size and h_size, grid_size.v_size and grid_size.h_size respectively.
  ArraySize grid_size;
  grid_size.size = 9;
  grid_size.v_size = 9 * 2 + 1;
  grid_size.h_size = 9 * 4 + 1;

  //struct to store the available walls for each player.
  Walls available_walls;
  available_walls.white_walls = 10;
  available_walls.black_walls = 10;

  //struct to store the current location of each player.
  Players_location pawns_location;
  //@command: so winner function won't print = true color by accident.
  pawns_location.black_location.v_coordinate = 0;
  //@command: so winner function won't print = true color by accident.
  pawns_location.white_location.v_coordinate = 0;

  //@command: create the stack to save the grid configurations in order to undo.
  Queue* my_stack = stack_construct();
  Queue_elem element;

  //struct to store the board configuration.
  int **grid = NULL;

  //boolean variable so you can quit the game when quit function is called.
  bool quit_game = false;
  while(!quit_game)
  {
    inputed_command = get_line(); //gets the user's input line from the stdin.

    //we edit the user's input as needed according to qtp rules.
    /*STRING EDITING FUNCTIONS*/
    replace_string_chars(inputed_command, 9, ' ');
    remove_char(inputed_command, 13);
    remove_comments(inputed_command);
    remove_extra_spaces(inputed_command);
    uncapitalize(inputed_command);
    /*END OF STRING EDITING FUNCTIONS*/

    //the command gets seperated from the user's input ex. playmove w c3, here the command is playmove.
    command = command_decode(inputed_command);
    //the arguments are seperated from the user's input in an 2D array ex. playmove w c3, here the arguments are w and c3.
    arguments = arguments_decode(inputed_command, &n_arguments);

    //the user specified command is executed by matching the user's command with the engine's available commands. if the user's command is not one of the engine's known commands then a descriptive error is printed.
    if(strcmp(command, "name") == 0)
      name();
    else if(strcmp(command, "known_command") == 0)
      known_command(arguments, n_arguments);
    else if(strcmp(command, "list_commands") == 0)
      list_commands();
    else if(strcmp(command, "quit") == 0)
      quit(&quit_game);
    else if(strcmp(command, "boardsize") == 0)
    {
      if(n_arguments == 1)
      {
        grid_size.size = atoi(arguments[0]);
        if(grid_size.size >= 3 && grid_size.size <= 25 && grid_size.size % 2 == 1)
        {
          if(grid != NULL)
          {
            freeGrid(grid, grid_size.v_size);
            grid = NULL;
          }
          grid_size.v_size = grid_size.size * 2 + 1;
          grid_size.h_size = grid_size.size * 4 + 1;
          grid = boardsize(grid_size, &pawns_location);
        }
        else
        {
          printf("? Error: you need to input an odd size between 3 and 26\n\n");
        }
      }
      else
      {
        printf("? Error: you need to give one(1) argument (ex. boardsize 9)\n\n");
      }
    }
    else if(strcmp(command, "clear_board") == 0)
      clear_board(grid, grid_size, &pawns_location);
    else if(strcmp(command, "walls") == 0)
      if(n_arguments == 1)
        walls(&available_walls, atoi(arguments[0]));
      else
        printf("? Error: you need to give one(1) argument (ex. walls 10)\n\n");
    else if(strcmp(command, "playmove") == 0)
    {
      if(grid != NULL && n_arguments == 2)
      {
        Move_info requested_move_info;
        if(arguments[1][2] != '\0')
        {
          requested_move_info.n_row = arguments[1][1] - '0';
          requested_move_info.n_row *= 10;
          requested_move_info.n_row += arguments[1][2] - '0';
        }
        else
        {
          requested_move_info.n_row = arguments[1][1] - '0';
        }
        requested_move_info.n_col = arguments[1][0] - 'a';

        if(strcmp(arguments[0], "w") == 0 || strcmp(arguments[0], "white") == 0)
        {
          requested_move_info.player = 'w';
        }
        else if(strcmp(arguments[0], "b") == 0 || strcmp(arguments[0], "black") == 0)
        {
          requested_move_info.player = 'b';
        }

        //@purpose: add the new grid conf to the stack so we can undo into it.
        element.grid = grid;
        element.pawns_location = pawns_location;
        stack_push(element, my_stack, grid_size);

        playmove(grid, grid_size, &pawns_location, requested_move_info); /*PLAYMOVE FUNCTION CALL*/

      }
      else if(grid == NULL)
      {
        printf("? Error: you need to create a board first (run: boardsize <desired_size>)\n\n");
      }
      else if(n_arguments != 2)
      {
        printf("? Error: you need to give two(2) arguments (ex. playmove w a1)\n\n");
      }
    }
    else if(strcmp(command, "playwall") == 0)
    {
      if(n_arguments == 3)
      {
        Wall_info requested_wall_info;
        if(arguments[1][2] != '\0')
        {
          requested_wall_info.n_row = arguments[1][1] - '0';
          requested_wall_info.n_row *= 10;
          requested_wall_info.n_row += arguments[1][2] - '0';
        }
        else
        {
          requested_wall_info.n_row = arguments[1][1] - '0';
        }
        requested_wall_info.n_col = arguments[1][0] - 'a';

        if(strcmp(arguments[0], "w") == 0 || strcmp(arguments[0], "white") == 0)
          requested_wall_info.player = 'w';
        else if(strcmp(arguments[0], "b") == 0 || strcmp(arguments[0], "black") == 0)
          requested_wall_info.player = 'b';

        if(strcmp(arguments[2], "v") == 0 || strcmp(arguments[2], "vertical") == 0)
          requested_wall_info.orientation = 'v';
        else if(strcmp(arguments[2], "h") == 0 || strcmp(arguments[2], "horizontal") == 0)
          requested_wall_info.orientation = 'h';

        //@purpose: add the new grid conf to the stack so we can undo into it.
        element.grid = grid;
        element.pawns_location = pawns_location;
        stack_push(element, my_stack, grid_size);

        playwall(grid, grid_size, &available_walls, requested_wall_info, pawns_location); /*PLAYWALL FUNCTION CALL*/

      }
      else
      {
        printf("? Error: you need to give 3 arguments (ex. playwall w a5 v)\n\n");
      }
    }
    else if(strcmp(command, "genmove") == 0)
    {
      if(n_arguments == 1)
      {
        //genmove(grid, grid_size, arguments[0]); /*GENMOVE FUNCTION CALL*/
      }
      else
      {
        printf("? Error: you need to give one(1) argument (ex. genmove w)\n\n");
      }
    }
    else if(strcmp(command, "undo") == 0)
    {
      if(n_arguments == 1)
      {
        unsigned n_undo = arguments[0][0] - '0';
        undo(&grid, n_undo, my_stack, &pawns_location, grid_size); /*UNDO FUNCTION CALL*/
      }
      else
      {
        printf("? Error: you need to give one(1) argument (ex. undo 4)\n\n");
      }
    }
    else if(strcmp(command, "winner") == 0)
    {
      winner(grid_size, &pawns_location);  /*WINNER FUNCTION CALL*/
    }
    else if(strcmp(command, "showboard") == 0)
    {
      if(grid != NULL)
      {
        showboard(grid, available_walls, grid_size);  /*SHOWBOARD FUNCTION CALL*/
      }
      else
      {
        printf("? Error: you need to create a board first (run: boardsize <desired_size>)\n\n");
      }
    }
    else
    {
      printf("? Error: unknown command (run: list_commands)\n\n");
    }
  }
}

/*********************************************************
-------------------------COMMANDS-------------------------
*********************************************************/

//TODO: add comments to each function. organize the code structure.

//name function prints the engine's name at stdout.
void name()
{
  printf("= DeepOrange\n\n");
}

//known_command function takes as argument one command and returns true if the command exists otherwise returns false.
void known_command(char** arguments, unsigned n_arguments)
{
  //checks if the user's arguments is one argument. if it is not one argument then the function returns.
  if(n_arguments != 1)
  {
    printf("? Error: you need to give one(1) argument (ex. known_command playwall)\n\n");
    return;
  }

  //stores the user's argument.
  char* command = arguments[0];

  //the number of available commands is 9.
  unsigned n_commands = 9;
  char* known_commands[] = {"name", "known_command", "list_commands", "quit", "boardsize", "clear_board", "walls", "playmove", "playwall", "genmove", "undo", "winner", "showboard"};

  unsigned n_rows = 0;
  for(n_rows = 0; n_rows < n_commands; n_rows++)
  {
    //checks if the user's argument one of the available commands. prinst true if it is, otherwise false.
    if(strcmp(command, known_commands[n_rows]) == 0)
    {
      printf("= true\n\n");
      return;
    }
  }
  printf("= false\n\n");
}

//list_commands function lists all the available commands.
void list_commands()
{
  //the number of available commands is 13.
  unsigned n_commands = 13;
  char* known_commands[] = {"name", "known_command", "list_commands", "quit", "boardsize", "clear_board", "walls", "playmove", "playwall", "genmove", "undo", "winner", "showboard"};

  printf("=\n");
  unsigned counter = 0;
  for(counter = 0; counter < n_commands; counter++)
  {
    printf("%s\n", known_commands[counter]);
  }
  printf("\n");
}

//quit function stops the game.
void quit(bool *quit_game)
{
  *quit_game = true;
  printf("= quitting game\n\n");
}

//boardsize creates the grid's base structure using build_grid.
int** boardsize(ArraySize grid_size, Players_location* pawns_location)
{
  int **grid = NULL;

  //build_grid creates the grid's basic structure.
  grid = build_grid(grid_size);

  //using the board's actual size the black pawn is placed at the visual first row.
  grid[1][grid_size.h_size / 2] = 'B';
  //using the board's actual size the white pawn is placed at the visual last row.
  grid[grid_size.v_size - 2][grid_size.h_size / 2] = 'W';

  //the pawns' location is updated. the pawns_location struct is used by the winner function.
  pawns_location->white_location.v_coordinate = grid_size.v_size - 2;
  pawns_location->white_location.h_coordinate = grid_size.h_size / 2;

  pawns_location->black_location.v_coordinate = 1;
  pawns_location->black_location.h_coordinate = grid_size.h_size / 2;

  printf("= board's size set to %dx%d\n\n", grid_size.size, grid_size.size);
  return grid;
}

//clear board populates the grid's basic structure by placing the pawns at the correct positions.
void clear_board(int** grid, ArraySize grid_size, Players_location* pawns_location)
{
  grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = ' ';
  grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = ' ';

  for(size_t counter_rows = 1; counter_rows < grid_size.v_size - 1; counter_rows++)
  {
    for(size_t counter_cols = 1; counter_cols < grid_size.h_size - 1; counter_cols++)
    {
      if(grid[counter_rows][counter_cols] == 'H')
      {
        if(counter_rows % 2 == 0)
          grid[counter_rows][counter_cols] = '+';
        else
          grid[counter_rows][counter_cols] = '|';
      }
      else if(grid[counter_rows][counter_cols] == '=')
      {
        if(counter_cols % 4 == 0)
          grid[counter_rows][counter_cols] = '+';
        else
          grid[counter_rows][counter_cols] = '-';
      }
    }
  }

  //using the board's actual size the black pawn is placed at the visual first row.
  grid[1][grid_size.h_size / 2] = 'B';
  //using the board's actual size the white pawn is placed at the visual last row.
  grid[grid_size.v_size - 2][grid_size.h_size / 2] = 'W';

  //the pawns' location is updated. the pawns_location struct is used by the winner function.
  pawns_location->white_location.v_coordinate = grid_size.v_size - 2;
  pawns_location->white_location.h_coordinate = grid_size.h_size / 2;

  pawns_location->black_location.v_coordinate = 1;
  pawns_location->black_location.h_coordinate = grid_size.h_size / 2;

  printf("= pawns and walls got removed\n\n");
  return;
}

//walls function updates each player's available walls, using Walls struct.
void walls(Walls* available_walls, unsigned input_n_walls)
{
  available_walls->white_walls = input_n_walls;
  available_walls->black_walls = input_n_walls;

  printf("= number of walls set to %d for each player\n\n", input_n_walls);
  return;
}

//playmove function takes the requested move and calculates if it's valid or not. if it is valid then it is played otherwise a descriptive error is printed.
void playmove(int** grid, ArraySize grid_size, Players_location* pawns_location, Move_info requested_move_info)
{
  //convert the users given coordinates to grid_size.size coordinates in order to check if the requested move is out of bounds, because the user's given coordinates are flipped because showboard shows the numbers in reverse.
  unsigned v_coordinate = grid_size.size - requested_move_info.n_row;
  //stores the users given coordinates in order to check if the requested move is out of bounds.
  unsigned h_coordinate = requested_move_info.n_col;

  //checks if the requested move is out of bounds. Note: there is no need to check if v_coordinate < 0 or if h_coordinate < 0 because the variables are unsigned so when v_coordinate is -1 then it's a very large positive number so the check is still triggered.
  if(v_coordinate >= grid_size.size || h_coordinate >= grid_size.size)
  {
    printf("? Error: requested move is out of board's bountaries\n\n");
    return;
  }

  //converts the v_coordinate into the actual board's coordinates.
  v_coordinate = v_coordinate * 2 + 1;
  //converts the h_coordinate into the actual board's coordinates.
  h_coordinate = h_coordinate * 4 + 2;

  //we will work with two different cases. the first when the white player wants to move and the second case when the black player wants to move.
  //this is the first case, for the white player.
  if(requested_move_info.player == 'w')
  {
    //checks if there is another opponent at the requested coordinates. if there is, prints a descriptive error.
    if(grid[v_coordinate][h_coordinate] == 'W')
    {
      printf("? Error: you have already played there\n");
      return;
    }
    else if(grid[v_coordinate][h_coordinate] == 'B')
    {
      printf("? Error: the black player has played there\n");
      return;
    }

    //checks if the requested coordinates are at one of the four sides of the board.
    if(v_coordinate == 1 || v_coordinate == grid_size.v_size - 2 ||
    h_coordinate == 2 || h_coordinate == grid_size.h_size - 3)
    {
      //checks the top side but not the corners.
      if(v_coordinate == 1 &&
      (h_coordinate != 2 && h_coordinate != grid_size.h_size - 3))
      {
        //debug : printf("YOU ARE PLAYING AT THE TOP BUT NOT AT THE CORNERS\n");
        //checks if there is the pawn that requested the move below the requested coordinates. if it is then the move is valid and the old pawn is removed.
        if(grid[v_coordinate + 2][h_coordinate] == 'W')
          grid[v_coordinate + 2][h_coordinate] = ' ';
        //checks if there is the pawn that requested the move left of the requested coordinates. if it is then the move is valid and the old pawn is removed.
        else if(grid[v_coordinate][h_coordinate - 4] == 'W')
          grid[v_coordinate][h_coordinate - 4] = ' ';
        //checks if there is the pawn that requested the move right of the requested coordinates. if it is then the move is valid and the old pawn is removed.
        else if(grid[v_coordinate][h_coordinate + 4] == 'W')
          grid[v_coordinate][h_coordinate + 4] = ' ';
        //else if there is not the pawn that requested the move, right, left or below the requested coordinates then the move is invalid and a descriptive error is printed. The function returns.
        else
        {
          printf("? Error: you can only play to one adjacent tile either vertically or horizontally\n\n");
          return;
        }

        //@note: when a pawn move it can only move up, down, left or right. therefore only one coordinate will change.
        //@purpose: check if there is a wall blocking the move.
        //@command: if evaluates to true then the move is horizontal.
        if(pawns_location->white_location.v_coordinate == v_coordinate)
        {
          //@command: if evaluates to true then the move is to the left.
          if((int)pawns_location->white_location.h_coordinate - (int)h_coordinate > 0)
          {
            //debug : printf("this is a left move\n");
            if(grid[v_coordinate][h_coordinate + 2] == 'H')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
          //@command: if evaluates to true then the move is to the right.
          else if((int)pawns_location->white_location.h_coordinate - (int)h_coordinate < 0)
          {
            //debug : printf("this is a right move\n");
            if(grid[v_coordinate][h_coordinate - 2] == 'H')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
        }
        //@command: if evaluates to true then the move is vertical.
        else if(pawns_location->white_location.h_coordinate == h_coordinate)
        {
          //@command: if evaluates to true then the move is upwards.
          if((int)pawns_location->white_location.v_coordinate - (int)v_coordinate > 0)
          {
            //debug : printf("this is a up move\n");
            if(grid[v_coordinate + 1][h_coordinate] == '=')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
          //@command: if evaluates to true the move is downwards.
          else if((int)pawns_location->white_location.v_coordinate - (int)v_coordinate < 0)
          {
            //debug : printf("this is a down move\n");
            if(grid[v_coordinate - 1][h_coordinate] == '=')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
        }

        //the new pawn is placed.
        grid[v_coordinate][h_coordinate] = 'W';

        //update the white pawn's location for clear_board and winner functions.
        pawns_location->white_location.v_coordinate = v_coordinate;
        pawns_location->white_location.h_coordinate = h_coordinate;

        printf("= white player moved to %c%d\n\n", requested_move_info.n_col + 'A', requested_move_info.n_row);
      }
      //checks the left side but not the corners.
      else if((v_coordinate != 1 && v_coordinate != grid_size.v_size - 2) &&
      h_coordinate == 2)
      {
        //debug : printf("YOU ARE PLAYING AT THE LEFT BUT NOT AT THE CORNERS\n");
        if(grid[v_coordinate][h_coordinate + 4] == 'W')
          grid[v_coordinate][h_coordinate + 4] = ' ';
        else if(grid[v_coordinate + 2][h_coordinate] == 'W')
          grid[v_coordinate + 2][h_coordinate] = ' ';
        else if(grid[v_coordinate - 2][h_coordinate] == 'W')
          grid[v_coordinate - 2][h_coordinate] = ' ';
        else
        {
          printf("? Error: you can only play to one adjacent tile either vertically or horizontally\n\n");
          return;
        }

        //@note: when a pawn move it can only move up, down, left or right. therefore only one coordinate will change.
        //@purpose: check if there is a wall blocking the move.
        //@command: if evaluates to true then the move is horizontal.
        if(pawns_location->white_location.v_coordinate == v_coordinate)
        {
          //@command: if evaluates to true then the move is to the left.
          if((int)pawns_location->white_location.h_coordinate - (int)h_coordinate > 0)
          {
            //debug : printf("this is a left move\n");
            if(grid[v_coordinate][h_coordinate + 2] == 'H')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
          //@command: if evaluates to true then the move is to the right.
          else if((int)pawns_location->white_location.h_coordinate - (int)h_coordinate < 0)
          {
            //debug : printf("this is a right move\n");
            if(grid[v_coordinate][h_coordinate - 2] == 'H')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
        }
        //@command: if evaluates to true then the move is vertical.
        else if(pawns_location->white_location.h_coordinate == h_coordinate)
        {
          //@command: if evaluates to true then the move is upwards.
          if((int)pawns_location->white_location.v_coordinate - (int)v_coordinate > 0)
          {
            //debug : printf("this is a up move\n");
            if(grid[v_coordinate + 1][h_coordinate] == '=')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
          //@command: if evaluates to true the move is downwards.
          else if((int)pawns_location->white_location.v_coordinate - (int)v_coordinate < 0)
          {
            //debug : printf("this is a down move\n");
            if(grid[v_coordinate - 1][h_coordinate] == '=')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
        }

        //the new pawn is placed.
        grid[v_coordinate][h_coordinate] = 'W';

        //update the white pawn's location for clear_board and winner functions.
        pawns_location->white_location.v_coordinate = v_coordinate;
        pawns_location->white_location.h_coordinate = h_coordinate;

        printf("= white player moved to %c%d\n\n", requested_move_info.n_col + 'A', requested_move_info.n_row);
      }
      //checks the right side but not the corners.
      else if((v_coordinate != 1 && v_coordinate != grid_size.v_size -2) &&
      h_coordinate == grid_size.h_size - 3)
      {
        //debug : printf("YOU ARE PLAYING AT THE RIGHT BUT NOT AT THE CORNERS\n");
        if(grid[v_coordinate][h_coordinate - 4] == 'W')
          grid[v_coordinate][h_coordinate - 4] = ' ';
        else if(grid[v_coordinate - 2][h_coordinate] == 'W')
          grid[v_coordinate - 2][h_coordinate] = ' ';
        else if(grid[v_coordinate + 2][h_coordinate] == 'W')
          grid[v_coordinate + 2][h_coordinate] = ' ';
        else
        {
          printf("? Error: you can only play to one adjacent tile either vertically or horizontally\n\n");
          return;
        }

        //@note: when a pawn move it can only move up, down, left or right. therefore only one coordinate will change.
        //@purpose: check if there is a wall blocking the move.
        //@command: if evaluates to true then the move is horizontal.
        if(pawns_location->white_location.v_coordinate == v_coordinate)
        {
          //@command: if evaluates to true then the move is to the left.
          if((int)pawns_location->white_location.h_coordinate - (int)h_coordinate > 0)
          {
            //debug : printf("this is a left move\n");
            if(grid[v_coordinate][h_coordinate + 2] == 'H')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
          //@command: if evaluates to true then the move is to the right.
          else if((int)pawns_location->white_location.h_coordinate - (int)h_coordinate < 0)
          {
            //debug : printf("this is a right move\n");
            if(grid[v_coordinate][h_coordinate - 2] == 'H')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
        }
        //@command: if evaluates to true then the move is vertical.
        else if(pawns_location->white_location.h_coordinate == h_coordinate)
        {
          //@command: if evaluates to true then the move is upwards.
          if((int)pawns_location->white_location.v_coordinate - (int)v_coordinate > 0)
          {
            //debug : printf("this is a up move\n");
            if(grid[v_coordinate + 1][h_coordinate] == '=')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
          //@command: if evaluates to true the move is downwards.
          else if((int)pawns_location->white_location.v_coordinate - (int)v_coordinate < 0)
          {
            //debug : printf("this is a down move\n");
            if(grid[v_coordinate - 1][h_coordinate] == '=')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
        }

        //the new pawn is placed.
        grid[v_coordinate][h_coordinate] = 'W';

        //update the white pawn's location for clear_board and winner functions.
        pawns_location->white_location.v_coordinate = v_coordinate;
        pawns_location->white_location.h_coordinate = h_coordinate;

        printf("= white player moved to %c%d\n", requested_move_info.n_col + 'A', requested_move_info.n_row);
      }
      //checks the bottom side but not the corners.
      else if(v_coordinate == grid_size.v_size - 2 &&
      (h_coordinate != 2 && h_coordinate != grid_size.h_size - 3))
      {
        //debug : printf("YOU ARE PLAYING AT THE BOTTOM BUT NOT AT THE CORNERS\n");
        if(grid[v_coordinate - 2][h_coordinate] == 'W')
          grid[v_coordinate - 2][h_coordinate] = ' ';
        else if(grid[v_coordinate][h_coordinate + 4] == 'W')
          grid[v_coordinate][h_coordinate + 4] = ' ';
        else if(grid[v_coordinate][h_coordinate - 4] == 'W')
          grid[v_coordinate][h_coordinate - 4] = ' ';
        else
        {
          printf("? Error: you can only play to one adjacent tile either vertically or horizontally\n\n");
          return;
        }

        //@note: when a pawn move it can only move up, down, left or right. therefore only one coordinate will change.
        //@purpose: check if there is a wall blocking the move.
        //@command: if evaluates to true then the move is horizontal.
        if(pawns_location->white_location.v_coordinate == v_coordinate)
        {
          //@command: if evaluates to true then the move is to the left.
          if((int)pawns_location->white_location.h_coordinate - (int)h_coordinate > 0)
          {
            //debug : printf("this is a left move\n");
            if(grid[v_coordinate][h_coordinate + 2] == 'H')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
          //@command: if evaluates to true then the move is to the right.
          else if((int)pawns_location->white_location.h_coordinate - (int)h_coordinate < 0)
          {
            //debug : printf("this is a right move\n");
            if(grid[v_coordinate][h_coordinate - 2] == 'H')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
        }
        //@command: if evaluates to true then the move is vertical.
        else if(pawns_location->white_location.h_coordinate == h_coordinate)
        {
          //@command: if evaluates to true then the move is upwards.
          if((int)pawns_location->white_location.v_coordinate - (int)v_coordinate > 0)
          {
            //debug : printf("this is a up move\n");
            if(grid[v_coordinate + 1][h_coordinate] == '=')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
          //@command: if evaluates to true the move is downwards.
          else if((int)pawns_location->white_location.v_coordinate - (int)v_coordinate < 0)
          {
            //debug : printf("this is a down move\n");
            if(grid[v_coordinate - 1][h_coordinate] == '=')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
        }

        //the new pawn is placed.
        grid[v_coordinate][h_coordinate] = 'W';

        //update the white pawn's location for clear_board and winner functions.
        pawns_location->white_location.v_coordinate = v_coordinate;
        pawns_location->white_location.h_coordinate = h_coordinate;

        printf("= white player moved to %c%d\n\n", requested_move_info.n_col + 'A', requested_move_info.n_row);
      }
      //now we are starting to check if the requested move is at one of the four corners.
      //checks for the top left corner.
      else if(v_coordinate == 1 && h_coordinate == 2)
      {
        //debug : printf("YOU ARE PLAYING AT THE TOP LEFT CORNER\n");
        if(grid[v_coordinate + 2][h_coordinate] == 'W')
          grid[v_coordinate + 2][h_coordinate] = ' ';
        else if(grid[v_coordinate][h_coordinate + 4] == 'W')
          grid[v_coordinate][h_coordinate + 4] = ' ';
        else
        {
          printf("? Error: you can only play to one adjacent tile either vertically or horizontally\n\n");
          return;
        }

        //@note: when a pawn move it can only move up, down, left or right. therefore only one coordinate will change.
        //@purpose: check if there is a wall blocking the move.
        //@command: if evaluates to true then the move is horizontal.
        if(pawns_location->white_location.v_coordinate == v_coordinate)
        {
          //@command: if evaluates to true then the move is to the left.
          if((int)pawns_location->white_location.h_coordinate - (int)h_coordinate > 0)
          {
            //debug : printf("this is a left move\n");
            if(grid[v_coordinate][h_coordinate + 2] == 'H')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
          //@command: if evaluates to true then the move is to the right.
          else if((int)pawns_location->white_location.h_coordinate - (int)h_coordinate < 0)
          {
            //debug : printf("this is a right move\n");
            if(grid[v_coordinate][h_coordinate - 2] == 'H')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
        }
        //@command: if evaluates to true then the move is vertical.
        else if(pawns_location->white_location.h_coordinate == h_coordinate)
        {
          //@command: if evaluates to true then the move is upwards.
          if((int)pawns_location->white_location.v_coordinate - (int)v_coordinate > 0)
          {
            //debug : printf("this is a up move\n");
            if(grid[v_coordinate + 1][h_coordinate] == '=')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
          //@command: if evaluates to true the move is downwards.
          else if((int)pawns_location->white_location.v_coordinate - (int)v_coordinate < 0)
          {
            //debug : printf("this is a down move\n");
            if(grid[v_coordinate - 1][h_coordinate] == '=')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
        }

        //the new pawn is placed.
        grid[v_coordinate][h_coordinate] = 'W';

        //update the white pawn's location for clear_board and winner functions.
        pawns_location->white_location.v_coordinate = v_coordinate;
        pawns_location->white_location.h_coordinate = h_coordinate;

        printf("= white player moved to %c%d\n\n", requested_move_info.n_col + 'A', requested_move_info.n_row);
      }
      //checks for the bottom left corner.
      else if(v_coordinate == grid_size.v_size - 2 && h_coordinate == 2)
      {
        //debug : printf("YOU ARE PLAYING AT THE BOTTOM LEFT CORNER\n");
        if(grid[v_coordinate - 2][h_coordinate] == 'W')
          grid[v_coordinate - 2][h_coordinate] = ' ';
        else if(grid[v_coordinate][h_coordinate + 4] == 'W')
          grid[v_coordinate][h_coordinate + 4] = ' ';
        else
        {
          printf("? Error: you can only play to one adjacent tile either vertically or horizontally\n\n");
          return;
        }

        //@note: when a pawn move it can only move up, down, left or right. therefore only one coordinate will change.
        //@purpose: check if there is a wall blocking the move.
        //@command: if evaluates to true then the move is horizontal.
        if(pawns_location->white_location.v_coordinate == v_coordinate)
        {
          //@command: if evaluates to true then the move is to the left.
          if((int)pawns_location->white_location.h_coordinate - (int)h_coordinate > 0)
          {
            //debug : printf("this is a left move\n");
            if(grid[v_coordinate][h_coordinate + 2] == 'H')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
          //@command: if evaluates to true then the move is to the right.
          else if((int)pawns_location->white_location.h_coordinate - (int)h_coordinate < 0)
          {
            //debug : printf("this is a right move\n");
            if(grid[v_coordinate][h_coordinate - 2] == 'H')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
        }
        //@command: if evaluates to true then the move is vertical.
        else if(pawns_location->white_location.h_coordinate == h_coordinate)
        {
          //@command: if evaluates to true then the move is upwards.
          if((int)pawns_location->white_location.v_coordinate - (int)v_coordinate > 0)
          {
            //debug : printf("this is a up move\n");
            if(grid[v_coordinate + 1][h_coordinate] == '=')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
          //@command: if evaluates to true the move is downwards.
          else if((int)pawns_location->white_location.v_coordinate - (int)v_coordinate < 0)
          {
            //debug : printf("this is a down move\n");
            if(grid[v_coordinate - 1][h_coordinate] == '=')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
        }

        //the new pawn is placed.
        grid[v_coordinate][h_coordinate] = 'W';

        //update the white pawn's location for clear_board and winner functions.
        pawns_location->white_location.v_coordinate = v_coordinate;
        pawns_location->white_location.h_coordinate = h_coordinate;

        printf("= white player moved to %c%d\n\n", requested_move_info.n_col + 'A', requested_move_info.n_row);
      }
      //checks for the top right corner.
      else if(v_coordinate == 1 && h_coordinate == grid_size.h_size - 3)
      {
        //debug : printf("YOU ARE PLAYING AT THE TOP RIGHT CORNER\n");
        if(grid[v_coordinate + 2][h_coordinate] == 'W')
          grid[v_coordinate + 2][h_coordinate] = ' ';
        else if(grid[v_coordinate][h_coordinate - 4] == 'W')
          grid[v_coordinate][h_coordinate - 4] = ' ';
        else
        {
          printf("? Error: you can only play to one adjacent tile either vertically or horizontally\n\n");
          return;
        }

        //@note: when a pawn move it can only move up, down, left or right. therefore only one coordinate will change.
        //@purpose: check if there is a wall blocking the move.
        //@command: if evaluates to true then the move is horizontal.
        if(pawns_location->white_location.v_coordinate == v_coordinate)
        {
          //@command: if evaluates to true then the move is to the left.
          if((int)pawns_location->white_location.h_coordinate - (int)h_coordinate > 0)
          {
            //debug : printf("this is a left move\n");
            if(grid[v_coordinate][h_coordinate + 2] == 'H')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
          //@command: if evaluates to true then the move is to the right.
          else if((int)pawns_location->white_location.h_coordinate - (int)h_coordinate < 0)
          {
            //debug : printf("this is a right move\n");
            if(grid[v_coordinate][h_coordinate - 2] == 'H')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
        }
        //@command: if evaluates to true then the move is vertical.
        else if(pawns_location->white_location.h_coordinate == h_coordinate)
        {
          //@command: if evaluates to true then the move is upwards.
          if((int)pawns_location->white_location.v_coordinate - (int)v_coordinate > 0)
          {
            //debug : printf("this is a up move\n");
            if(grid[v_coordinate + 1][h_coordinate] == '=')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
          //@command: if evaluates to true the move is downwards.
          else if((int)pawns_location->white_location.v_coordinate - (int)v_coordinate < 0)
          {
            //debug : printf("this is a down move\n");
            if(grid[v_coordinate - 1][h_coordinate] == '=')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
        }

        //the new pawn is placed.
        grid[v_coordinate][h_coordinate] = 'W';

        //update the white pawn's location for clear_board and winner functions.
        pawns_location->white_location.v_coordinate = v_coordinate;
        pawns_location->white_location.h_coordinate = h_coordinate;

        printf("= white player moved to %c%d\n\n", requested_move_info.n_col + 'A', requested_move_info.n_row);
      }
      //checks for the bottom right corner.
      else if(v_coordinate == grid_size.v_size - 2 && h_coordinate == grid_size.h_size - 3)
      {
        //debug : printf("YOU ARE PLAYING AT THE BOTTOM RIGHT CORNER\n");
        if(grid[v_coordinate - 2][h_coordinate] == 'W')
          grid[v_coordinate - 2][h_coordinate] = ' ';
        else if(grid[v_coordinate][h_coordinate - 4] == 'W')
          grid[v_coordinate][h_coordinate - 4] = ' ';
        else
        {
          printf("? Error: you can only play to one adjacent tile either vertically or horizontally\n\n");
          return;
        }

        //@note: when a pawn move it can only move up, down, left or right. therefore only one coordinate will change.
        //@purpose: check if there is a wall blocking the move.
        //@command: if evaluates to true then the move is horizontal.
        if(pawns_location->white_location.v_coordinate == v_coordinate)
        {
          //@command: if evaluates to true then the move is to the left.
          if((int)pawns_location->white_location.h_coordinate - (int)h_coordinate > 0)
          {
            //debug : printf("this is a left move\n");
            if(grid[v_coordinate][h_coordinate + 2] == 'H')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
          //@command: if evaluates to true then the move is to the right.
          else if((int)pawns_location->white_location.h_coordinate - (int)h_coordinate < 0)
          {
            //debug : printf("this is a right move\n");
            if(grid[v_coordinate][h_coordinate - 2] == 'H')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
        }
        //@command: if evaluates to true then the move is vertical.
        else if(pawns_location->white_location.h_coordinate == h_coordinate)
        {
          //@command: if evaluates to true then the move is upwards.
          if((int)pawns_location->white_location.v_coordinate - (int)v_coordinate > 0)
          {
            //debug : printf("this is a up move\n");
            if(grid[v_coordinate + 1][h_coordinate] == '=')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
          //@command: if evaluates to true the move is downwards.
          else if((int)pawns_location->white_location.v_coordinate - (int)v_coordinate < 0)
          {
            //debug : printf("this is a down move\n");
            if(grid[v_coordinate - 1][h_coordinate] == '=')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
        }

        //the new pawn is placed.
        grid[v_coordinate][h_coordinate] = 'W';

        //update the white pawn's location for clear_board and winner functions.
        pawns_location->white_location.v_coordinate = v_coordinate;
        pawns_location->white_location.h_coordinate = h_coordinate;

        printf("= white player moved to %c%d\n\n", requested_move_info.n_col + 'A', requested_move_info.n_row);
      }
    }
    //checks for the rest of the possible moves. i.e. if the requested move is not at the sides or if its a center square.
    else
    {
      if(grid[v_coordinate + 2][h_coordinate] == 'W')
        grid[v_coordinate + 2][h_coordinate] = ' ';
      else if(grid[v_coordinate - 2][h_coordinate] == 'W')
        grid[v_coordinate - 2][h_coordinate] = ' ';
      else if(grid[v_coordinate][h_coordinate + 4] == 'W')
        grid[v_coordinate][h_coordinate + 4] = ' ';
      else if(grid[v_coordinate][h_coordinate - 4] == 'W')
        grid[v_coordinate][h_coordinate - 4] = ' ';
      else
      {
        printf("? Error: you can only play to one adjacent tile either vertically or horizontally\n\n");
        return;
      }

      //@note: when a pawn move it can only move up, down, left or right. therefore only one coordinate will change.
      //@purpose: check if there is a wall blocking the move.
      //@command: if evaluates to true then the move is horizontal.
      if(pawns_location->white_location.v_coordinate == v_coordinate)
      {
        //@command: if evaluates to true then the move is to the left.
        if((int)pawns_location->white_location.h_coordinate - (int)h_coordinate > 0)
        {
          //debug : printf("this is a left move\n");
          if(grid[v_coordinate][h_coordinate + 2] == 'H')
          {
            //debug : printf("found a wall!\n");
            //@command: replaces the old pawn.
            grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
            printf("? Error: there is a wall blocking the path\n\n");
            return;
          }
        }
        //@command: if evaluates to true then the move is to the right.
        else if((int)pawns_location->white_location.h_coordinate - (int)h_coordinate < 0)
        {
          //debug : printf("this is a right move\n");
          if(grid[v_coordinate][h_coordinate - 2] == 'H')
          {
            //debug : printf("found a wall!\n");
            //@command: replaces the old pawn.
            grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
            printf("? Error: there is a wall blocking the path\n\n");
            return;
          }
        }
      }
      //@command: if evaluates to true then the move is vertical.
      else if(pawns_location->white_location.h_coordinate == h_coordinate)
      {
        //@command: if evaluates to true then the move is upwards.
        if((int)pawns_location->white_location.v_coordinate - (int)v_coordinate > 0)
        {
          //debug : printf("this is a up move\n");
          if(grid[v_coordinate + 1][h_coordinate] == '=')
          {
            //debug : printf("found a wall!\n");
            //@command: replaces the old pawn.
            grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
            printf("? Error: there is a wall blocking the path\n\n");
            return;
          }
        }
        //@command: if evaluates to true the move is downwards.
        else if((int)pawns_location->white_location.v_coordinate - (int)v_coordinate < 0)
        {
          //debug : printf("this is a down move\n");
          if(grid[v_coordinate - 1][h_coordinate] == '=')
          {
            //debug : printf("found a wall!\n");
            //@command: replaces the old pawn.
            grid[pawns_location->white_location.v_coordinate][pawns_location->white_location.h_coordinate] = 'W';
            printf("? Error: there is a wall blocking the path\n\n");
            return;
          }
        }
      }

      //the new pawn is placed.
      grid[v_coordinate][h_coordinate] = 'W';

      //update the white pawn's location for clear_board and winner functions.
      pawns_location->white_location.v_coordinate = v_coordinate;
      pawns_location->white_location.h_coordinate = h_coordinate;

      printf("= white player moved to %c%d\n\n", requested_move_info.n_col + 'A', requested_move_info.n_row);
    }
  }
  //now we check the same things for the black as we did for the white.
  else if(requested_move_info.player == 'b')
  {
    //checks if there is another opponent at the requested coordinates. if there is, prints a descriptive error.
    if(grid[v_coordinate][h_coordinate] == 'B')
    {
      printf("? Error: you have already played there\n");
      return;
    }
    else if(grid[v_coordinate][h_coordinate] == 'W')
    {
      printf("? Error: the white player has played there\n");
      return;
    }

    //checks if the requested coordinates are at one of the four sides of the board.
    if(v_coordinate == 1 || v_coordinate == grid_size.v_size - 2 ||
    h_coordinate == 2 || h_coordinate == grid_size.h_size - 3)
    {
      //checks the top side but not the corners.
      if(v_coordinate == 1 &&
      (h_coordinate != 2 && h_coordinate != grid_size.h_size - 3))
      {
        //debug : printf("YOU ARE PLAYING AT THE TOP BUT NOT AT THE CORNERS\n");
        //checks if there is the pawn that requested the move below the requested coordinates. if it is then the move is valid and the old pawn is removed.
        if(grid[v_coordinate + 2][h_coordinate] == 'B')
          grid[v_coordinate + 2][h_coordinate] = ' ';
        //checks if there is the pawn that requested the move left of the requested coordinates. if it is then the move is valid and the old pawn is removed.
        else if(grid[v_coordinate][h_coordinate - 4] == 'B')
          grid[v_coordinate][h_coordinate - 4] = ' ';
        //checks if there is the pawn that requested the move right of the requested coordinates. if it is then the move is valid and the old pawn is removed.
        else if(grid[v_coordinate][h_coordinate + 4] == 'B')
          grid[v_coordinate][h_coordinate + 4] = ' ';
        //else if there is not the pawn that requested the move, right, left or below the requested coordinates then the move is invalid and a descriptive error is printed. The function returns.
        else
        {
          printf("? Error: you can only play to one adjacent tile either vertically or horizontally\n\n");
          return;
        }

        //@note: when a pawn move it can only move up, down, left or right. therefore only one coordinate will change.
        //@purpose: check if there is a wall blocking the move.
        //@command: if evaluates to true then the move is horizontal.
        if(pawns_location->black_location.v_coordinate == v_coordinate)
        {
          //@command: if evaluates to true then the move is to the left.
          if((int)pawns_location->black_location.h_coordinate - (int)h_coordinate > 0)
          {
            //debug : printf("this is a left move\n");
            if(grid[v_coordinate][h_coordinate + 2] == 'H')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
          //@command: if evaluates to true then the move is to the right.
          else if((int)pawns_location->black_location.h_coordinate - (int)h_coordinate < 0)
          {
            //debug : printf("this is a right move\n");
            if(grid[v_coordinate][h_coordinate - 2] == 'H')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
        }
        //@command: if evaluates to true then the move is vertical.
        else if(pawns_location->black_location.h_coordinate == h_coordinate)
        {
          //@command: if evaluates to true then the move is upwards.
          if((int)pawns_location->black_location.v_coordinate - (int)v_coordinate > 0)
          {
            //debug : printf("this is a up move\n");
            if(grid[v_coordinate + 1][h_coordinate] == '=')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
          //@command: if evaluates to true the move is downwards.
          else if((int)pawns_location->black_location.v_coordinate - (int)v_coordinate < 0)
          {
            //debug : printf("this is a down move\n");
            if(grid[v_coordinate - 1][h_coordinate] == '=')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
        }

        //the new pawn is placed.
        grid[v_coordinate][h_coordinate] = 'B';

        //update the black pawn's location for clear_board and winner functions.
        pawns_location->black_location.v_coordinate = v_coordinate;
        pawns_location->black_location.h_coordinate = h_coordinate;

        printf("= black player moved to %c%d\n\n", requested_move_info.n_col + 'A', requested_move_info.n_row);
      }
      else if((v_coordinate != 1 && v_coordinate != grid_size.v_size - 2) &&
      h_coordinate == 2)  //left but not corners.
      {
        //debug : printf("YOU ARE PLAYING AT THE LEFT BUT NOT AT THE CORNERS\n");
        if(grid[v_coordinate][h_coordinate + 4] == 'B')
          grid[v_coordinate][h_coordinate + 4] = ' ';
        else if(grid[v_coordinate + 2][h_coordinate] == 'B')
          grid[v_coordinate + 2][h_coordinate] = ' ';
        else if(grid[v_coordinate - 2][h_coordinate] == 'B')
          grid[v_coordinate - 2][h_coordinate] = ' ';
        else
        {
          printf("? Error: you can only play to one adjacent tile either vertically or horizontally\n\n");
          return;
        }

        //@note: when a pawn move it can only move up, down, left or right. therefore only one coordinate will change.
        //@purpose: check if there is a wall blocking the move.
        //@command: if evaluates to true then the move is horizontal.
        if(pawns_location->black_location.v_coordinate == v_coordinate)
        {
          //@command: if evaluates to true then the move is to the left.
          if((int)pawns_location->black_location.h_coordinate - (int)h_coordinate > 0)
          {
            //debug : printf("this is a left move\n");
            if(grid[v_coordinate][h_coordinate + 2] == 'H')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
          //@command: if evaluates to true then the move is to the right.
          else if((int)pawns_location->black_location.h_coordinate - (int)h_coordinate < 0)
          {
            //debug : printf("this is a right move\n");
            if(grid[v_coordinate][h_coordinate - 2] == 'H')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
        }
        //@command: if evaluates to true then the move is vertical.
        else if(pawns_location->black_location.h_coordinate == h_coordinate)
        {
          //@command: if evaluates to true then the move is upwards.
          if((int)pawns_location->black_location.v_coordinate - (int)v_coordinate > 0)
          {
            //debug : printf("this is a up move\n");
            if(grid[v_coordinate + 1][h_coordinate] == '=')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
          //@command: if evaluates to true the move is downwards.
          else if((int)pawns_location->black_location.v_coordinate - (int)v_coordinate < 0)
          {
            //debug : printf("this is a down move\n");
            if(grid[v_coordinate - 1][h_coordinate] == '=')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
        }

        //the new pawn is placed.
        grid[v_coordinate][h_coordinate] = 'B';

        //update the black pawn's location for clear_board and winner functions.
        pawns_location->black_location.v_coordinate = v_coordinate;
        pawns_location->black_location.h_coordinate = h_coordinate;

        printf("= black player moved to %c%d\n\n", requested_move_info.n_col + 'A', requested_move_info.n_row);
      }
      else if((v_coordinate != 1 && v_coordinate != grid_size.v_size -2) &&
      h_coordinate == grid_size.h_size - 3) //right but not corners.
      {
        //debug : printf("YOU ARE PLAYING AT THE RIGHT BUT NOT AT THE CORNERS\n");
        if(grid[v_coordinate][h_coordinate - 4] == 'B')
          grid[v_coordinate][h_coordinate - 4] = ' ';
        else if(grid[v_coordinate - 2][h_coordinate] == 'B')
          grid[v_coordinate - 2][h_coordinate] = ' ';
        else if(grid[v_coordinate + 2][h_coordinate] == 'B')
          grid[v_coordinate + 2][h_coordinate] = ' ';
        else
        {
          printf("? Error: you can only play to one adjacent tile either vertically or horizontally\n\n");
          return;
        }

        //@note: when a pawn move it can only move up, down, left or right. therefore only one coordinate will change.
        //@purpose: check if there is a wall blocking the move.
        //@command: if evaluates to true then the move is horizontal.
        if(pawns_location->black_location.v_coordinate == v_coordinate)
        {
          //@command: if evaluates to true then the move is to the left.
          if((int)pawns_location->black_location.h_coordinate - (int)h_coordinate > 0)
          {
            //debug : printf("this is a left move\n");
            if(grid[v_coordinate][h_coordinate + 2] == 'H')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
          //@command: if evaluates to true then the move is to the right.
          else if((int)pawns_location->black_location.h_coordinate - (int)h_coordinate < 0)
          {
            //debug : printf("this is a right move\n");
            if(grid[v_coordinate][h_coordinate - 2] == 'H')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
        }
        //@command: if evaluates to true then the move is vertical.
        else if(pawns_location->black_location.h_coordinate == h_coordinate)
        {
          //@command: if evaluates to true then the move is upwards.
          if((int)pawns_location->black_location.v_coordinate - (int)v_coordinate > 0)
          {
            //debug : printf("this is a up move\n");
            if(grid[v_coordinate + 1][h_coordinate] == '=')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
          //@command: if evaluates to true the move is downwards.
          else if((int)pawns_location->black_location.v_coordinate - (int)v_coordinate < 0)
          {
            //debug : printf("this is a down move\n");
            if(grid[v_coordinate - 1][h_coordinate] == '=')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
        }

        grid[v_coordinate][h_coordinate] = 'B';

        //update the black pawn's location for clear_board and winner functions.
        pawns_location->black_location.v_coordinate = v_coordinate;
        pawns_location->black_location.h_coordinate = h_coordinate;

        printf("= black player moved to %c%d\n\n", requested_move_info.n_col + 'A', requested_move_info.n_row);
      }
      else if(v_coordinate == grid_size.v_size - 2 &&
      (h_coordinate != 2 && h_coordinate != grid_size.h_size - 3))  //bottom but not corners.
      {
        //debug : printf("YOU ARE PLAYING AT THE BOTTOM BUT NOT AT THE CORNERS\n");
        if(grid[v_coordinate][h_coordinate - 4] == 'B')
          grid[v_coordinate][h_coordinate - 4] = ' ';
        else if(grid[v_coordinate - 2][h_coordinate] == 'B')
          grid[v_coordinate - 2][h_coordinate] = ' ';
        else if(grid[v_coordinate + 2][h_coordinate] == 'B')
          grid[v_coordinate + 2][h_coordinate] = ' ';
        else
        {
          printf("? Error: you can only play to one adjacent tile either vertically or horizontally\n\n");
          return;
        }

        //@note: when a pawn move it can only move up, down, left or right. therefore only one coordinate will change.
        //@purpose: check if there is a wall blocking the move.
        //@command: if evaluates to true then the move is horizontal.
        if(pawns_location->black_location.v_coordinate == v_coordinate)
        {
          //@command: if evaluates to true then the move is to the left.
          if((int)pawns_location->black_location.h_coordinate - (int)h_coordinate > 0)
          {
            //debug : printf("this is a left move\n");
            if(grid[v_coordinate][h_coordinate + 2] == 'H')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
          //@command: if evaluates to true then the move is to the right.
          else if((int)pawns_location->black_location.h_coordinate - (int)h_coordinate < 0)
          {
            //debug : printf("this is a right move\n");
            if(grid[v_coordinate][h_coordinate - 2] == 'H')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
        }
        //@command: if evaluates to true then the move is vertical.
        else if(pawns_location->black_location.h_coordinate == h_coordinate)
        {
          //@command: if evaluates to true then the move is upwards.
          if((int)pawns_location->black_location.v_coordinate - (int)v_coordinate > 0)
          {
            //debug : printf("this is a up move\n");
            if(grid[v_coordinate + 1][h_coordinate] == '=')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
          //@command: if evaluates to true the move is downwards.
          else if((int)pawns_location->black_location.v_coordinate - (int)v_coordinate < 0)
          {
            //debug : printf("this is a down move\n");
            if(grid[v_coordinate - 1][h_coordinate] == '=')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
        }

        grid[v_coordinate][h_coordinate] = 'B';

        //update the black pawn's location for clear_board and winner functions.
        pawns_location->black_location.v_coordinate = v_coordinate;
        pawns_location->black_location.h_coordinate = h_coordinate;

        printf("= black player moved to %c%d\n\n", requested_move_info.n_col + 'A', requested_move_info.n_row);
      }
      else if(v_coordinate == 1 && h_coordinate == 2) //top left corner.
      {
        if(grid[v_coordinate + 2][h_coordinate] == 'B')
          grid[v_coordinate + 2][h_coordinate] = ' ';
        else if(grid[v_coordinate][h_coordinate + 4] == 'B')
          grid[v_coordinate][h_coordinate + 4] = ' ';
        else
        {
          printf("? Error: you can only play to one adjacent tile either vertically or horizontally\n\n");
          return;
        }

        //@note: when a pawn move it can only move up, down, left or right. therefore only one coordinate will change.
        //@purpose: check if there is a wall blocking the move.
        //@command: if evaluates to true then the move is horizontal.
        if(pawns_location->black_location.v_coordinate == v_coordinate)
        {
          //@command: if evaluates to true then the move is to the left.
          if((int)pawns_location->black_location.h_coordinate - (int)h_coordinate > 0)
          {
            //debug : printf("this is a left move\n");
            if(grid[v_coordinate][h_coordinate + 2] == 'H')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
          //@command: if evaluates to true then the move is to the right.
          else if((int)pawns_location->black_location.h_coordinate - (int)h_coordinate < 0)
          {
            //debug : printf("this is a right move\n");
            if(grid[v_coordinate][h_coordinate - 2] == 'H')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
        }
        //@command: if evaluates to true then the move is vertical.
        else if(pawns_location->black_location.h_coordinate == h_coordinate)
        {
          //@command: if evaluates to true then the move is upwards.
          if((int)pawns_location->black_location.v_coordinate - (int)v_coordinate > 0)
          {
            //debug : printf("this is a up move\n");
            if(grid[v_coordinate + 1][h_coordinate] == '=')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
          //@command: if evaluates to true the move is downwards.
          else if((int)pawns_location->black_location.v_coordinate - (int)v_coordinate < 0)
          {
            //debug : printf("this is a down move\n");
            if(grid[v_coordinate - 1][h_coordinate] == '=')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
        }

        grid[v_coordinate][h_coordinate] = 'B';

        //update the black pawn's location for clear_board and winner functions.
        pawns_location->black_location.v_coordinate = v_coordinate;
        pawns_location->black_location.h_coordinate = h_coordinate;

        printf("= black player moved to %c%d\n\n", requested_move_info.n_col + 'A', requested_move_info.n_row);
      }
      else if(v_coordinate == grid_size.v_size - 2 && h_coordinate == 2) //bottom left corner.
      {
        if(grid[v_coordinate - 2][h_coordinate] == 'B')
          grid[v_coordinate - 2][h_coordinate] = ' ';
        else if(grid[v_coordinate][h_coordinate + 4] == 'B')
          grid[v_coordinate][h_coordinate + 4] = ' ';
        else
        {
          printf("? Error: you can only play to one adjacent tile either vertically or horizontally\n\n");
          return;
        }

        //@note: when a pawn move it can only move up, down, left or right. therefore only one coordinate will change.
        //@purpose: check if there is a wall blocking the move.
        //@command: if evaluates to true then the move is horizontal.
        if(pawns_location->black_location.v_coordinate == v_coordinate)
        {
          //@command: if evaluates to true then the move is to the left.
          if((int)pawns_location->black_location.h_coordinate - (int)h_coordinate > 0)
          {
            //debug : printf("this is a left move\n");
            if(grid[v_coordinate][h_coordinate + 2] == 'H')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
          //@command: if evaluates to true then the move is to the right.
          else if((int)pawns_location->black_location.h_coordinate - (int)h_coordinate < 0)
          {
            //debug : printf("this is a right move\n");
            if(grid[v_coordinate][h_coordinate - 2] == 'H')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
        }
        //@command: if evaluates to true then the move is vertical.
        else if(pawns_location->black_location.h_coordinate == h_coordinate)
        {
          //@command: if evaluates to true then the move is upwards.
          if((int)pawns_location->black_location.v_coordinate - (int)v_coordinate > 0)
          {
            //debug : printf("this is a up move\n");
            if(grid[v_coordinate + 1][h_coordinate] == '=')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
          //@command: if evaluates to true the move is downwards.
          else if((int)pawns_location->black_location.v_coordinate - (int)v_coordinate < 0)
          {
            //debug : printf("this is a down move\n");
            if(grid[v_coordinate - 1][h_coordinate] == '=')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
        }

        grid[v_coordinate][h_coordinate] = 'B';

        //update the black pawn's location for clear_board and winner functions.
        pawns_location->black_location.v_coordinate = v_coordinate;
        pawns_location->black_location.h_coordinate = h_coordinate;

        printf("= black player moved to %c%d\n\n", requested_move_info.n_col + 'A', requested_move_info.n_row);
      }
      else if(v_coordinate == 1 && h_coordinate == grid_size.h_size - 3) //top right corner.
      {
        if(grid[v_coordinate + 2][h_coordinate] == 'B')
          grid[v_coordinate + 2][h_coordinate] = ' ';
        else if(grid[v_coordinate][h_coordinate - 4] == 'B')
          grid[v_coordinate][h_coordinate - 4] = ' ';
        else
        {
          printf("? Error: you can only play to one adjacent tile either vertically or horizontally\n\n");
          return;
        }

        //@note: when a pawn move it can only move up, down, left or right. therefore only one coordinate will change.
        //@purpose: check if there is a wall blocking the move.
        //@command: if evaluates to true then the move is horizontal.
        if(pawns_location->black_location.v_coordinate == v_coordinate)
        {
          //@command: if evaluates to true then the move is to the left.
          if((int)pawns_location->black_location.h_coordinate - (int)h_coordinate > 0)
          {
            //debug : printf("this is a left move\n");
            if(grid[v_coordinate][h_coordinate + 2] == 'H')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
          //@command: if evaluates to true then the move is to the right.
          else if((int)pawns_location->black_location.h_coordinate - (int)h_coordinate < 0)
          {
            //debug : printf("this is a right move\n");
            if(grid[v_coordinate][h_coordinate - 2] == 'H')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
        }
        //@command: if evaluates to true then the move is vertical.
        else if(pawns_location->black_location.h_coordinate == h_coordinate)
        {
          //@command: if evaluates to true then the move is upwards.
          if((int)pawns_location->black_location.v_coordinate - (int)v_coordinate > 0)
          {
            //debug : printf("this is a up move\n");
            if(grid[v_coordinate + 1][h_coordinate] == '=')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
          //@command: if evaluates to true the move is downwards.
          else if((int)pawns_location->black_location.v_coordinate - (int)v_coordinate < 0)
          {
            //debug : printf("this is a down move\n");
            if(grid[v_coordinate - 1][h_coordinate] == '=')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
        }

        grid[v_coordinate][h_coordinate] = 'B';

        //update the black pawn's location for clear_board and winner functions.
        pawns_location->black_location.v_coordinate = v_coordinate;
        pawns_location->black_location.h_coordinate = h_coordinate;

        printf("= black player moved to %c%d\n\n", requested_move_info.n_col + 'A', requested_move_info.n_row);
      }
      else if(v_coordinate == grid_size.v_size - 2 && h_coordinate == grid_size.h_size - 3) //bottom right corner.
      {
        if(grid[v_coordinate - 2][h_coordinate] == 'B')
          grid[v_coordinate - 2][h_coordinate] = ' ';
        else if(grid[v_coordinate][h_coordinate - 4] == 'B')
          grid[v_coordinate][h_coordinate - 4] = ' ';
        else
        {
          printf("? Error: you can only play to one adjacent tile either vertically or horizontally\n\n");
          return;
        }

        //@note: when a pawn move it can only move up, down, left or right. therefore only one coordinate will change.
        //@purpose: check if there is a wall blocking the move.
        //@command: if evaluates to true then the move is horizontal.
        if(pawns_location->black_location.v_coordinate == v_coordinate)
        {
          //@command: if evaluates to true then the move is to the left.
          if((int)pawns_location->black_location.h_coordinate - (int)h_coordinate > 0)
          {
            //debug : printf("this is a left move\n");
            if(grid[v_coordinate][h_coordinate + 2] == 'H')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
          //@command: if evaluates to true then the move is to the right.
          else if((int)pawns_location->black_location.h_coordinate - (int)h_coordinate < 0)
          {
            //debug : printf("this is a right move\n");
            if(grid[v_coordinate][h_coordinate - 2] == 'H')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
        }
        //@command: if evaluates to true then the move is vertical.
        else if(pawns_location->black_location.h_coordinate == h_coordinate)
        {
          //@command: if evaluates to true then the move is upwards.
          if((int)pawns_location->black_location.v_coordinate - (int)v_coordinate > 0)
          {
            //debug : printf("this is a up move\n");
            if(grid[v_coordinate + 1][h_coordinate] == '=')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
          //@command: if evaluates to true the move is downwards.
          else if((int)pawns_location->black_location.v_coordinate - (int)v_coordinate < 0)
          {
            //debug : printf("this is a down move\n");
            if(grid[v_coordinate - 1][h_coordinate] == '=')
            {
              //debug : printf("found a wall!\n");
              //@command: replaces the old pawn.
              grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
              printf("? Error: there is a wall blocking the path\n\n");
              return;
            }
          }
        }

        grid[v_coordinate][h_coordinate] = 'B';

        //update the black pawn's location for clear_board and winner functions.
        pawns_location->black_location.v_coordinate = v_coordinate;
        pawns_location->black_location.h_coordinate = h_coordinate;

        printf("= black player moved to %c%d\n\n", requested_move_info.n_col + 'A', requested_move_info.n_row);
      }
    }
    else  //the requested move is at a center square.
    {
      if(grid[v_coordinate + 2][h_coordinate] == 'B')
        grid[v_coordinate + 2][h_coordinate] = ' ';
      else if(grid[v_coordinate - 2][h_coordinate] == 'B')
        grid[v_coordinate - 2][h_coordinate] = ' ';
      else if(grid[v_coordinate][h_coordinate + 4] == 'B')
        grid[v_coordinate][h_coordinate + 4] = ' ';
      else if(grid[v_coordinate][h_coordinate - 4] == 'B')
        grid[v_coordinate][h_coordinate - 4] = ' ';
      else
      {
        printf("? Error: you can only play to one adjacent tile either vertically or horizontally\n\n");
        return;
      }

      //@note: when a pawn move it can only move up, down, left or right. therefore only one coordinate will change.
      //@purpose: check if there is a wall blocking the move.
      //@command: if evaluates to true then the move is horizontal.
      if(pawns_location->black_location.v_coordinate == v_coordinate)
      {
        //@command: if evaluates to true then the move is to the left.
        if((int)pawns_location->black_location.h_coordinate - (int)h_coordinate > 0)
        {
          //debug : printf("this is a left move\n");
          if(grid[v_coordinate][h_coordinate + 2] == 'H')
          {
            //debug : printf("found a wall!\n");
            //@command: replaces the old pawn.
            grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
            printf("? Error: there is a wall blocking the path\n\n");
            return;
          }
        }
        //@command: if evaluates to true then the move is to the right.
        else if((int)pawns_location->black_location.h_coordinate - (int)h_coordinate < 0)
        {
          //debug : printf("this is a right move\n");
          if(grid[v_coordinate][h_coordinate - 2] == 'H')
          {
            //debug : printf("found a wall!\n");
            //@command: replaces the old pawn.
            grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
            printf("? Error: there is a wall blocking the path\n\n");
            return;
          }
        }
      }
      //@command: if evaluates to true then the move is vertical.
      else if(pawns_location->black_location.h_coordinate == h_coordinate)
      {
        //@command: if evaluates to true then the move is upwards.
        if((int)pawns_location->black_location.v_coordinate - (int)v_coordinate > 0)
        {
          //debug : printf("this is a up move\n");
          if(grid[v_coordinate + 1][h_coordinate] == '=')
          {
            //debug : printf("found a wall!\n");
            //@command: replaces the old pawn.
            grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
            printf("? Error: there is a wall blocking the path\n\n");
            return;
          }
        }
        //@command: if evaluates to true the move is downwards.
        else if((int)pawns_location->black_location.v_coordinate - (int)v_coordinate < 0)
        {
          //debug : printf("this is a down move\n");
          if(grid[v_coordinate - 1][h_coordinate] == '=')
          {
            //debug : printf("found a wall!\n");
            //@command: replaces the old pawn.
            grid[pawns_location->black_location.v_coordinate][pawns_location->black_location.h_coordinate] = 'B';
            printf("? Error: there is a wall blocking the path\n\n");
            return;
          }
        }
      }

      grid[v_coordinate][h_coordinate] = 'B';

      //update the black pawn's location for clear_board and winner functions.
      pawns_location->black_location.v_coordinate = v_coordinate;
      pawns_location->black_location.h_coordinate = h_coordinate;

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
  {
    if(available_walls->black_walls == 0)
    {
      printf("? Error: the black player has not any walls left\n");
      return;
    }
  }

  //convert the users given coordinates to grid_size.size coordinates in order to check if the requested wall placement is out of bounds, because the user's given coordinates are flipped because showboard shows the numbers in reverse order.
  unsigned v_coordinate = grid_size.size - requested_wall_info.n_row;
  //stores the users given coordinates in order to check if the requested move is out of bounds.
  unsigned h_coordinate = requested_wall_info.n_col;

  //checks if the requested move is out of bounds. Note: there is no need to check if v_coordinate < 0 or if h_coordinate < 0 because the variables are unsigned so when v_coordinate is -1 then it's a very large positive number so the check is still triggered.
  if(v_coordinate >= grid_size.size - 1 || h_coordinate >= grid_size.size - 1)
  {
    printf("? Error: requested move is out of board's bountaries\n\n");
    return;
  }

  //converts the v_coordinate into the actual board's coordinates.
  v_coordinate = v_coordinate * 2 + 1;
  //converts the h_coordinate into the actual board's coordinates.
  h_coordinate = h_coordinate * 4 + 2;

  //debug : printf("v_coordinate: %d\n", v_coordinate);
  //debug : printf("h_coordinate: %d\n", h_coordinate);

  //checks for vertical and horizontal wall placement seperately because v walls use character 'H' and horizontal walls use character '='.
  if(requested_wall_info.orientation == 'v')
  {
    //checks if there is already a vertical wall there. if there is then a descriptive error messsage is printed.
    if(grid[v_coordinate][h_coordinate + 2] == 'H' ||
    grid[v_coordinate + 2][h_coordinate + 2] == 'H' ||
    grid[v_coordinate + 1][h_coordinate + 2] == '=')
    {
      printf("? Error: there is already a wall there\n");
      return;
    }

    //places the actual wall.
    grid[v_coordinate][h_coordinate + 2] = 'H';
    grid[v_coordinate + 1][h_coordinate + 2] = 'H';
    grid[v_coordinate + 2][h_coordinate + 2] = 'H';

    //@purpose: checks if there is an available path to win for both players.
    bool available_path_white = false;
    available_path(grid, grid_size, pawns_location.white_location.v_coordinate, pawns_location.white_location.h_coordinate, &available_path_white, 'w');
    bool available_path_black = false;
    available_path(grid, grid_size, pawns_location.black_location.v_coordinate, pawns_location.black_location.h_coordinate, &available_path_black, 'b');
    //@command: resets the placed wall since it is against the game's rules.
    if(available_path_white == false || available_path_black == false)
    {
      grid[v_coordinate][h_coordinate + 2] = '|';
      grid[v_coordinate + 1][h_coordinate + 2] = '+';
      grid[v_coordinate + 2][h_coordinate + 2] = '|';
      printf("? Error: you can't block every available path\n\n");
      return;
    }

    if(requested_wall_info.player == 'w')
    {
      printf("= white player played a vertical wall at %c%d\n\n", requested_wall_info.n_col + 'A', requested_wall_info.n_row);
      available_walls->white_walls--;
    }
    else if(requested_wall_info.player == 'b')
    {
      printf("= black player played a vertical wall at %c%d\n\n", requested_wall_info.n_col + 'A', requested_wall_info.n_row);
      available_walls->black_walls--;
    }
  }
  else if(requested_wall_info.orientation == 'h')
  {
    if(grid[v_coordinate + 1][h_coordinate] == '=' ||
    grid[v_coordinate + 1][h_coordinate + 2] == 'H' ||
    grid[v_coordinate + 1][h_coordinate + 4] == '=')
    {
      printf("? Error: there is already a wall there\n");
      return;
    }

    //places the actual wall.
    grid[v_coordinate + 1][h_coordinate - 1] = '=';
    grid[v_coordinate + 1][h_coordinate] = '=';
    grid[v_coordinate + 1][h_coordinate + 1] = '=';
    grid[v_coordinate + 1][h_coordinate + 2] = '=';
    grid[v_coordinate + 1][h_coordinate + 3] = '=';
    grid[v_coordinate + 1][h_coordinate + 4] = '=';
    grid[v_coordinate + 1][h_coordinate + 5] = '=';

    //@purpose: checks if there is an available path to win for both players.
    bool available_path_white = false;
    available_path(grid, grid_size, pawns_location.white_location.v_coordinate, pawns_location.white_location.h_coordinate, &available_path_white, 'w');
    bool available_path_black = false;
    available_path(grid, grid_size, pawns_location.black_location.v_coordinate, pawns_location.black_location.h_coordinate, &available_path_black, 'b');
    //@command: resets the placed wall since it is against the game's rules.
    if(available_path_white == false || available_path_black == false)
    {
      grid[v_coordinate + 1][h_coordinate - 1] = '-';
      grid[v_coordinate + 1][h_coordinate] = '-';
      grid[v_coordinate + 1][h_coordinate + 1] = '-';
      grid[v_coordinate + 1][h_coordinate + 2] = '+';
      grid[v_coordinate + 1][h_coordinate + 3] = '-';
      grid[v_coordinate + 1][h_coordinate + 4] = '-';
      grid[v_coordinate + 1][h_coordinate + 5] = '-';
      printf("? Error: you can't block every available path\n\n");
      return;
    }

    if(requested_wall_info.player == 'w')
    {
      printf("= white player played a horizontal wall at %c%d\n\n", requested_wall_info.n_col + 'A', requested_wall_info.n_row);
      available_walls->white_walls--;
    }
    else if(requested_wall_info.player == 'b')
    {
      printf("= black player played a horizontal wall at %c%d\n\n", requested_wall_info.n_col + 'A', requested_wall_info.n_row);
      available_walls->black_walls--;
    }
  }
  return;
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
