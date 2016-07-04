/*
File implementation : command.hpp
Authors             : Platon-Nikolaos Kiorpelidis
Purpose             : data structures declarations
Date created        : 28-01-2016
Date last modified  : 04-07-2016
Language            : C++
*/

#include <iostream>
#include <string>

class Quoridors_game
{
  private:

    struct Player{
      private:
        struct Vertex{
          uint32_t x;  //player's horizontal coordinate.
          uint32_t y;  //player's vertical coordinate.
        }
      public:
        Vertex location;     //player's location.
        uint32_t num_walls;  //player's available walls.
    }

    struct Board{
      private:
        struct Square{  //that's how the engine representes the walls.
          bool can_move_up;    //setted to true if there is no wall above.
          bool can_move_down;  //setted to true if there is no wall below.
          bool can_move_right; //setted to true if there is no wall right.
          bool can_move_left;  //setted to true if there is no wall left.
        }
      public:
        uint32_t board_size;   //stores the board size.
        Square **board_config; //stores the current board configuration.
    }

    Player white_player;  //stores white player's information.
    Player black_player;  //stores black player's information.
    Board board;          //stores board's information.

  public:

    Quoridors_game(uint32_t);  //sets board size to user given size.
    Quoridors_game();          //sets board size to 9x9.
    ~Quoridos_game();

    bool set_board_size(uint32_t);    //sets the board's size.
    bool set_num_walls(uint32_t);     //sets the amount of available_walls for each player.
    bool set_board_config();          //sets the board for a new game.

    bool playmove(char, uint32_t, uint32_t);        //moves a player at the requested location.
    bool playwall(char, uint32_t, uint32_t, char);  //places a wall at the requested location.

    bool undo_move(uint32_t);  //undoes user given moves.

    bool genmove(char);  //generates an AI move.

    bool showboard(); //prints to stdout the current board configuration.

    bool winner();    //checks if there is a winner.
    bool quit();      //quits the game.
}

namespace admin_commands
{
  namespace valid
  {
    bool valid_engine_name();
    bool valid_known_command();  //checks if the command has the correct.
    bool valid_list_commands();
  }

  namespace execute
  {
    void engine_name();          //prints the engine's name at stdout.
    void known_command(string);  //checks if the given command exists.
    void list_commands();        //prints all the available commands to stdout.
  }
}

namespace user_input_control
{
  void user_input_decode();  //reads from the stdin and calls the correct engine's function.
}
